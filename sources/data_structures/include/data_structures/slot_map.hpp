#pragma once

#include <limits>
#include <memory>
#include <utility>
#include <vector>
#include <iterator>

#include <fundamental/compiler_macros.hpp>
#include <fundamental/debug.hpp>
#include <fundamental/types.hpp>

#include "standard_containers.hpp"

namespace Argon
{
template <typename ObjType, uint32 NUM_OBJECTS_PER_PAGE = 64>
class SlotMap final
{
	struct DirectMemPage;

public:
	inline static uint32 s_numObjectPerPage = NUM_OBJECTS_PER_PAGE;

	class Iterator final
	{
	public:
		using value_type = ObjType;
		using pointer = ObjType*;
		using const_pointer = const ObjType*;
		using reference = ObjType&;
		using const_reference = const ObjType&;
		using difference_type = ptrdiff;
		using iterator_category = std::random_access_iterator_tag;

		Iterator();
		Iterator(DirectMemPage *pages, uint32 index);

		reference operator*();
		const_reference operator*() const;
		pointer operator->();
		const_pointer operator->() const;
		reference operator[](int m);
		const_reference operator[](int m) const;

		Iterator& operator++();
		Iterator& operator--();
		Iterator operator++(int);
		Iterator operator--(int);

		Iterator& operator+=(int n);
		Iterator& operator-=(int n);

		Iterator operator+(int n) const;
		Iterator operator-(int n) const;

		difference_type operator-(const Iterator &rhs) const { return m_index - rhs.m_index; }

		bool operator<(const Iterator &rhs) const { return m_index < rhs.m_index; }
		bool operator<=(const Iterator &rhs) const { return m_index <= rhs.m_index; }
		bool operator>(const Iterator &rhs) const { return m_index > rhs.m_index; }
		bool operator>=(const Iterator &rhs) const { return m_index >= rhs.m_index; }
		bool operator!=(const Iterator &rhs) const { return m_index != rhs.m_index; }
		bool operator==(const Iterator &rhs) const { return m_index == rhs.m_index; }

	private:
		DirectMemPage* m_pages;
		uint32 m_index;
		AR_ATTR_UNUSED byte _pad[4];
	};

	class Slot final
	{
	public:
		Slot();

		bool operator==(const Slot &rhs);
		bool operator!=(const Slot &rhs);

		uint32 getIndex() const { return m_index; }
		uint64 getGeneration() const { return m_generation; }

	private:
		friend class SlotMap;

		uint32 m_index : 24;
		uint64 m_generation : 40;
	};

	using iterator_type = Iterator;
	using const_iterator_type = const Iterator;

	SlotMap();
	~SlotMap();

	template <typename ...Args>
	Slot allocate(Args &&...args);
	ObjType& at(Slot slot) const;
	void erase(Slot slot);
	bool isSlotValid(Slot slot) const;
	
	sizet size() const { return static_cast<sizet>(m_size); }

	const_iterator_type cbegin() const { return const_iterator_type(m_directPages.data(), 0u); }
	const_iterator_type begin() const { return const_iterator_type(m_directPages.data(), 0u); }
	iterator_type begin() { return iterator_type(m_directPages.data(), 0u); }

	const_iterator_type cend() const { return const_iterator_type(m_directPages.data(), m_size); }
	const_iterator_type end() const { return const_iterator_type(m_directPages.data(), m_size); }
	iterator_type end() { return iterator_type(m_directPages.data(), m_size); }

private:
	struct DirectMemPage
	{
		std::unique_ptr<ObjType[]> m_memory;
	};
	using DirectMemPages = vector<DirectMemPage>;
	using RedirectSlots = vector<Slot>;
	using BackwardsMapping = vector<uint32>;

	void _pushDirectPage();
	void _refitRedirectSlots();
	uint32 _getPageNumber(uint32 val) const;
	uint32 _getPageOffset(uint32 val) const;

	// TODO: consider making redirect vector and backwardsMapping paginated
	// TODO: consider providing a choice to have a contigious memory instead of directPages
	RedirectSlots m_redirect;
	DirectMemPages m_directPages;
	BackwardsMapping m_backwardsMapping;

	uint32 m_size;
	// head will point outside of the array when allocated space is fully occupied
	// any allocation should check for this condition and grow the memory
	uint32 m_head;
};

template <typename ObjType, uint32 NUM_OBJECTS_PER_PAGE>
SlotMap<ObjType, NUM_OBJECTS_PER_PAGE>::Iterator::Iterator()
	: m_pages(nullptr)
	, m_index(0)
{
}

template <typename ObjType, uint32 NUM_OBJECTS_PER_PAGE>
SlotMap<ObjType, NUM_OBJECTS_PER_PAGE>::Iterator::Iterator(DirectMemPage* pages, uint32 index)
	: m_pages(pages)
	, m_index(index)
{
}

template <typename ObjType, uint32 NUM_OBJECTS_PER_PAGE>
typename SlotMap<ObjType, NUM_OBJECTS_PER_PAGE>::Iterator::reference
SlotMap<ObjType, NUM_OBJECTS_PER_PAGE>::Iterator::operator*()
{
	return m_pages[m_index / s_numObjectPerPage].m_memory[m_index % s_numObjectPerPage];
}

template <typename ObjType, uint32 NUM_OBJECTS_PER_PAGE>
typename SlotMap<ObjType, NUM_OBJECTS_PER_PAGE>::Iterator::const_reference
SlotMap<ObjType, NUM_OBJECTS_PER_PAGE>::Iterator::operator*() const
{
	return m_pages[m_index / s_numObjectPerPage].m_memory[m_index % s_numObjectPerPage];
}

template <typename ObjType, uint32 NUM_OBJECTS_PER_PAGE>
typename SlotMap<ObjType, NUM_OBJECTS_PER_PAGE>::Iterator::pointer
SlotMap<ObjType, NUM_OBJECTS_PER_PAGE>::Iterator::operator->()
{
	return &m_pages[m_index / s_numObjectPerPage].m_memory[m_index % s_numObjectPerPage];
}

template <typename ObjType, uint32 NUM_OBJECTS_PER_PAGE>
typename SlotMap<ObjType, NUM_OBJECTS_PER_PAGE>::Iterator::const_pointer
SlotMap<ObjType, NUM_OBJECTS_PER_PAGE>::Iterator::operator->() const
{
	return &m_pages[m_index / s_numObjectPerPage].m_memory[m_index % s_numObjectPerPage];
}

template <typename ObjType, uint32 NUM_OBJECTS_PER_PAGE>
typename SlotMap<ObjType, NUM_OBJECTS_PER_PAGE>::Iterator::reference
SlotMap<ObjType, NUM_OBJECTS_PER_PAGE>::Iterator::operator[](int m)
{
	const auto index = m_index + static_cast<uint32>(m);
	return m_pages[index / s_numObjectPerPage].m_memory[index % s_numObjectPerPage];
}

template <typename ObjType, uint32 NUM_OBJECTS_PER_PAGE>
typename SlotMap<ObjType, NUM_OBJECTS_PER_PAGE>::Iterator::const_reference
SlotMap<ObjType, NUM_OBJECTS_PER_PAGE>::Iterator::operator[](int m) const
{
	const auto index = m_index + static_cast<uint32>(m);
	return m_pages[index / s_numObjectPerPage].m_memory[index % s_numObjectPerPage];
}

template <typename ObjType, uint32 NUM_OBJECTS_PER_PAGE>
typename SlotMap<ObjType, NUM_OBJECTS_PER_PAGE>::Iterator&
SlotMap<ObjType, NUM_OBJECTS_PER_PAGE>::Iterator::operator++()
{
	++m_index;
	return *this;
}

template <typename ObjType, uint32 NUM_OBJECTS_PER_PAGE>
typename SlotMap<ObjType, NUM_OBJECTS_PER_PAGE>::Iterator&
SlotMap<ObjType, NUM_OBJECTS_PER_PAGE>::Iterator::operator--()
{
	--m_index;
	return *this;
}

template <typename ObjType, uint32 NUM_OBJECTS_PER_PAGE>
typename SlotMap<ObjType, NUM_OBJECTS_PER_PAGE>::Iterator
SlotMap<ObjType, NUM_OBJECTS_PER_PAGE>::Iterator::operator++(int)
{
	Iterator r(*this);
	++m_index;
	return r;
}

template <typename ObjType, uint32 NUM_OBJECTS_PER_PAGE>
typename SlotMap<ObjType, NUM_OBJECTS_PER_PAGE>::Iterator
SlotMap<ObjType, NUM_OBJECTS_PER_PAGE>::Iterator::operator--(int)
{
	Iterator r(*this);
	--m_index;
	return r;
}

template <typename ObjType, uint32 NUM_OBJECTS_PER_PAGE>
typename SlotMap<ObjType, NUM_OBJECTS_PER_PAGE>::Iterator&
SlotMap<ObjType, NUM_OBJECTS_PER_PAGE>::Iterator::operator+=(int n)
{
	m_index += n;
	return *this;
}

template <typename ObjType, uint32 NUM_OBJECTS_PER_PAGE>
typename SlotMap<ObjType, NUM_OBJECTS_PER_PAGE>::Iterator&
SlotMap<ObjType, NUM_OBJECTS_PER_PAGE>::Iterator::operator-=(int n)
{
	m_index -= n;
	return *this;
}

template <typename ObjType, uint32 NUM_OBJECTS_PER_PAGE>
typename SlotMap<ObjType, NUM_OBJECTS_PER_PAGE>::Iterator
SlotMap<ObjType, NUM_OBJECTS_PER_PAGE>::Iterator::operator+(int n) const
{
	Iterator r(*this);
	return r += n;
}

template <typename ObjType, uint32 NUM_OBJECTS_PER_PAGE>
typename SlotMap<ObjType, NUM_OBJECTS_PER_PAGE>::Iterator
SlotMap<ObjType, NUM_OBJECTS_PER_PAGE>::Iterator::operator-(int n) const
{
	Iterator r(*this);
	return r += n;
}

template <typename ObjType, uint32 NUM_OBJECTS_PER_PAGE>
SlotMap<ObjType, NUM_OBJECTS_PER_PAGE>::Slot::Slot()
	: m_index(0xFFFFFF)
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
	const auto capacity = m_directPages.size() * NUM_OBJECTS_PER_PAGE;

		AR_CRITICAL(m_head <= capacity,
		"Free list head must point at most at one element after the end of memory");

	if (m_head == capacity)
	{
		_pushDirectPage();
	}

	Slot &redirectSlot = m_redirect[m_head];
	const uint32 freeObjectIndex = m_size;

	DirectMemPage& page = m_directPages[freeObjectIndex / NUM_OBJECTS_PER_PAGE];
	new (&page.m_memory[freeObjectIndex % NUM_OBJECTS_PER_PAGE]) ObjType(std::forward<Args>(args)...);

	m_backwardsMapping[m_size] = m_head;
	m_head = redirectSlot.m_index;
	redirectSlot.m_index = m_size;

	Slot newSlot = redirectSlot;
	newSlot.m_index = m_backwardsMapping[m_size];
	
	++m_size;

	return newSlot;
}

template <typename ObjType, uint32 NUM_OBJECTS_PER_PAGE>
ObjType& SlotMap<ObjType, NUM_OBJECTS_PER_PAGE>::at(Slot slot) const
{
	AR_ASSERT(isSlotValid(slot));

	const uint32 page = _getPageNumber(slot.m_index);
	const uint32 offset =  _getPageOffset(slot.m_index);

	return m_directPages[page].m_memory[offset];
}

template <typename ObjType, uint32 NUM_OBJECTS_PER_PAGE>
void SlotMap<ObjType, NUM_OBJECTS_PER_PAGE>::erase(Slot slot)
{
	AR_ASSERT(isSlotValid(slot));

	const uint32 page = _getPageNumber(slot.m_index);
	const uint32 offset = _getPageOffset(slot.m_index);

	m_directPages[page].m_memory[offset].~ObjType();

	AR_CRITICAL(m_size != 0,
		"Container variable indicating its size is about to underflow");

	const auto backPage = (m_size - 1) / NUM_OBJECTS_PER_PAGE;
	const auto backOffset = (m_size - 1) % NUM_OBJECTS_PER_PAGE;

	m_directPages[page].m_memory[offset] = std::move(m_directPages[backPage].m_memory[backOffset]);
	m_redirect[m_backwardsMapping[m_size - 1]].m_index = m_redirect[slot.m_index].m_index;
	m_backwardsMapping[slot.m_index] = m_backwardsMapping[m_size - 1];

	m_redirect[slot.m_index].m_index = m_head;
	++m_redirect[slot.m_index].m_generation;
	m_head = slot.m_index;

	--m_size;
}

template <typename ObjType, uint32 NUM_OBJECTS_PER_PAGE>
bool SlotMap<ObjType, NUM_OBJECTS_PER_PAGE>::isSlotValid(Slot slot) const
{
	return slot.m_index < m_redirect.size() && m_redirect[slot.m_index].m_generation == slot.m_generation;
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

template <typename ObjType, uint32 NUM_OBJECTS_PER_PAGE>
uint32 SlotMap<ObjType, NUM_OBJECTS_PER_PAGE>::_getPageNumber(uint32 val) const
{
	return m_redirect[val].m_index / NUM_OBJECTS_PER_PAGE;
}

template <typename ObjType, uint32 NUM_OBJECTS_PER_PAGE>
uint32 SlotMap<ObjType, NUM_OBJECTS_PER_PAGE>::_getPageOffset(uint32 val) const
{
	return m_redirect[val].m_index % NUM_OBJECTS_PER_PAGE;
}
} // namespace Argon
