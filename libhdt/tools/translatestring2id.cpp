
#include <../include/HDT.hpp>
#include <../include/HDTManager.hpp>

#include <string>
#include <getopt.h>
#include <iostream>
#include <fstream>


#include "../src/triples/BitmapTriples.hpp"
#include "../src/util/StopWatch.hpp"


using namespace hdt;
using namespace std;

void split(const string& s, char c, vector<string>& v) {
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



int main(int argc, char **argv) {

    if (argc < 5) {
        printf("Usage: %s <HDTFile> <StringFile> <IDsFile> <number of lines>\n", argv[0]);
        exit(1);
    }

    string inputFile = argv[optind];
    string strFile = argv[optind+1];
    string idFile = argv[optind+2];

    int nqueries = atoi(argv[4]);

    if(inputFile=="") {
        cout << "ERROR: You must supply an HDT input file" << endl << endl;
        return 1;
    }

    if(strFile=="") {
        cout << "ERROR: You must supply an file of ID triples " << endl << endl;
        return 1;
    }


    try {
        HDT *hdt = HDTManager::loadHDT(inputFile.c_str());
        cout<<"aqui"<<endl;
        ofstream out (idFile.c_str());
        std::ifstream fqueries(strFile);
        std::string line;
        vector<string> v;
        for (int i = 0; i < nqueries; i++) {
            v.clear();
            std::getline(fqueries, line);
            cout<<line<<endl;
            split(line, ';', v);
            cout<<"-----------------"<<endl<<v[2]<<endl;
            cout<<"-----------------"<<hdt->getDictionary()->stringToId(v[2],OBJECT)<<endl;
            std::stringstream ss;
            ss<<hdt->getDictionary()->stringToId(v[0],SUBJECT)<<";"<<hdt->getDictionary()->stringToId(v[1],PREDICATE)<<";"<<hdt->getDictionary()->stringToId(v[2],OBJECT)<<endl;
            cout<<hdt->getDictionary()->stringToId(v[0],SUBJECT)<<";"<<hdt->getDictionary()->stringToId(v[1],PREDICATE)<<";"<<hdt->getDictionary()->stringToId(v[2],OBJECT)<<endl;
            std::string text=ss.str();
            out<<text;

        }
        cout<<"-----------------"<<endl<<hdt->getDictionary()->idToString(2111,OBJECT)<<endl;
        out.close();
        fqueries.close();
        delete hdt;

    } catch (char *exception) {
        cerr << "ERROR: " << exception << endl;
    } catch (const char *e) {
        cout << "ERROR: " << e << endl;
    }

}

