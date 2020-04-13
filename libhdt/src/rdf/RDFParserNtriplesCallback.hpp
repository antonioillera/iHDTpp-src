#ifndef RDFPARSERNTRIPLESCALLBACK_H
#define RDFPARSERNTRIPLESCALLBACK_H


#include <stdint.h>

#include <../include/HDTEnums.hpp>

#include "../include/RDFParser.hpp"

namespace hdt {

class RDFParserNtriplesCallback : public RDFParserCallback {

private:
    RDFCallback *callback;

    const char *getParserType(RDFNotation notation);
public:
    RDFParserNtriplesCallback();
    virtual ~RDFParserNtriplesCallback();

    void doParse(const char *fileName, const char *baseUri, RDFNotation notation, bool ignoreErrors, RDFCallback *callback);
};



}


#endif // RDFPARSERNTRIPLESCALLBACK_H
