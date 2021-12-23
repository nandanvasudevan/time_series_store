//! @file: time_series_store.hpp
//! @brief:
//
// Created by: nandanv
// Created at: 23/12/2021 20:42
//


#include <cstdint>
#include <cstddef>
#include <functional>
#include <string>
#include <map>
#include <optional>
#include <vector>

#ifndef TIME_BASED_KEYVALUE_STORE_TIME_SERIES_STORE_HPP
#define TIME_BASED_KEYVALUE_STORE_TIME_SERIES_STORE_HPP

class TimeSeriesStore {

public:
	using key_t = size_t;
	using value_t = std::string;
	using callbackGetTime_t = std::function<size_t(void)>;
	using unixTime_t = size_t;

	/**
	 * @brief Constructor
	 * @param fnGetTime Function to get current time
	 */
	explicit TimeSeriesStore(callbackGetTime_t &&fnGetTime);

	//! Modifies the callback to get current time
	void set_callback_unixTime(callbackGetTime_t &&fnCallback);

	/**
	 * @brief Gets the value specified by key and time
	 * @param uiKey Key associated with required value
	 * @param tTimeReference Latest value before this time instance
	 * @return Value if found, else std::nullopt
	 */
	std::optional<value_t> getValue(const key_t &uiKey, const unixTime_t &tTimeReference);

	//! Gets the latest time
	size_t get_time();

	/**
	 * @brief Insets a value into the next key
	 * @param tValue Value to be inserted
	 * @return Key used for insertion
	 */
	size_t insert(const value_t &tValue);

	//! @retval true Indicates the store is empty
	[[nodiscard]] bool is_empty() const;

private:
	//! Holds a value and it's time of insertion
	typedef struct SValue {
		value_t tValue;
		unixTime_t tTime;

		//! Create a new 'value'
		SValue(const value_t &value, const unixTime_t &tInsertionTime) :
				tValue(value),
				tTime(tInsertionTime) {}
	} SValue;

	//! Count of elements in the store
	size_t m_uiCount;

	//! Default reservation size for new store element
	size_t m_uiReservation_forVector;

	//! Functor to get the latest unix time
	std::function<size_t(void)> m_fnGetUnixTime;

	//! The store
	std::map<key_t, std::vector<SValue>> m_timeMapStore;
};

#endif //TIME_BASED_KEYVALUE_STORE_TIME_SERIES_STORE_HPP
