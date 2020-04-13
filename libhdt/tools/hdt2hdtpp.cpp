#include <HDTManager.hpp>
#include "../src/hdtpp/HDTpp.hpp"

using namespace hdt;

void help() {
        cout <<endl<< "$ hdt2hdtpp <hdt input file> <hdt++ output file> " << endl;
}

int main(int argc, char **argv) {
        int c;
        string inputFile, outputFile;

        if(argc-optind<2) {
                cout << "ERROR: You must supply an input and output" << endl << endl;
                help();
                return 1;
        }

        inputFile = argv[optind];
        outputFile = argv[optind+1];

        if(inputFile=="") {
                cout << "ERROR: You must supply an HDT input file" << endl << endl;
                help();
                return 1;
        }

        if(outputFile=="") {
                cout << "ERROR: You must supply an HDT++ output file" << endl << endl;
                help();
                return 1;
        }

        try {
                HDT *hdt = HDTManager::loadHDT(inputFile.c_str());
                HDTpp *hdtpp = new HDTpp(hdt);
                hdtpp->saveToHDTpp(argv[optind+1]);
                delete hdtpp;
        } catch (char *exception) {
                cerr << "ERROR: " << exception << endl;
        } catch (const char *e) {
                cout << "ERROR: " << e << endl;
        }

}
