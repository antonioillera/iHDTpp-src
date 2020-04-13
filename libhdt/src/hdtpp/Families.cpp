#include "BitmapTriplespp.hpp"

namespace hdt {

Families::Families(AL *f, bool TP){
    vector < int_vector<> > familiesP, predicatesF;
    size_t idF=1;
    for (size_t i=0; i < f->size(); i++){
        unsigned int idP = f->access(i);
        if(idP!=0){
            if (familiesP.size() < idP) familiesP.resize(idP);
            if (familiesP[idP-1].size() == 0)
                familiesP[idP-1] = int_vector<>(1,idF);
            else {
                size_t s = familiesP[idP-1].size();
                familiesP[idP-1].resize(s+1);
                familiesP[idP-1][s]=idF;
            }

            if (TP){
                if (predicatesF.size()<idF) predicatesF.resize(idF);
                if (predicatesF[idF-1].size() == 0)
                    predicatesF[idF-1]=int_vector<>(1,idP);
                else{
                    size_t s = predicatesF[idF-1].size();
                    predicatesF[idF-1].resize(s+1);
                    predicatesF[idF-1][s]=idP;
                }
            }

            if (f->getBit(i)==1) idF++;
        } else idF++;
    }

    vector < int_vector<>* > *v = new vector < int_vector<>* >(familiesP.size());
    for (unsigned int i=0; i<familiesP.size(); i++){
        util::bit_compress(familiesP[i]);
        (*v)[i] = new int_vector<>(familiesP[i]);
        util::clear(familiesP[i]);
    }

    util::clear(familiesP);
    vector < int_vector<> > ().swap(familiesP);
    fp = new ALV(v);
    families = idF-1;

    if(TP){
        vector < int_vector<>* > *w = new vector < int_vector<>* >(predicatesF.size());
        for (unsigned int i=0; i<predicatesF.size(); i++){
            util::bit_compress(predicatesF[i]);
            (*w)[i] = new int_vector<>(predicatesF[i]);
            util::clear(predicatesF[i]);
        }
        pf = new ALV(w);
        predicatesF.clear();
        vector < int_vector<> > ().swap(predicatesF);
    }
}


size_t Families::getNumberOfFamilies(){
    return families;
}


void Families::getFamilies(size_t predicateID){
    fp->traverse(predicateID-1);
}

bool Families::hasMoreFamilies(){
    return fp->hasMoreElements();
}

size_t Families::nextFamily(){
    return fp->nextElement();
}

size_t Families::getFamily(size_t posFamily, size_t idPredicate){
    // Nos dice el id Family dada la posicion de la familia dentro del predicado idPredicate
    return (*fp->neigh(idPredicate-1))[posFamily-1];
}

void Families::getPredicates(size_t familyID){
    pf->traverse(familyID-1);
}

bool Families::hasMorePredicates(){
    return pf->hasMoreElements();
}

size_t Families::nextPredicate(){
    return pf->nextElement();
}

bool Families::hasPredicates(size_t idFamily){
    return (*pf->neigh(idFamily-1)).size();
}


size_t Families::getPosition(size_t idFamily, size_t idPredicate){
    // Nos dice la posicion de la familia idFamily dentro del predicado idPredicate
    return fp->adj(idPredicate-1, idFamily);
}

unsigned int Families::sizeBytes(){
    unsigned int size = fp->sizeBytes();
    if (pf) size += pf->sizeBytes();
    return size;
}

Families::~Families(){
    if (pf){
        delete pf;
        pf =NULL;
    }
    delete fp;
    fp = NULL;

}

}
