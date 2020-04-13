#ifndef HDTPP_H
#define HDTPP_H

#include "BitmapTriplespp.hpp"
#include <HDTVocabulary.hpp>
#include "../hdt/HDTFactory.hpp"
#include "../header/PlainHeader.hpp"
#include "../dictionary/FourSectionDictionary.hpp"
#include "../util/fileUtil.hpp"
#include "../util/StopWatch.hpp"

namespace hdt {

class HDTpp
{
public:

    HDTpp(HDT* hdt);
    HDTpp(const char *file, ProgressListener *listener=NULL);
    ~HDTpp();

    Header *getHeader(){return header;}
    Dictionary *getDictionary(){return dictionary;}
    BitmapTriplespp *getTriples(){return triplespp;}

    unsigned int getIdRdfType(){return idRdfType;}

    void saveToHDTpp(const char *fileName, ProgressListener *listener = NULL);
    void saveToHDTpp(std::ostream &out, ProgressListener *listener = NULL);
    void saveHeaderToHDTpp(std::ostream &out, ProgressListener *listener = NULL);
    void saveDictionaryToHDTpp(std::ostream &out, ProgressListener *listener = NULL);
    void saveTriplesToHDTpp(std::ostream &out, ProgressListener *listener = NULL);
    void saveIndex(ProgressListener *listener = NULL);

    void saveToRDF(std::ostream &out);
    void saveToRDF(RDFSerializer &serializer);
    void decode();
    void extractPSO(const char *outputFile);
    unsigned int getidRdfType(){return idRdfType;}
	void constructMemoryStructures(bool TP);
	
private:

    //Header and Dictionary in HDT++ are the same as HDT
    Header *header = NULL;
    Dictionary *dictionary = NULL;
    BitmapTriples *triples = NULL;
    BitmapTriplespp *triplespp = NULL;
	string filename;
    ofstream out;
    unsigned int idRdfType = 0;

    //Family structure
    struct Family{
        unsigned int familyID;
        vector <unsigned int> idPredicates; // Predicates belonging to the family
        vector <unsigned int> idRdfTypes;  // Types related to the family
        unsigned int nSubjects=1;           //Number of subjects pointing to the family
    };
    vector <Family*> vFamilias;

    void extractFamilies(); //Extracts predicate families from HDT
    void setTypes(); //Extracts predicate families from HDT
    void setFamilies();
    void setMapO(vector < vector <unsigned int> > *fo, vector < vector <unsigned char> > *bmfo);
    void setObjectsSequences(vector < map < unsigned int,unsigned int > > *m, vector<unsigned int> *o, bit_vector *bOF, bit_vector *bOS); //Creates sequences for HDT++

    void deleteComponents();
    void createComponents();
    void loadFromHDTpp(std::istream & input, ProgressListener *listener);


};
}

#endif // HDTPP_H
