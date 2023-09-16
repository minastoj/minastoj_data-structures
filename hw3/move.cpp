#include "move.h"


// Complete
Move::Move(Board* board)
{
    m.first = Vehicle::INVALID_ID;
    m.second = 0;
    b = board;
    g = 0;
    h = 0;
    prev = NULL;
}

// COMPLETE!!!
Move::Move(const Board::MovePair& move, Board* b,  Move *parent)
{
    m = move;
    this->b = b;
    h = 0;
    g = parent->g + 1;
    prev = parent;
}

// COMPLETE!!!
Move::~Move()
{
    delete b;
}

// COMPLETE!!!
bool Move::operator<(const Move& rhs) const
{
    return (this->g + this->h) < (rhs.g + rhs.h);
}

// COMPLETE!!!
void Move::score(Heuristic *heur) 
{
    h = heur->compute(*b);
}
