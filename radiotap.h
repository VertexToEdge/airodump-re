#ifndef RADIOTAP_H
#define RADIOTAP_H
#include <pch.h>

#include <radiotap_struct.h>
#include <customanalyzer.h>

struct rtap_namespace_info{
    size_t align;
    size_t size;
};

typedef std::map<u_int32_t,rtap_namespace_info> typeRadiotapFieldsInfo;
static typeRadiotapFieldsInfo RadiotapFieldsInfo={{IEEE80211_RADIOTAP_TSFT, {8, 8}},
                                           {IEEE80211_RADIOTAP_FLAGS, {1, 1}},
                                           {IEEE80211_RADIOTAP_RATE, {1, 1}},
                                           {IEEE80211_RADIOTAP_CHANNEL, {2, 4}},
                                           {IEEE80211_RADIOTAP_FHSS, {2, 2}},
                                           {IEEE80211_RADIOTAP_DBM_ANTSIGNAL, {1, 1}},
                                           {IEEE80211_RADIOTAP_DBM_ANTNOISE, {1, 1}},
                                           {IEEE80211_RADIOTAP_LOCK_QUALITY, {2, 2}},
                                           {IEEE80211_RADIOTAP_TX_ATTENUATION, {2, 2}},
                                           {IEEE80211_RADIOTAP_DB_TX_ATTENUATION, {2, 2}},
                                           {IEEE80211_RADIOTAP_DBM_TX_POWER, {1, 1}},
                                           {IEEE80211_RADIOTAP_ANTENNA, {1, 1}},
                                           {IEEE80211_RADIOTAP_DB_ANTSIGNAL, {1, 1}},
                                           {IEEE80211_RADIOTAP_DB_ANTNOISE, {1, 1}},
                                           {IEEE80211_RADIOTAP_RX_FLAGS, {2, 2}},
                                           {IEEE80211_RADIOTAP_TX_FLAGS, {2, 2}},
                                           {IEEE80211_RADIOTAP_RTS_RETRIES, {1, 1}},
                                           {IEEE80211_RADIOTAP_DATA_RETRIES, {1, 1}},
                                           {IEEE80211_RADIOTAP_MCS, {1, 3}},
                                           {IEEE80211_RADIOTAP_AMPDU_STATUS, {4, 8}},
                                           {IEEE80211_RADIOTAP_VHT, {2, 12}},
                                           {IEEE80211_RADIOTAP_TIMESTAMP, {8, 12}}};
/*
typedef std::map<std::string,std::any> callbackResult;

typedef std::shared_ptr<callbackResult> pCallbackResult;
typedef std::function<void(BinContainer,callbackResult&)> funcCallback;

template<typename codeType,typename FuncType,typename ResultType>

*/
typedef u_int32_t radiotap_callback_code;
typedef std::map<std::string,std::any> radiotap_callback_reseult;
typedef std::function<void(BinContainer,radiotap_callback_reseult&)> radiotap_callback_func;


class Radiotap:public CustomAnalyzer<radiotap_callback_code,radiotap_callback_func,radiotap_callback_reseult>
{
private:
    void init();
    size_t size=0;
public:
    Radiotap();
    Radiotap(BinContainer &data);
    void analyze(BinContainer &data);
    size_t getSize(){
        return size;
    };
};

#endif // RADIOTAP_H
