#include <fundamental/debug.hpp>

#include "sparse_storage.hpp"

namespace Argon
{
SlotGenerator::Slot::Slot()
	: m_index(INVALID_INDEX)
	, m_generation(0u)
{
}

SlotGenerator::SlotGenerator()
	: m_head(0u)
{
	_allocatePage();
}

SlotGenerator::~SlotGenerator() = default;

SlotGenerator::Slot SlotGenerator::acquire()
{
	if (m_head == m_memPages.size() * SLOTS_PER_PAGES)
	{
		_allocatePage();
	}

	const uint32 pageNum = m_head / SLOTS_PER_PAGES;
	const uint32 pageOffset = m_head % SLOTS_PER_PAGES;
	Slot newSlot = m_memPages[pageNum].m_memory[pageOffset];
	m_head = newSlot.m_index;

	newSlot.m_index = pageNum * SLOTS_PER_PAGES + pageOffset;
	return newSlot;
}

void SlotGenerator::release(Slot slot)
{
	AR_CRITICAL(isValid(slot), "SlotGenerator::release :: slot is invalid");

	const uint32 pageNum = slot.m_index / SLOTS_PER_PAGES;
	const uint32 pageOffset = slot.m_index % SLOTS_PER_PAGES;

	Slot& internalSlot = m_memPages[pageNum].m_memory[pageOffset];
	++internalSlot.m_generation;
	uint32 newHead = slot.m_index;
	slot.m_index = m_head;
	m_head = newHead;
}

bool SlotGenerator::isValid(Slot slot) const
{
	return slot.m_index < m_memPages.size() * SLOTS_PER_PAGES
		&& slot.m_generation == m_memPages[slot.m_index / SLOTS_PER_PAGES]
			.m_memory[slot.m_index % SLOTS_PER_PAGES].m_generation;
}

void SlotGenerator::_allocatePage()
{
	const uint32 lastPageInd = static_cast<uint32>(m_memPages.size());
	m_memPages.push_back({std::make_unique<Slot[]>(SLOTS_PER_PAGES)});

	for (uint32 i = 0; i < SLOTS_PER_PAGES; ++i)
	{
		m_memPages[lastPageInd].m_memory[i].m_index = lastPageInd * SLOTS_PER_PAGES + i + 1;
	}
}
} // namespace Argon
