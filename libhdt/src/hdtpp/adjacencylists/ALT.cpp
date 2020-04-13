#include "ALT.hpp"

ALT::ALT(int_vector<> *s, bit_vector *b):
    sequence(s),
    bits(b) {

}

ALT::ALT(istream & in){
    bits = new bit_vector();
    sequence = new int_vector<>();
    load(*bits, in);
    load(*sequence, in);
}

void ALT::saveSequence(ostream & out) {
    serialize(*sequence, out);
}

void ALT::saveBits(ostream & out) {
    serialize(*bits, out);
}

void ALT::save(ostream & out) {
    saveBits(out);
    saveSequence(out);
}

bool ALT::hasElements(size_t v){
    return !getBit(b_sel1(v)+1);
}

void ALT::traverse(size_t v) {
    ptrB0 = b_sel1(v)+1;
    if ((*bits)[ptrB0]==0){
        ptrB1 = b_sel1(v+1);
        ptrS0 = b_rank0(ptrB0)+1;
        ptrS1 = ptrS0 + ptrB1 - ptrB0 - 1;
    }
}

void ALT::traverse() {
    ptrS0 = 1;
    ptrS1 = sequence->size()-1;
    ptrB0 = 0;
    if (hasMoreElements()){
        do {
            if (getBit(ptrB0)) cont++;
            ptrB0++;
        } while (getBit(ptrB0));
    }
}



unsigned int ALT::nextElement(){
    unsigned int e = access(ptrS0++);
    if (hasMoreElements()){
        do {
            if (getBit(ptrB0)) cont++;
            ptrB0++;
        } while (getBit(ptrB0));
    }
    return e;
}

unsigned int ALT::currentElement(){
    return access(ptrS0);
}


unsigned int ALT::access(size_t pos){
    return (*sequence)[pos];
}


unsigned int ALT::adj(size_t v, size_t u){
    traverse(v);
    while(hasMoreElements()){
        size_t c = ((ptrS1-ptrS0)/2)+ptrS0;
        size_t val = access(c);
        if (val == u) return ++c;
        else if (u < val) {
            if (!c) return 0;
            ptrS1 = c-1;
        } else ptrS0 = c+1;
    }
    return 0;
}

bool ALT::hasMoreElements() {
    return ptrS0 <= ptrS1;
}

unsigned int ALT::sizeBytes(){
   unsigned int size = size_in_bytes(*bits)+size_in_bytes(*sequence);
   size += size_in_bytes(b_sel1);
   //size += size_in_bytes(b_rank1);
   size += size_in_bytes(b_rank0);
   return size;
}

ALT::~ALT()
{
    util::clear(*bits);
    util::clear(*sequence);
    util::clear(b_sel1);
    //util::clear(b_rank1);
    util::clear(b_rank0);

    delete bits;
    delete sequence;
    bits=NULL;
    sequence=NULL;

}
