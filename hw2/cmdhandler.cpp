#include "cmdhandler.h"
#include "util.h"
using namespace std;

//
//
// QUIT HANDLER PORTION
//
//

// Complete
QuitHandler::QuitHandler()
{

}

// Complete
QuitHandler::QuitHandler(Handler* next)
    : Handler(next)
{

}

// Complete
bool QuitHandler::canHandle(const std::string& cmd) const
{
    return cmd == "QUIT";
}

// Complete
Handler::HANDLER_STATUS_T QuitHandler::process(SearchEng* eng, std::istream& instr, std::ostream& ostr)
{
    return HANDLER_QUIT;
}

//
//
// PRINT HANDLER PORTION
//
//

// Complete
PrintHandler::PrintHandler()
{

}

// Complete
PrintHandler::PrintHandler(Handler* next)
    : Handler(next)
{

}

// Complete
bool PrintHandler::canHandle(const std::string& cmd) const
{
    return cmd == "PRINT";
}

// Complete
Handler::HANDLER_STATUS_T PrintHandler::process(SearchEng* eng, std::istream& instr, std::ostream& ostr)
{
    string name;

    if (!(instr >> name)) {
        return HANDLER_ERROR;
    }
    try {
        eng->display_page(ostr, name);
    }
    catch (std::exception& e) {
        return HANDLER_ERROR;
    }
    return HANDLER_OK;
}

//
//
// INCOMING HANDLER PORTION
//
//

// Complete
IncomingHandler::IncomingHandler()
{

}

// Complete
IncomingHandler::IncomingHandler(Handler* next)
    : Handler(next)
{

}

// COMPLETE
bool IncomingHandler::canHandle(const std::string& cmd) const
{
    return cmd == "INCOMING";
}

// COMPLETE
Handler::HANDLER_STATUS_T IncomingHandler::process(SearchEng* eng, std::istream& instr, std::ostream& ostr)
{
    string name;

    if (!(instr >> name)) { // page name doesn't exist
        return HANDLER_ERROR;
    }
    try {

        WebPage* page_ptr = eng->retrieve_page(name); // get WebPage* that matches name
        if(!(page_ptr == NULL)) // retrieve_page returns null if the name doesn't exist
        {   
            WebPageSet links = page_ptr->incoming_links(); // get incoming links
            int num_links = links.size(); // number of incoming links is the size of the in_links_ set

            cout << num_links << endl; // display num_links
            WebPageSet::iterator it;
            for(it = links.begin(); it != links.end(); ++it)
            {
                cout << *it << endl;
            }
            display_hits(links, ostr);
        }
        else return HANDLER_ERROR; // page_ptr == NULL
    }
    catch (std::exception& e) {
        return HANDLER_ERROR;
    }
    return HANDLER_OK;
}

//
//
// OUTGOING HANDLER PORTION
//
//

// Complete
OutgoingHandler::OutgoingHandler()
{

}

// Complete
OutgoingHandler::OutgoingHandler(Handler* next)
    : Handler(next)
{

}

// COMPLETE
bool OutgoingHandler::canHandle(const std::string& cmd) const
{
    return cmd == "OUTGOING";
}

// COMPLETE
Handler::HANDLER_STATUS_T OutgoingHandler::process(SearchEng* eng, std::istream& instr, std::ostream& ostr)
{
    string name;

    if (!(instr >> name)) { // page name doesn't exist
        return HANDLER_ERROR;
    }
    try {
        WebPage* page_ptr = eng->retrieve_page(name); // get WebPage* that matches name
        if(!(page_ptr == NULL))
        {
            WebPageSet links = page_ptr->outgoing_links(); // get outgoing links
            int num_links = links.size(); // number of outgoing links is the size of the out_links_ set

            cout << num_links << endl; // display num_links
            WebPageSet::iterator it;
            for(it = links.begin(); it != links.end(); ++it)
            {
                cout << *it << endl;
            }
            display_hits(links, ostr);
        }
        else return HANDLER_ERROR; // page_ptr == NULL
    }
    catch (std::exception& e) {
        return HANDLER_ERROR;
    }
    return HANDLER_OK;
}

//
//
// AND HANDLER PORTION
//
//

// COMPLETE
AndHandler::AndHandler()
{

}

// COMPLETE
AndHandler::AndHandler(Handler* next)
    : Handler(next)
{

}

// COMPLETE
bool AndHandler::canHandle(const std::string& cmd) const
{
    return cmd == "AND";
}

// COMPLETE
Handler::HANDLER_STATUS_T AndHandler::process(SearchEng* eng, std::istream& instr, std::ostream& ostr)
{
    string name;
    std::vector<std::string> terms; // create a set with all the passed in terms
    
    std::string curr;
    instr >> curr; // get the first term

    while(!instr.fail())
    {
        terms.push_back(curr); // push back a term
        instr >> curr;
        if(instr.fail()) break;
    }

    AndWebPageSetCombiner* andcomb = new AndWebPageSetCombiner;
    // call SearchEngine's search with an AndCombiner
    WebPageSet result = eng->search(terms, andcomb);
    display_hits(result, ostr);
    return HANDLER_OK;
}

//
//
// OR HANDLER PORTION
//
//

// COMPLETE
OrHandler::OrHandler()
{

}

// COMPLETE
OrHandler::OrHandler(Handler* next)
    : Handler(next)
{

}

// COMPLETE
bool OrHandler::canHandle(const std::string& cmd) const
{
    return cmd == "OR";
}

// COMPLETE
Handler::HANDLER_STATUS_T OrHandler::process(SearchEng* eng, std::istream& instr, std::ostream& ostr)
{
    string name;
    std::vector<std::string> terms; // create a set with all the passed in terms
    
    std::string curr;
    instr >> curr; // get the first term

    while(!instr.fail())
    {
        terms.push_back(curr); // push back a term
        instr >> curr;
        if(instr.fail()) break;
    }

    OrWebPageSetCombiner* orcomb = new OrWebPageSetCombiner;
    // call SearchEngine's search with an AndCombiner
    WebPageSet result = eng->search(terms, orcomb);
    display_hits(result, ostr);
    return HANDLER_OK;
}

//
//
// DIFF HANDLER PORTION
//
//

// COMPLETE
DiffHandler::DiffHandler()
{

}

// COMPLETE
DiffHandler::DiffHandler(Handler* next)
    : Handler(next)
{

}

// COMPLETE
bool DiffHandler::canHandle(const std::string& cmd) const
{
    return cmd == "DIFF";
}

// COMPLETE
Handler::HANDLER_STATUS_T DiffHandler::process(SearchEng* eng, std::istream& instr, std::ostream& ostr)
{
    string name;
    std::vector<std::string> terms; // create a set with all the passed in terms
    
    std::string curr;
    instr >> curr; // get the first term
    
    while(!instr.fail())
    {
        terms.push_back(curr); // push back a term
        instr >> curr;
        if(instr.fail()) break;
    }

    DiffWebPageSetCombiner* diffcomb = new DiffWebPageSetCombiner;
    // call SearchEngine's search with an AndCombiner
    WebPageSet result = eng->search(terms, diffcomb);
    display_hits(result, ostr);
    return HANDLER_OK;
}