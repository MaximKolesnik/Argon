#include <algorithm>

#include <gtest/gtest.h>

#include <data_structures/sparse_storage.hpp>
#include <data_structures/standard_containers.hpp>

TEST(SlotGenerator, Small)
{
	argon::SlotGenerator slotGenerator;
	argon::vector<argon::SlotGenerator::Slot> slots;
	argon::vector<argon::SlotGenerator::Slot> erasedSlots;

	for (argon::uint32 i = 0; i < argon::SlotGenerator::SLOTS_PER_PAGES * 2; ++i)
	{
		slots.emplace_back(slotGenerator.acquire());
	}

	for (const auto& s : slots)
	{
		EXPECT_TRUE(slotGenerator.isValid(s));
	}

	for (argon::sizet i = 0; i < slots.size(); ++i)
	{
		if (i % 2)
		{
			slotGenerator.release(slots[i]);
			erasedSlots.push_back(slots[i]);
		}
	}

	for (const auto& es : erasedSlots)
	{
		slots.erase(std::remove(slots.begin(), slots.end(), es));
	}

	for (const auto& s : slots)
	{
		EXPECT_TRUE(slotGenerator.isValid(s));
	}

	for (const auto& es : erasedSlots)
	{
		EXPECT_FALSE(slotGenerator.isValid(es));
	}
}

TEST(SlotGenerator, Large)
{
	argon::SlotGenerator slotGenerator;
	argon::vector<argon::SlotGenerator::Slot> slots;
	argon::vector<argon::SlotGenerator::Slot> erasedSlots;

	for (argon::uint32 i = 0; i < argon::SlotGenerator::SLOTS_PER_PAGES * 50; ++i)
	{
		slots.emplace_back(slotGenerator.acquire());
	}

	for (const auto& s : slots)
	{
		EXPECT_TRUE(slotGenerator.isValid(s));
	}

	for (argon::sizet i = 0; i < slots.size(); ++i)
	{
		if (i % 3)
		{
			slotGenerator.release(slots[i]);
			erasedSlots.push_back(slots[i]);
		}
	}

	for (const auto& es : erasedSlots)
	{
		slots.erase(std::remove(slots.begin(), slots.end(), es));
	}

	for (const auto& s : slots)
	{
		EXPECT_TRUE(slotGenerator.isValid(s));
	}

	for (const auto& es : erasedSlots)
	{
		EXPECT_FALSE(slotGenerator.isValid(es));
	}
}

TEST(SparseStorage, SlotCheck)
{
	argon::SlotGenerator slotGenerator;
	argon::vector<argon::SlotGenerator::Slot> slots;
	argon::SparseStorage<int> storage;

	for (argon::uint32 i = 0; i < argon::SlotGenerator::SLOTS_PER_PAGES * 2; ++i)
	{
		argon::SlotGenerator::Slot slot = slotGenerator.acquire();
		slots.push_back(slot);
		storage.assign(slot, i);
	}

	{
		argon::uint32 i = 0;
		for (const auto& s : slots)
		{
			EXPECT_TRUE(storage.has(s));
			EXPECT_EQ(storage.at(s), i);
			++i;
		}
	}

	argon::vector<argon::SlotGenerator::Slot> erasedSlots;
	{
		int i = 0;
		for (auto it = slots.begin(); it != slots.end(); ++i)
		{
			if (i % 2 == 0)
			{
				erasedSlots.push_back(*it);
				storage.erase(*it);
				it = slots.erase(it);
				continue;
			}

			++it;
		}
	}
	for (const auto& s : slots)
	{
		EXPECT_TRUE(storage.has(s));
	}

	for (const auto& s : erasedSlots)
	{
		EXPECT_FALSE(storage.has(s));
	}

	for (argon::uint32 i = 0; i < argon::SlotGenerator::SLOTS_PER_PAGES * 2; ++i)
	{
		argon::SlotGenerator::Slot slot = slotGenerator.acquire();
		slots.push_back(slot);
		storage.assign(slot, i);
	}

	for (const auto& s : slots)
	{
		EXPECT_TRUE(storage.has(s));
	}

	for (const auto& s : erasedSlots)
	{
		EXPECT_FALSE(storage.has(s));
	}
}

TEST(SparseStorage, StorageAccess)
{
	argon::SlotGenerator slotGenerator;
	argon::vector<argon::SlotGenerator::Slot> slots;
	argon::vector<argon::uint32> data;
	argon::SparseStorage<int> storage;

	for (argon::uint32 i = 0; i < argon::SlotGenerator::SLOTS_PER_PAGES * 10; ++i)
	{
		argon::SlotGenerator::Slot slot = slotGenerator.acquire();
		slots.push_back(slot);
		storage.assign(slot, i);
		data.push_back(i);
	}

	for (argon::sizet i = 0; i < slots.size(); ++i)
	{
		EXPECT_TRUE(storage.has(slots[i]));
		EXPECT_EQ(storage.at(slots[i]), data[i]);
	}

	{
		argon::sizet i = 0;
		for (auto& v : storage)
		{
			EXPECT_EQ(v, i);
			++i;
		}

		i = 0;
		for (const auto& v : storage)
		{
			EXPECT_EQ(v, i);
			++i;
		}
	}

	{
		int i = 0;
		for (auto it = slots.begin(); it != slots.end(); ++i)
		{
			if (i % 2 == 0)
			{
				storage.erase(*it);
				it = slots.erase(it);
				continue;
			}

			++it;
		}

		i = 0;
		for (auto it = data.begin(); it != data.end(); ++i)
		{
			if (i % 2 == 0)
			{
				it = data.erase(it);
				continue;
			}

			++it;
		}
	}

	for (argon::sizet i = 0; i < slots.size(); ++i)
	{
		EXPECT_TRUE(storage.has(slots[i]));
		EXPECT_EQ(storage.at(slots[i]), data[i]);
	}

	EXPECT_EQ(storage.size(), data.size());

	for (argon::uint32 i = 0; i < argon::SlotGenerator::SLOTS_PER_PAGES * 10; ++i)
	{
		argon::SlotGenerator::Slot slot = slotGenerator.acquire();
		slots.push_back(slot);
		storage.assign(slot, i);
		data.push_back(i + argon::SlotGenerator::SLOTS_PER_PAGES * 10);
	}

	{
		int i = 0;
		for (auto it = slots.begin(); it != slots.end(); ++i)
		{
			if (i % 2 == 0)
			{
				storage.erase(*it);
				it = slots.erase(it);
				continue;
			}

			++it;
		}

		i = 0;
		for (auto it = data.begin(); it != data.end(); ++i)
		{
			if (i % 2 == 0)
			{
				it = data.erase(it);
				continue;
			}

			++it;
		}
	}
}
