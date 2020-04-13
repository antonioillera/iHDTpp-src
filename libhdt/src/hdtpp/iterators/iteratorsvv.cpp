#include "../BitmapTriplespp.hpp"
namespace hdt {

iteratorsvv::iteratorsvv(BitmapTriplespp *trip, TripleID &pat) :
    triples(trip),
    pattern(pat)
{
    family = trip->getMapS()->getFamily(pat.getSubject());
    if (trip->getFamilies()->hasPredicates(family)){
        trip->getFamilies()->getPredicates(family);
        predicate = trip->getFamilies()->nextPredicate();
        pattern.setPredicate(predicate);
        it = new iteratorspv(triples, pattern, family);
    } else {
        moreT =false;
        pattern.setPredicate(triples->getIdRDFType());
        it = new iteratorstv(triples, pattern, family);
    }
}

void iteratorsvv::updateOutput() {
    returnTriple.setAll(subject,predicate,object);
}

bool iteratorsvv::hasNext()
{
    return more;
}

TripleID *iteratorsvv::next(){
    TripleID *t = it->next();
     if (!it->hasNext()){
        delete it;
        if (triples->getFamilies()->hasMorePredicates()){
            predicate = triples->getFamilies()->nextPredicate();
            pattern.setPredicate(predicate);
            it = new iteratorspv(triples, pattern, family);
        } else if (moreT){
            moreT=false;
            pattern.setPredicate(triples->getIdRDFType());
            it = new iteratorstv(triples, pattern, family);
            if (!it->hasNext()) more = false;
        } else more =false;
     }
    return t;
}
}
