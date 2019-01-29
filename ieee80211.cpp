#include "ieee80211.h"


template <typename T>
void pushIfNotEmpty(T &d, T &s){
    if(s!=0){
        d=s;
    }
}
template <typename T>
void addIfNotEmpty(T &d, T &s){
    if(s!=0){
        d+=s;
    }
}
void IEEE80211::analyze(const u_char *data,size_t size,u_int64_t mil_sec)
{
    //u_int32_t mil_sec = t.tv_sec*1000 + t.tv_usec*1000 ;
    BinContainer PacketData(data,size);

    Radiotap radiotap(PacketData);
    PacketData.setPosition(0);
    float rate=0;
    int8_t power=-1;

    try {
        rate = std::any_cast<float>( radiotap.getCallbackResult(IEEE80211_RADIOTAP_RATE)["Rate"]);
    } catch (std::bad_any_cast) {

    }



    try{
        power = std::any_cast<int8_t>( radiotap.getCallbackResult(IEEE80211_RADIOTAP_DBM_ANTSIGNAL)["Power"]);
    } catch (std::bad_any_cast) {

    }

    IEEE80211_MAC mac(PacketData.splitRight(radiotap.getSize()), rate,power,mil_sec);

    APInfo resultAP = mac.getAPInfo();
    STInfo resultST = mac.getSTInfo();

    auto i = this->FindbyInAP([&](APInfo a)->bool{return a.BSSID==resultAP.BSSID;});

    if(i==this->ap_infos.end()){
        if(resultAP.isBeacon)
            ap_infos.push_back(resultAP);
    }
    else{
        pushIfNotEmpty(i->power,resultAP.power);
        addIfNotEmpty(i->beacon,resultAP.beacon);
        if(resultAP.isData){
            i->data_freq.push_back(mil_sec);
        }
    }


    auto s = this->FindbyInST([&](STInfo a)->bool{return a.mac==resultST.mac;});

    if(s==this->st_infos.end()){
        if(resultST.mac != "")
            st_infos.push_back(resultST);
    }
    else{
        pushIfNotEmpty(s->power,resultST.power);
        pushIfNotEmpty(s->ap_rate,resultST.ap_rate);
        pushIfNotEmpty(s->station_rate,resultST.station_rate);
        addIfNotEmpty(s->frame,resultST.frame);
        if (resultST.Seq_target != ""){
            if(s != st_infos.end()){
                if(s->lastSeq.find(resultST.Seq_target) != s->lastSeq.end())
                {
                    auto originSeq = s->lastSeq[resultST.Seq_target];

                    if(originSeq > resultST.Seq_num){
                        for(int i=0;i<4095-originSeq+resultST.Seq_num;i++){
                            s->lost.push_back(mil_sec);
                        }
                        s->lastSeq[resultST.Seq_target] = resultST.Seq_num;
                    }
                    else{
                        for(int i=0;i<resultST.Seq_num-originSeq-1;i++){
                            s->lost.push_back(mil_sec);
                        }
                        s->lastSeq[resultST.Seq_target] = resultST.Seq_num;
                    }
                }
                else{
                    s->lastSeq[resultST.Seq_target] = resultST.Seq_num;
                }
            }
        }
    }

}


using namespace std;
//      17             3     9          8      5    4   5   5      7     5     5+@
// BSSID            11PWR11Beacons  11#Data,11#/s11CH11MB 11ENC  CIPHER AUTH ESSID
//      16      " "       18         " "  5 " "   7 " "  7 " "   7  " "  20
// BSSID              STATION            PWR   Rate    Lost    Frames  Probe
/*void IEEE80211::printALL(int ch,u_int32_t elapsed,u_int32_t mils){

    time_t t = time(0);   // get time now
    tm* now = localtime(&t);

    cout << "\033[2J\033[1;1H";
    cout << endl;
    printf(" CH %d ][ Elapsed: %d s ][ %4d-%02d-%02d %02d:%02d\n",ch,elapsed,now->tm_year + 1900,now->tm_mon + 1
           ,now->tm_mday,now->tm_hour,now->tm_min);

    printf(" %-19s%-5s%-11s%-8s%-5s%-4s%-5s%-5s%-7s%-5s%-20s\n\n","BSSID","PWR","Beacons","#Data","#/s","CH",
           "MB","ENC","CIPHER","AUTH","ESSID");
    for(auto i:this->getAPlist()){
        char MB_buf[5];
        sprintf(MB_buf,"%.1f%c",i.mb,i.isExtended?".":"");
        printf(" %-19s%-5d%-11d%-8ld%-5d%-4d%-5s %-7s%-7s%-5s%-20s\n",i.BSSID.c_str(),i.power,i.beacon,i.cntData(),
               i.cntDataPer10(mils),i.channel,
               MB_buf,i.encryption.c_str(),i.cipher.c_str(),i.auth.c_str(),i.essid.c_str());
    }
    printf("\n");
    printf(" %-17s %-18s %-5s %-7s %-7s %-7s %-20s\n","BSSID","STATION","PWR","Rate","Lost",
           "Frames","Probe");
    for(auto i:this->getSTlist()){
        std::string buf;
        int cnt=0;
        for(auto j:i.probeESSID){
            if(++cnt==3) break;
            buf += j;
        }

        //sprintf(MB_buf,"%.1f%c",i.mb,i.isExtended?".":"");
        printf(" %-17s %-18s %-5d %2.0f-%-4.0f %-7d %-7ld %-20s\n",i.bssid.c_str(),i.mac.c_str(),i.power,i.station_rate,i.ap_rate,i.cntLostPer10(mils),
               i.frame,buf.c_str());

    }


}*/
