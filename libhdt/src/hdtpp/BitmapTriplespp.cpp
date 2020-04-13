#include "BitmapTriplespp.hpp"

namespace hdt {

BitmapTriplespp::BitmapTriplespp() {
    MapS_flat = NULL;
    Families_flat = NULL;
    Types_flat = NULL;
    MapO_flat = NULL;
}

BitmapTriplespp::~BitmapTriplespp() {
    if (mapS){
        delete mapS;
        mapS=NULL;
    }
    if (MapS_flat){
        util::clear(*MapS_flat);
        delete MapS_flat;
        MapS_flat=NULL;
    }
    if (mapO){
        delete mapO;
        mapO=NULL;
    }
    if (MapO_flat){
        delete MapO_flat;
        MapO_flat=NULL;
    }
    if (families){
        delete families;
        families=NULL;
    }
    if (Families_flat){
        delete Families_flat;
        Families_flat=NULL;
    }
    if (types){
       delete types;
       types=NULL;
       Types_flat=NULL;
    }
    if (ps){
        delete ps;
        ps = NULL;
    }
    if(iobjects){
        delete iobjects;
        iobjects=NULL;
    }
}

void BitmapTriplespp::save(std::ostream & output)
{
    this->getFamilies_flat()->save(output);
    this->getTypes_flat()->save(output);
    this->getMapO_flat()->save(output);
    serialize(*(this->getMapS_flat()),output);
    unsigned int sizePs=0;
    for (size_t i=0; i<nPredicados; i++){
        this->getPs_flat()[i]->save(output);
        sizePs+=this->getPs_flat()[i]->sizeBytes();
        //NO necesario para descomprimir
        serialize(*(this->getBf_flat()[i]),output);
    }
}

void BitmapTriplespp::load(std::istream &input)
{
    this->Families_flat = new AL(input);
    this->Types_flat = new ALT(input);
    this->MapO_flat = new AL(input);
    MapS_flat = new int_vector<>();
    sdsl::load(*MapS_flat, input);
    for (size_t i=0; i<nPredicados; i++){
        AL *a = new AL(input);
        Ps_flat.push_back(a);
        bit_vector *b = new bit_vector();
        sdsl::load(*b, input);
        Bf_flat.push_back(b);
    }
    cout<<endl<<"HDT++ loaded from disk"<<endl;
}

IteratorTripleID *BitmapTriplespp::search(TripleID &pattern){
    if (pattern.getPredicate()==idRDFType){
        if (pattern.getSubject() == 0){
            if (pattern.getObject() == 0){
                return new iteratorvtv(this, pattern);
            } else {
                return new iteratorvto(this, pattern);
            }
        } else {
            if (pattern.getObject() == 0){
                return new iteratorstv(this, pattern);
            } else {
                return new iteratorsto(this, pattern);
            }
        }
    } else if (pattern.getPredicate() == 0){
        if (pattern.getSubject() == 0){
            if (pattern.getObject() == 0){
                //return new iteratorvvv(this, pattern);
            } else {
                return new iteratorvvo(this, pattern);
            }
        } else {
            if (pattern.getObject() == 0){
                return new iteratorsvv(this, pattern);
            } else {
                return new iteratorsvo(this, pattern);
            }
        }
    } else{
        if (pattern.getSubject() == 0){
            if (pattern.getObject() == 0){
                return new iteratorvpv(this, pattern);
            } else {
                return new iteratorvpo(this, pattern);
            }
        } else {
            if (pattern.getObject() == 0){
                return new iteratorspv(this, pattern);
            } else {
                return new iteratorspo(this, pattern);
            }
        }
    }
    return NULL;
}

void BitmapTriplespp::buildiObjects(std::ostream & output){
    iobjects = new iObjects(ps, mapO, idRDFType, types, nObjects);
    iobjects->save(output);
}

void BitmapTriplespp::loadiObjects(std::istream &input){
    iobjects = new iObjects(input);
}

void BitmapTriplespp::constructMemoryStructures(bool TP){
    types = new Types(Types_flat);
    mapO = new MapO(MapO_flat, TP);
    mapS = new MapS(MapS_flat, TP);
    ps = new Ps(Ps_flat,Bf_flat);
    families = new Families(Families_flat, TP);
    if (!TP){
        delete MapS_flat;
        delete MapO_flat;
    }
    MapS_flat = NULL;
    delete Families_flat;
    Families_flat = NULL;
    MapO_flat = NULL;
}
}
