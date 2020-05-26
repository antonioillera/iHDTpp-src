#include "BitmapTriplespp.hpp"

namespace hdt {

MapO::MapO(AL *mapO, bool TP){

    vector < int_vector<> > aux;
    vector < enc_vector<CODER, TDENS>* > *v = new vector < enc_vector<CODER, TDENS>* >();

    if (TP){
        adjacencylist = mapO;
        adjacencylist->select1support();
    }

    size_t pos = 0;
    for (size_t i=0; i<mapO->size(); i++){
        unsigned int predicate = mapO->access(i);
        if (predicate!=0){
            if (aux.size() < predicate) aux.resize(predicate);
            if (aux[predicate-1].size() == 0)
                aux[predicate-1] = int_vector<>(1,pos);
            else {
                size_t s = aux[predicate-1].size();
                aux[predicate-1].resize(s+1);
                aux[predicate-1][s]=pos;
            }
            if (mapO->getBit(i)==1) pos++;
        } else pos++;
    }
    for (size_t i=0; i<aux.size(); i++){
        util::bit_compress(aux[i]);
        v->push_back(new enc_vector<CODER, TDENS>(aux[i]));
        util::clear(aux[i]);
    }
    util::clear(aux);
    op = new ALCV(v);
}



size_t MapO::getDictID(size_t predicateID, size_t localObjectID){
    return (*op->neigh(predicateID-1))[localObjectID-1]+1;
}

size_t MapO::getLocalObjectID(size_t predicateID, size_t ObjectID){
    return op->adj(predicateID-1, ObjectID-1);
}


void MapO::getPredicates(size_t objectID){
    adjacencylist->traverse(objectID);
}

bool MapO::hasMorePredicates(){
    return adjacencylist->hasMoreElements();
}

size_t MapO::nextPredicate(){
    return adjacencylist->nextElement();
}

bool MapO::hasPredicates(){
    return adjacencylist->currentElement();
}

size_t MapO::POrelationship(size_t predicateID, size_t objectID){
    // Devuelve la posicion en la que ha encontrado la relación.
    return adjacencylist->adj(predicateID, objectID);
}

size_t MapO::getCurrentPosition(){
    return adjacencylist->getCurrentPosition();
}

size_t MapO::getNObjects(size_t predicateID){
    return op->getNElements(predicateID-1);
}

unsigned int MapO::sizeBytes(){
    unsigned int size = op->sizeBytes();
    if (adjacencylist) size += adjacencylist->sizeBytes();
    return size;
}


MapO::~MapO(){
    delete op;
    op = NULL;
    if (adjacencylist){
        delete adjacencylist;
        adjacencylist = NULL;
    }
}

}
