#include "radiotap.h"

void analyzeDBAntennaSignal(BinContainer data, std::map<std::string,std::any> &result){
    result["Power"] = (int8_t)(data.getData()[0]);
}
void analyzeDBMAntennaSignal(BinContainer data, std::map<std::string,std::any> &result){
    result["Power"] = (int8_t)(data.getData()[0]);
}
void analyzeRate(BinContainer data, std::map<std::string,std::any> &result){
    result["Rate"] = (float)((float)(int8_t)(data.getData()[0]))/2;
}

void Radiotap::init(){
    this->registerCallback(IEEE80211_RADIOTAP_DB_ANTSIGNAL,analyzeDBAntennaSignal);
    this->registerCallback(IEEE80211_RADIOTAP_DBM_ANTSIGNAL,analyzeDBMAntennaSignal);
    this->registerCallback(IEEE80211_RADIOTAP_RATE,analyzeRate);

}

Radiotap::Radiotap(BinContainer &data)
{
    this->analyze(data);
}
Radiotap::Radiotap()
{

}

void Radiotap::analyze(BinContainer &data){
    this->init();
    radiotap_header *radiotapHeader = (radiotap_header *)data.getData();
    this->size = radiotapHeader->length;
    data.addPosition(4);
    u_int32_t presents=0;
    memcpy(&presents,data.getData(),sizeof(radiotap_present));


    std::bitset<32> present_bits(presents);
    while(present_bits.test(0)){
        data.addPosition(4);

        u_int32_t tmpPresent;  // convert 4byte present to u_int32_t present
        memcpy(&tmpPresent,data.getData(),sizeof(radiotap_present));
        std::bitset<32> tmpPresent_bit(tmpPresent);

        for(size_t i=0;i<31;i++){  //collect present flag
            if(tmpPresent_bit.test(i)){
                present_bits.set(i);      //to here
            }
        }

        if(tmpPresent_bit.test(31)){   //set ext flag bit
            present_bits.set(31);
        }
        else{
            present_bits.reset(31);
        }

    }
    data.addPosition(4); //move flag to next present
    BinContainer dataPresents = data.splitRight(0);

    for(u_int32_t i=0;i<31;i++){  //check presents
        if(present_bits.test(i)){
            rtap_namespace_info InfoSize = RadiotapFieldsInfo[i];
            dataPresents.adjAlign(InfoSize.align);
            this->callCallback((unsigned int)i,dataPresents.left(InfoSize.size));
            dataPresents.addPosition(InfoSize.size);
        }
    }
}
