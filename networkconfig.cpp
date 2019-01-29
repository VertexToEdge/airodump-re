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
