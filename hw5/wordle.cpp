// For debugging
#include <iostream>
// For std::remove
#include <algorithm> 
#include <map>
#include <set>
#include "wordle.h"
#include "dict-eng.h"
using namespace std;


// Add prototypes of helper functions here
void helper(
    const int length_word, 
    const std::string& floating, 
    const std::set<std::string>& dict,
    std::set<std::string>& possible_words,
    int pos, 
    std::string curr, 
    int num_blanks);


// Definition of primary wordle function
std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
    // Add your code here
    std::set<std::string> possible_words;
    int num_blanks = 0;
    int length_word = (int)in.length();
    std::string in_copy = in;

    // find number of blanks in the word
    for(int i = 0; i  < in.length(); i++)
    {
        if(in[i] == '-') num_blanks++;
    }

    helper(length_word, floating, dict, possible_words, 0, in_copy, num_blanks);
    
    return possible_words;
}


// Define any helper functions here
void helper(
    const int length_word,
    const std::string& floating, 
    const std::set<std::string>& dict,
    std::set<std::string>& possible_words,
    int pos,
    std::string curr, 
    int num_blanks)
{
    if(pos >= length_word) // at the end of the word length
    {
        if(dict.find(curr) != dict.end())
        {
            possible_words.insert(curr); // add curr to possible words if in the dictionary
        }
        return;
    }
    else if(pos < length_word) // not at the end of the word
    {
        // have to use floating letters 
        if(num_blanks <= floating.length())
        {
            if(curr[pos] != '-')
            {
                helper(length_word, floating, dict, possible_words, pos + 1, curr, num_blanks);
            }
            else
            {
                for(int j = 0; j < floating.length(); j++)
                {
                    curr[pos] = floating[j]; // attempt a floating letter
                    std::string temp = floating;
                    temp.erase(j, 1);
                    helper(length_word, temp, dict, possible_words, pos + 1, curr, num_blanks - 1);
                    curr[j] = '-';
                }
            }
        }
        else // go through whole alphabet
        {
            if(curr[pos] != '-')
            {
                helper(length_word, floating, dict, possible_words, pos + 1, curr, num_blanks);
            }
            else
            {
                for(char k = 'a'; k <= 'z'; k++)
                {
                    curr[pos] = k; // attempt a random letter
                    std::string temp = floating;
                    for(int l = 0; l < floating.length(); l++)
                    {
                        if(k == floating[l])
                        {
                            temp.erase(l, 1);
                            break;
                        }
                    }
                    helper(length_word, temp, dict, possible_words, pos + 1, curr, num_blanks - 1);
                    curr[pos] = '-';
                }
            }
        }
    }
    else
    {
        helper(length_word, floating, dict, possible_words, pos, curr, num_blanks);
    }
}