#include "../BitmapTriplespp.hpp"
namespace hdt {

iteratorspo::iteratorspo(BitmapTriplespp *trip, TripleID &pat, size_t idfam) :
    triples(trip),
    pattern(pat),
    family(idfam)
{
    predicate = pat.getPredicate();
    subject = pat.getSubject();
    object = pat.getObject();
    //Mejora: quitar comprobacion iteratorsvo no calcula la familia
    if (!family) family = trip->getMapS()->getFamily(subject);
    int familypos = triples->getFamilies()->getPosition(family, predicate);
    if (familypos!=-1){
        size_t localSubject = trip->getMapS()->getLocalSubject(subject,family);
        size_t localObject = triples->getMapO()->getLocalObjectID(predicate,object);
        if (!triples->getPs()->searchLocalObject(localObject,predicate, familypos, localSubject)) more =false;
    }
    else more = false;
}

void iteratorspo::updateOutput() {
    returnTriple.setAll(subject,predicate,object);
}

bool iteratorspo::hasNext()
{
    return more;
}

TripleID *iteratorspo::next(){
    more = false;
    updateOutput();
    return &returnTriple;
}
}
