#include <pch.h>

#ifndef BINCONTAINER_H
#define BINCONTAINER_H


class BinContainer
{
private:
    const u_char *data;
    size_t totalSize;
    size_t current_position=0;
    std::vector<std::shared_ptr<BinContainer>> childs;
public:
    BinContainer(const u_char *data, size_t totalSize);
    //~BinContainer();

    BinContainer& mid(size_t offset, size_t length);
    BinContainer& left(size_t length);
    BinContainer& right(size_t length);
    BinContainer& splitLeft(size_t offset);
    BinContainer& splitRight(size_t offset);
    std::tuple<BinContainer,BinContainer> split(size_t offset);

    const u_char* getData();
    size_t getRemainSize();

    void setPosition(size_t absOffset);
    void addPosition(size_t offset);
    void adjAlign(size_t align);
    void addPosition(size_t offset, size_t align);

    template<typename T>
    void copyTo(T &destination);

    std::string getMACAddress();
    std::string toString(){  // string constructor( char *, size)
       return std::string((char *)this->data+this->current_position,this->getRemainSize());
    }
};

inline bool operator==(BinContainer &lhs, BinContainer &rhs){
    size_t thisRemainSize = lhs.getRemainSize();
    size_t otherRemainSize = rhs.getRemainSize();
    if (thisRemainSize != otherRemainSize){
        return false;
    }
    else{
        return 0 == memcmp((void *)lhs.getData(),(void *)rhs.getData(), thisRemainSize);
    }
}

/*template<typename T>
inline bool operator==(BinContainer &rhs,const T &other){
    size_t thisRemainSize = rhs.getRemainSize();
    size_t otherRemainSize = sizeof(T);
    if(thisRemainSize < otherRemainSize){
        return false;
    }
    else{
        return memcmp((void *)rhs.getData(),(void *)&other, otherRemainSize);
    }
}*/
inline bool operator==(BinContainer &lhs,const int &rhs){
    size_t thisRemainSize = lhs.getRemainSize();
    size_t otherRemainSize = sizeof(int);
    if(thisRemainSize < otherRemainSize){
        return false;
    }
    else{
        return memcmp((void *)lhs.getData(),(void *)&rhs, otherRemainSize);
    }
}
inline bool operator==(int &lhs,BinContainer &rhs){
    size_t thisRemainSize = rhs.getRemainSize();
    size_t otherRemainSize = sizeof(int);
    if(thisRemainSize < otherRemainSize){
        return false;
    }
    else{
        return memcmp((void *)rhs.getData(),(void *)&lhs, otherRemainSize);
    }
}

template<typename T>
void BinContainer::copyTo(T &destination){
    memcpy(&destination, data,sizeof(T));
}

#endif // BINCONTAINER_H
