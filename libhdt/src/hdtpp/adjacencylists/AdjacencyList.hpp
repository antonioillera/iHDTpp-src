#ifndef ADJACENCYLIST_HPP
#define ADJACENCYLIST_HPP

class AdjacencyList
{
public:
    virtual ~AdjacencyList(){ }
    virtual unsigned int adj(size_t v, size_t u) = 0;
    virtual void traverse(size_t v)= 0;
    virtual bool hasMoreElements() = 0;
    virtual unsigned int nextElement() = 0;
    virtual unsigned int currentElement() = 0;
    virtual unsigned int sizeBytes() = 0;
};


#endif // ADJACENCYLIST_HPP

