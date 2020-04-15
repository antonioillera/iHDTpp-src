#include "../BitmapTriplespp.hpp"
namespace hdt {

iteratorsvo::iteratorsvo(BitmapTriplespp *trip, TripleID &pat) :
    triples(trip),
    pattern(pat)
{
    subject = pattern.getSubject();
    object = pattern.getObject();
    triples->getMapO()->getPredicates(object);
    if (triples->getMapO()->hasPredicates()){
        while (triples->getMapO()->hasMorePredicates()) {
            predicate = triples->getMapO()->nextPredicate();
            pattern.setPredicate(predicate);
            it = new iteratorspo(triples, pattern);
            if (it->hasNext()) break;
            delete it;
            it = NULL;
        }
    }
    if (it==NULL){
        predicate = triples->getIdRDFType();
        pattern.setPredicate(triples->getIdRDFType());
        it = new iteratorsto(triples, pattern);
        if (!it->hasNext()){
            more = false;
            delete it;
            it=NULL;
        }
    }
}

bool iteratorsvo::hasNext()
{
    return more;
}

TripleID *iteratorsvo::next(){
    TripleID *t = it->next();
    delete it;
    it = NULL;
    if (predicate == triples->getIdRDFType()) more = false;
    else{
        while (triples->getMapO()->hasMorePredicates()) {
            predicate = triples->getMapO()->nextPredicate();
            pattern.setPredicate(predicate);
            it = new iteratorspo(triples, pattern);
            if (it->hasNext()) break;
            delete it;
            it = NULL;
        }
        if (it==NULL){
            predicate = triples->getIdRDFType();
            pattern.setPredicate(triples->getIdRDFType());
            it = new iteratorsto(triples, pattern);
            if (!it->hasNext()){
                more = false;
                delete it;
                it=NULL;
            }
        }
    }
    return t;
}


}
