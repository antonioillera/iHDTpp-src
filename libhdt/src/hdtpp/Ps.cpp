#include "BitmapTriplespp.hpp"

namespace hdt {

Ps::Ps(vector<AL*> o){
    op = o;
}

Ps::Ps(vector<AL*> o, vector<bit_vector*> vbm){
    op = o;
    fb = vbm;
    for (size_t i=0; i<o.size(); i++){
        fb_sel1.push_back(bit_vector::select_1_type(vbm[i]));
        fb_rank1.push_back(bit_vector::rank_1_type(vbm[i]));
        op[i]->select1support();
        op[i]->rank1support();
    }
}

size_t Ps::getNumberOfPredicates(){
    return op.size();
}

bool Ps::isNewSubjectList(size_t idPredicate){
    return op[idPredicate-1]->isNewList();
}

size_t Ps::access(size_t idPredicate, size_t pos){
    return op[idPredicate-1]->access(pos);
}

void Ps::getObjSubject(size_t predicateID, size_t familypos, size_t localSubjectID){
    size_t ptr0=0;
    size_t ptr_auxF=0, ptr_auxS=localSubjectID;
    if (familypos>1){
        ptr_auxF = fb_sel1[predicateID-1](familypos-1)+1;
        size_t begin = op[predicateID-1]->rank1(ptr_auxF-1)+localSubjectID;
        ptr0 = op[predicateID-1]->select1(begin)+1;
        ptr_auxS = begin+1;
    } else if (localSubjectID > 1){
        ptr0 = op[predicateID-1]->select1(localSubjectID-1)+1;
    }
    size_t ptr1 = op[predicateID-1]->select1(ptr_auxS);
    op[predicateID-1]->setPtrs(ptr0, ptr1);
}

size_t Ps::searchLocalObject(size_t localObjectID, size_t predicateID, size_t familypos, size_t localSubjectID){
    getObjSubject(predicateID,familypos,localSubjectID);
    return op[predicateID-1]->bsearch(localObjectID-1);
}

void Ps::getLocalObjects(size_t predicateID){
    op[predicateID-1]->traverse();
}

size_t Ps::nextLocalObject(size_t predicateID){
    return op[predicateID-1]->nextElement()+1;
}

bool Ps::hasMoreObjects(size_t predicateID){
    return op[predicateID-1]->hasMoreElements();
}

size_t Ps::size(size_t idPredicate){
    return (op[idPredicate-1]->size());
}

size_t Ps::getFamilyOrder(size_t idPredicate, size_t pos){
    size_t order = fb_rank1[idPredicate-1](pos);
    if ((*fb[idPredicate-1])[pos-1]==0) order++;
    return order;
}


size_t Ps::getLocalSubject(size_t idPredicate, size_t pos, size_t *familyOrder){
    //Dada una posición, devuleve el sujeto local y por parámetro la posición de la familia en el predicado
    size_t posiniFamilyOrder = 0, unoshastaaqui=1;
    *familyOrder =  getFamilyOrder(idPredicate,pos+1);
    if (*familyOrder>1){
        posiniFamilyOrder = fb_sel1[idPredicate-1](*familyOrder-1)+1;
        unoshastaaqui= op[idPredicate-1]->rank1(posiniFamilyOrder)+1;
    }
    size_t unoshastalli= op[idPredicate-1]->rank1(pos+1);
    size_t slocal= unoshastalli-unoshastaaqui+1;
    if (op[idPredicate-1]->getBit(pos)==0) slocal++;
    return slocal;
}

unsigned int Ps::sizeBytes(){
    unsigned int size=0;
    for (size_t i=0; i<op.size(); i++)
        size += op[i]->sizeBytes();
    for (size_t i=0; i<fb.size(); i++){
        size += size_in_bytes(*fb[i]);
        size += size_in_bytes(fb_sel1[i]);
        size += size_in_bytes(fb_rank1[i]);
    }
    size += sizeof(std::vector<AL*>);
    size += sizeof(std::vector<bit_vector*>);
    size += sizeof(vector<bit_vector::select_1_type>);
    size += sizeof(vector<bit_vector::rank_1_type>);
    return size;
}

Ps::~Ps(){
    for (std::vector< AL* >::iterator it = op.begin() ; it != op.end(); ++it) delete (*it);
    for (size_t i=0; i<fb.size(); i++){
        util::clear(*fb[i]);
        util::clear(fb_sel1[i]);
        util::clear(fb_rank1[i]);
    }
    op.clear();
    fb.clear();
    fb_sel1.clear();
    fb_rank1.clear();
    vector < AL*>().swap(op);
    vector<bit_vector*>().swap(fb);
    vector<bit_vector::select_1_type>().swap(fb_sel1);
    vector<bit_vector::rank_1_type>().swap(fb_rank1);
}

}
