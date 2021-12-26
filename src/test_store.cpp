//! @file: test_store.cpp
//! @brief:
//
// Created by: nandanv
// Created at: 23/12/2021 20:42
//

#include "time_series_store.hpp"

#include <catch2/catch_all.hpp>

#include <chrono>

//! Stores the last generated unix time
static size_t uiLastGeneratedEpoch = 0;

//************************************************************
// Created by: nandanv
// Created at: 23-Dec-2021 21:12
// Comments:
//
//************************************************************
size_t
getTime() {
	const auto timePoint = std::chrono::system_clock::now();
	const size_t timeCount
			= std::chrono::duration_cast<std::chrono::nanoseconds>(timePoint.time_since_epoch()).count();

	uiLastGeneratedEpoch = timeCount;
	return timeCount;
}

//************************************************************************************
// Created by: nandanv
// Created at: 25-Dec-2021 22:12
// Comments:
//
//************************************************************************************
auto
insertElement(TimeSeriesStore &store,
              const TimeSeriesStore::key_t &key,
              const TimeSeriesStore::value_t &value) {
	const auto uiCount = store.insert(key, value);
	const auto insertionTime = uiLastGeneratedEpoch;

	return std::make_pair(uiCount, insertionTime);
}


TEST_CASE("Initial condition", "[time_keyValue_store]")
{
	SECTION("Empty constructor")
	{
		TimeSeriesStore tssTest(getTime);

		REQUIRE(tssTest.is_empty());
		REQUIRE(tssTest.get_time());
		REQUIRE(tssTest.is_empty(5));
	}
}

TEST_CASE("Insert element", "[time_series_store]")
{
	SECTION("Insert a value") {
		TimeSeriesStore tssStore(getTime);

		constexpr size_t uiExpectedCount = 1;
		const TimeSeriesStore::key_t tKey = 1;
		const TimeSeriesStore::value_t tInsertedValue = "nv";

		const auto[countForKey, tLastUpdateTime] = insertElement(tssStore,
		                                                         tKey,
		                                                         tInsertedValue);

		REQUIRE(uiExpectedCount == countForKey);
		REQUIRE_FALSE(tssStore.is_empty());

		SECTION("Retrieve inserted value")
		{
			SECTION("At the insertion time instant")
			{
				const TimeSeriesStore::value_t
						tSetValue = tssStore.get_value(tKey,
						                               tLastUpdateTime).value_or("");
				REQUIRE(tInsertedValue == tSetValue);
			}

			SECTION("After the insertion time event")
			{
				const TimeSeriesStore::value_t
						tSetValue = tssStore.get_value(tKey,
						                               tLastUpdateTime + 1).value_or(
						"");
				REQUIRE(tInsertedValue == tSetValue);
			}
		}

		SECTION("Insert another value")
		{
			constexpr size_t uiExpectedCount_afterSecond = uiExpectedCount + 1;
			const TimeSeriesStore::value_t tInsertedValue_second = "ab";

			const size_t uiCount = tssStore.insert(tKey, tInsertedValue_second);
			const TimeSeriesStore::unixTime_t tInsertionTime = uiLastGeneratedEpoch;

			REQUIRE(uiExpectedCount_afterSecond == uiCount);

			SECTION("Retrieve inserted values")
			{
				REQUIRE(tInsertedValue ==
				        tssStore.get_value(tKey, tInsertionTime - 1).value_or(""));
				REQUIRE(tInsertedValue_second ==
				        tssStore.get_value(tKey, tInsertionTime).value_or(""));
				REQUIRE(tInsertedValue_second ==
				        tssStore.get_value(tKey, tInsertionTime + 1).value_or(""));
			}
		}
	}
}

TEST_CASE("Remove element", "[time_series_store]")
{
	SECTION("Insert an element")
	{
		TimeSeriesStore tssStore(getTime);

		constexpr size_t uiExpectedCount = 1;
		const TimeSeriesStore::key_t tKey = 1;
		const TimeSeriesStore::value_t tInsertedValue = "nv";

		const auto[countForKey, tInsertionTime] = insertElement(tssStore,
		                                                        tKey,
		                                                        tInsertedValue);

		REQUIRE(uiExpectedCount == countForKey);

		SECTION("Remove the inserted element")
		{
			REQUIRE(tssStore.remove(tKey, tInsertedValue));
			REQUIRE((uiExpectedCount - 1) == tssStore.get_count_for_key(tKey));
			REQUIRE(tssStore.is_empty(tKey));
		}

		SECTION("Remove non-existent element from existing key")
		{
			REQUIRE_FALSE(tssStore.remove(tKey, tInsertedValue + "s"));
		}
	}

	SECTION("Attempt to remove from non-existent key")
	{
		TimeSeriesStore tssStore(getTime);

		REQUIRE_FALSE(tssStore.remove(0, ""));
	}
}

#ifdef TEST_ENABLE_BENCHMARKING
TEST_CASE("Benchmarks")
{
	TimeSeriesStore tssStore(getTime);
	BENCHMARK("Element insertion")
				{
					return tssStore.insert(0, "");
				};

	SECTION("Removing an element")
	{
		tssStore.insert(0, "");
		BENCHMARK("Element deletion")
					{
						return tssStore.remove(0, "");
					};
	}
}
#endif // TEST_ENABLE_BENCHMARKING