#include <iostream>
#include <map>
#include <set>
#include <algorithm>
#include "solver.h"

using namespace std;

// COMPLETE!!!
Solver::Solver(const Board& b, Heuristic *heur)
{
    b_ = b;
    heur_ = heur;
}

// To be completed
Solver::~Solver()
{

}

// To be completed
bool Solver::run()
{
    MoveHeap open_list;
    MoveSet closed_list;

    // push start state to open_list AND closed_list
    Move* s = new Move(new Board(b_));
    open_list.push(s);
    closed_list.insert(s);

    while(!open_list.empty())
    {
        Move* curr = open_list.top(); // get best f score move
        open_list.pop(); // pop the open_list

        // if s = goal node then trace path back to start
        if(curr->b->solved())
        {
            while(curr->prev != NULL)
            {
                solution_.push_back(curr->m);
                expansions_++;
                curr = curr->prev;
            }
            reverse(solution_.begin(), solution_.end());

            // delete things in closed_list
            std::set<Move*, MoveBoardComp>:: iterator it;
            for(it = closed_list.begin(); it != closed_list.end(); ++it)
            {
                delete *it;
            }
            return true;
        }

        // Generate successors/neighbors of s
        Board::MovePairList potential = curr->b->potentialMoves();
        for(int i = 0; i < (int)potential.size(); i++)
        {
            Board* new_board = new Board(*(curr->b));
            new_board->move(potential[i].first, potential[i].second);

            Move* next = new Move(potential[i], new_board, curr);

            // if they're not in the closed_list, add them to open_list
            std::set<Move*, MoveBoardComp>:: iterator it2 = closed_list.find(next);
            if(it2 == closed_list.end()) // not in closed_list
            {
                next->score(heur_);  // compute their f values
                open_list.push(next);
                closed_list.insert(next);
            }
            else {
                delete next;
            }
        }

        closed_list.insert(curr);
    }
    // delete everything in the closed list anyway if we don't find a solution
    std::set<Move*, MoveBoardComp>::iterator it3;
    for(it3 = closed_list.begin(); it3 != closed_list.end(); ++it3)
    {
        delete *it3;
    }
    return false;
}

// COMPLETE!!!
Board::MovePairList Solver::solution() const
{
    return solution_;
}

// Complete
size_t Solver::numExpansions() const
{
    return expansions_;
}
