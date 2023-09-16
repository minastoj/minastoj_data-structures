#include <iostream>
#include <fstream>
#include <stdexcept>
#include <cctype>
#include "md_parser.h"
#include "util.h"

//using namespace std;

typedef enum { NORMALTEXT, LINKTEXT, ISLINK, LINKURL } PARSE_STATE_T;

// To be completed
void MDParser::parse(std::istream& istr, std::set<std::string>& allSearchableTerms, std::set<std::string>& allOutgoingLinks)
{
    // Remove any contents of the sets before starting to parse.
    allSearchableTerms.clear();
    allOutgoingLinks.clear();

    // variable to represent the current term
    std::string term = "";

    // get the first term in the file
    char prev = '\0'; // represents the char that was before the current one, starts as empty or NULL
    char c = istr.get();

    // Continue reading from the file until input fails.
    while(!istr.fail())
    {
        // Is c a character to split terms?
        if (!isalnum(c))
        {
            // if the curr char is ( and the one before it was ], then we have a link in question
            if(c == '(' && prev == ']')
            {
                // If we have a term to add, convert it to a standard case and add it
                if(term != "")
                {
                    term = conv_to_lower(term);
                    allSearchableTerms.insert(term);
                }
                term = ""; 

                while(c != ')')
                {
                    prev = c; // track old c
                    c = istr.get(); // new c
                    if(c == ')') break;
                    term += c; // build link string
                }

                if(term != "") // only insert and convert if there is something to actually push
                {
                    // once loop is over and c == ) push term
                    term = conv_to_lower(term);
                    allOutgoingLinks.insert(term);
                    term = ""; // reset term to be empty
                }
            }
            else // if we aren't dealing with a link, do the normal stuff
            {
                // If we have a term to add, convert it to a standard case and add it
                if(term != "")
                {
                    term = conv_to_lower(term);
                    allSearchableTerms.insert(term);
                }
                term = "";
            }
        }
        // Otherwise we continually add to the end of the current term.
        else
        {
            term += c;
        }

        // set prev to this round's value of c and then change c
        prev = c;
        // Attempt to get another character from the file.
        c = istr.get();
    }
    // Since the last term in the file may not have punctuation, there may be a valid term in
    // the "term" variable, so we need to insert it into the allSearchableTerms set.
    if(term != "")
    {
        term = conv_to_lower(term);
        allSearchableTerms.insert(term);
    }

}

// COMPLETE
std::string MDParser::display_text(std::istream& istr)
{
    std::string result;

    char prev = '\0';
    char c = istr.get();

    // Continue reading from the file until input fails.
    while (!istr.fail()) {
        if(c == '(' && prev == ']')
        {
            while(c != ')')
            {
                prev = c; // track old c
                c = istr.get(); // new c
                if(c == ')') break;
            }
            c = istr.get(); // get a new character
            if(istr.fail()) return result; // return result as is
        }
        result += c;

        // set prev to this round's value of c and then change c
        prev = c;
        // Attempt to get another character from the file.
        c = istr.get();
    }
    return result;
}


