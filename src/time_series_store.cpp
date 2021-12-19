//
// Created by nandanv on 17/12/2021.
//

#include <optional>
#include <iostream>
#include "time_series_store.hpp"

TimeSeriesStore::TimeSeriesStore(callbackGetTime_t fnGetTime) :
        m_uiCount(0),
        uiReservation_forVector(0),
        fnGetUnixTime(fnGetTime) {

}

std::optional<TimeSeriesStore::value_t>
TimeSeriesStore::getValue(const size_t uiIndex, const unixTime_t tTimeReference) {
    const auto &valueStore = m_timeMapStore.at(uiIndex);
    std::optional<value_t> optValue = std::nullopt;

    for (const auto &value: valueStore) {
        if (tTimeReference <= value.tTime) {
            optValue = value.tValue;
        }
    }

    return optValue;
}

size_t TimeSeriesStore::get_time() {
    return fnGetUnixTime();
}

size_t TimeSeriesStore::insert(value_t tValue) {
    const auto tTime = get_time();

    if (!m_timeMapStore.contains(m_uiCount)) {
        const auto valueStore = std::vector(1, SValue(tValue, get_time()));
        m_timeMapStore.emplace(std::make_pair(m_uiCount, valueStore));
    } else {
        auto &valueStore = m_timeMapStore[m_uiCount];
        valueStore.emplace_back(SValue(tValue, tTime));

    }

    return (m_uiCount++);
}

bool TimeSeriesStore::is_empty() {
    return !m_uiCount;
}

