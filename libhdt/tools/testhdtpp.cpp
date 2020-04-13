
#include <HDT.hpp>
#include <HDTManager.hpp>
#include "../src/hdtpp/HDTpp.hpp"

#include <string>
#include <getopt.h>
#include <iostream>
#include <fstream>

using namespace hdt;

void help() {
    cout << "$ leehdtpp  <rdf input file> " << endl;
        cout << "\t-h\t\t\tThis help" << endl;
}


int main(int argc, char **argv) {

    string inputFile;
        inputFile = argv[optind];

        if(inputFile=="") {
        cout << "ERROR: You must supply an HDT++ input file" << endl << endl;
                help();
                return 1;
        }

        try {



        //----------- DECODE -------------
        /*HDTpp *hdtpp = new HDTpp(inputFile.c_str());
        hdtpp->constructMemoryStructures(false);
        hdtpp->decode();
        cout<<hdtpp->getTriples()->getMapS()->sizeBytes()<<endl;
        delete hdtpp;*/
        //----------- DECODE -------------


        HDTpp *hdtpp = new HDTpp(inputFile.c_str());
        hdtpp->constructMemoryStructures(true);
        string rdftypeLiteral = "http://www.w3.org/1999/02/22-rdf-syntax-ns#type";
        unsigned int idRDFType = hdtpp->getDictionary()->stringToId(rdftypeLiteral, PREDICATE);
        ofstream out2 ("/home/antonio/vpo-hdtpp.txt");
        /*
        TripleID tid;
        tid.setSubject(0);
        tid.setPredicate(1);
        tid.setObject(3);
        IteratorTripleID *it=hdtpp->getTriples()->search(tid);
        while (it->hasNext()) {
            TripleID *t = it->next();
            cout<<t->getSubject()<<"-"<<t->getPredicate()<<"-"<<t->getObject()<<endl;
            std::stringstream ss;
            ss<<t->getSubject()<<"-"<<t->getPredicate()<<"-"<<t->getObject()<<endl;
            std::string text=ss.str();
            out2<<text;
        }
        delete it;
        out2.close();
        delete hdtpp;
        */


        TripleID tid;
        //for (int i=1; i<=hdtpp->getDictionary()->getNsubjects(); i++){
            for (int j=1; j<=hdtpp->getDictionary()->getNpredicates(); j++){
               //if (j==idRDFType) continue;
            for (int k=1; k<=hdtpp->getDictionary()->getNobjects(); k++){
            tid.setSubject(0);
            tid.setPredicate(j);
            tid.setObject(k);
            //cout<<"?-"<<j<<"-"<<k<<endl;
            IteratorTripleID *it=hdtpp->getTriples()->search(tid);
            while (it->hasNext()) {
                TripleID *t = it->next();
                //cout<<t->getSubject()<<"-"<<t->getPredicate()<<"-"<<t->getObject()<<endl;
                std::stringstream ss;
                ss<<t->getSubject()<<"-"<<t->getPredicate()<<"-"<<t->getObject()<<endl;
                std::string text=ss.str();
                out2<<text;
            }
            delete it;
       }}//}
        out2.close();
        delete hdtpp;

        /*
        ifstream fe("/home/antonio/triples-hdt.txt");
        size_t sid,pid,oid;
        char cadena[128];
        std::string delimiter = "-";
        while(!fe.eof()) {
            fe.getline(cadena, 128);
            std::string s;
            s+= cadena;
            if (s.length()>0){
                size_t pos = 0;
                std::string token;
                pos = s.find(delimiter);
                sid = std::stoul(s.substr(0, pos),nullptr,0);
                s.erase(0, pos + delimiter.length());
                pos = s.find(delimiter);
                pid = std::stoul(s.substr(0, pos),nullptr,0);
                s.erase(0, pos + delimiter.length());
                oid = std::stoul(s,nullptr,0);
                TripleID tid;

                tid.setSubject(sid);
                tid.setPredicate(pid);
                tid.setObject(oid);
                //cout<<sid<<";"<<pid<<";"<<oid<<endl;
                IteratorTripleID *it=hdtpp->getTriples()->search(tid);

                while (it->hasNext()) {

                    TripleID *t = it->next();
                    //cout<<t->getSubject()<<"*"<<t->getPredicate()<<"*"<<t->getObject()<<endl;
                    //cout<<"-----------------------------"<<endl;
                    std::stringstream ss;
                    ss<<t->getSubject()<<"-"<<t->getPredicate()<<"-"<<t->getObject()<<endl;
                    std::string text=ss.str();
                    out2<<text;
                }
                delete it;
            }
          }
        fe.close();
        out2.close();
        delete hdtpp;
        */

        /*cout<<"Tamaños:"<<endl;
        cout<<"==========================="<<endl;
        cout<<"MapS = "<<hdtpp->getTriples()->getMapS()->sizeBytes()<<" Bytes"<<endl;
        cout<<"MapO = "<<hdtpp->getTriples()->getMapO()->sizeBytes()<<" Bytes"<<endl;
        cout<<"Families = "<<hdtpp->getTriples()->getFamilies()->sizeBytes()<<" Bytes"<<endl;
        cout<<"Types = "<<hdtpp->getTriples()->getTypes()->sizeBytes()<<" Bytes"<<endl;
        cout<<"Ps = "<<hdtpp->getTriples()->getPs()->sizeBytes()<<" Bytes"<<endl;
        cout<<"iObjects = "<<hdtpp->getTriples()->getIObjects()->sizeBytes()<<" Bytes"<<endl;
        cout<<"==========================="<<endl;
        */

/*

        TripleID tid;
        tid.setSubject(0);
        tid.setPredicate(0);
        tid.setObject(70);
        IteratorTripleID *it=hdtpp->getTriples()->search(tid);
        while (it->hasNext()) {
            TripleID *t = it->next();
            cout<<t->getSubject()<<"-"<<t->getPredicate()<<"-"<<t->getObject()<<endl;
        }
        delete it;
        */



        } catch (char *exception) {
                cerr << "ERROR: " << exception << endl;
        } catch (const char *e) {
                cout << "ERROR: " << e << endl;
        }

}



