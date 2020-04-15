#include "HDTpp.hpp"

namespace hdt {

HDTpp::HDTpp(HDT *hdt)
{
    string rdftypeLiteral = "http://www.w3.org/1999/02/22-rdf-syntax-ns#type";
    header = hdt->getHeader();
    dictionary = hdt->getDictionary();
    triples = dynamic_cast<BitmapTriples*>(hdt->getTriples());
    triplespp = new BitmapTriplespp();
    idRdfType = dictionary->stringToId(rdftypeLiteral, PREDICATE);
    extractFamilies();
}

HDTpp::HDTpp(const char *file, ProgressListener *listener){
	this->filename=file;
    DecompressStream stream(file);
    istream *in = stream.getStream();
    if(!in->good()){
        throw "Error opening file to load HDT++.";
    }
    this->loadFromHDTpp(*in, listener);
    stream.close();
}

void HDTpp::loadFromHDTpp(std::istream & input, ProgressListener *listener){
    try {
        ControlInformation controlInformation;
        IntermediateListener iListener(listener);
        controlInformation.load(input);
        std::string hdtFormat = controlInformation.getFormat();
        if(hdtFormat!=HDTVocabulary::HDT_CONTAINER) {
            throw "This software cannot open this version of HDT++ File.";
        }

        iListener.setRange(0,5);
        controlInformation.load(input);
        header = HDTFactory::readHeader(controlInformation);
        header->load(input, controlInformation, &iListener);

        iListener.setRange(5, 60);
        controlInformation.load(input);
        dictionary = HDTFactory::readDictionary(controlInformation);
        dictionary->load(input, controlInformation, &iListener);

        iListener.setRange(60,100);
        triplespp = new BitmapTriplespp();
        triplespp->setnPredicados(dictionary->getMaxPredicateID());
        triplespp->setnObjects(dictionary->getMaxObjectID());
        triplespp->load(input);
        string rdftypeLiteral = "http://www.w3.org/1999/02/22-rdf-syntax-ns#type";
		size_t id = dictionary->stringToId(rdftypeLiteral, PREDICATE);
		if (id>0) triplespp->setIdRDFType(id);

    } catch (const char *ex) {
        cout << "Exception loading HDT++: " << ex;
        deleteComponents();
        createComponents();
        throw ex;
    }
}


void HDTpp::extractFamilies(){
    map<string, Family*> predicateFamilies;
    int_vector<> *MapS_flat = new int_vector<>(dictionary->getNsubjects());
    vector <unsigned int> seqFamilyObjects;
    vector < vector <unsigned int> > FamiliesObjects;
    vector <unsigned char> bmFamilyObjects; //Objects per family
    vector < vector <unsigned char> > bmFamiliesObjects;
    unsigned int familyID=0;

    for (unsigned int s=1; s<=dictionary->getNsubjects(); s++){
        string familyKey = "";
        seqFamilyObjects.clear();
        bmFamilyObjects.clear();
        Family f;
        TripleID t=TripleID(s,0,0);
        IteratorTripleID *it = triples->search(t);
        unsigned int pant=0;
        while (it->hasNext()) {
            TripleID *tid = it->next();
            unsigned int p = tid->getPredicate();
            if (p!=idRdfType){
                // Predicate is not rdf:type
                unsigned int o = tid->getObject();
                seqFamilyObjects.push_back(o);
                if (p > pant){
                    f.idPredicates.push_back(p);
                    char predStr [33];
                    snprintf(predStr, sizeof(predStr), "%d", p);
                    familyKey = familyKey + predStr + "#";
                    if (pant!=0) bmFamilyObjects.push_back(1);
                } else{
                    bmFamilyObjects.push_back(0);
                }
                pant=p;
            }else{
                f.idRdfTypes.push_back(tid->getObject());
            }
        }

        bmFamilyObjects.push_back(1);
        delete it;
        it=NULL;

        // Family has no types
        size_t ntiposF = f.idRdfTypes.size();
        if (ntiposF==0) f.idRdfTypes.push_back(0);

        //Family has no predicates
        if (familyKey=="") familyKey="0";

        familyKey= familyKey+"%";
        for (size_t i=0; i<ntiposF; i++){
           char objStr [33];
            snprintf(objStr, sizeof(objStr), "%d", f.idRdfTypes[i]);
            familyKey = familyKey + objStr + "#";
        }
        map<string,Family*>::iterator it2 = predicateFamilies.find(familyKey);
        if(it2 != predicateFamilies.end()){
           FamiliesObjects[it2->second->familyID-1].insert(FamiliesObjects[it2->second->familyID-1].end(), seqFamilyObjects.begin(), seqFamilyObjects.end());
           bmFamiliesObjects[it2->second->familyID-1].insert(bmFamiliesObjects[it2->second->familyID-1].end(), bmFamilyObjects.begin(), bmFamilyObjects.end());
           (*MapS_flat)[s-1]=it2->second->familyID-1;
           it2->second->nSubjects++;
        } else {
            familyID++;
            Family *ptrF = new Family();
            ptrF->idPredicates=f.idPredicates;
            ptrF->idRdfTypes=f.idRdfTypes;
            ptrF->familyID=familyID;
            FamiliesObjects.push_back(seqFamilyObjects);
            bmFamiliesObjects.push_back(bmFamilyObjects);
            predicateFamilies.insert(std::pair<string,Family*>(familyKey,ptrF));
            (*MapS_flat)[s-1]=familyID-1;
        }
    }
    seqFamilyObjects.clear();
    bmFamilyObjects.clear();
    util::bit_compress(*MapS_flat);

    //Reorganizing familyIDs
    vFamilias.resize(familyID);
    for (std::map<string,Family*>::iterator it=predicateFamilies.begin(); it!=predicateFamilies.end(); ++it){
        vFamilias[it->second->familyID-1]=it->second;
    }
    predicateFamilies.clear();
    delete triples;
    triples=NULL;
    triplespp->setMapS_flat(MapS_flat);

    setTypes();
    setFamilies();
    setMapO(&FamiliesObjects, &bmFamiliesObjects);
}

void HDTpp::setTypes(){

    //ficticious element at the beginning
    bit_vector *bmTypes = new bit_vector(1,1);
    int_vector<> *seqTypes = new int_vector<>(1,0);
    for (size_t f=0; f<vFamilias.size(); f++){
        //RDF:Types of current family
        size_t nTypesF = 0;
        size_t seqSize = seqTypes->size();
        size_t bmSize = bmTypes->size();
        if (vFamilias[f]->idRdfTypes[0]!=0)
            nTypesF=vFamilias[f]->idRdfTypes.size();
        seqTypes->resize(seqSize+nTypesF);
        bmTypes->resize(bmSize+nTypesF+1);
        for (size_t i=0; i<nTypesF; i++){
            (*seqTypes)[seqSize+i]=vFamilias[f]->idRdfTypes[i];
            (*bmTypes)[bmSize+i]=0;
         }
         (*bmTypes)[bmSize+nTypesF]=1;
    }
    util::bit_compress(*seqTypes);
    ALT *Types_flat = new ALT(seqTypes,bmTypes);
    triplespp->setTypes_flat(Types_flat);

}

void HDTpp::setFamilies(){
    int_vector<> *seqFamilies_flat = new int_vector<>();
    bit_vector *bmFamilies_flat = new bit_vector();
    for (unsigned int f=0; f<vFamilias.size(); f++){
        // Number of predicates of the current family
        size_t nPredicatesF = vFamilias[f]->idPredicates.size();
        if (nPredicatesF==0){
            //No predicates (Only RDF:Type ones)
            size_t s = bmFamilies_flat->size();
            bmFamilies_flat->resize(s+1);
            seqFamilies_flat->resize(s+1);
            (*bmFamilies_flat)[s]=1;
            (*seqFamilies_flat)[s]=0;
        } else {
            for (size_t i=1; i<=nPredicatesF; i++){
                unsigned int predicateID = vFamilias[f]->idPredicates[i-1];
                size_t s = bmFamilies_flat->size();
                seqFamilies_flat->resize(s+1);
                bmFamilies_flat->resize(s+1);
                (*seqFamilies_flat)[s]=predicateID;
                if (i == nPredicatesF) (*bmFamilies_flat)[s]=1;
                else (*bmFamilies_flat)[s]=0;
            }
        }
    }
    util::bit_compress(*seqFamilies_flat);
    AL *Families_flat = new AL(seqFamilies_flat, bmFamilies_flat);
    triplespp->setFamilies_flat(Families_flat);
}

void HDTpp::setMapO(vector < vector <unsigned int> > *fo, vector < vector <unsigned char> > *bmfo){
    vector < map < unsigned int,unsigned int > > objectMapping(dictionary->getNpredicates()); //Mapping between object local and global IDs
    vector<unsigned int> So_globalIds;
    bit_vector bmOF, bmOS;

    for (unsigned int f=0; f<vFamilias.size(); f++){
        // Number of subjects, predicates and types per family
        size_t nPredicatesF = vFamilias[f]->idPredicates.size();
        bit_vector bitmapObjetosF_sdsl = bit_vector(bmfo->at(f).size());
        for (unsigned int i=0; i<bmfo->at(f).size(); i++){
            bitmapObjetosF_sdsl[i] = bmfo->at(f)[i];
        }

        select_support_mcl<1,1> b_sel1(&bitmapObjetosF_sdsl);
        size_t pant=0;
        size_t index = bmOS.size();

        for (size_t i=1; i<=nPredicatesF; i++){
            unsigned int predicateID = vFamilias[f]->idPredicates[i-1];
            for (size_t j=0; j<vFamilias[f]->nSubjects; j++){
                size_t posicionInicial = 0;
                size_t n_uno = i+(nPredicatesF*j)-1;
                if (n_uno>0) posicionInicial = b_sel1(n_uno)+1;

                size_t posicionFinal = 0;
                n_uno = i+(nPredicatesF*j);
                if (n_uno>0) posicionFinal = b_sel1(n_uno);
                bmOS.resize(bmOS.size()+posicionFinal-posicionInicial+1);
                So_globalIds.resize(So_globalIds.size()+posicionFinal-posicionInicial+1);
                for (size_t k=posicionInicial; k<=posicionFinal; k++){
                    unsigned int objectID = (fo->at(f))[k];
                    So_globalIds[index]=objectID;
                    bmOS[index]=bitmapObjetosF_sdsl[k];
                    index++;
                    objectMapping[predicateID-1].insert(std::pair<unsigned int, unsigned int>(objectID,0));
                    if (i > pant) {
                        if  (pant!=0){
                            bmOF.resize(bmOF.size()+1);
                            bmOF[bmOF.size()-1]=1;
                        }
                        pant=i;
                    } else{
                         bmOF.resize(bmOF.size()+1);
                         bmOF[bmOF.size()-1]=0;
                    }
                }
            }
            pant=i;
        }

        //Family with no predicates (only rdf:type)
        if (nPredicatesF >0){
            bmOF.resize(bmOF.size()+1);
            bmOF[bmOF.size()-1]=1;
        }

        bmfo->at(f).clear();
        fo->at(f).clear();
        vFamilias[f]->idPredicates.clear();
        vFamilias[f]->idRdfTypes.clear();
        vector <unsigned char>().swap(bmfo->at(f));
        vector <unsigned int>().swap(fo->at(f));
        vector <unsigned int>().swap(vFamilias[f]->idPredicates);
        vector <unsigned int>().swap(vFamilias[f]->idRdfTypes);
        delete vFamilias[f];
    }

    fo->clear();
    bmfo->clear();
    vector <Family*>().swap(vFamilias);
    triplespp->setnPredicados(dictionary->getNpredicates());
    vector < vector <unsigned int> > vObjsEnPredicados(dictionary->getNobjects());
    int_vector<> *seqMapO_flat = new int_vector<>();
    bit_vector *bmMapO_flat = new bit_vector();

    for (size_t i = 0; i < dictionary->getNpredicates(); i++){
        if (i!=idRdfType-1){
            unsigned int localObjectID=0;
            for (std::map<unsigned int ,unsigned int>::iterator it=objectMapping[i].begin(); it!=objectMapping[i].end(); ++it){
                it->second=localObjectID;
                unsigned int objectID=it->first;
                if (vObjsEnPredicados[objectID-1].size()==0){
                    vector <unsigned int> seqPredicados(1,i+1);
                    vObjsEnPredicados[objectID-1]=seqPredicados;
                } else {
                    vObjsEnPredicados[objectID-1].push_back(i+1);
                }
                localObjectID++;
            }
        }
    }

    size_t pos=0;
    for (size_t i=0; i<vObjsEnPredicados.size(); i++){
        if (vObjsEnPredicados[i].size()>0){
            size_t s = vObjsEnPredicados[i].size();
            seqMapO_flat->resize(seqMapO_flat->size()+s);
            bmMapO_flat->resize(bmMapO_flat->size()+s);
            for (size_t j=0; j<s; j++){
                (*seqMapO_flat)[pos]=vObjsEnPredicados[i][j];
                if (j==s-1){
                    (*bmMapO_flat)[pos]=1;
                } else{
                    (*bmMapO_flat)[pos]=0;
                }
                pos++;
            }
        } else {
            seqMapO_flat->resize(seqMapO_flat->size()+1);
            bmMapO_flat->resize(bmMapO_flat->size()+1);
            (*seqMapO_flat)[pos]=0;
            (*bmMapO_flat)[pos]=1;
            pos++;
        }
        vObjsEnPredicados[i].clear();
        vector <unsigned int>().swap(vObjsEnPredicados[i]);
    }
    vector < vector <unsigned int> >().swap(vObjsEnPredicados);
    util::bit_compress(*seqMapO_flat);
    AL *MapO_flat = new AL(seqMapO_flat, bmMapO_flat);
    triplespp->setMapO_flat(MapO_flat);
    setObjectsSequences(&objectMapping, &So_globalIds, &bmOF, &bmOS);
}

void HDTpp::setObjectsSequences(vector<map<unsigned int, unsigned int> > *m,  vector<unsigned int> *o, bit_vector *bOF, bit_vector *bOS){
    vector <AL *> Ps_flat(dictionary->getNpredicates());
    vector < int_vector<>* > So_flat;
    vector <bit_vector*> Bo_flat;
    vector <bit_vector*> Bf_flat;

    for (size_t i = 0; i < dictionary->getNpredicates(); i++){
        int_vector<> *so = new int_vector<>();
        bit_vector *bo = new bit_vector();
        bit_vector *bf = new bit_vector();
        if (i==idRdfType-1){
            so->resize(1);
            bo->resize(1);
            bf->resize(1);
            (*so)[0]=0;
            (*bo)[0]=1;
            (*bf)[0]=1;
       }
        So_flat.push_back(so);
        Bo_flat.push_back(bo);
        Bf_flat.push_back(bf);
  }
  size_t j=0;
  for (size_t i=0; i<triplespp->getFamilies_flat()->size(); i++){
        unsigned int predicateID = triplespp->getFamilies_flat()->access(i);
        if (predicateID!=0){
            //predicateID=0 => Family with no predicates
            do{
                unsigned int localObjectID = m->at(predicateID-1).find(o->at(j))->second;
                size_t s =So_flat[predicateID-1]->size();
                So_flat[predicateID-1]->resize(s+1);
                (*So_flat[predicateID-1])[s]=localObjectID;
                Bf_flat[predicateID-1]->resize(s+1);
                (*Bf_flat[predicateID-1])[s]=(*bOF)[j];
                Bo_flat[predicateID-1]->resize(s+1);
                (*Bo_flat[predicateID-1])[s]=(*bOS)[j];
                j++;
            } while ((*bOF)[j-1]==0);
        }
    }
    m->clear();
    o->clear();
    util::clear(*bOF);
    util::clear(*bOS);
    for (size_t i=0; i<So_flat.size(); i++){
        util::bit_compress(*So_flat[i]);
        Ps_flat[i] = new AL(So_flat[i], Bo_flat[i]);
    }
    triplespp->setPs_flat(Ps_flat);
    triplespp->setBf_flat(Bf_flat);
}

void HDTpp::saveToHDTpp(const char *fileName, ProgressListener *listener)
{
    try {
        ofstream out(fileName, ios::binary | ios::out | ios::trunc);
        if(!out.good()){
            throw "Error opening file to save HDT++.";
        }
        this->saveToHDTpp(out, listener);
        out.close();
    } catch (const char *ex) {
        throw ex;
    }
}

void HDTpp::saveToHDTpp(std::ostream & output, ProgressListener *listener)
{
    saveHeaderToHDTpp(output);
    saveDictionaryToHDTpp(output);
    saveTriplesToHDTpp(output);
}

void HDTpp::saveHeaderToHDTpp(std::ostream & output, ProgressListener *listener)
{
    ControlInformation controlInformation;
    IntermediateListener iListener(listener);
    controlInformation.clear();
    controlInformation.setType(GLOBAL);
    controlInformation.setFormat(HDTVocabulary::HDT_CONTAINER);
    controlInformation.save(output);
    controlInformation.clear();
    controlInformation.setType(HEADER);
    iListener.setRange(0,100);
    header->save(output, controlInformation, &iListener);
}

void HDTpp::saveDictionaryToHDTpp(std::ostream & output, ProgressListener *listener)
{
    ControlInformation controlInformation;
    IntermediateListener iListener(listener);
    controlInformation.clear();
    controlInformation.setType(DICTIONARY);
    iListener.setRange(5,70);
    dictionary->save(output, controlInformation, &iListener);
}

void HDTpp::saveTriplesToHDTpp(std::ostream & output, ProgressListener *listener)
{
    ControlInformation controlInformation;
    IntermediateListener iListener(listener);
    controlInformation.clear();
    controlInformation.setType(TRIPLES);
    iListener.setRange(70,100);
    triplespp->save(output);
}


void HDTpp::saveToRDF(RDFSerializer &serializer, ProgressListener *listener)
{
    TripleID tid;
    tid.setSubject(0);
    tid.setPredicate(0);
    tid.setObject(0);
    IteratorTripleID *it = this->getTriples()->search(tid);
    TripleIDStringIterator* itStr = new TripleIDStringIterator(dictionary, it);
    serializer.serialize(itStr, listener);
    delete it;
}

void HDTpp::decode() {
    /*
     *
     *  JUST FOR TESTING PURPOSES
     *
     */

    Families *families = triplespp->getFamilies();
    MapS *mapS = triplespp->getMapS();
    Types *types = triplespp->getTypes();
    MapO *mapO = triplespp->getMapO();
    Ps *ps = triplespp->getPs();
    size_t nFamilies = families->getNumberOfFamilies();
    size_t nPredicates = dictionary->getNpredicates();

    out.open("/home/antonio/triples_hdtpp.txt", ios::out);
    StopWatch sw;
    for (size_t predicate=1; predicate <= nPredicates; predicate++){
       families->getFamilies(predicate);
       if (!families->hasMoreFamilies()){
           // Types
           for (size_t familyID=1; familyID <= nFamilies; familyID++){
               if (types->hasTypes(familyID)){
                   mapS->getSubjects(familyID);
                   while (mapS->hasMoreSubjects()){
                       uint subject = mapS->nextSubject();
                       types->getTypes(familyID);
                       while (types->hasMoreTypes()){
                            unsigned int type = types->nextType();
                            std::stringstream ss;
                            ss<<subject<<"-"<<predicate<<"-"<<type<<endl;
                            std::string t=ss.str();
                            out<<t;
                       }
                   }
               }
           }
       } else {
           ps->getLocalObjects(predicate);
           while (families->hasMoreFamilies()){
               unsigned int familyID = families->nextFamily();
               mapS->getSubjects(familyID);
               while (mapS->hasMoreSubjects()){
                   uint subject = mapS->nextSubject();
                   do{
                        unsigned int localObjectID = ps->nextLocalObject(predicate);
                        unsigned int object = mapO->getDictID(predicate,localObjectID);
                        std::stringstream ss;
                        ss<<subject<<"-"<<predicate<<"-"<<object<<endl;
                        std::string t=ss.str();
                        out<<t;
                   } while (!ps->isNewSubjectList(predicate));
               }
           }
       }
   }
   cout<<endl<<"Decompression time: "<<sw<<endl;
   out.close();
}

void HDTpp::extractPSO(const char *outputFile) {
    Families *families = triplespp->getFamilies();
    MapS *mapS = triplespp->getMapS();
    Ps *ps = triplespp->getPs();
    FILE *file;
    file = fopen(outputFile, "w+" );

    for (int32_t p=1; p<=dictionary->getNpredicates(); p++){
       families->getFamilies(p);
       if (!families->hasMoreFamilies()){
           ps->getLocalObjects(p);
           while (families->hasMoreFamilies()){
               unsigned int familyID = families->nextFamily();
               int32_t s=1;
               mapS->getSubjects(familyID);
               while (mapS->hasMoreSubjects()){
                   mapS->nextSubject();
                   do{
                        int32_t idLocalObject = ps->nextLocalObject(p);
                        int32_t triple[3] = {p, s, idLocalObject};
                        fwrite(triple, sizeof(int32_t), 3, file);
                   } while (!ps->isNewSubjectList(p));
                   s++;
               }
           }
       }
   }

   fclose(file);

}


void HDTpp::constructMemoryStructures(bool TP) {
    this->getTriples()->constructMemoryStructures(TP);
    if (TP){
        string fileiobj=this->filename+".iobj";
        ifstream f(fileiobj.c_str());
        if (f.good()){
			cout<<"loading iObjects "<<fileiobj<<endl;
            this->getTriples()->loadiObjects(f);
        } else {
            ofstream out(fileiobj.c_str(), ios::binary | ios::out | ios::trunc);
            if(!out.good()){
                throw "Error opening file to save iObjects.";
            }
			cout<<"building iObjects"<<fileiobj<<endl;
            this->getTriples()->buildiObjects(out);
            out.close();

        }
        f.close();
    }
}

void HDTpp::createComponents() {
    header = new PlainHeader();
    dictionary = new FourSectionDictionary();
    triplespp = new BitmapTriplespp();
}

void HDTpp::deleteComponents() {
    if (header){
        delete header;
        header = NULL;
    }

    if (triplespp){
        delete triplespp;
        triplespp = NULL;
    }

    if (triples){
        delete triples;
        triples = NULL;
    }

    if (dictionary){
        delete dictionary;
        dictionary = NULL;
    }

}

HDTpp::~HDTpp()
{
    deleteComponents();
}

}

