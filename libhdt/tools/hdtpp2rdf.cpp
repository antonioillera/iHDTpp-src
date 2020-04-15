#include "../src/hdtpp/HDTpp.hpp"

#include <string>
#include <getopt.h>
#include <iostream>
#include <fstream>

using namespace hdt;
using namespace std;

void help() {
    cout << "$ hdtpp2rdf [options] <HDT++ input file> <RDF output file> " << endl;
    cout << "\t-h\t\t\tThis help" << endl;
    cout << "\t-f\t<format>\tRDF Format of the output" << endl;
    cout << "\t-p\tPrints a progress indicator." << endl;
    cout << "\t-v\tVerbose output" << endl;
}

int main(int argc, char **argv) {
    int c;
    string rdfFormat, inputFile, outputFile;
    RDFNotation notation = NTRIPLES;
    bool verbose=false;
    bool showProgress=false;

    while( (c = getopt(argc,argv,"vpf:"))!=-1) {
        switch(c) {
        case 'v':
            verbose = true;
            break;
        case 'p':
            showProgress = true;
            break;
        case 'f':
            rdfFormat = optarg;
            break;
        default:
            cerr << "ERROR: Unknown option" << endl;
            help();
            return 1;
        }
    }

#define vout if (!verbose) {} else std::cerr /* Verbose output */

    if(argc-optind<2) {
        cerr << "ERROR: You must supply an input and output" << endl << endl;
        help();
        return 1;
    }

    if(rdfFormat!="") {
        if(rdfFormat=="ntriples") {
            notation = NTRIPLES;
        } else if(rdfFormat=="n3") {
            notation = TURTLE;
        } else if(rdfFormat=="turtle") {
            notation = TURTLE;
        } else {
            cerr << "ERROR: The RDF output format must be one of: (ntriples, n3, turtle)" << endl;
            help();
            return 1;
        }
        vout << "Format: " << rdfFormat << endl;
    }

    inputFile = argv[optind];
    outputFile = argv[optind+1];

    if(inputFile=="") {
        cerr << "ERROR: You must supply an HDT++ input file" << endl << endl;
        help();
        return 1;
    }

    if(outputFile=="") {
        cerr << "ERROR: You must supply an RDF output file" << endl << endl;
        help();
        return 1;
    }

    try {
        ProgressListener* progress = showProgress ? new StdoutProgressListener() : NULL;
        HDTpp *hdtpp = new HDTpp(inputFile.c_str());
        hdtpp->constructMemoryStructures(false);

        if(outputFile!="-") {
            RDFSerializer *serializer = RDFSerializer::getSerializer(outputFile.c_str(), notation);
            hdtpp->saveToRDF(*serializer);
            delete serializer;
        } else {
            RDFSerializer *serializer = RDFSerializer::getSerializer(cout, notation);
            hdtpp->saveToRDF(*serializer);
            delete serializer;
        }
        delete hdtpp;
        delete progress;
    } catch (std::exception& e) {
        cerr << "ERROR: " << e.what() << endl;
        return 1;
    }

}
