#include <iostream>
#include <sstream>
#include "graphiso.h"

using namespace std;

// ================= Complete - Begin Graph class implementation ===================
Graph::Graph(std::istream& istr)
{
    string aline;
    while(getline(istr,aline))
    {
        istringstream iss(aline);
        string u, v;
        if(iss >> u){
            VERTEX_SET_T neighbors;
            while(iss >> v)
            {
                neighbors.insert(v);
            }
            adj_.insert(std::make_pair(u,neighbors));
        }
    }
}


bool Graph::edgeExists(const VERTEX_T& u, const VERTEX_T& v) const
{
    if((adj_.find(u) != adj_.end()) && (adj_.at(u).find(v) != adj_.at(u).end()))
    {
        return true;
    }
    return false;
}
const VERTEX_SET_T& Graph::neighbors(const VERTEX_T& v) const
{
    if(adj_.find(v) == adj_.end()){
        throw std::invalid_argument("Neighbors: invalid vertex - not in  map");
    }
    return adj_.at(v);
}
VERTEX_LIST_T Graph::vertices() const
{
    VERTEX_LIST_T verts;
    for(const auto& p : adj_)
    {
        verts.push_back(p.first);
    }
    return verts;
}
// ================= Complete - End Graph class implementation ===================

// Prototype and helper functions here
bool helper(const Graph& g1, const Graph& g2, VERTEX_ID_MAP_T& mapping, VERTEX_LIST_T& g1verts, VERTEX_LIST_T& g2verts, int g1_index, VERTEX_SET_T& g2_used);

// To be completed
bool isConsistent(const Graph& g1, const Graph& g2, VERTEX_ID_MAP_T& mapping)
{
    // Feel free to change or remove this starter code
    VERTEX_LIST_T g1verts = g1.vertices();
    for(int i = 0; i < (int)g1verts.size(); i++)
    {
        // Check mappings for necessary vertices to see if there is any violation
        // and return false
        VERTEX_T g1u = g1verts[i]; // get vertex

        if(mapping.find(g1u) != nullptr) // make sure it exists in the other map
        {
            VERTEX_T g2u = mapping[g1u]; // get matching vertex in g2

            VERTEX_SET_T g1_neigh = g1.neighbors(g1u); 
            VERTEX_SET_T g2_neigh = g2.neighbors(g2u);

            if((int)mapping.size() == 1)
            {
                if(g1_neigh.size() == g2_neigh.size())
                {
                    return true;
                }
                else return false;
            }

            if(g1_neigh.size() == g2_neigh.size()) // same degree
            {
                bool status = false;
    
                for(VERTEX_SET_T::iterator it = g1_neigh.begin(); it != g1_neigh.end(); ++it)
                {
                    status = false; // status needs to start as false every time
                    for(VERTEX_SET_T::iterator it2 = g2_neigh.begin(); it2 != g2_neigh.end(); ++it2)
                    {
                        VERTEX_T curr_g1_n = *it;
                        VERTEX_T curr_g2_n = *it2;

                        // it has been mapped already
                        if(mapping.find(curr_g1_n) != nullptr)
                        {
                            if(mapping[curr_g1_n] == curr_g2_n)
                            {
                                status = true;
                            }
                        }
                        else
                        {
                            status = true;
                        }
                    }
                    if(status == false) return false;
                }
            }
            else
            {
                return false;
            }
        }
    }
    return true;
}

// Add any helper functions you deem useful

// To be completed
bool graphIso(const Graph& g1, const Graph& g2, VERTEX_ID_MAP_T& mapping)
{
    VERTEX_LIST_T g1verts = g1.vertices();
    VERTEX_LIST_T g2verts = g2.vertices();
    if(g1verts.size() != g2verts.size())
    {
        return false;
    }
    
    VERTEX_SET_T g2_used;
    return helper(g1, g2, mapping, g1verts, g2verts, 0, g2_used);

    // Delete this and return the correct value...
    // This is just placeholder to allow compilation
    //return false;
}

bool helper(const Graph& g1, const Graph& g2, VERTEX_ID_MAP_T& mapping, VERTEX_LIST_T& g1verts, VERTEX_LIST_T& g2verts, int g1_index, VERTEX_SET_T& g2_used)
{
    if(g1_index == (int)g1verts.size())
    {
        return true;
    }
    else
    {
        VERTEX_T g1_curr = g1verts[g1_index];

        // loop through vertices in g2
        for(int i = 0; i < (int)g2verts.size(); i++)
        {
            VERTEX_T g2_curr = g2verts[i];
            mapping.insert(make_pair(g1_curr, g2_curr)); // insert into mapping

            //cout << g1_curr << " " << g2_curr << endl;

            if(g2_used.find(g2_curr) != g2_used.end()) // that g2 vertex has already been used
            {
                continue;
            }
            g2_used.insert(g2_curr);

            if(isConsistent(g1, g2, mapping)) // check if is consistent
            {
                if(helper(g1, g2, mapping, g1verts, g2verts, g1_index + 1, g2_used)) // if everything else also works, return true
                {
                    return true;
                }
            }
            g2_used.erase(g2_curr);
        }

        // if we got here, inserted thing didn't work
        mapping.remove(g1_curr); // remove it
        return false;
    }
}

