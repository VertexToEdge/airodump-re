#ifndef IEEE80211MAC_H
#define IEEE80211MAC_H
#include <pch.h>

#include <customanalyzer.h>
#include <bincontainer.h>

#include <apinfo.h>
#include <stinfo.h>

#define MANAGE_FRAME 0b00

#define ASSOCIATION_REQUEST 0b0000
#define RE_ASSOCIATION_REQUEST 0b0010
#define ASSOCIATION_RESPONSE 0b0011
#define PROBE_REQUEST 0b0100
#define PROBE_RESPONSE 0b0101
#define BEACON 0b1000
#define ATIM 0b1001
#define DEAUTH 0b1010
#define AUTH_FRAME 0b1011

#define CONTROL_FRAME 0b01

#define DATA_FRAME 0b10


#define IE_ESSID 0
#define IE_SUPPORTED_RATE 1
#define IE_CURRENT_CHANNEL 3
#define IE_EXTENDED_SUPPORTED_RATE 50
#define IE_RSN 48


struct struct80211Header{
    u_int8_t ProtocolVer:2;
    u_int8_t Type:2;
    u_int8_t Subtype:4;
    bool ToDS:1;
    bool FromDS:1;
    bool moreFrags:1;
    bool retry:1;
    bool powerMGT:1;
    bool moreData:1;
    bool protectedFlag:1;
    bool orderFlag:1;
};

struct structCapability{
    bool ESS:1;
    bool IBSS:1;
    u_int8_t CFP:2;
    bool privacy:1;
};

struct structIE{
    u_int8_t tagID;
    u_int8_t size;
};

struct structAuth{
    u_int16_t auth_algorithm;
    u_int16_t auth_seq;
    u_int16_t auth_status;
};

class structData{
private:

    u_int32_t padd;
    u_char destination[6];
    u_char transmitter[6];
    u_char source[6];
    u_int8_t frag_num;
    u_int32_t seq_num:12;

public:

    std::string toMAC(u_char* mac){  //must be use immediately
        u_char test[19]={0,};
        sprintf((char*)test,"%02x:%02x:%02x:%02x:%02x:%02x",mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);
        std::string result((char *)test,18);
        return result;
    }
    std::string getDestination(){
        return toMAC(this->destination);
    }
    std::string getTransmitter(){
        return toMAC(this->transmitter);
    }
    std::string getSource(){
        return toMAC(this->source);
    }
    u_int32_t getSeqNum(){
        return seq_num;
    }
};

//typedef std::tuple<std::string,APInfo> tupleAPInfo;
//typedef std::tuple<std::string,STInfo> tupleSTInfo;
typedef int16_t dot11_callback_code;
typedef std::tuple<APInfo&,STInfo&> dot11_callback_parameter;

typedef std::any dot11_callback_result;
typedef std::function<void(BinContainer,std::any&)> dot11_callback_func;

class IEEE80211_MAC
{
private:
    APInfo apInfo;
    STInfo stInfo;
    CustomAnalyzer<dot11_callback_code,dot11_callback_func,dot11_callback_result> ieAnalyzer;
    void caInit();
public:
    IEEE80211_MAC();
    IEEE80211_MAC(BinContainer &data, float rate,int8_t power,u_int32_t milsec);
    void analyze(BinContainer &data, float rate,int8_t power,u_int32_t milsec);

    APInfo getAPInfo(){
        return apInfo;
    }
    STInfo getSTInfo(){
        return stInfo;
    }
};

#endif // IEEE80211MAC_H
