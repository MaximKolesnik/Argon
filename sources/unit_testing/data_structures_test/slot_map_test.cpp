#include <gtest/gtest.h>

#include <data_structures/slot_map.hpp>

TEST(SlotMap, DefaultConstructedHandle) 
{
	Argon::SlotMap<int> slotMap;
	Argon::SlotMap<int>::Slot slot;

	EXPECT_FALSE(slotMap.isSlotValid(slot));
	const auto newSlot = slotMap.allocate(10);
	EXPECT_FALSE(slotMap.isSlotValid(slot));
	EXPECT_TRUE(slotMap.isSlotValid(newSlot));
}
