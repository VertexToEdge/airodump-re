#include <pch.h>
#include <bincontainer.h>

#include <ieee80211.h>
#include <net/if.h>
#include <netlink/netlink.h>
#include <netlink/genl/genl.h>
#include <netlink/genl/ctrl.h>
#include <linux/nl80211.h>

bool changeChannel(char *device,int channel){

    int frequencyMhz = 2412 + (channel-1) *5;


    struct nl_sock *sckt = nl_socket_alloc();
    genl_connect(sckt);

    struct nl_msg *mesg = nlmsg_alloc();

    enum nl80211_commands command = NL80211_CMD_SET_WIPHY;

    genlmsg_put(mesg, 0, 0, genl_ctrl_resolve(sckt, "nl80211"), 0, 0, command, 0);

    NLA_PUT_U32(mesg, NL80211_ATTR_IFINDEX, if_nametoindex(device));
    NLA_PUT_U32(mesg, NL80211_ATTR_WIPHY_FREQ, frequencyMhz);

    nl_send_auto_complete(sckt, mesg);
    //printf("%d Bytes Sent\n", ret);

    nlmsg_free(mesg);
    return true;

    nla_put_failure:
        nlmsg_free(mesg);
        //printf("PUT Failure\n");
        return false;
}


/*void printA(){
    std::cout << "A Fucntion" << std::endl;
}
void printB(){
    std::cout << "B Fucntion" << std::endl;
}


void callcallback(std::function<void (void)> f){
    f();
}*/

u_int64_t toMills(timeval t){
    return t.tv_sec*1000 + t.tv_usec*1000;
}

int main(int argc,char *argv[])
{
    /*
    u_char k[] = {0,0,0,'a'};
    //u_int j = 97;

    BinContainer testBin(k,4);
    int a =97;
    //BinContainer a((u_char *)&j,4);
    if(a==testBin)
        std::cout << "Hello World!" << std::endl;

    int d=10;
    auto modVar = [&d]() -> void {d=20;};
     */

    //char device[100] = "wlan3";

    char* dev = argv[1];
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t* handle = pcap_open_live(dev, BUFSIZ, 1, 1000, errbuf);
    if (handle == NULL) {
        printf("couldn't open device %s: %s\n", dev, errbuf);
        return -1;
    }
    IEEE80211 frame;
    std::chrono::system_clock::time_point Start = std::chrono::system_clock::now();
    std::chrono::system_clock::time_point lastPrint;
    std::chrono::system_clock::time_point lastHop;
    int ch=1;

    std::mutex m;

    while (true) {
        struct pcap_pkthdr* header;
        const u_char* packet;
        pcap_next_ex(handle, &header, &packet);
        std::chrono::system_clock::time_point now = std::chrono::system_clock::now();

        std::chrono::milliseconds printTime  = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastPrint);
        std::chrono::milliseconds hopTime  = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastHop);
        std::chrono::milliseconds elapsed  = std::chrono::duration_cast<std::chrono::milliseconds>(now - Start);

        if(hopTime.count() >300){
            ch += 4;
            ch =ch %14 +1;
            changeChannel(argv[1],ch);
        }
        if(printTime.count() > 300){
            lastPrint = now;

            auto fut= std::async(std::launch::async,frame.printALL,ch,elapsed.count(),frame.getAPlist(),frame.getSTlist(),&m);
            //frame.printALL(ch,elapsed.count()/1000,elapsed.count());
        }else{
            m.lock();
            frame.analyze(packet,header->len,elapsed.count());
            m.unlock();
        }



    }

    getchar();
    return 0;
}
