

#include <set>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
// add or remove necessary headers as you please
#include "schedwork.h"

using namespace std;

// a constant that can be used to indicate an INVALID 
// worker ID if that is useful to your implementation.
// Feel free to not use or delete.
static const Worker_T INVALID_ID = (unsigned int)-1;


// Add prototypes for any helper functions here
bool helper(
    const AvailabilityMatrix& avail,
    const size_t need,
    const size_t max,
    DailySchedule& sched,
    int row,
    int col,
    int num_workers);

bool is_available(
    const AvailabilityMatrix& avail,
    const size_t need,
    const size_t max,
    DailySchedule& sched,
    int worker_num, 
    int date);

// Add your implementation of schedule() and other helper functions here

bool schedule(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched
)
{
    if(avail.size() == 0U){
        return false;
    }
    sched.clear();

    // Add your code below

    for(int j = 0; j < (int)avail.size(); j++)
    {
        std::vector<Worker_T> temp;
        for(int k = 0; k < (int)dailyNeed; k++)
        {
            temp.push_back(INVALID_ID);
        }
        sched.push_back(temp);
    }

    int num_workers = (int)sched[0].size();

    return helper(avail, dailyNeed, maxShifts, sched, 0, 0, num_workers);
}

bool helper(
    const AvailabilityMatrix& avail,
    const size_t need,
    const size_t max,
    DailySchedule& sched,
    int row,
    int col,
    int num_workers)
{
    if(row == (int)sched.size()) // reached final row
    {
        return true;
    }
    if(col == (int)need) // reached end of a column
    {
        return helper(avail, need, max, sched, row + 1, 0, num_workers);
    }
    else 
    {
        // spot is empty
        if(sched[row][col] == INVALID_ID)
        {
            // loop through workers
            for(int i = 0; i < num_workers; i++)
            {
                if(avail[row][i] == true)
                {
                    sched[row][col] = i;

                    if(is_available(avail, need, max, sched, i, row))
                    {
                        if(helper(avail, need, max, sched, row, col + 1, num_workers))
                        {
                            return true;
                        }
                    }
                }
            }
            sched[row][col] = INVALID_ID;
            return false;
        }
        else
        {
            // move one column over bc spot is already filled
            return helper(avail, need, max, sched, row, col + 1, num_workers);
        }
  }
}

bool is_available(
    const AvailabilityMatrix& avail,
    const size_t need,
    const size_t max,
    DailySchedule& sched,
    int worker_num, 
    int date)
{
    int shift_count = 0;
    int row_shift = 0;

    for(int i = 0; i < (int)sched.size(); i++)
    {
        for(int j = 0; j < (int)sched[i].size(); j++)
        {
            if((int)sched[i][j] == worker_num) shift_count++;
        }
    }

    if(shift_count <= (int)max)
    {
        for(int k = 0; k < (int)sched[date].size(); k++)
        {
            if((int)sched[date][k] == worker_num) row_shift++;
        }

        if(row_shift == 1)
        {
            return true;
        }
        else return false;
    }
    return false;
}