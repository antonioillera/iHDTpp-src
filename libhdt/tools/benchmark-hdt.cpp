
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

typedef struct squery {
    int s;
    int p;
    int o;
    int expectednres;
} tquery;

tquery * readQueries(const char *filename, int nQueries);
tquery * readQueries2(const char *filename,  int *nQueries);
void alighnnum(char *str, int len);


#define LOGTIMES
/**/ FILE *gnuFile;
/**/ char operation[10];
/**/ char *indexbasename;

int main(int argc, char **argv) {
    int c;
    string rdfFormat, inputFile, queryFile;

    if (argc < 4) {
        printf("Usage: %s <HDTFile> <queryFile> <iters> [<numQueries>]\n", argv[0]);
        exit(1);
    }

    inputFile = argv[optind];
    queryFile = argv[optind+1];
    int iters = atoi(argv[3]);
    int nqueries=-1;
    if (argc == 5) nqueries = atoi(argv[4]);

    if(inputFile=="") {
        cout << "ERROR: You must supply an HDT input file" << endl << endl;
        return 1;
    }

    if(queryFile=="") {
        cout << "ERROR: You must supply an query file" << endl << endl;
        return 1;
    }

    tquery * queries;
    if (nqueries == -1)
        queries = readQueries2(queryFile.c_str(),&nqueries);
    else
        queries = readQueries(queryFile.c_str(),nqueries);

    if (!queries) {
        exit(0);
    }
    else
        fprintf(stderr,"\nI have read %d queries. \n",nqueries);

    try {
        HDT *hdt = HDTManager::loadIndexedHDT(inputFile.c_str());
        int z, i,totalres;
        cout<<"Size triples = "<<hdt->getTriples()->size()<<" Bytes"<<endl;
        StopWatch sw;
        for (z = 0; z < iters; z++) {
            totalres = 0;
            for (i = 0; i < nqueries; i++) {
                tquery query = queries[i];
                TripleID tid;
                tid.setSubject(query.s);
                tid.setPredicate(query.p);
                tid.setObject(query.o);
                IteratorTripleID *it=hdt->getTriples()->search(tid);
                while (it->hasNext()) {
                    TripleID *t = it->next();
                    totalres++;
                }
                delete it;
            }
        }
        sw.stop();

        cout<<sw.getRealStr()<<endl;
        ulong microsecs = sw.getReal()/iters; //to microsecsprint
        printf("\n <index>\t<query-set>\t<microsec>\t<nqueries>\t<totalres>\t<microsec/query>\t<microsec/totalres>");
        printf("\n %s\t%s\t%ld\t%d\t%lu\t%lf\t%lf\n", argv[1], argv[2], microsecs, nqueries, (ulong)totalres, (double)microsecs/nqueries, (double)microsecs/totalres);

        delete hdt;

    } catch (char *exception) {
        cerr << "ERROR: " << exception << endl;
    } catch (const char *e) {
        cout << "ERROR: " << e << endl;
    }

}



tquery * readQueries(const char *filename, int nQueries) {
    unsigned int i;

    FILE * fqueries;
    if ((fqueries = fopen(filename, "r")) == NULL) {
            fprintf(stderr, "error: no se pudo abrir el fichero %s\n", filename);
        return NULL;
    }

    tquery *queries = (tquery *) malloc (nQueries * sizeof(tquery));
    for (i=0; i<nQueries;i++) {
        if (-1==fscanf(fqueries,"%d;%d;%d;%d\n",  &queries[i].s,&queries[i].p,&queries[i].o,&queries[i].expectednres)){
            fprintf(stderr,"error al leer queries\n");exit(1);
        }
    }
    fclose(fqueries);
    if (i==nQueries)
        return queries;

    free(queries);
    fclose(fqueries);
    return NULL;
}



tquery * readQueries2(const char *filename, int *nQueries) {
    unsigned int i=0;

    FILE * fqueries;
    if ((fqueries = fopen(filename, "r")) == NULL) {
            fprintf(stderr, "error: no se pudo abrir el fichero %s\n", filename);
        return NULL;
    }

    int maxQ=2; *nQueries=0;
    tquery *queries = (tquery *) malloc (maxQ * sizeof(tquery));

    while  (EOF!=fscanf(fqueries,"%d;%d;%d;%d\n",  &queries[i].s,&queries[i].p,&queries[i].o,&queries[i].expectednres)){
        (*nQueries)++;
        if (*nQueries >= maxQ) {
            maxQ *=2;
            queries = (tquery *) realloc(queries,maxQ * sizeof(tquery));
        }
        i++;
    }
    fclose(fqueries);

    return queries;
}



/* used for gnuplot formatting file */
void alighnnum(char *str, int len){
    int n=strlen(str);
    int i;
    if (n<len) {
      int shift = len-n;
      for (i=len-1; i>=shift;i--)
         str[i] = str[i-shift];
      for (i=0;i<shift;i++) str[i]=' ';

    }
}

