#include "searcheng.h"
#include "combiners.h"

WebPageSet AndWebPageSetCombiner::combine(const WebPageSet& setA, const WebPageSet& setB)
{
    WebPageSet result;

    // loop through setA and add the webpages that are in both WebPageSets
    std::set<WebPage*>::iterator it;
    for(it = setA.begin(); it != setA.end(); ++it)
    {
        // if the same webpage is in setB
        if( setB.find(*it) != setB.end())
        {
            result.insert(*it);
        }
    }

    return result;

}

WebPageSet OrWebPageSetCombiner::combine(const WebPageSet& setA, const WebPageSet& setB)
{
    WebPageSet result;

    // loop through setA and add all webpages in setA
    std::set<WebPage*>::iterator it;
    for(it = setA.begin(); it != setA.end(); ++it)
    {
        result.insert(*it);
    }

    // loop through setB and add all other webpages that haven't already been added
    for(it = setB.begin(); it != setB.end(); ++it)
    {
        result.insert(*it);
    }

    return result;
}

WebPageSet DiffWebPageSetCombiner::combine(const WebPageSet& setA, const WebPageSet& setB)
{
    WebPageSet result;
    
    // loop through setA and add any webpages in setA that are not also in setB
    std::set<WebPage*>::iterator it;
    for(it = setA.begin(); it != setA.end(); ++it)
    {
        // if a link in setA is not in setB, add it to result
        if(setB.find(*it) == setB.end())
        {
            result.insert(*it);
        }
    }

    return result;
}