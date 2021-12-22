//
// Created by nandanv on 17/12/2021.
//

#include <optional>
#include "time_series_store.hpp"

TimeSeriesStore::TimeSeriesStore(callbackGetTime_t &&fnGetTime) :
        m_uiCount(0),
        m_uiReservation_forVector(0),
        m_fnGetUnixTime(std::move(fnGetTime)) {

}

std::optional<TimeSeriesStore::value_t>
TimeSeriesStore::getValue(const key_t &uiKey, const unixTime_t &tTimeReference) {
    const auto &valueStore = m_timeMapStore.at(uiKey);
    std::optional<value_t> optValue = std::nullopt;

    for (const auto &value: valueStore) {
        if (tTimeReference <= value.tTime) {
            optValue = value.tValue;
        }
    }

    return optValue;
}

size_t TimeSeriesStore::get_time() {
    return m_fnGetUnixTime();
}

size_t TimeSeriesStore::insert(const value_t &tValue) {
    const auto tTime = get_time();

    if (!m_timeMapStore.contains(m_uiCount)) {
        auto valueStore = std::vector(1, SValue(tValue, get_time()));
        valueStore.reserve(m_uiReservation_forVector);
        m_timeMapStore.emplace(std::make_pair(m_uiCount, valueStore));
    } else {
        auto &valueStore = m_timeMapStore[m_uiCount];
        valueStore.emplace_back(SValue(tValue, tTime));

    }

    return (m_uiCount++);
}

bool TimeSeriesStore::is_empty() const {
    return m_timeMapStore.empty();
}

void TimeSeriesStore::set_callback_unixTime(TimeSeriesStore::callbackGetTime_t &&fnCallback) {
    m_fnGetUnixTime = std::move(fnCallback);
}

