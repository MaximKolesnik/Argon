#pragma once

#include <limits>
#include <memory>
#include <utility>
#include <vector>

#include <fundamental/compiler_macros.hpp>
#include <fundamental/debug.hpp>
#include <fundamental/types.hpp>

namespace Argon
{
// TODO consider removing backwardsMapping for objects that store Slots
// TODO add iterators
template <typename ObjType, uint32 NUM_OBJECTS_PER_PAGE = 64>
class SlotMap
{
public:
	class Slot
	{
	public:
		Slot();

		bool operator==(const Slot &rhs);
		bool operator!=(const Slot &rhs);

	private:
		friend class SlotMap;

		uint32 m_index : 24;
		uint64 m_generation : 40;
	};

	SlotMap();
	~SlotMap();

	template <typename ...Args>
	Slot allocate(Args &&...args);
	ObjType& at(Slot slot) const;
	void erase(Slot slot);
	bool isSlotValid(Slot slot) const;

private:
	struct DirectMemPage
	{
		std::unique_ptr<ObjType[]> m_memory;
	};
	using DirectMemPages = std::vector<DirectMemPage>;
	using RedirectSlots = std::vector<Slot>;
	using BackwardsMapping = std::vector<uint32>;

	void _pushDirectPage();
	void _refitRedirectSlots();

	// TODO: consider making redirect vector and backwardsMapping paginated
	RedirectSlots m_redirect;
	DirectMemPages m_directPages;
	BackwardsMapping m_backwardsMapping;

	uint32 m_size;
	// head will point outside of the array when allocated space is fully occupied
	// any allocation should check for this condition and grow the memory
	uint32 m_head;
};

template <typename ObjType, uint32 NUM_OBJECTS_PER_PAGE>
SlotMap<ObjType, NUM_OBJECTS_PER_PAGE>::Slot::Slot()
	: m_index(0)
	, m_generation(0)
{
	static_assert (sizeof(Slot) == sizeof(intptr),
		"Slot size should be equal to a pointer size");
}

template <typename ObjType, uint32 NUM_OBJECTS_PER_PAGE>
bool SlotMap<ObjType, NUM_OBJECTS_PER_PAGE>::Slot::operator==(const Slot &rhs)
{
	return this->m_index == rhs.m_index && this->m_generation == rhs.m_generation;
}

template <typename ObjType, uint32 NUM_OBJECTS_PER_PAGE>
bool SlotMap<ObjType, NUM_OBJECTS_PER_PAGE>::Slot::operator!=(const Slot &rhs)
{
	return !(*this == rhs);
}

template <typename ObjType, uint32 NUM_OBJECTS_PER_PAGE>
SlotMap<ObjType, NUM_OBJECTS_PER_PAGE>::SlotMap()
	: m_size(0)
	, m_head(0)
{
	_pushDirectPage();
}

template <typename ObjType, uint32 NUM_OBJECTS_PER_PAGE>
SlotMap<ObjType, NUM_OBJECTS_PER_PAGE>::~SlotMap()
{
}

template <typename ObjType, uint32 NUM_OBJECTS_PER_PAGE>
template <typename ...Args>
typename SlotMap<ObjType, NUM_OBJECTS_PER_PAGE>::Slot
SlotMap<ObjType, NUM_OBJECTS_PER_PAGE>::allocate(Args &&...args)
{
	AR_CRITICAL(m_head <= m_size,
		"Free list head must point at most at one element after the end of memory");

	if (m_head == m_size)
	{
		_pushDirectPage();
	}

	Slot &newSlot = m_redirect[m_head];
	const uint32 freeObjectIndex = m_size;

	DirectMemPage& page = m_directPages[freeObjectIndex % NUM_OBJECTS_PER_PAGE];
	new (&page.m_memory[freeObjectIndex / NUM_OBJECTS_PER_PAGE])
		ObjType(std::forward<Args>(args)...);

	m_backwardsMapping[m_size] = m_head;
	m_head = newSlot.m_index;
	newSlot.m_index = m_size;
	++m_size;

	return newSlot;
}

template <typename ObjType, uint32 NUM_OBJECTS_PER_PAGE>
ObjType& SlotMap<ObjType, NUM_OBJECTS_PER_PAGE>::at(Slot slot) const
{
	AR_ASSERT(isSlotValid(slot));

	const uint32 page = slot.m_index % NUM_OBJECTS_PER_PAGE;
	const uint32 offset = slot.m_index / NUM_OBJECTS_PER_PAGE;

	return m_directPages[page].m_memory[offset];
}

template <typename ObjType, uint32 NUM_OBJECTS_PER_PAGE>
void SlotMap<ObjType, NUM_OBJECTS_PER_PAGE>::erase(Slot slot)
{
	AR_ASSERT(isSlotValid(slot));

	const uint32 page = slot.m_index % NUM_OBJECTS_PER_PAGE;
	const uint32 offset = slot.m_index / NUM_OBJECTS_PER_PAGE;

	m_directPages[page].m_memory[offset].~ObjType();

	AR_CRITICAL(m_size != 0,
		"Container variable indicating its size is about to underflow");

	if (slot.m_index != m_size - 1)
	{
		const uint32 backPage = m_size % NUM_OBJECTS_PER_PAGE;
		const uint32 backOffset = m_size / NUM_OBJECTS_PER_PAGE;

		m_directPages[page].m_memory[offset] =
			std::move(m_directPages[backPage].m_memory[backOffset]);
		m_backwardsMapping[slot.m_index] = m_backwardsMapping[m_size - 1];
	}

	m_redirect[slot.m_index].m_index = m_head;
	m_head = slot.m_index;

	--m_size;
}

template <typename ObjType, uint32 NUM_OBJECTS_PER_PAGE>
bool SlotMap<ObjType, NUM_OBJECTS_PER_PAGE>::isSlotValid(Slot slot) const
{
	return slot.m_index < m_size
		&& m_redirect[slot.m_index].m_generation == slot.m_generation;
}

template <typename ObjType, uint32 NUM_OBJECTS_PER_PAGE>
void SlotMap<ObjType, NUM_OBJECTS_PER_PAGE>::_pushDirectPage()
{
	m_directPages.push_back({std::make_unique<ObjType[]>(NUM_OBJECTS_PER_PAGE)});
	m_backwardsMapping.resize(m_backwardsMapping.size() + NUM_OBJECTS_PER_PAGE);

	const uint32 numPages = static_cast<uint32>(m_directPages.size());
	if (numPages * NUM_OBJECTS_PER_PAGE > m_redirect.size())
	{
		_refitRedirectSlots();
	}
}

template <typename ObjType, uint32 NUM_OBJECTS_PER_PAGE>
void SlotMap<ObjType, NUM_OBJECTS_PER_PAGE>::_refitRedirectSlots()
{
	const uint32 prevSize = static_cast<uint32>(m_redirect.size());
	const uint32 newSize = prevSize + NUM_OBJECTS_PER_PAGE;

	m_redirect.resize(prevSize + NUM_OBJECTS_PER_PAGE);

	for (uint32 i = prevSize + 1; i <= newSize; ++i)
	{
		m_redirect[i - 1].m_index = i;
	}
}
} // namespace Argon
