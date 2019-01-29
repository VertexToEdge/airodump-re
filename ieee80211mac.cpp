#include "ieee80211mac.h"


//type and subtype to int
#define convTST(a,b) (a<<4|b)


IEEE80211_MAC::IEEE80211_MAC()
{

}

IEEE80211_MAC::IEEE80211_MAC(BinContainer &data,float rate,int8_t power,u_int32_t milsec ){
    this->stInfo = {};
    this->apInfo = {};
    this->analyze(data,rate,power,milsec);
}
/*
#define IE_ESSID 0
#define IE_SUPPORTED_RATE 1
#define IE_CURRENT_CHANNEL 3
#define IE_EXTENDED_SUPPORTED_RATE 50
#define IE_RSN 48
 */
void analyzeESSID(BinContainer data, std::any &result){
    dot11_callback_parameter param = std::any_cast<dot11_callback_parameter>(result);
    APInfo &apInfo = std::get<0>(param);
    if(data.getRemainSize()){
        apInfo.essid = data.toString();
    }
    else{
        apInfo.essid = "<length:  0>";
    }

}
void analyzeSurportedRate(BinContainer data, std::any &result){
    dot11_callback_parameter param = std::any_cast<dot11_callback_parameter>(result);
    APInfo &apInfo = std::get<0>(param);
    for(int i=0;i<data.getRemainSize();i++){
        float rate =(float)( ((u_int8_t *)data.getData())[i]&0b01111111) /2;
        apInfo.mb = std::max(apInfo.mb,rate);
    }
}
void analyzeExtendedSurportedRate(BinContainer data, std::any &result){
    dot11_callback_parameter param = std::any_cast<dot11_callback_parameter>(result);
    APInfo &apInfo = std::get<0>(param);
    for(int i=0;i<data.getRemainSize();i++){
        float rate =(float)( ((u_int8_t *)data.getData())[i]) /2;
        apInfo.mb = std::max(apInfo.mb,rate);
    }
    apInfo.isExtended = true;
}
void analyzeChannel(BinContainer data, std::any &result){
    dot11_callback_parameter param = std::any_cast<dot11_callback_parameter>(result);
    APInfo &apInfo = std::get<0>(param);
    apInfo.channel = ((u_int8_t *)data.getData())[0];
}

void analyzeRSN(BinContainer data, std::any &result){
    dot11_callback_parameter param = std::any_cast<dot11_callback_parameter>(result);
    APInfo &apInfo = std::get<0>(param);

    apInfo.encryption = "WPA2";

    data.addPosition(6);

    //determine Cipher
    u_int16_t cipherCnt =((u_int16_t *)data.getData())[0];  //parse Cipher gaesu
    data.addPosition(2);
    for(int i=0;i<cipherCnt;i++){
        data.addPosition(3);
        u_int8_t cipherCode = ((u_int8_t *)data.getData())[0];
        switch(cipherCode){
        case 1:
            apInfo.cipher = "WEP";
            break;
        case 2:
            apInfo.cipher = "TKIP";
            apInfo.encryption = "WPA";
            break;
        case 3:
            apInfo.cipher = "WRAP";
            break;
        case 4:
            apInfo.cipher = "CCMP";
            break;
        case 5:
            apInfo.cipher = "WEP104";
            break;
        case 9:
            apInfo.cipher = "GCMP";
            break;
        default:
            break;
        }
        data.addPosition(1);
    }


    u_int16_t authCnt = ((u_int16_t *)data.getData())[0];  //parse AUTH gaesu
    data.addPosition(2);
    for(int i=0;i<authCnt;i++){
        data.addPosition(3);
        u_int8_t authCode = ((u_int8_t *)data.getData())[0];
        switch(authCode){
        case 1:
            apInfo.auth = "MGT";
            break;
        case 2:
            apInfo.auth = "PSK";
            break;
        default:
            break;
        }
        data.addPosition(1);
    }
}

void IEEE80211_MAC::caInit(){
    ieAnalyzer.registerCallback(IE_ESSID,analyzeESSID);
    ieAnalyzer.registerCallback(IE_SUPPORTED_RATE,analyzeSurportedRate);
    ieAnalyzer.registerCallback(IE_EXTENDED_SUPPORTED_RATE,analyzeExtendedSurportedRate);
    ieAnalyzer.registerCallback(IE_RSN,analyzeRSN);
    ieAnalyzer.registerCallback(IE_CURRENT_CHANNEL,analyzeChannel);
}

void IEEE80211_MAC::analyze(BinContainer &data,float rate,int8_t power,u_int32_t milsec){
    this->caInit();

    //memset(this->apInfo,-1,sizeof(APInfo));  //initialize Information Struct
    //memset(this->stInfo,-1,sizeof(STInfo));

    struct80211Header header;
    //= (struct80211Header *)data.getData();
    memcpy(&header,data.getData(),sizeof(struct80211Header));

    switch (header.Type) {
    case 0:{ //Management Frame
        switch(header.Subtype){
        case BEACON:{
            this->apInfo.beacon = 1;
            this->apInfo.isBeacon=true;
            data.addPosition(16);

            this->apInfo.BSSID = data.getMACAddress();
            data.addPosition(8);    //end of MAC header

            BinContainer TagData = data.splitRight(12);//skip timestamp and beacon interval
            data.addPosition(10);
            structCapability *cap = (structCapability*)data.getData();

            if(cap->privacy == 0){
                apInfo.encryption = "OPEN";
            }
            else{
                apInfo.encryption = "WEP";
            }

            while(TagData.getRemainSize() > 0){
                structIE *ie = (structIE *)TagData.getData();
                TagData.addPosition(2);

                std::tuple<APInfo&,STInfo&> result(apInfo,stInfo);
                std::any AnyWrapper = result;
                this->ieAnalyzer.callCallback(ie->tagID,TagData.left(ie->size),AnyWrapper);

                TagData.addPosition(ie->size);
            }
            apInfo.power = power;
            break;
        }
        case AUTH_FRAME:{
            data.addPosition(4);
            std::string stationMAC = data.getMACAddress();
            data.addPosition(12);
            std::string apMAC = data.getMACAddress();
            data.addPosition(8);
            structAuth *auth = (structAuth*) data.getData();

            if(auth->auth_seq==2 && auth->auth_status==0){
                stInfo.mac = stationMAC;
                stInfo.bssid = apMAC;

            }
            break;
        }
        case PROBE_REQUEST:{
            data.addPosition(10);
            std::string stationMAC = data.getMACAddress();
            data.addPosition(14);
            BinContainer TagData = data.splitRight(0);
            stInfo.mac = stationMAC;
            stInfo.power = power;
            APInfo dummyAP;

            while(TagData.getRemainSize() > 0){
                structIE *ie = (structIE *)TagData.getData();
                TagData.addPosition(2);

                std::tuple<APInfo&,STInfo&> result(dummyAP,stInfo);
                std::any AnyWrapper = result;
                this->ieAnalyzer.callCallback(ie->tagID,TagData.left(ie->size),AnyWrapper);

                TagData.addPosition(ie->size);
            }
            stInfo.probeESSID.push_back(dummyAP.essid);
            break;
        }

        default:
        break;
    }
        break;
    }
    case 1:{  //Control Framdot11_callback_funce
        break;
        }
    case 2:{  //Data Frame

        if(header.ToDS==0 && header.FromDS==1){
            stInfo.ap_rate = rate;
            stInfo.power = power;
            structData *d = (structData*) data.getData();
            stInfo.mac = d->getSource();
            //stInfo.lastSeq[d->getTransmitter()] = d->getSeqNum();
            stInfo.Seq_target = d->getTransmitter();
            stInfo.Seq_num = d->getSeqNum();

            apInfo.BSSID = d->getTransmitter();
            //apInfo.data_freq.push_back(milsec);
            apInfo.isData = true;
        }
        else if(header.ToDS==1 && header.FromDS==0){
            structData *d = (structData*) data.getData();
            stInfo.mac = d->getTransmitter();
            stInfo.station_rate = rate;
            stInfo.power = power;
            stInfo.frame =1;
        }

        break;
    }
    default:
        break;
    }
}
