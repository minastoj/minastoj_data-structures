#include <iostream>
#include <fstream>
#include "graphiso.h"

using namespace std;

int main(int argc, char* argv[])
{
    if(argc < 3){
        cerr << "Usage: ./graphiso graph-file-1 graph-file-2" << endl;
        return 1;
    }
    ifstream if1(argv[1]);
    if(if1.fail()){
        cerr << "Cannot open file: " << argv[1] << endl;
        return 1;
    }
    ifstream if2(argv[2]);
    if(if2.fail()){
        cerr << "Cannot open file: " << argv[2] << endl;
        return 1;
    }
    Graph g1(if1);
    Graph g2(if2);
    if1.close();
    if2.close();

    VERTEX_ID_MAP_T mapping;
    bool exists = graphIso(g1, g2, mapping);
    if(exists){
        cout << "Isomorphism exists: " << endl;
        // Since we don't have a hash table iterator, we'll just get the 
        // list of vertices and iterate through it, using the hash table
        // to translate to the isomorphic vertex
        VERTEX_LIST_T g1verts = g1.vertices();
        for(const auto& g1v : g1verts){
            cout << "G1: " << g1v << " <=> G2: " << mapping[g1v]  << endl;
        }

    }
    else {
        cout << "No isomorphism exists" << endl;
    }
    return 0;
}