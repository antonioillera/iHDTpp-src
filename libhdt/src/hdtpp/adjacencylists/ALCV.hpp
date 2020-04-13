#ifndef ALCV_H
#define ALCV_H

#define TDENS 2
#define CODER coder::elias_delta

#include <sdsl/vectors.hpp>
#include "AdjacencyList.hpp"

using namespace sdsl;
using namespace std;

class ALCV : public AdjacencyList {

private:
    vector < enc_vector<CODER, TDENS>* > *al;
    random_access_const_iterator<enc_vector<CODER, TDENS>> ptr0 = NULL, ptr1 = NULL;

public:
    ALCV(vector < enc_vector<CODER, TDENS> *> *v);
    ~ALCV();

    enc_vector<CODER, TDENS>* neigh(size_t v);
    unsigned int adj(size_t v, size_t u);
    void traverse(size_t v);
    bool hasMoreElements();
    unsigned int nextElement();
    unsigned int currentElement();
    unsigned int sizeBytes();

};

#endif // ALCV_H
