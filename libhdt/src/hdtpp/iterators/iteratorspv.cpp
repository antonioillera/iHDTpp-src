#include "../BitmapTriplespp.hpp"
namespace hdt {

iteratorspv::iteratorspv(BitmapTriplespp *trip, TripleID &pat, size_t idfam) :
    triples(trip),
    pattern(pat),
    family(idfam)
{

    predicate = pat.getPredicate();
    subject = pat.getSubject();
    if (!family) family = trip->getMapS()->getFamily(subject);
    size_t localSubject = trip->getMapS()->getLocalSubject(subject,family);
    int idf = triples->getFamilies()->getPosition(family, predicate);
    if (idf!=-1){
        triples->getPs()->getObjSubject(predicate, idf, localSubject);
    } else more = false;
}

void iteratorspv::updateOutput() {
    returnTriple.setAll(subject,predicate,object);
}

bool iteratorspv::hasNext()
{
    return more;
}

TripleID *iteratorspv::next(){
    size_t localObject = triples->getPs()->nextLocalObject(predicate);
    object = triples->getMapO()->getDictID(predicate, localObject);
    if (!triples->getPs()->hasMoreObjects(predicate)) more = false;
    updateOutput();
    return &returnTriple;
}
}
