#ifndef GRAPHISO_H
#define GRAPHISO_H
#include <string>
#include <vector>
#include <map>
#include <set>
#include "ht.h"

// Graph class for use with graph isomorphism function
typedef std::string VERTEX_T;
typedef std::set<VERTEX_T> VERTEX_SET_T;
typedef std::vector<VERTEX_T> VERTEX_LIST_T;
class Graph {
public:
    Graph(std::istream& istr);
    bool edgeExists(const VERTEX_T& u, const VERTEX_T& v) const;
    const VERTEX_SET_T& neighbors(const VERTEX_T& v) const;
    VERTEX_LIST_T vertices() const;
private:
    std::map<std::string, VERTEX_SET_T > adj_;
};


/// Use your hashtable for the mapping between vertices
typedef HashTable<VERTEX_T, VERTEX_T> VERTEX_ID_MAP_T;

/**
 * @brief Determines if two graphs are isomorphic
 * 
 * @param g1 Graph 1
 * @param g2 Graph 2
 * @param mapping isomorphic mapping between vertices of graph 1 (key) and graph 2 (value)
 * @return true If the two graphs are isomorphic
 * @return false Otherwise
 */
bool graphIso(  const Graph& g1, 
                const Graph& g2, 
                VERTEX_ID_MAP_T& mapping);

#endif