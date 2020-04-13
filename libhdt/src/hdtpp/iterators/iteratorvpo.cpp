#include "../BitmapTriplespp.hpp"

namespace hdt {

iteratorvpo::iteratorvpo(BitmapTriplespp *trip, TripleID &pat, size_t position) :
    triples(trip),
    pattern(pat)
{
    //position si se llama desde vvo
    predicate = pat.getPredicate();
    object = pat.getObject();
    size_t pos = position;
    if (position==0) pos = triples->getMapO()->POrelationship(predicate, object);
    if (pos){
        triples->getIObjects()->getPositions(pos-1);
        calculateNextSubject();
    } else more =false;


}

void iteratorvpo::updateOutput() {
    returnTriple.setAll(subject,predicate,object);
}

bool iteratorvpo::hasNext()
{
    return more;
}

TripleID *iteratorvpo::next(){
    updateOutput();
    if (triples->getIObjects()->hasMorePositions()) calculateNextSubject();
    else more=false;
    return &returnTriple;
}

void iteratorvpo::calculateNextSubject(){
    size_t familyPredicate;
    size_t posPs=triples->getIObjects()->nextPosition();
    size_t localSubject = triples->getPs()->getLocalSubject(predicate,posPs,&familyPredicate);
    family = triples->getFamilies()->getFamily(familyPredicate, predicate);
    subject = triples->getMapS()->getDictID(family, localSubject);
}

}
