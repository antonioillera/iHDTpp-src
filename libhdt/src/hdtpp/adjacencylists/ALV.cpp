#include "ALV.hpp"

ALV::ALV(vector<int_vector<> *> *v):
    al(v){

}

int_vector<>* ALV::neigh(size_t v) {
    return al->at(v);
}

void ALV::traverse(size_t v) {
    ptr0 = neigh(v)->begin();
    ptr1 = neigh(v)->end();
}

bool ALV::hasMoreElements() {
    return ptr0 != ptr1;
}

unsigned int ALV::nextElement(){
    return *ptr0++;
}

unsigned int ALV::currentElement(){
    return *ptr0;
}

unsigned int ALV::adj(size_t v, size_t u){
    //size_t inf = 0, sup = neigh(v)->size()-1;
    int inf = 0, sup = neigh(v)->size()-1;
    while(inf <= sup){
      size_t c = ((sup-inf)/2)+inf;
      unsigned int val = (*neigh(v))[c];
      if (val == u) return c+1;
      else if (u < val) sup = c-1;
      else inf = c+1;
    }
    return -1;
}

unsigned int ALV::sizeBytes(){
    unsigned int size = size_in_bytes(*al);
    for (size_t i=0; i<al->size(); i++)
        size += size_in_bytes(*al->at(i));
    return size;
}

ALV::~ALV()
{
    for (int i=0; i<al->size(); i++) util::clear(*al->at(i));
    util::clear(*al);
}


