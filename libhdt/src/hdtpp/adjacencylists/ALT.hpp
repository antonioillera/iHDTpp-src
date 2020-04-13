#ifndef ALT_H
#define ALT_H

#include <iostream>
#include <sdsl/bit_vectors.hpp>
#include "AdjacencyList.hpp"

using namespace sdsl;
using namespace std;


class ALT : public AdjacencyList {

private:
    int_vector<> *sequence = NULL;
    bit_vector *bits = NULL;
    bit_vector::rank_0_type b_rank0;
    bit_vector::select_1_type b_sel1;

    size_t ptr0 = 0, ptr1 = 0;
    size_t ptrB0 = 0, ptrB1 = 0, ptrS0 = 0, ptrS1= 0;
    size_t cont = 0;


public:
    ALT(int_vector<> *s, bit_vector *b);
    ALT(istream & in);
    ~ALT();

    void rank0support() {b_rank0 = bit_vector::rank_0_type(bits);}
    void select1support() {b_sel1 = bit_vector::select_1_type(bits);}

    unsigned int adj(size_t v, size_t u);
    void traverse(size_t v);
    void traverse();
    bool hasMoreElements();
    unsigned int nextElement();
    unsigned int currentElement();
    unsigned int sizeBytes();

    bool hasElements(size_t v);
    unsigned int access(size_t pos);
    unsigned int getBit(size_t pos) {return (*bits)[pos];}
    void saveSequence(ostream & out);
    void saveBits(ostream & out);
    void save(ostream & out);
    size_t size(){return bits->size();}
    size_t seqsize(){return sequence->size();}
    size_t getCont(){return cont;}

    bit_vector *getBits(){return bits;}
    //int_vector<> *getSequence(){return sequence;}

};

#endif // ALT_H
