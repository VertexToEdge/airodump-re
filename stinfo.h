#ifndef STINFO_H
#define STINFO_H
#include <pch.h>

class STInfo
{
public:
    std::string bssid;
    std::string mac;
    int8_t power;
    u_int32_t frame;
    float station_rate;
    float ap_rate;
    std::string Seq_target;
    u_int32_t Seq_num;
    std::map<std::string,u_int32_t> lastSeq;
    std::list<u_int64_t> lost;
    u_int32_t packets;
    std::vector<std::string> probeESSID;
public:
    STInfo(){
        this->power=0;
        this->station_rate =0;
        this->ap_rate=0;
        this->packets=0;
        this->frame=0;
        this->Seq_num=0;

    }
    u_int32_t cntLostPer10(u_int64_t now){
        u_int32_t result=0;
        for(auto i=lost.begin();i!=lost.end();i++){
            if(now-(*i) < 10000){
                result++;
            }
            else{
                lost.erase(i);
            }
        }
        return result;
    }
};

#endif // STINFO_H
