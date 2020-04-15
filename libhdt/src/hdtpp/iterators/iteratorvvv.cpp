#include "../BitmapTriplespp.hpp"
namespace hdt {

iteratorvvv::iteratorvvv(BitmapTriplespp *trip, TripleID &pat) :
    triples(trip),
    pattern(pat)
{
    pattern.setPredicate(predicate);
    if (predicate==triples->getIdRDFType())
        it = new iteratorvtv(triples, pattern);
    else
        it = new iteratorvpv(triples, pattern);
}

bool iteratorvvv::hasNext(){
    return more;
}

TripleID *iteratorvvv::next(){
    TripleID *t = it->next();
    if (!it->hasNext()){
        delete it;
        it=NULL;
        if (++predicate > triples->getNPredicates()) more =false;
        else{
            pattern.setPredicate(predicate);
            if (predicate==triples->getIdRDFType())
                it = new iteratorvtv(triples, pattern);
            else
                it = new iteratorvpv(triples, pattern);
        }
    }
    return t;
}
}
