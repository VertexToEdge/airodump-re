#ifndef RADIOTAP_STRUCT_H
#define RADIOTAP_STRUCT_H

#include <stdint.h>

#pragma pack(push,1)
struct radiotap_present{
    bool tsft:1;
    bool flags:1;
    bool rate:1;
    bool channel:1;
    bool fhss:1;
    bool dbm_antenna_signal:1;
    bool dbm_antenna_noise:1;
    bool lock_quality:1;
    bool tx_attention:1;
    bool antenna:1;
    bool db_antenna_signal:1;
    bool db_antenna_noise:1;
    bool rx_flags:1;
    bool padd:3;
    bool channal_:1;
    bool mcs_information:1;
    bool a_mpdu_status:1;
    bool vht_information:1;
    bool frame_timestamp:1;
    bool he_information:1;
    bool he_mu_information:1;
    bool reserved:4;
    bool radiotap_ns_next:1;
    bool vender_ns_next:1;
    bool ext:1;
};
struct radiotap_header{
    uint8_t revision=0;
    uint8_t padding=0;
    uint16_t length=0;
    uint8_t present_length=1;
    radiotap_present *present;
};



//radiotap fields
struct tsft{
    uint64_t mactime;
};

struct flags{
    uint8_t flags;
};


typedef uint8_t rate;

struct channel{
    uint16_t frequency;
    uint16_t flags;
};

struct fhss{
    uint8_t hop_set;
    uint8_t hop_pattern;
};

typedef int8_t antenna_signal;

typedef int8_t antenna_noise;

typedef uint16_t lock_quality;

typedef uint16_t tx_attenuation;

typedef uint16_t db_tx_attenuation;

typedef int8_t dbm_tx_power;

typedef uint8_t antenna;

typedef uint8_t db_antenna_signal;

typedef uint8_t db_antenna_noise;

typedef uint16_t rx_flags;

struct tx_flags{
    uint16_t flags;
};

struct hardware_queue{
    uint8_t queue;
};

struct rssi{
    uint8_t rssi;
    uint8_t max_rssi;
};

struct rts_retries{
    uint8_t retries;
};

struct data_retries{
    uint8_t retries;
};

struct xchannel{
    uint32_t flags;
    uint16_t freq;
    uint8_t channel;
    uint8_t maxpower;
};

struct mcs{
    uint8_t known;
    uint8_t flags;
    uint8_t mcs;
};

struct a_mpdu_status{
    uint32_t reference_number;
    uint16_t flags;
    uint8_t delimiter_crc_value;
    uint8_t reserved;
};

struct vht{
    uint16_t known;
    uint8_t flags;
    uint8_t bandwidth;
    uint8_t mcs_nss[4];
    uint8_t coding;
    uint8_t group_id;
    uint16_t partial_aid;
};

struct timestamp{
    uint64_t timestamp;
    uint16_t accuracy;
    uint8_t unit_position;
    uint8_t flags;
};

struct extended_flags{
    uint32_t flags;
};

struct he{
    uint16_t data1;
    uint16_t data2;
    uint16_t data3;
    uint16_t data4;
    uint16_t data5;
    uint16_t data6;
};

struct he_mu{
    uint16_t flags1;
};

struct he_mu_other_user{
    uint16_t per_user_1;
    uint16_t per_user_2;
};

struct zero_length_psdu{
    uint8_t type;
};

struct l_sig{
    uint16_t data1;
    uint16_t data2;
};

struct radiotap_namespace{
     //no_contents;
};

struct vendor_namespace{
    uint8_t oui[3];
    uint8_t sub_namespace;
    uint16_t skip_length;
};

#pragma pack(pop)


enum ieee80211_radiotap_presence {
    IEEE80211_RADIOTAP_TSFT = 0,
    IEEE80211_RADIOTAP_FLAGS = 1,
    IEEE80211_RADIOTAP_RATE = 2,
    IEEE80211_RADIOTAP_CHANNEL = 3,
    IEEE80211_RADIOTAP_FHSS = 4,
    IEEE80211_RADIOTAP_DBM_ANTSIGNAL = 5,
    IEEE80211_RADIOTAP_DBM_ANTNOISE = 6,
    IEEE80211_RADIOTAP_LOCK_QUALITY = 7,
    IEEE80211_RADIOTAP_TX_ATTENUATION = 8,
    IEEE80211_RADIOTAP_DB_TX_ATTENUATION = 9,
    IEEE80211_RADIOTAP_DBM_TX_POWER = 10,
    IEEE80211_RADIOTAP_ANTENNA = 11,
    IEEE80211_RADIOTAP_DB_ANTSIGNAL = 12,
    IEEE80211_RADIOTAP_DB_ANTNOISE = 13,
    IEEE80211_RADIOTAP_RX_FLAGS = 14,
    IEEE80211_RADIOTAP_TX_FLAGS = 15,
    IEEE80211_RADIOTAP_RTS_RETRIES = 16,
    IEEE80211_RADIOTAP_DATA_RETRIES = 17,
    /* 18 is XChannel, but it's not defined yet */
    IEEE80211_RADIOTAP_MCS = 19,
    IEEE80211_RADIOTAP_AMPDU_STATUS = 20,
    IEEE80211_RADIOTAP_VHT = 21,
    IEEE80211_RADIOTAP_TIMESTAMP = 22,

    /* valid in every it_present bitmap, even vendor namespaces */
    IEEE80211_RADIOTAP_RADIOTAP_NAMESPACE = 29,
    IEEE80211_RADIOTAP_VENDOR_NAMESPACE = 30,
    IEEE80211_RADIOTAP_EXT = 31
};

#endif // RADIOTAP_STRUCT_H
