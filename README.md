# Foundations

This is the iHDT++ source code. iHDT++[1] allows to query (SPARQL Triple Patterns) RDF/HDT++ files. HDT++ uses RDF-TR (RDF Triples Reorganization)[2], an RDF triples reorganization technique mainly based on predicate-families extraction. A seminal paper of HDT++ was presented at the DCC (Data Compression Conference)[3] in 2015.

The datasets queried and evaluated in [1,2] (as well as others) can be found at the following address: http://www.rdfhdt.org/datasets/
* [DBLP](http://gaia.infor.uva.es/hdt/dblp-2012-11-28.hdt.gz)
* [DBTune](http://www.rdfhdt.org/datasets/)
* [US Census](http://www.rdfhdt.org/datasets/)
* [LinkedGeoData](http://www.rdfhdt.org/datasets/)

# HDT++

To recompress any HDT file into HDT++ just:
> [iHDT++ Path]/tools/hdt2hdtpp <yourfile.hdt> <yourfile.hdtpp>

# iHDT++

Load an RDF/HDT++ file

```
HDTpp *hdtpp = new HDTpp(inputFile.c_str());
```

Construct de data structures to query de dataset

```
hdtpp->constructMemoryStructures(true);
```

Query de dataset by dictionary ID; "0" : Variable. Looking for all triples related to predicate ID = 5 (Same operations are available with string literals)

```
TripleID tid;
tid.setSubject(0);
tid.setPredicate(5);
tid.setObject(0);
IteratorTripleID *it=hdtpp->getTriples()->search(tid);
while (it->hasNext()) {
     TripleID *t = it->next();
     cout<<t->getSubject()<<"-"<<t->getPredicate()<<"-"<<t->getObject()<<endl;
}
delete it;
```

# References

The following references provide the theoretical content on HDT++ and iHDT++.

[1] A. Hernández-Illera, M.A. Martínez-Prieto, J.D. Fernández, and A. Fariña. RDF-TR: iHDT++: Improving HDT for SPARQL Triple Pattern Resolution. In 7th International Symposium on Language & Knowledge Engineering (LKE), under review.

[2] A. Hernández-Illera, M.A. Martínez-Prieto, and J.D. Fernández. RDF-TR: Exploiting Structural Redundancies to boost RDF Compression. In Information Sciences, under review. 

[3] A. Hernández-Illera, M.A. Martínez-Prieto, and J.D. Fernández. Serializing RDF in Compressed Space. In Proceedings of the Data Compression Conference (DCC), pages 363–372, 2015.


