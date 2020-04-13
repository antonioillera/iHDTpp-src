#include <HDT.hpp>
#include <HDTManager.hpp>

#include <string>
#include <getopt.h>
#include <iostream>
#include <fstream>

#include "../src/triples/BitmapTriples.hpp"
#include "../src/util/StopWatch.hpp"


using namespace hdt;
using namespace std;
void split(const string& s, char c,
           vector<string>& v) {
   string::size_type i = 0;
   string::size_type j = s.find(c);

   while (j != string::npos) {
      v.push_back(s.substr(i, j-i));
      i = ++j;
      j = s.find(c, j);

      if (j == string::npos)
         v.push_back(s.substr(i, s.length()));
   }
}
void help() {
    cout << "$ rdf2hdt [options] <rdf input file> <hdt output file> " << endl;
    cout << "\t-h\t\t\tThis help" << endl;
    cout << "\t-i\t\tAlso generate index to solve all triple patterns." << endl;
    cout << "\t-c\t<configfile>\tHDT Config options file" << endl;
    cout << "\t-o\t<options>\tHDT Additional options (option1=value1;option2=value2;...)" << endl;
    cout << "\t-f\t<format>\tFormat of the RDF input (ntriples, nquad, n3, turtle, rdfxml)" << endl;
    cout << "\t-B\t\"<base URI>\"\tBase URI of the dataset." << endl;
    //cout << "\t-v\tVerbose output" << endl;
}

class ConvertProgress : public ProgressListener {
private:
public:
    virtual ~ConvertProgress() { }

    void notifyProgress(float level, const char *section) {
        cout << section << ": " << level << " %";
        cout << "\r " << section << ": " << level << " %                      \r";
        cout.flush();
    }

};


int main(int argc, char **argv) {
    int c;
    string rdfFormat, inputFile, outputFile;
    RDFNotation notation = NTRIPLES;

    while( (c = getopt(argc,argv,"f:"))!=-1) {
        switch(c) {
        case 'f':
            rdfFormat = optarg;
            cout << "Format: " << rdfFormat << endl;
            break;
        default:
            cout << "ERROR: Unknown option" << endl;
            help();
            return 1;
        }
    }

    if(rdfFormat!="") {
        if(rdfFormat=="ntriples") {
            notation = NTRIPLES;
        } else if(rdfFormat=="n3") {
            notation = N3;
        } else if(rdfFormat=="turtle") {
            notation = TURTLE;
        } else if(rdfFormat=="rdfxml") {
            notation = XML;
        } else {
            cout << "ERROR: The RDF output format must be one of: (ntriples, n3, turtle, rdfxml)" << endl;
            help();
            return 1;
        }
    }

    inputFile = argv[optind];

    if(inputFile=="") {
        cout << "ERROR: You must supply an HDT input file" << endl << endl;
        help();
        return 1;
    }

    try {
        //HDT *hdt = HDTManager::mapHDT(inputFile.c_str());

        HDT *hdt = HDTManager::loadIndexedHDT(inputFile.c_str());
        string rdftypeLiteral = "http://www.w3.org/1999/02/22-rdf-syntax-ns#type";
        unsigned int idRDFType = hdt->getDictionary()->stringToId(rdftypeLiteral, PREDICATE);
        cout<<"idRDFType = " << idRDFType<<endl;

        ofstream out2 ("/home/antonio/vvo-hdt.txt");
        /*TripleID tid;
        tid.setSubject(23);
        tid.setPredicate(17);
        tid.setObject(0);
        IteratorTripleID *it=hdt->getTriples()->search(tid);
        while (it->hasNext()) {
            TripleID *t = it->next();
            //cout<<t->getSubject()<<"-"<<t->getPredicate()<<"-"<<t->getObject()<<endl;
            std::stringstream ss;
            ss<<t->getSubject()<<"-"<<t->getPredicate()<<"-"<<t->getObject()<<endl;
            std::string text=ss.str();
            out2<<text;
        }
        delete it;
        */

        TripleID tid;
        //for (int i=1; i<=hdt->getDictionary()->getNsubjects(); i++){
            //for (int j=1; j<=hdt->getDictionary()->getNpredicates(); j++){
               // if (j==idRDFType) continue;
                for (int k=1; k<=hdt->getDictionary()->getNobjects(); k++){
            tid.setSubject(0);
            tid.setPredicate(0);
            tid.setObject(k);
            IteratorTripleID *it=hdt->getTriples()->search(tid);
            while (it->hasNext()) {
                TripleID *t = it->next();
                //cout<<t->getSubject()<<"-"<<t->getPredicate()<<"-"<<t->getObject()<<endl;
                std::stringstream ss;
                ss<<t->getSubject()<<"-"<<t->getPredicate()<<"-"<<t->getObject()<<endl;
                std::string text=ss.str();
                out2<<text;
            }
            delete it;


       }//}//}






/*
        IteratorTripleID *it = hdt->getTriples()->searchAll();
        StopWatch sw;
        while (it->hasNext()) {
            TripleID *tid = it->next();
            std::stringstream ss;
            ss<<tid->getSubject()<<"-"<<tid->getPredicate()<<"-"<<tid->getObject()<<endl;
            std::string text=ss.str();
            out2<<text;
        }
        */
        out2.close();

       delete hdt;
    } catch (char *exception) {
        cerr << "ERROR: " << exception << endl;
    } catch (const char *e) {
        cout << "ERROR: " << e << endl;
    }

}





