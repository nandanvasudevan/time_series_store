//
// Created by nandanv on 17/12/2021.
//

#include <cstdint>
#include <cstddef>
#include <functional>
#include <string>
#include <map>
#include <vector>

#ifndef TIME_BASED_KEYVALUE_STORE_TIME_SERIES_STORE_HPP
#define TIME_BASED_KEYVALUE_STORE_TIME_SERIES_STORE_HPP

class TimeSeriesStore {

public:
    using key_t = size_t;
    using value_t = std::string;
    using callbackGetTime_t = std::function<size_t(void)>;
    using unixTime_t = size_t;

    explicit TimeSeriesStore(callbackGetTime_t &&fnGetTime);

    void set_callback_unixTime(callbackGetTime_t &&fnCallback);

    std::optional<value_t> getValue(const key_t &uiKey, const unixTime_t &tTimeReference);

    size_t get_time();

    size_t insert(const value_t &tValue);

    [[nodiscard]] bool is_empty() const;

private:
    typedef struct SValue {
        value_t tValue;
        unixTime_t tTime;

        SValue(const value_t &value, const unixTime_t &tInsertionTime) :
                tValue(value),
                tTime(tInsertionTime) {}

    } SValue;

    size_t m_uiCount;

    size_t m_uiReservation_forVector;

    std::function<size_t(void)> m_fnGetUnixTime;

    std::map<key_t , std::vector<SValue>> m_timeMapStore;
};

#endif //TIME_BASED_KEYVALUE_STORE_TIME_SERIES_STORE_HPP
