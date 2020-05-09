#include <gtest/gtest.h>

#include <data_structures/slot_map.hpp>
#include <data_structures/standard_containers.hpp>

TEST(SlotMap, DefaultConstructedHandle) 
{
	argon::SlotMap<int> slotMap;
	argon::SlotMap<int>::Slot slot;

	EXPECT_FALSE(slotMap.isSlotValid(slot))
		<< "Slot is valid, although it was created in invalid state";

	const auto newSlot = slotMap.allocate(10);
	EXPECT_FALSE(slotMap.isSlotValid(slot))
		<< "Constructed slot used invalid generation and indix";

	EXPECT_TRUE(slotMap.isSlotValid(newSlot))
		<< "Slot happens to be invalid, althoug it was just constructed";

	ASSERT_DEATH(slotMap.at(slot), "Assertion")
		<< "Dereferencing invalid slot should be fatal";
}

TEST(SlotMap, SimpleCreation)
{
	argon::SlotMap<int> slotMap;

	const auto newSlot = slotMap.allocate(10);
	EXPECT_TRUE(slotMap.isSlotValid(newSlot))
		<< "Data was just allocated, slot should be valid";

	EXPECT_EQ(10, slotMap.at(newSlot))
		<< "Wrong data is retrived from the container";

	slotMap.erase(newSlot);
	EXPECT_FALSE(slotMap.isSlotValid(newSlot))
		<< "Data was just erased, thus slot should be invalid";
}

TEST(SlotMap, AllocationSmall)
{
	argon::SlotMap<argon::sizet> slotMap;
	argon::vector<argon::SlotMap<argon::sizet>::Slot> slots;

	for (argon::sizet i = 0; i < 500; ++i)
	{
		auto slot = slotMap.allocate(i);
		EXPECT_TRUE(slotMap.isSlotValid(slot));

		EXPECT_EQ(i, slotMap.at(slot));

		slots.push_back(slot);
	}

	for (argon::sizet i = 0; i < 500; ++i)
	{
		EXPECT_TRUE(slotMap.isSlotValid(slots[i]));
		EXPECT_EQ(i, slotMap.at(slots[i]));
	}
}

TEST(SlotMap, AllocationBig)
{
	argon::SlotMap<argon::sizet> slotMap;
	argon::vector<argon::SlotMap<argon::sizet>::Slot> slots;

	for (argon::sizet i = 0; i < 100000; ++i)
	{
		auto slot = slotMap.allocate(i);
		EXPECT_TRUE(slotMap.isSlotValid(slot));

		EXPECT_EQ(i, slotMap.at(slot));

		slots.push_back(slot);
	}

	for (argon::sizet i = 0; i < 100000; ++i)
	{
		EXPECT_TRUE(slotMap.isSlotValid(slots[i]));
		EXPECT_EQ(i, slotMap.at(slots[i]));
	}
}

TEST(SlotMap, GenerationTest)
{
	argon::SlotMap<argon::uint32> slotMap;

	auto newSlot = slotMap.allocate(10u);
	EXPECT_EQ(newSlot.getGeneration(), 0u);

	slotMap.erase(newSlot);
	newSlot = slotMap.allocate(15u);
	EXPECT_EQ(newSlot.getGeneration(), 1u);

	slotMap.erase(newSlot);
	newSlot = slotMap.allocate(20u);
	EXPECT_EQ(newSlot.getGeneration(), 2u);
}

TEST(SlotMap, SmallRemap)
{
	argon::SlotMap<argon::uint32> slotMap;

	auto slot0 = slotMap.allocate(0u);
	auto slot1 = slotMap.allocate(1u);
	auto slot2 = slotMap.allocate(2u);
	auto slot3 = slotMap.allocate(3u);
	auto slot4 = slotMap.allocate(4u);

	EXPECT_EQ(slotMap.at(slot0), 0u);
	EXPECT_EQ(slotMap.at(slot1), 1u);
	EXPECT_EQ(slotMap.at(slot2), 2u);
	EXPECT_EQ(slotMap.at(slot3), 3u);
	EXPECT_EQ(slotMap.at(slot4), 4u);

	slotMap.erase(slot1);
	EXPECT_FALSE(slotMap.isSlotValid(slot1));

	EXPECT_EQ(slotMap.at(slot0), 0u);
	EXPECT_EQ(slotMap.at(slot2), 2u);
	EXPECT_EQ(slotMap.at(slot3), 3u);
	EXPECT_EQ(slotMap.at(slot4), 4u);

	slotMap.erase(slot4);
	EXPECT_FALSE(slotMap.isSlotValid(slot4));
	EXPECT_EQ(slotMap.at(slot0), 0u);
	EXPECT_EQ(slotMap.at(slot2), 2u);
	EXPECT_EQ(slotMap.at(slot3), 3u);

	slotMap.erase(slot0);
	EXPECT_FALSE(slotMap.isSlotValid(slot0));
	EXPECT_EQ(slotMap.at(slot2), 2u);
	EXPECT_EQ(slotMap.at(slot3), 3u);

	slot0 = slotMap.allocate(10u);
	slot4 = slotMap.allocate(40u);
	EXPECT_EQ(slotMap.at(slot0), 10u);
	EXPECT_EQ(slotMap.at(slot4), 40u);
	EXPECT_EQ(slotMap.at(slot2), 2u);
	EXPECT_EQ(slotMap.at(slot3), 3u);
}

TEST(SlotMap, BigRemap)
{
	argon::SlotMap<argon::sizet> slotMap;
	argon::vector<argon::SlotMap<argon::sizet>::Slot> slots;
	argon::vector<argon::sizet> vals;

	for (argon::sizet i = 0; i < slotMap.s_numObjectPerPage * 10; ++i)
	{
		slots.emplace_back(slotMap.allocate(i));
		vals.emplace_back(i);
	}

	for (argon::sizet i = 0; i < slotMap.s_numObjectPerPage * 10; ++i)
	{
		EXPECT_TRUE(slotMap.isSlotValid(slots[i]));
		EXPECT_EQ(slotMap.at(slots[i]), vals[i]);
	}

	for (int i = static_cast<int>(slotMap.s_numObjectPerPage * 10 - 1); i >= 0; --i)
	{
		if (i % 3 == 0)
		{
			slotMap.erase(slots[static_cast<argon::sizet>(i)]);
			slots.erase(slots.begin() + i);
			vals.erase(vals.begin() + i);
		}
	}

	for (argon::sizet i = 0; i < slots.size(); ++i)
	{
		EXPECT_TRUE(slotMap.isSlotValid(slots[i]));
		EXPECT_EQ(slotMap.at(slots[i]), vals[i]);
	}

	for (argon::sizet i = 0; i < slotMap.s_numObjectPerPage * 5; ++i)
	{
		slots.emplace_back(slotMap.allocate(10000u + i));
		vals.emplace_back(10000u + i);
	}

	for (argon::sizet i = 0; i < slots.size(); ++i)
	{
		EXPECT_TRUE(slotMap.isSlotValid(slots[i]));
		EXPECT_EQ(slotMap.at(slots[i]), vals[i]);
	}
}

TEST(SlotMap, Iterators)
{
	argon::SlotMap<argon::sizet> slotMap;
	argon::vector<argon::SlotMap<argon::sizet>::Slot> slots;
	argon::vector<argon::sizet> vals;

	for (argon::sizet i = 0; i < slotMap.s_numObjectPerPage * 10; ++i)
	{
		slots.emplace_back(slotMap.allocate(i));
		vals.emplace_back(i);
	}

	for (argon::sizet i = 0; i < slotMap.s_numObjectPerPage * 10; ++i)
	{
		EXPECT_TRUE(slotMap.isSlotValid(slots[i]));
		EXPECT_EQ(slotMap.at(slots[i]), vals[i]);
	}

	{
		argon::sizet i = 0;
		for (const auto& v : slotMap)
		{
			EXPECT_EQ(v, vals[i]);
			++i;
		}
	}
}
