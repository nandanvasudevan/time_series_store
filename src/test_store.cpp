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

TEST_CASE("Initial condition", "[time_keyValue_store]")
{
	SECTION("Empty constructor")
	{
		TimeSeriesStore tssTest(getTime);

		REQUIRE(tssTest.is_empty());
		REQUIRE(tssTest.get_time());
	}
}

TEST_CASE("Insert element", "[time_series_store]")
{
	SECTION("Insert a value") {
		TimeSeriesStore tssStore(getTime);
		TimeSeriesStore::unixTime_t tLastUpdateTime;

		constexpr size_t uiExpectedCount = 1;
		const TimeSeriesStore::key_t tKey = 1;
		const TimeSeriesStore::value_t tInsertedValue = "nv";

		REQUIRE(uiExpectedCount == tssStore.insert(tInsertedValue, tKey));
		REQUIRE_FALSE(tssStore.is_empty());

		tLastUpdateTime = uiLastGeneratedEpoch;

		SECTION("Retrieve inserted value")
		{
			SECTION("At the insertion time instant")
			{
				const TimeSeriesStore::value_t
						tSetValue = tssStore.getValue(tKey, tLastUpdateTime).value_or("");
				REQUIRE(tInsertedValue == tSetValue);
			}

			SECTION("After the insertion time event")
			{
				const TimeSeriesStore::value_t
						tSetValue = tssStore.getValue(tKey, tLastUpdateTime + 1).value_or(
						"");
				REQUIRE(tInsertedValue == tSetValue);
			}
		}

		SECTION("Insert another value")
		{
			constexpr size_t uiExpectedCount_afterSecond = uiExpectedCount + 1;
			const TimeSeriesStore::value_t tInsertedValue_second = "ab";

			const size_t uiCount = tssStore.insert(tInsertedValue_second, tKey);
			const TimeSeriesStore::unixTime_t tInsertionTime = uiLastGeneratedEpoch;

			REQUIRE(uiExpectedCount_afterSecond == uiCount);

			SECTION("Retrieve inserted values")
			{
				REQUIRE(tInsertedValue ==
				        tssStore.getValue(tKey, tInsertionTime - 1).value_or(""));
				REQUIRE(tInsertedValue_second ==
				        tssStore.getValue(tKey, tInsertionTime).value_or(""));
				REQUIRE(tInsertedValue_second ==
				        tssStore.getValue(tKey, tInsertionTime + 1).value_or(""));
			}
		}
	}
}