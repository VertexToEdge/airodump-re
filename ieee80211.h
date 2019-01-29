#ifndef IEEE80211_H
#define IEEE80211_H


#include <radiotap.h>
#include <ieee80211mac.h>

#include <apinfo.h>
#include <stinfo.h>


using namespace std;
class IEEE80211
{
private:
    std::vector<APInfo> ap_infos;
    std::vector<STInfo> st_infos;
public:
    IEEE80211(){

    }

    void analyze(const u_char *data,size_t size, u_int64_t mils);
    IEEE80211(const u_char *data,size_t size, u_int64_t mils){
        this->analyze(data,size,mils);
    }

    std::vector<APInfo>::iterator FindbyInAP(std::function<bool(APInfo)> chkFunc){
        for(auto i=ap_infos.begin();i!=ap_infos.end();i++){
            if(chkFunc(*i)){
                return i;
            }
        }
        return ap_infos.end();
    }
    std::vector<STInfo>::iterator FindbyInST(std::function<bool(STInfo)> chkFunc){
        for(auto i=st_infos.begin();i!=st_infos.end();i++){
            if(chkFunc(*i)){
                return i;
            }
        }
        return st_infos.end();
    }
    std::vector<APInfo> getAPlist(){
        return ap_infos;
    }
    std::vector<STInfo> getSTlist(){
        return st_infos;
    }
    static void printALL(int ch,u_int32_t mils,std::vector<APInfo> aplist,std::vector<STInfo> stlist,std::mutex *m){
        m->lock();

        time_t t = time(0);   // get time now
        tm* now = localtime(&t);

        cout << "\033[2J\033[1;1H";
        cout << endl;
        printf(" CH %d ][ Elapsed: %d s ][ %4d-%02d-%02d %02d:%02d\n",ch,mils/1000,now->tm_year + 1900,now->tm_mon + 1
               ,now->tm_mday,now->tm_hour,now->tm_min);

        printf(" %-19s%-5s%-11s%-8s%-5s%-4s%-5s%-5s%-7s%-5s%-20s\n\n","BSSID","PWR","Beacons","#Data","#/s","CH",
               "MB","ENC","CIPHER","AUTH","ESSID");
        for(auto i:aplist){
            char MB_buf[5];
            sprintf(MB_buf,"%.1f%c",i.mb,i.isExtended?".":"");
            printf(" %-19s%-5d%-11d%-8ld%-5d%-4d%-5s %-7s%-7s%-5s%-20s\n",i.BSSID.c_str(),i.power,i.beacon,i.cntData(),
                   i.cntDataPer10(mils),i.channel,
                   MB_buf,i.encryption.c_str(),i.cipher.c_str(),i.auth.c_str(),i.essid.c_str());
        }
        printf("\n");
        printf(" %-17s %-18s %-5s %-7s %-7s %-7s %-20s\n","BSSID","STATION","PWR","Rate","Lost",
               "Frames","Probe");
        for(auto i:stlist){
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
        m->unlock();

    }
};

#endif // IEEE80211_H
