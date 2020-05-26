#include "BitmapTriplespp.hpp"

namespace hdt {

MapS::MapS(int_vector<> *mapS, bool TP){

    vector < int_vector<> > aux;
    vector < enc_vector<CODER, TDENS>* > *v = new vector < enc_vector<CODER, TDENS>* >();

    if (TP) list=mapS;
    for (size_t i=0; i<mapS->size(); i++){
        unsigned int family = (*mapS)[i]+1;
        if (aux.size() < family) aux.resize(family);
        if (aux[family-1].size() == 0)
            aux[family-1] = int_vector<>(1,i);
        else {
            size_t s = aux[family-1].size();
            aux[family-1].resize(s+1);
            aux[family-1][s]=i;
        }
    }
    for (size_t i=0; i<aux.size(); i++){
        util::bit_compress(aux[i]);
        v->push_back(new enc_vector<CODER, TDENS>(aux[i]));
        util::clear(aux[i]);
    }
    util::clear(aux);
    sf = new ALCV(v);
}

size_t MapS::getDictID(size_t familyID, size_t localSubjectID){
    return (*sf->neigh(familyID-1))[localSubjectID-1]+1;
}

size_t MapS::getFamily(size_t subjectID){
    return (*list)[subjectID-1]+1;
}

void MapS::getSubjects(size_t familyID){
    sf->traverse(familyID-1);
}

size_t MapS::getNSubjects(size_t familyID){
    return sf->getNElements(familyID-1);
}

bool MapS::hasMoreSubjects(){
    return sf->hasMoreElements();
}

size_t MapS::nextSubject(){
    return sf->nextElement()+1;
}

size_t MapS::getLocalSubject(size_t subjectID, size_t familyID){
    return sf->adj(familyID-1, subjectID-1);
}

unsigned int MapS::sizeBytes(){
    unsigned int size = sf->sizeBytes();
    if (list) size += size_in_bytes(*list);
    return size;
}

MapS::~MapS(){
    delete sf;
    sf=NULL;
    if (list) {
        util::clear(*list);
        list=NULL;
    }
}

}
