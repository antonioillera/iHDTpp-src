#ifndef AL_H
#define AL_H

#include <iostream>
#include <sdsl/bit_vectors.hpp>
#include "AdjacencyList.hpp"

using namespace sdsl;
using namespace std;


class AL : public AdjacencyList {

private:
    int_vector<> *sequence = NULL;
    bit_vector *bits = NULL;
    bit_vector::rank_1_type b_rank1;
    bit_vector::rank_0_type b_rank0;
    bit_vector::select_1_type b_sel1;
    size_t ptr0 = 0, ptr1 = 0, ptrTemp0 = 0, ptrTemp1 = 0;
public:
    AL(int_vector<> *s, bit_vector *b);
    AL(istream & in);
    ~AL();

    void rank1support() {b_rank1 = bit_vector::rank_1_type(bits);}
    void rank0support() {b_rank0 = bit_vector::rank_0_type(bits);}
    void select1support() {b_sel1 = bit_vector::select_1_type(bits);}
    size_t select1(size_t n){return b_sel1(n);}
    size_t rank1(size_t n){return b_rank1(n);}
    size_t rank0(size_t n){return b_rank0(n);}

    unsigned int adj(size_t v, size_t u);
    void traverse(size_t v);
    bool hasMoreElements();
    unsigned int nextElement();
    unsigned int currentElement();
    unsigned int sizeBytes();

    unsigned int bsearch(size_t v);
    void traverse();

    bool isNewList();
    unsigned int getCurrentPosition();
    unsigned int access(size_t pos);
    void setPtrs(size_t p0, size_t p1);
    unsigned int getBit(size_t pos) {return (*bits)[pos];}
    void saveSequence(ostream & out);
    void saveBits(ostream & out);
    void save(ostream & out);
    size_t size(){return bits->size();}
    //size_t seqsize(){return sequence->size();}

    //bit_vector *getBits(){return bits;}
    //int_vector<> *getSequence(){return sequence;}

};

#endif // AL_H
