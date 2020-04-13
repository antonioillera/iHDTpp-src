#include "../BitmapTriplespp.hpp"
namespace hdt {

iteratorvtv::iteratorvtv(BitmapTriplespp *trip, TripleID &pat) :
    triples(trip),
    pattern(pat)
{
    predicate = pat.getPredicate();
    triples->getTypes()->getTypes();
    if (triples->getTypes()->hasMoreTypes()){
        family = triples->getTypes()->getCurrentFamily();
        object = triples->getTypes()->nextType();
        triples->getMapS()->getSubjects(family);
        subject = triples->getMapS()->nextSubject();
    } else more = false;
}

void iteratorvtv::updateOutput() {
    returnTriple.setAll(subject,predicate,object);
}

bool iteratorvtv::hasNext(){
    return more;
}

TripleID *iteratorvtv::next(){
    updateOutput();
    if (triples->getMapS()->hasMoreSubjects()){
        subject = triples->getMapS()->nextSubject();
    } else {
        if (triples->getTypes()->hasMoreTypes()){
            family = triples->getTypes()->getCurrentFamily();
            object = triples->getTypes()->nextType();
            triples->getMapS()->getSubjects(family);
            subject = triples->getMapS()->nextSubject();
        } else more = false;
    }
    return &returnTriple;
}
}
