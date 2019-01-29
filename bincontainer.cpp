#include "bincontainer.h"

BinContainer::BinContainer(const u_char *data, size_t totalSize){
    this->data = data;
    this->totalSize = totalSize;
}

/*BinContainer::~BinContainer(){
    for(auto element: childs){
        delete element;
    }
}*/

void chkBoundary(size_t totalSize, size_t &offset){
    if(totalSize < offset){
        offset = totalSize;
    }
}

BinContainer& BinContainer::mid(size_t offset, size_t length){
    offset += this->current_position;
    chkBoundary(this->totalSize,offset);
    length = std::min(this->totalSize-offset,length);

    std::shared_ptr<BinContainer> result(new BinContainer(this->data+offset,length));
    this->childs.push_back(result);
    return *result;
}
BinContainer& BinContainer::left(size_t length){
    return this->mid(0,length);
}

BinContainer& BinContainer::right(size_t length){
    return this->mid(this->totalSize-length,length);
}

BinContainer& BinContainer::splitLeft(size_t offset){
    return this->left(offset);
}

BinContainer& BinContainer::splitRight(size_t offset){
    return this->right(this->totalSize-offset);
}
std::tuple<BinContainer,BinContainer> BinContainer::split(size_t size){
    //return std::make_tuple(this->Left(size),this->Right(size));
    return std::make_tuple(this->splitLeft(size),this->splitRight(size));
}


void BinContainer::setPosition(size_t absOffset){
    chkBoundary(this->totalSize,absOffset);
    this->current_position = absOffset;
}

void BinContainer::adjAlign(size_t align){
    if(this->current_position % align != 0){
        this->current_position += align - this->current_position % align;
    }
    chkBoundary(this->totalSize,this->current_position);
}

void BinContainer::addPosition(size_t offset){   //
    this->current_position += offset;
    chkBoundary(this->totalSize,this->current_position);
}

void BinContainer::addPosition(size_t offset, size_t align){
    this->adjAlign(align);

    this->current_position += offset;
    chkBoundary(this->totalSize,this->current_position);
}
const u_char* BinContainer::getData(){
    return this->data + this->current_position;
}
size_t BinContainer::getRemainSize(){
    return this->totalSize - this->current_position;
}
std::string BinContainer::getMACAddress(){
    char buf[18];
    for(int i=0;i<5;i++){
        sprintf(buf+i*3,"%02x:",this->data[this->current_position + i]);
    }
    sprintf(buf+15,"%02x",this->data[this->current_position + 5]);
    return buf;

}
