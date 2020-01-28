#include <algorithm>

#include <gtest/gtest.h>

#include <data_structures/sparse_storage.hpp>
#include <data_structures/standard_containers.hpp>

TEST(SlotGenerator, Small)
{
	Argon::SlotGenerator slotGenerator;
	Argon::vector<Argon::SlotGenerator::Slot> slots;
	Argon::vector<Argon::SlotGenerator::Slot> erasedSlots;

	for (Argon::uint32 i = 0; i < Argon::SlotGenerator::SLOTS_PER_PAGES * 2; ++i)
	{
		slots.emplace_back(slotGenerator.acquire());
	}

	for (const auto& s : slots)
	{
		EXPECT_TRUE(slotGenerator.isValid(s));
	}

	for (Argon::sizet i = 0; i < slots.size(); ++i)
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
	Argon::SlotGenerator slotGenerator;
	Argon::vector<Argon::SlotGenerator::Slot> slots;
	Argon::vector<Argon::SlotGenerator::Slot> erasedSlots;

	for (Argon::uint32 i = 0; i < Argon::SlotGenerator::SLOTS_PER_PAGES * 50; ++i)
	{
		slots.emplace_back(slotGenerator.acquire());
	}

	for (const auto& s : slots)
	{
		EXPECT_TRUE(slotGenerator.isValid(s));
	}

	for (Argon::sizet i = 0; i < slots.size(); ++i)
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
	Argon::SlotGenerator slotGenerator;
	Argon::vector<Argon::SlotGenerator::Slot> slots;
	Argon::SparseStorage<int> storage;

	for (Argon::uint32 i = 0; i < Argon::SlotGenerator::SLOTS_PER_PAGES * 2; ++i)
	{
		Argon::SlotGenerator::Slot slot = slotGenerator.acquire();
		slots.push_back(slot);
		storage.assign(slot, i);
	}

	{
		Argon::uint32 i = 0;
		for (const auto& s : slots)
		{
			EXPECT_TRUE(storage.has(s));
			EXPECT_EQ(storage.at(s), i);
			++i;
		}
	}

	Argon::vector<Argon::SlotGenerator::Slot> erasedSlots;
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

	for (Argon::uint32 i = 0; i < Argon::SlotGenerator::SLOTS_PER_PAGES * 2; ++i)
	{
		Argon::SlotGenerator::Slot slot = slotGenerator.acquire();
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
	Argon::SlotGenerator slotGenerator;
	Argon::vector<Argon::SlotGenerator::Slot> slots;
	Argon::vector<Argon::uint32> data;
	Argon::SparseStorage<int> storage;

	for (Argon::uint32 i = 0; i < Argon::SlotGenerator::SLOTS_PER_PAGES * 10; ++i)
	{
		Argon::SlotGenerator::Slot slot = slotGenerator.acquire();
		slots.push_back(slot);
		storage.assign(slot, i);
		data.push_back(i);
	}

	for (Argon::sizet i = 0; i < slots.size(); ++i)
	{
		EXPECT_TRUE(storage.has(slots[i]));
		EXPECT_EQ(storage.at(slots[i]), data[i]);
	}

	{
		Argon::sizet i = 0;
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

	for (Argon::sizet i = 0; i < slots.size(); ++i)
	{
		EXPECT_TRUE(storage.has(slots[i]));
		EXPECT_EQ(storage.at(slots[i]), data[i]);
	}

	EXPECT_EQ(storage.size(), data.size());

	for (Argon::uint32 i = 0; i < Argon::SlotGenerator::SLOTS_PER_PAGES * 10; ++i)
	{
		Argon::SlotGenerator::Slot slot = slotGenerator.acquire();
		slots.push_back(slot);
		storage.assign(slot, i);
		data.push_back(i + Argon::SlotGenerator::SLOTS_PER_PAGES * 10);
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
