#ifndef APINFO_H
#define APINFO_H
#include <pch.h>

class APInfo
{
public:
    std::string BSSID;
    int8_t power;
    int8_t rxq;
    uint32_t beacon;
    bool isData;
    std::vector<u_int64_t> data_freq;
    int8_t channel;
    float mb;
    bool isQOS;
    bool isBeacon;
    bool isExtended;
    std::string encryption;
    std::string cipher;
    std::string auth ;
    std::string essid;
public:
    APInfo(){
        this->power = 0;
        this->rxq =0;
        this->beacon =0;
        this->channel =0;
        this->mb =0;
        this->isQOS=0;
        this->isData=0;
        this->isBeacon=0;
        this->isExtended=0;
    }
    size_t cntData(){
        return data_freq.size();
    }
    u_int32_t cntDataPer10(u_int32_t now){
        u_int32_t result=0;
        for(auto i=data_freq.begin();i!=data_freq.end();i++){
            if(now-*i < 10000){
                result++;
            }
            else{
                //data_freq.erase(i);
            }
        }
    }
};

#endif // APINFO_H
