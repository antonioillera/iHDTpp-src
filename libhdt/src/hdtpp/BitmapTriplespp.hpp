#ifndef BITMAPTRIPLESPP_H
#define BITMAPTRIPLESPP_H

#include "./adjacencylists/AL.hpp"
#include "./adjacencylists/ALCV.hpp"
#include "./adjacencylists/ALV.hpp"
#include "./adjacencylists/ALT.hpp"
#include "../triples/BitmapTriples.hpp"

#undef size_t

namespace hdt {

class MapS{
private:
    ALCV *sf = NULL;            //subjects described by each family
    int_vector<> *list = NULL;  //MapS'
public:
    MapS(int_vector<> *mapS, bool TP=false);
    ~MapS();

    size_t getLocalSubject(size_t subjectID, size_t familyID);
    size_t getFamily(size_t subjectID);
    size_t getDictID(size_t familyID, size_t localSubjectID);
    void getSubjects(size_t familyID);
    bool hasMoreSubjects();
    size_t nextSubject();
    unsigned int sizeBytes();
};

class MapO{
private:
    ALCV *op = NULL; //objects related to each predicate
    AL *adjacencylist = NULL; //MapO'
public:
    MapO(AL *mapO, bool TP=false);
    ~MapO();

    size_t getDictID(size_t predicateID, size_t localObjectID);
    size_t getLocalObjectID(size_t predicateID, size_t ObjectID);
    void getPredicates(size_t objectID);
    bool hasMorePredicates();
    bool hasPredicates();
    size_t nextPredicate();
    size_t getCurrentPosition();
    size_t POrelationship(size_t predicateID, size_t objectID);
    unsigned int sizeBytes();
};

class Types{
private:
     ALT *altypes;
     size_t ptrF0 = 0, ptrF1 = 0; //Positions where start and end types of a given family
     size_t ptrT0 = 0, ptrT1 = 0;
     unsigned int family = 0; //Current family of ptrF0
public:
    Types(ALT *t);
    ~Types();

    bool hasTypes(size_t familyID);
    void getTypes(size_t familyID);
    void getTypes(); //Todos los tipos
    bool hasMoreTypes();
    size_t nextType();
    size_t getSizeTypes();
    size_t getNTypedFamilies();
    size_t getCurrentFamily();
    bool findType(size_t familyID, size_t type);
    size_t access(size_t pos);
    size_t getBit(size_t pos);
    unsigned int sizeBytes();
};

class Ps{
private:
     vector<AL*> op; //Objects in each Predicate
     vector<bit_vector*> fb; // families bitmap:no es necesario para descomprimir
     vector<bit_vector::select_1_type> fb_sel1;
     vector<bit_vector::rank_1_type> fb_rank1;
public:
    Ps(vector<AL*> o);
    Ps(vector<AL*> o, vector<bit_vector*> vbm);
    ~Ps();

    void getLocalObjects(size_t predicateID);
    void getObjSubject(size_t predicateID, size_t familypos, size_t localSubjectID);
    size_t searchLocalObject(size_t localObjectID, size_t predicateID, size_t familypos, size_t localSubjectID);

    bool isNewSubjectList(size_t idPredicate);
    size_t getNumberOfPredicates();
    size_t access(size_t idPredicate, size_t pos);
    size_t size(size_t idPredicate);

    bool hasMoreObjects(size_t predicateID);
    size_t nextLocalObject(size_t predicateID);
    size_t getFamilyOrder(size_t idPredicate, size_t pos);
    size_t getFamily(size_t idPredicate, size_t pos);
    size_t getLocalSubject(size_t idPredicate, size_t pos, size_t *familyOrder);
    unsigned int sizeBytes();
};

class Families{
private:
     ALV *fp = NULL, *pf = NULL; // families y families'
     unsigned int families=0;
public:
    Families(AL *f, bool TP = false);
    ~Families();

    size_t getNumberOfFamilies();
    size_t getPosition(size_t idFamily, size_t idPredicate);
    size_t getFamily(size_t posFamily, size_t idPredicate);

    void getFamilies(size_t predicateID);
    bool hasMoreFamilies();
    size_t nextFamily();
    void getPredicates(size_t idFamily);
    bool hasMorePredicates();
    bool hasPredicates(size_t idFamily);
    size_t nextPredicate();
    unsigned int sizeBytes();
};

class iObjects{
private:
     AL *idxObjects;
     struct sTypes{
        bit_vector *isType;
        bit_vector::rank_1_type isType_rank1;
        AL *families;
     } idxTypes;

public:
    iObjects(Ps *localobjs, MapO *mapO, size_t idRDFType, Types *types, size_t nObjects);
	iObjects(std::istream &input);
    ~iObjects();

    void getPositions(size_t posPredicate);
    size_t hasMorePositions();
    size_t nextPosition();
    bool hasTypedFamilies(size_t objectID);
    bool hasMoreTypedFamilies();
    size_t nextTypedFamily();
    bool isType(size_t idObject);
    unsigned int sizeBytes();
	void save(std::ostream & output);
};


class BitmapTriplespp {
private:

    unsigned int nObjects=0, nPredicados=0, idRDFType, nFamilias=0;
    //Estructuras de cargar/salvar de datos de disco
    int_vector<> *MapS_flat;
    AL *MapO_flat, *Families_flat;
    ALT *Types_flat;
    vector <AL *> Ps_flat;
    vector <bit_vector *> Bf_flat; //No necesaria para descomprimir

    //Estructuras de consulta en memoria
    Families *families = NULL;
    Types *types = NULL;
    MapO *mapO = NULL;
    MapS *mapS = NULL;
    Ps *ps = NULL;
    iObjects *iobjects = NULL;


public:
    BitmapTriplespp();

    void constructMemoryStructures(bool TP = false);

    void setMapS_flat(int_vector<> *ps){delete MapS_flat; MapS_flat = ps;}
    void setPs_flat(vector <AL *> a){Ps_flat=a;}
    void setBf_flat(vector <bit_vector*> b){Bf_flat=b;}
    void setMapO_flat(AL *a){MapO_flat=a;}
    void setTypes_flat(ALT *a){Types_flat=a;}
    void setFamilies_flat(AL *a){Families_flat=a;}

    int_vector<> *getMapS_flat(){return MapS_flat;}
    vector <AL*> getPs_flat(){return Ps_flat;}
    vector <bit_vector*> getBf_flat(){return Bf_flat;}
    AL *getMapO_flat(){return MapO_flat;}
    ALT *getTypes_flat(){return Types_flat;}
    AL *getFamilies_flat(){return Families_flat;}
    Families *getFamilies(){return families;}
    Types *getTypes(){return types;}
    MapO *getMapO(){return mapO;}
    MapS *getMapS(){return mapS;}
    Ps *getPs(){return ps;}
    iObjects *getIObjects(){return iobjects;}

    void setnPredicados(unsigned int np){nPredicados=np;}
    void setnObjects(unsigned int no){nObjects=no;}
    unsigned int getNFamilies(){return families->getNumberOfFamilies();}
    unsigned int getIdRDFType(){return idRDFType;}
    void setIdRDFType(unsigned int t){idRDFType=t;}
	void buildiObjects(std::ostream & output);
	void loadiObjects(std::istream &input);


    ~BitmapTriplespp();

    /**
     * Saves the triples
     *
     * @param output
     * @return
     */

    void save(std::ostream &output);
    /**
     * Loads triples from a file
     *
     * @param input
     * @return
     */

    void load(std::istream &input);

    /**
     * Returns a vector of triples matching the pattern
     *
     * @param triple
     * @return
     */

    IteratorTripleID *search(TripleID &pattern);
};


class iteratorstv : public IteratorTripleID {
private:
    BitmapTriplespp *triples;
    TripleID pattern, returnTriple;

    unsigned int subject,predicate, object, family;
    bool more=true;
    void updateOutput();

public:
    iteratorstv(BitmapTriplespp *triples, TripleID &pat, size_t idfam=0);
    bool hasNext();
    TripleID *next();
};

class iteratorsvv : public IteratorTripleID {
private:
    BitmapTriplespp *triples;
    TripleID pattern, returnTriple;

    IteratorTripleID *it;

    unsigned int subject,predicate, object, family;
    unsigned int localSubject;
    bool more=true,moreT=true;
    void updateOutput();

public:
    iteratorsvv(BitmapTriplespp *triples, TripleID &pat);
    bool hasNext();
    TripleID *next();
};

class iteratorspv : public IteratorTripleID {
private:
    BitmapTriplespp *triples;
    TripleID pattern, returnTriple;

    unsigned int subject=0, predicate=0, object=0, family=0;
    bool more=true;
    void updateOutput();

public:
    iteratorspv(BitmapTriplespp *triples, TripleID &pat, size_t idfam=0);
    bool hasNext();
    TripleID *next();
};


class iteratorspo : public IteratorTripleID {
private:
    BitmapTriplespp *triples;
    TripleID pattern, returnTriple;

    unsigned int subject, predicate, object, family;
    bool more=true;
    void updateOutput();

public:
    iteratorspo(BitmapTriplespp *triples, TripleID &pat, size_t idfam=0);
    bool hasNext();
    TripleID *next();
};

class iteratorsto : public IteratorTripleID {
private:
    BitmapTriplespp *triples;
    TripleID pattern, returnTriple;

    unsigned int subject,predicate, object, family;
    bool more=true;
    void updateOutput();

public:
    iteratorsto(BitmapTriplespp *triples, TripleID &pat, size_t idfam=0);
    bool hasNext();
    TripleID *next();
};

class iteratorsvo : public IteratorTripleID {
private:
    BitmapTriplespp *triples;
    TripleID pattern, returnTriple;

    IteratorTripleID *it=NULL;
    bool more=true;
    unsigned int subject,predicate, object, family;
    void updateOutput();
public:
    iteratorsvo(BitmapTriplespp *triples, TripleID &pat);
    bool hasNext();
    TripleID *next();
};


class iteratorvtv : public IteratorTripleID {
private:
    BitmapTriplespp *triples;
    TripleID pattern, returnTriple;

    unsigned int subject,predicate, object, family;
    bool more=true;
    void updateOutput();

public:
    iteratorvtv(BitmapTriplespp *triples, TripleID &pat);
    bool hasNext();
    TripleID *next();
};

class iteratorvpv : public IteratorTripleID {
private:
    BitmapTriplespp *triples;
    TripleID pattern, returnTriple;

    unsigned int subject,predicate, object, family;
    unsigned int localObject, posF = 0;
    bool more = true;
    void updateOutput();

public:
    iteratorvpv(BitmapTriplespp *triples, TripleID &pat);
    bool hasNext();
    TripleID *next();
};

class iteratorvpo : public IteratorTripleID {
private:
    BitmapTriplespp *triples;
    TripleID pattern, returnTriple;

    unsigned int subject, predicate, object, family;
    bool more = true;
    void updateOutput();
    void calculateNextSubject();

public:
    iteratorvpo(BitmapTriplespp *triples, TripleID &pat, size_t position=0);
    bool hasNext();
    TripleID *next();
};

class iteratorvto : public IteratorTripleID {
private:
    BitmapTriplespp *triples;
    TripleID pattern, returnTriple;

    unsigned int subject, predicate, object, family;
    bool more = true;
    void updateOutput();


public:
    iteratorvto(BitmapTriplespp *triples, TripleID &pat);
    bool hasNext();
    TripleID *next();
};


class iteratorvvo : public IteratorTripleID {
private:
    BitmapTriplespp *triples;
    TripleID pattern, returnTriple;

    IteratorTripleID *it=NULL;
    unsigned int subject=0, predicate=0, object, family=0;
    bool more=true;
    void updateOutput();
public:
    iteratorvvo(BitmapTriplespp *triples, TripleID &pat);
    bool hasNext();
    TripleID *next();
};


}

#endif /* BITMAPTRIPLES_HPP_ */
