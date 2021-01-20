#include "BitmapTriplespp.hpp"


namespace hdt {


iObjects::iObjects(std::istream &input){
    idxObjects = new AL(input);
    idxTypes.families = new AL(input);
    idxTypes.isType = new bit_vector();
    sdsl::load(*idxTypes.isType, input);

    idxObjects->select1support();
    idxTypes.families->select1support();
    idxTypes.isType_rank1 = bit_vector::rank_1_type(idxTypes.isType);
}


iObjects::iObjects(Ps *localobjs, MapO *mapO, size_t idRDFType, Types *types, size_t nObjects){
    vector< vector <unsigned int> > idxSeq;
    vector< vector <bool> > idxBitmap;
    vector<unsigned int> objs; //Objetos que han sido usados en ese predicado

    idxTypes.isType = new bit_vector(nObjects,0);
    map<unsigned int, int_vector<>> mapTypes;

    types->getTypes();
    while (types->hasMoreTypes()){
        size_t f = types->getCurrentFamily();
        size_t t = types->nextType();
        map<unsigned int,int_vector<>>::const_iterator it = mapTypes.find(t);
        if (it == mapTypes.end())
            mapTypes.insert(std::pair<unsigned int, int_vector<>>(t,int_vector<>(1,f)));
        else{
            size_t s = mapTypes[t].size();
            mapTypes[t].resize(s+1);
            mapTypes[t][s]=f;
        }
    }

    int_vector<> *SeqTypesFamilies = new int_vector<>(types->getSizeTypes());
    bit_vector *BmTypesFamilies = new bit_vector(types->getSizeTypes(),0);

    size_t ptrFamilies=0;
    for (std::map<unsigned int, int_vector<>>::iterator it = mapTypes.begin(); it!=mapTypes.end(); it++){
        (*idxTypes.isType)[it->first-1]=1;
        for (size_t i=0; i<it->second.size(); i++)
            (*SeqTypesFamilies)[ptrFamilies++]=it->second[i];
        (*BmTypesFamilies)[ptrFamilies-1]=1;
        util::clear(it->second);
        int_vector<>().swap(it->second);
    }
    mapTypes.clear();
    map<unsigned int, int_vector<>>().swap(mapTypes);
    util::bit_compress(*SeqTypesFamilies);
    idxTypes.families = new AL(SeqTypesFamilies,BmTypesFamilies);
    idxTypes.families->select1support();
    idxTypes.isType_rank1 = bit_vector::rank_1_type(idxTypes.isType);

    for (size_t p=1; p<=localobjs->getNumberOfPredicates(); p++){
       if (p!=idRDFType){
           localobjs->getLocalObjects(p);
           size_t i=0;
           while (localobjs->hasMoreObjects(p)){
               size_t idOLocal = localobjs->nextLocalObject(p);
               size_t idO = mapO->getDictID(p,idOLocal);
               if (idxSeq.size()<idO){
                   idxSeq.resize(idO);
                   idxBitmap.resize(idO);
               }
               idxSeq[idO-1].push_back(i);
               idxBitmap[idO-1].push_back(0);
               objs.push_back(idO);
               i++;
           }

           for (size_t j=0; j<objs.size(); j++){
               size_t obj = objs[j];
               idxBitmap[obj-1][idxBitmap[obj-1].size()-1]=1;
           }
           objs.clear();
       }
    }

    vector<unsigned int>().swap(objs);
    bit_vector *bitmap = new bit_vector;
    int_vector<> *seq = new int_vector<>;

    size_t size;
    for (size_t i=0; i<idxBitmap.size(); i++){
        if (idxBitmap[i].size()==0){
            size = bitmap->size();
            bitmap->resize(size+1);
            seq->resize(size+1);
            bitmap->operator [](size)=1;
            seq->operator [](size)=0;
        } else {
            size = bitmap->size();
            bitmap->resize(size+idxBitmap[i].size());
            seq->resize(size+idxSeq[i].size());
            for (size_t j=0; j<idxBitmap[i].size(); j++){
                bitmap->operator [](size+j)=idxBitmap[i][j];
                seq->operator [](size+j)=idxSeq[i][j];
            }
        }
        util::clear(idxSeq[i]);
        util::clear(idxBitmap[i]);
    }
    vector < vector<unsigned int> >().swap(idxSeq);
    vector< vector <bool> >().swap(idxBitmap);
    util::bit_compress(*seq);

    idxObjects = new AL(seq,bitmap);
    idxObjects->select1support();
}


void iObjects::getPositions(size_t posPredicate){
    idxObjects->traverse(posPredicate+1);
}

size_t iObjects::hasMorePositions(){
    return idxObjects->hasMoreElements();
}

size_t iObjects::nextPosition(){
    return idxObjects->nextElement();
}

bool iObjects::hasMoreTypedFamilies(){
    return idxTypes.families->hasMoreElements();
}

size_t iObjects::nextTypedFamily(){
    return idxTypes.families->nextElement();
}

bool iObjects::hasTypedFamilies(size_t objectID){
    if (isType(objectID)){
        //size_t pos = idxTypes.isType_rank1(objectID);
        idxTypes.families->traverse(idxTypes.isType_rank1(objectID));
        return true;
    } else return false;
}

bool iObjects::isType(size_t idObject){
    return ((*idxTypes.isType)[idObject-1]);

}

unsigned int iObjects::sizeBytes(){
    unsigned int size = idxObjects->sizeBytes();
    size += size_in_bytes(*idxTypes.isType);
    size += idxTypes.families->sizeBytes();
    size += size_in_bytes(idxTypes.isType_rank1);
    unsigned int cont = 0;
    for (unsigned int i=0; i<idxTypes.isType->size();i++)
        if(!(*idxTypes.isType)[i]) cont++;
    cont = 0;
    for (unsigned int i=0; i<idxTypes.isType->size();i++)
        if((*idxTypes.isType)[i]) cont++;
    return size;
}


void iObjects::save(std::ostream & output){
    idxObjects->save(output);
    idxTypes.families->save(output);
    serialize(*idxTypes.isType,output);
}



iObjects::~iObjects(){
    delete idxObjects;
    delete idxTypes.families;
    util::clear(*(idxTypes.isType));
    delete idxTypes.isType;
    idxObjects = NULL;
    idxTypes.families = NULL;
    idxTypes.isType = NULL;
}


}
