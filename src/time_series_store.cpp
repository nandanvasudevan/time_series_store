//! @file: time_series_store.cpp
//! @brief:
//
// Created by: nandanv
// Created at: 23/12/2021 20:42
//

#include "time_series_store.hpp"

//************************************************************
// Created by: nandanv
// Created at: 23-Dec-2021 21:12
// Comments:
//
//************************************************************
TimeSeriesStore::TimeSeriesStore(callbackGetTime_t &&fnGetTime) :
		m_uiCount(0),
		m_uiReservation_forVector(0),
		m_fnGetUnixTime(std::move(fnGetTime)) {

}

//************************************************************
// Created by: nandanv
// Created at: 23-Dec-2021 21:12
// Comments:
//
//************************************************************
std::optional<TimeSeriesStore::value_t>
TimeSeriesStore::getValue(const key_t &uiKey, const unixTime_t &tTimeReference) {

	//*****************************************
	// User: nandanv
	// Date: 25-Dec-2021 19:12
	// Unit: Check if key exists in map
	//*****************************************
	if (!m_timeMapStore.contains(uiKey)) {
		return std::nullopt;
	}

	//*****************************************
	// User: nandanv
	// Date: 25-Dec-2021 19:12
	// Unit: Retrieve value from map when it exists
	//*****************************************
	{
		std::optional<value_t> optValue = std::nullopt;
		const auto &valueStore = m_timeMapStore.at(uiKey);

		for (const auto &value: valueStore) {
			if (tTimeReference <= value.tTime) {
				optValue = value.tValue;
			}
		}

		return optValue;
	}

}

//************************************************************
// Created by: nandanv
// Created at: 23-Dec-2021 21:12
// Comments:
//
//************************************************************
size_t
TimeSeriesStore::get_time() {
	return m_fnGetUnixTime();
}

//************************************************************
// Created by: nandanv
// Created at: 23-Dec-2021 21:12
// Comments:
//
//************************************************************
size_t
TimeSeriesStore::insert(const value_t &tValue, const key_t &key) {
	const auto tTime = get_time();

	if (!m_timeMapStore.contains(key)) {
		auto valueStore = std::vector(1, SValue(tValue, get_time()));
		valueStore.reserve(m_uiReservation_forVector);
		m_timeMapStore.emplace(std::make_pair(key, valueStore));
	} else {
		auto &valueStore = m_timeMapStore[key];
		valueStore.emplace_back(SValue(tValue, tTime));

	}

	return (m_uiCount++);
}

//************************************************************
// Created by: nandanv
// Created at: 23-Dec-2021 21:12
// Comments:
//
//************************************************************
bool
TimeSeriesStore::is_empty() const {
	return m_timeMapStore.empty();
}

//************************************************************
// Created by: nandanv
// Created at: 23-Dec-2021 21:12
// Comments:
//
//************************************************************
void
TimeSeriesStore::set_callback_unixTime(TimeSeriesStore::callbackGetTime_t &&fnCallback) {
	m_fnGetUnixTime = std::move(fnCallback);
}

