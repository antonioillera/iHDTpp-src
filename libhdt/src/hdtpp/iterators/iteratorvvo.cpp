
#include "../BitmapTriplespp.hpp"

namespace hdt {

iteratorvvo::iteratorvvo(BitmapTriplespp *trip, TripleID &pat) :
    triples(trip),
    pattern(pat)
{
    object = pat.getObject();
    triples->getMapO()->getPredicates(object);
    if (triples->getMapO()->hasPredicates()){
        predicate=triples->getMapO()->nextPredicate();
        pattern.setPredicate(predicate);
        it = new iteratorvpo(triples, pattern,triples->getMapO()->getCurrentPosition());
    } else {
        predicate = triples->getIdRDFType();
        pattern.setPredicate(predicate);
        it = new iteratorvto(triples, pattern);
        if (!it->hasNext()){
            more = false;
            delete it;
            it=NULL;
        }
    }
}

void iteratorvvo::updateOutput() {
    returnTriple.setAll(subject,predicate,object);
}

bool iteratorvvo::hasNext()
{
    return more;
}

TripleID *iteratorvvo::next(){
    TripleID *t = it->next();
    if (!it->hasNext()){
        delete it;
        it=NULL;
        if (predicate == triples->getIdRDFType()) more = false;
        else if (triples->getMapO()->hasMorePredicates()){
            predicate=triples->getMapO()->nextPredicate();
            pattern.setPredicate(predicate);
            it = new iteratorvpo(triples, pattern, triples->getMapO()->getCurrentPosition());
        } else {
            predicate = triples->getIdRDFType();
            pattern.setPredicate(predicate);
            it = new iteratorvto(triples, pattern);
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
