#include "../BitmapTriplespp.hpp"

namespace hdt {

iteratorvto::iteratorvto(BitmapTriplespp *trip, TripleID &pat) :
    triples(trip),
    pattern(pat)
{
    predicate = pat.getPredicate();
    object = pat.getObject();
    if (triples->getIObjects()->hasTypedFamilies(object)){
        family = triples->getIObjects()->nextTypedFamily();
        triples->getMapS()->getSubjects(family);
        subject = triples->getMapS()->nextSubject();
    } else more= false;


}

void iteratorvto::updateOutput() {
    returnTriple.setAll(subject,predicate,object);
}

bool iteratorvto::hasNext()
{
    return more;
}

TripleID *iteratorvto::next(){

    updateOutput();
    if (triples->getMapS()->hasMoreSubjects()){
        subject = triples->getMapS()->nextSubject();
    } else if (triples->getIObjects()->hasMoreTypedFamilies()){
            family = triples->getIObjects()->nextTypedFamily();
            triples->getMapS()->getSubjects(family);
            subject = triples->getMapS()->nextSubject();
    } else more = false;

    return &returnTriple;
}



}
