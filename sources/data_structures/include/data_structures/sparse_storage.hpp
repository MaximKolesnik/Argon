#pragma once

#include <memory>

#include <fundamental/compiler_macros.hpp>
#include <fundamental/debug.hpp>
#include <fundamental/types.hpp>

#include "standard_containers.hpp"

namespace Argon
{
class AR_SYM_EXPORT SlotGenerator final
{
public:
	class Slot final
	{
		inline static constexpr uint32 INVALID_INDEX = 0xFFFFFF;

	public:
		Slot();

		bool operator==(const Slot &rhs)
		{
			return this->m_index == rhs.m_index && this->m_generation == rhs.m_generation;
		}
		bool operator!=(const Slot &rhs) { return !(*this == rhs); }

		uint32 getIndex() const { return m_index; }
		uint64 getGeneration() const { return m_generation; }

	private:
		friend class SlotGenerator;
		template <typename> friend class SparseStorage;

		uint32 m_index : 24;
		uint64 m_generation : 40;
	};

	inline static constexpr uint32 SLOTS_PER_PAGES = 64u;

	SlotGenerator();
	~SlotGenerator();

	Slot acquire();
	void release(Slot slot);
	bool isValid(Slot slot) const;

private:
	struct MemPage
	{
		std::unique_ptr<Slot[]> m_memory;
	};

	using MemPages = vector<MemPage>;

	void _allocatePage();

	MemPages m_memPages;
	uint32 m_head;
	AR_ATTR_UNUSED byte _pad[4];
};

template <typename TData>
class SparseStorage final
{
public:
	class Iterator
	{
	public:
		using value_type = TData;
		using pointer = TData*;
		using const_pointer = const TData*;
		using reference = TData&;
		using const_reference = const TData&;
		using difference_type = ptrdiff;
		using iterator_category = std::random_access_iterator_tag;

		Iterator() : m_data(nullptr), m_index(0) {}

		reference operator*() { return m_data[m_index]; }
		const_reference operator*() const { return m_data[m_index]; }
		pointer operator->() { return &m_data[m_index]; }
		const_pointer operator->() const { return &m_data[m_index]; }
		reference operator[](int m) { return m_data[m_index + static_cast<uint32>(m)]; }
		const_reference operator[](int m) const { return m_data[m_index + static_cast<uint32>(m)]; }

		Iterator& operator++() { ++m_index; return *this; }
		Iterator& operator--() { --m_index; return *this; }
		Iterator operator++(int) { Iterator r(*this); ++m_index; return r; }
		Iterator operator--(int) { Iterator r(*this); --m_index; return r; }

		Iterator& operator+=(int n) { m_index += n; return *this; }
		Iterator& operator-=(int n) { m_index -= n; return *this; }

		Iterator operator+(int n) const { Iterator r(*this); return r += n; }
		Iterator operator-(int n) const { Iterator r(*this); return r -= n; }

		difference_type operator-(const Iterator &rhs) const { return m_index - rhs.m_index; }

		bool operator<(const Iterator &rhs) const { return m_index < rhs.m_index; }
		bool operator<=(const Iterator &rhs) const { return m_index <= rhs.m_index; }
		bool operator>(const Iterator &rhs) const { return m_index > rhs.m_index; }
		bool operator>=(const Iterator &rhs) const { return m_index >= rhs.m_index; }
		bool operator!=(const Iterator &rhs) const { return m_index != rhs.m_index; }
		bool operator==(const Iterator &rhs) const { return m_index == rhs.m_index; }

	private:
		friend class SparseStorage;

		Iterator(TData* data, sizet index) : m_data(data), m_index(index) {}

		TData* m_data;
		sizet m_index;
	};

	using iterator_type = Iterator;
	using const_iterator_type = const Iterator;

	SparseStorage() = default;

	template <typename ...Args>
	void assign(SlotGenerator::Slot slot, Args &&...args);
	bool has(SlotGenerator::Slot slot) const;
	void erase(SlotGenerator::Slot slot);

	TData& at(SlotGenerator::Slot slot);
	const TData& at(SlotGenerator::Slot slot) const;

	sizet size() const { return m_storage.size(); }

	const_iterator_type cbegin() const { return const_iterator_type(m_storage.data(), 0u); }
	const_iterator_type begin() const { return const_iterator_type(m_storage.data(), 0u); }
	iterator_type begin() { return iterator_type(m_storage.data(), 0u); }

	const_iterator_type cend() const { return const_iterator_type(m_storage.data(), m_storage.size()); }
	const_iterator_type end() const { return const_iterator_type(m_storage.data(), m_storage.size()); }
	iterator_type end() { return iterator_type(m_storage.data(), m_storage.size()); }

private:
	struct RedirectMemPage
	{
		std::unique_ptr<SlotGenerator::Slot[]> m_memory;
	};

	using RedirectMemPages = vector<RedirectMemPage>;
	using DirectStorage = vector<TData>;

	void _prepareRedirectionMemory(SlotGenerator::Slot slot);
	
	RedirectMemPages m_redirection;
	DirectStorage m_storage;
};

template <typename TData>
template <typename ...Args>
void SparseStorage<TData>::assign(SlotGenerator::Slot slot, Args &&...args)
{
	AR_CRITICAL(!has(slot), "Some data is already assigned to this slot");

	_prepareRedirectionMemory(slot);

	const uint32 pageNum = slot.m_index / SlotGenerator::SLOTS_PER_PAGES;
	const uint32 offset = slot.m_index % SlotGenerator::SLOTS_PER_PAGES;

	SlotGenerator::Slot& internalSlot = m_redirection[pageNum].m_memory[offset];
	internalSlot.m_index = static_cast<uint32>(m_storage.size());
	internalSlot.m_generation = slot.m_generation;

	m_storage.emplace_back(std::forward<Args>(args)...);
}

template <typename TData>
bool SparseStorage<TData>::has(SlotGenerator::Slot slot) const
{
	const uint32 pageNum = slot.m_index / SlotGenerator::SLOTS_PER_PAGES;
	const uint32 offset = slot.m_index % SlotGenerator::SLOTS_PER_PAGES;

	return pageNum < m_redirection.size()
		&& m_redirection[pageNum].m_memory
		&& m_redirection[pageNum].m_memory[offset].m_index != SlotGenerator::Slot::INVALID_INDEX
		&& m_redirection[pageNum].m_memory[offset].m_generation == slot.m_generation;
}

template <typename TData>
void SparseStorage<TData>::erase(SlotGenerator::Slot slot)
{
	AR_CRITICAL(has(slot), "Slot is not assigned to the storage");

	const uint32 pageNum = slot.m_index / SlotGenerator::SLOTS_PER_PAGES;
	const uint32 offset = slot.m_index % SlotGenerator::SLOTS_PER_PAGES;

	SlotGenerator::Slot& internalSlot = m_redirection[pageNum].m_memory[offset];
	const uint32 location = internalSlot.m_index;
	++internalSlot.m_generation;
	internalSlot.m_index = SlotGenerator::Slot::INVALID_INDEX;

	m_storage[location] = std::move(m_storage.back());
	m_storage.pop_back();
}

template <typename TData>
TData& SparseStorage<TData>::at(SlotGenerator::Slot slot)
{
	AR_CRITICAL(has(slot), "Slot is not assigned to the storage");
	const uint32 pageNum = slot.m_index / SlotGenerator::SLOTS_PER_PAGES;
	const uint32 offset = slot.m_index % SlotGenerator::SLOTS_PER_PAGES;

	return m_storage[m_redirection[pageNum].m_memory[offset].m_index];
}

template <typename TData>
const TData& SparseStorage<TData>::at(SlotGenerator::Slot slot) const
{
	AR_CRITICAL(has(slot), "Slot is not assigned to the storage");
	const uint32 pageNum = slot.m_index / SlotGenerator::SLOTS_PER_PAGES;
	const uint32 offset = slot.m_index % SlotGenerator::SLOTS_PER_PAGES;

	return m_storage[m_redirection[pageNum].m_memory[offset].m_index];
}

template <typename TData>
void SparseStorage<TData>::_prepareRedirectionMemory(SlotGenerator::Slot slot)
{
	const uint32 pageNum = slot.m_index / SlotGenerator::SLOTS_PER_PAGES;

	if (const uint32 size = static_cast<uint32>(m_redirection.size()) * SlotGenerator::SLOTS_PER_PAGES;
		slot.m_index <= size)
	{
		m_redirection.resize(pageNum + 1);
	}

	if (!m_redirection[pageNum].m_memory)
	{
		m_redirection[pageNum].m_memory =
			std::make_unique<SlotGenerator::Slot[]>(SlotGenerator::SLOTS_PER_PAGES);
	}
}
} // namespace Argon
