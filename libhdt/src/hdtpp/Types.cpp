#include "BitmapTriplespp.hpp"


namespace hdt {

Types::Types(ALT *t){
    altypes = t;
    altypes->select1support();
    //altypes->select10support();
    altypes->rank0support();

}

bool Types::hasTypes(size_t familyID){
    family = familyID;
    return altypes->hasElements(familyID);
    //return (!altypes->getBit(altypes->select1(familyID)+1));
    //TODO
    return 0;
}

void Types::getTypes(size_t familyID){
    family = familyID;
    altypes->traverse(familyID);
}

bool Types::findType(size_t familyID, size_t type){
    family = familyID;
    return altypes->adj(familyID, type);
}

void Types::getTypes(){
    altypes->traverse();
}


bool Types::hasMoreTypes(){
    return altypes->hasMoreElements();
}

size_t Types::nextType(){
    return altypes->nextElement();
}


size_t Types::getSizeTypes(){
    return altypes->seqsize();
}

size_t Types::getCurrentFamily(){
    return altypes->getCont();
}

size_t Types::getNTypedFamilies(){
    size_t n = 0;
    rank_support_v<01,2> b_r01(altypes->getBits());
    n = b_r01(altypes->size());
    util::clear(b_r01);
    return n;
}

size_t Types::access(size_t pos){
    return altypes->access(pos);
}
size_t Types::getBit(size_t pos){
    return altypes->getBit(pos);
}

unsigned int Types::sizeBytes(){
    return altypes->sizeBytes();
}

Types::~Types(){
    if (altypes){
        delete altypes;
        altypes = NULL;
    }
}


}
