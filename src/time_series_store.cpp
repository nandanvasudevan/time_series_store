//! @file: time_series_store.cpp
//! @brief:
//
// Created by: nandanv
// Created at: 23/12/2021 20:42
//

#include <cassert>
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
std::optional<TimeSeriesStore::value_t>
TimeSeriesStore::get_value(const key_t &uiKey, const unixTime_t &tTimeReference) {

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
		const auto &valueStore = m_timeMapStore.at(uiKey);

		const auto &iResult = std::find_if(valueStore.rbegin(),
		                                   valueStore.rend(),
		                                   [&](const auto &item) {
			                                   if (item.tTime <= tTimeReference) {
				                                   return true;
			                                   }
			                                   return false;
		                                   });

		if (iResult != valueStore.rend()) {
			return iResult->tValue;
		}
	}

	return std::nullopt;
}

//************************************************************
// Created by: nandanv
// Created at: 23-Dec-2021 21:12
// Comments:
//
//************************************************************
size_t
TimeSeriesStore::insert(const key_t &key, const value_t &tValue) {
	const auto tTime = get_time();
	auto store = getStore_forKey(key);

	//*****************************************
	// User: nandanv
	// Date: 25-Dec-2021 19:12
	// Unit: Create a new vector and insert into it.
	//          Always returns 1.
	//*****************************************
	if (!store) {
		auto valueStore = std::vector(1, SValue(std::move(tValue), get_time()));
		valueStore.reserve(m_uiReservation_forVector);
		m_timeMapStore.emplace(std::make_pair(key, valueStore));

		assert(valueStore.size() == 1);
		return valueStore.size();
	}

	//*****************************************
	// User: nandanv
	// Date: 25-Dec-2021 19:12
	// Unit: Insert into an existing vector
	//*****************************************
	{
		store->get().emplace_back(SValue(std::move(tValue), tTime));
		return store->get().size();
	}
}

//************************************************************************************
// Created by: nandanv
// Created at: 25-Dec-2021 23:12
// Comments:
//
//************************************************************************************
bool
TimeSeriesStore::remove(const TimeSeriesStore::key_t &key,
                        const TimeSeriesStore::value_t &value) {
	const auto store = getStore_forKey(key);

	if (!store) {
		return false;
	}

	bool bErased = !!std::erase_if(store->get(), [&](const auto &item) {
		if (item.tValue == value) {
			return true;
		}
		return false;
	});

	return bErased;
}

//************************************************************************************
// Created by: nandanv
// Created at: 25-Dec-2021 23:12
// Comments:
//
//************************************************************************************
size_t
TimeSeriesStore::get_count_for_key(const TimeSeriesStore::key_t &key) {
	const auto store = getStore_forKey(key);

	if (!store) {
		return 0;
	}

	return store->get().size();
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

//************************************************************************************
// Created by: nandanv
// Created at: 25-Dec-2021 23:12
// Comments:
//
//************************************************************************************
bool
TimeSeriesStore::is_empty(const TimeSeriesStore::key_t &key) {

	const auto store = getStore_forKey(key);

	if (!store) {
		return true;
	}

	return store->get().empty();
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

//************************************************************************************
// Created by: nandanv
// Created at: 25-Dec-2021 22:12
// Comments:
//  Returns the store for a given map.
//************************************************************************************
std::optional<TimeSeriesStore::refValueStore_t>
TimeSeriesStore::getStore_forKey(const TimeSeriesStore::key_t &key) {

	if (!m_timeMapStore.contains(key)) {
		return std::optional<refValueStore_t>(std::nullopt);
	}

	return std::optional<refValueStore_t>(
			std::reference_wrapper<valueStore_t>(m_timeMapStore.at(
					key)));
}