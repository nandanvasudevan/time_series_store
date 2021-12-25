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
			= std::chrono::duration_cast<std::chrono::seconds>(timePoint.time_since_epoch()).count();

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

		constexpr size_t uiExpectedIndex = 0;
		const TimeSeriesStore::key_t tKey = 1;
		const TimeSeriesStore::value_t tValue = "nv";

		REQUIRE(uiExpectedIndex == tssStore.insert("nv", tKey));
		REQUIRE_FALSE(tssStore.is_empty());

		tLastUpdateTime = uiLastGeneratedEpoch;

		SECTION("Retrieve inserted value")
		{
			const TimeSeriesStore::value_t
					tSetValue = tssStore.getValue(tKey, tLastUpdateTime).value_or("");
			REQUIRE(tValue == tSetValue);
		}
	}
}