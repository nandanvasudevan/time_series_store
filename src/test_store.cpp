#include <catch2/catch_all.hpp>

#include "time_series_store.hpp"

#include <chrono>

static size_t uiLastGeneratedEpoch = 0;

size_t getTime() {
    const auto timePoint = std::chrono::system_clock::now();
    const size_t timeCount = std::chrono::duration_cast<std::chrono::seconds>(timePoint.time_since_epoch()).count();

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
        const TimeSeriesStore::value_t tValue = "nv";

        REQUIRE(uiExpectedIndex == tssStore.insert("nv"));
        REQUIRE_FALSE(tssStore.is_empty());

        tLastUpdateTime = uiLastGeneratedEpoch;

        SECTION("Retrieve inserted value")
        {
            const TimeSeriesStore::value_t tSetValue = tssStore.getValue(0, tLastUpdateTime).value_or("");
            REQUIRE(tValue == tSetValue);
        }
    }
}