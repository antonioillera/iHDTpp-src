#include "../BitmapTriplespp.hpp"
namespace hdt {

iteratorsto::iteratorsto(BitmapTriplespp *trip, TripleID &pat, size_t idfam) :
    triples(trip),
    pattern(pat),
    family(idfam)
{
    predicate = pat.getPredicate();
    subject = pat.getSubject();
    object = pat.getObject();

    if (!family) family = trip->getMapS()->getFamily(subject);
    more = trip->getTypes()->findType(family, object);

}

void iteratorsto::updateOutput() {
    returnTriple.setAll(subject,predicate,object);
}

bool iteratorsto::hasNext()
{
    return more;
}

TripleID *iteratorsto::next(){
    more = false;
    updateOutput();
    return &returnTriple;
}
}
