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
    using value_t = std::string;
    using callbackGetTime_t = std::function<size_t(void)>;
    using unixTime_t = size_t;

    TimeSeriesStore(callbackGetTime_t fnGetTime);

    void set_callback_unixTime(callbackGetTime_t fnCallback);

    std::optional<value_t> getValue(const size_t uiIndex, const unixTime_t tTimeReference);

    size_t get_time();

    size_t insert(value_t tValue);

    bool is_empty(void);

private:
    typedef struct _SValue{
        value_t tValue;
        unixTime_t tTime;

        _SValue(const value_t value, const unixTime_t tInsertionTime):
            tValue(value),
            tTime(tInsertionTime)
        {}

    } SValue;

    size_t m_uiCount;

    size_t uiReservation_forVector;

    std::function<size_t(void)> fnGetUnixTime;

    std::map<int, std::vector<SValue>> m_timeMapStore;
};

#endif //TIME_BASED_KEYVALUE_STORE_TIME_SERIES_STORE_HPP
