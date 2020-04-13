#include "AL.hpp"

AL::AL(int_vector<> *s, bit_vector *b):
    sequence(s),
    bits(b) {

}

AL::AL(istream & in){
    bits = new bit_vector();
    sequence = new int_vector<>();
    load(*bits, in);
    load(*sequence, in);
}

void AL::saveSequence(ostream & out) {
    serialize(*sequence, out);
}

void AL::saveBits(ostream & out) {
    serialize(*bits, out);
}

void AL::save(ostream & out) {
    saveBits(out);
    saveSequence(out);
}


void AL::traverse() {
    ptr0 = 0;
    ptr1 = sequence->size()-1;
}

bool AL::isNewList() {
    return (*bits)[ptr0-1];
}

void AL::traverse(size_t v) {
    if (v>1) ptr0 = b_sel1(v-1)+1;
    else ptr0 = 0;
    ptr1 = b_sel1(v);
}

bool AL::hasMoreElements() {
    return ptr0 <= ptr1;
}

unsigned int AL::nextElement(){
    return access(ptr0++);
}

unsigned int AL::currentElement(){
    return access(ptr0);
}

unsigned int AL::access(size_t pos){
    return (*sequence)[pos];
}
unsigned int AL::getCurrentPosition(){
    return ptr0;
}

void AL::setPtrs(size_t p0, size_t p1){
    ptr0 = p0;
    ptr1 = p1;
}

unsigned int AL::adj(size_t v, size_t u){
    traverse(u);
    return bsearch(v);
  }

unsigned int AL::bsearch(size_t v){
    while(hasMoreElements()){
        size_t c = ((ptr1-ptr0)/2)+ptr0;
        size_t val = access(c);
        if (val == v) return ++c;
        else if (v < val) {
            if (!c) return 0;
            ptr1 = c-1;
        } else ptr0 = c+1;
    }
    return 0;
}


unsigned int AL::sizeBytes(){
   unsigned int size = size_in_bytes(*bits)+size_in_bytes(*sequence);
   size += size_in_bytes(b_sel1);
   size += size_in_bytes(b_rank1);
   size += size_in_bytes(b_rank0);
   return size;
}

AL::~AL()
{
    util::clear(*bits);
    util::clear(*sequence);
    util::clear(b_sel1);
    util::clear(b_rank1);
    util::clear(b_rank0);

    delete bits;
    delete sequence;
    bits=NULL;
    sequence=NULL;

}

