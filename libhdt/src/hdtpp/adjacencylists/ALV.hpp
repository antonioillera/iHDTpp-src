#ifndef ALV_H
#define ALV_H

#include <sdsl/vectors.hpp>
#include "AdjacencyList.hpp"

using namespace sdsl;
using namespace std;

class ALV : public AdjacencyList {

private:
    vector < int_vector<> *> *al;
    sdsl::int_vector_const_iterator<int_vector<>> ptr0 = NULL, ptr1 = NULL;

public:
    ALV(vector < int_vector<> *>*v);
    ~ALV();

    int_vector<>* neigh(size_t v) ;
    unsigned int adj(size_t v, size_t u);
    void traverse(size_t v);
    bool hasMoreElements();
    unsigned int nextElement();
    unsigned int currentElement();
    unsigned int sizeBytes();
};

#endif // ALV_H
