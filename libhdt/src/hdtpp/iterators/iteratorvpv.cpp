#include "../BitmapTriplespp.hpp"
namespace hdt {

iteratorvpv::iteratorvpv(BitmapTriplespp *trip, TripleID &pat) :
    triples(trip),
    pattern(pat)
{
    predicate = pat.getPredicate();
    triples->getFamilies()->getFamilies(predicate);
    family = triples->getFamilies()->nextFamily();
    triples->getMapS()->getSubjects(family);
    subject = triples->getMapS()->nextSubject();
    triples->getPs()->getLocalObjects(predicate);
}

void iteratorvpv::updateOutput() {
    returnTriple.setAll(subject,predicate,object);
}

bool iteratorvpv::hasNext(){
    return more;
}

TripleID *iteratorvpv::next(){
    localObject = triples->getPs()->nextLocalObject(predicate);
    object = triples->getMapO()->getDictID(predicate, localObject);
    updateOutput();
    if (triples->getPs()->hasMoreObjects(predicate)){
        if (triples->getPs()->isNewSubjectList(predicate)){
            if (!triples->getMapS()->hasMoreSubjects()){
                family = triples->getFamilies()->nextFamily();
                triples->getMapS()->getSubjects(family);
            }
            subject = triples->getMapS()->nextSubject();
        }
    } else more = false;
    return &returnTriple;
}
}
