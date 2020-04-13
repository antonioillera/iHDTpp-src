#include "ALCV.hpp"

ALCV::ALCV(vector < enc_vector<CODER, TDENS> *> *v):
    al(v){

}

enc_vector<CODER, TDENS>* ALCV::neigh(size_t v) {
    return al->at(v);
}

void ALCV::traverse(size_t v) {
    ptr0 = neigh(v)->begin();
    ptr1 = neigh(v)->end();
}

bool ALCV::hasMoreElements() {
    return ptr0 != ptr1;
}

unsigned int ALCV::nextElement(){
    return *ptr0++;
}

unsigned int ALCV::currentElement(){
    return *ptr0;
}

unsigned int ALCV::adj(size_t v, size_t u){
    size_t pos0, pos1;
    size_t tam = (*neigh(v)).size();
    size_t lastBlock = (tam/TDENS)-1;
    if (tam%TDENS > 0) lastBlock++;

    if ((*neigh(v))[lastBlock*TDENS] <= u){
        pos0 = lastBlock*TDENS;
        pos1 = tam;
    } else {
        size_t block0 = 0, block1 = lastBlock, c;
        while(block1 - block0 > 1){
           c = (block1+block0)/2;
           if (u < (*neigh(v))[c*TDENS]) block1=c;
           else block0 = c;
        }
        pos0=block0*TDENS;
        pos1=block1*TDENS;
    }

    size_t cont = pos0;
    for (random_access_const_iterator<enc_vector<CODER, TDENS>> it = neigh(v)->begin()+pos0; it != neigh(v)->begin()+pos1; it++){
        if (*it == u) return ++cont;
        else cont++;
    }
    return 0;
}


unsigned int ALCV::sizeBytes(){
    unsigned int size = size_in_bytes(*al);
    for (size_t i=0; i<al->size(); i++)
        size += size_in_bytes(*al->at(i));
    return size;
}


ALCV::~ALCV()
{
    for (int i=0; i<al->size(); i++) util::clear(*al->at(i));
    util::clear(*al);

}

