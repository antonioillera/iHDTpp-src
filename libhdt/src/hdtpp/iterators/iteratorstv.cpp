
#include "../BitmapTriplespp.hpp"
namespace hdt {

iteratorstv::iteratorstv(BitmapTriplespp *trip, TripleID &pat, size_t idfam) :
    triples(trip),
    pattern(pat),
    family(idfam)
{
    predicate = pat.getPredicate();
    subject = pat.getSubject();
    if (!family) family = trip->getMapS()->getFamily(subject);
    if (!trip->getTypes()->hasTypes(family)) more=false;
    else trip->getTypes()->getTypes(family);
}

void iteratorstv::updateOutput() {
    returnTriple.setAll(subject,predicate,object);
}

bool iteratorstv::hasNext()
{
    return more;
}

TripleID *iteratorstv::next(){
    object = triples->getTypes()->nextType();
    more = triples->getTypes()->hasMoreTypes();
    updateOutput();
    return &returnTriple;
}
}
