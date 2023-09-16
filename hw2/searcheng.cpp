#include <map>
#include <set>
#include <vector>
#include <iostream>
#include <fstream>
#include "searcheng.h"

using namespace std;

// Helper function that will extract the extension of a filename
std::string extract_extension(const std::string& filename);

std::string extract_extension(const std::string& filename)
{
    size_t idx = filename.rfind(".");
    if (idx == std::string::npos) {
        return std::string();
    }
    return filename.substr(idx + 1);
}


// To be updated as needed 
SearchEng::SearchEng()
{

}

// To be completed
SearchEng::~SearchEng()
{
    // loop through filenames_ and deallocate all the WebPage*
    std::map<std::string, WebPage*>::iterator it;
    for(it = filenames_.begin(); it != filenames_.end(); ++it)
    {
        delete it->second;
    }
}

// Complete
void SearchEng::register_parser(const std::string& extension, PageParser* parser)
{
    if (parsers_.find(extension) != parsers_.end())
    {
        throw std::invalid_argument("parser for provided extension already exists");
    }
    parsers_.insert(make_pair(extension, parser));
}

// Complete
void SearchEng::read_pages_from_index(const std::string& index_file)
{
    ifstream ifile(index_file.c_str());
    if(ifile.fail()) {
        cerr << "Unable to open index file: " << index_file << endl;
    }

    // Parse all the files
    string filename;
    while(ifile >> filename) {
#ifdef DEBUG
        cout << "Reading " << filename << endl;
#endif
        read_page(filename);
    }
    ifile.close();
}

// COMPLETE
void SearchEng::read_page(const string& filename)
{
    string ext = extract_extension(filename);

    // check if the filename exists
    ifstream ifile(filename.c_str());
    if(ifile.fail()) {
        throw std::invalid_argument("Filename doesn't exist");
    }

    // find the proper parser
    std::map<std::string, PageParser*>::iterator parser_ptr = parsers_.find(ext);

    // if parser doesn't exist, throw logic error
    if(parser_ptr == parsers_.end())
    {
        throw std::logic_error("No parser provided for extension type");
    }

    // various iterators
    std::set<std::string> found_outlinks;
    std::set<std::string> found_terms;
    std::map<std::string, WebPage*>::iterator file_ptr = filenames_.find(filename);
    WebPage* page;
    std::set<std::string>::iterator iter;
    std::map<std::string, WebPage*>::iterator outlink_locator;
    std::map<std::string, WebPageSet>::iterator term_locator;

    // WebPage object doesn't exist
    if(file_ptr == filenames_.end())
    {
        page = new WebPage(filename);
        // add the WebPage to filenames_
        filenames_.insert(make_pair(filename, page));
    }
    else // WebPage already exists
    {
        page = file_ptr->second;
    }

    // parse the file
    parser_ptr->second->parse(ifile, found_terms, found_outlinks);
    // add the terms to the webpage
    page->all_terms(found_terms);

    // loop through terms
    for(iter = found_terms.begin(); iter != found_terms.end(); ++iter)
    {
        term_locator = term_sets.find(*iter); // find the term
        if(term_locator == term_sets.end()) // current term doesn't exist yet
        {
            WebPageSet set;
            set.insert(page);
            term_sets.insert(make_pair(*iter, set));
        }
        else // current term already exists
        {
            term_sets[*iter].insert(page);
        }
    }

    // loop through outgoing links
    for(iter = found_outlinks.begin(); iter != found_outlinks.end(); ++iter)
    {
        WebPage* out;
        outlink_locator = filenames_.find(*iter); // find via file name
        // this link isn't in filenames_
        if(outlink_locator == filenames_.end())
        {
            out = new WebPage(*iter); // create new page with that name
            filenames_.insert(make_pair(*iter, out)); // add it to filenames_
        }
        else
        {
            out = outlink_locator->second; // out holds already found outgoing link webpage
        }

        // make page one of out's incoming links
        out->add_incoming_link(page);
        // make out one of page's outgoing links
        page->add_outgoing_link(out);
    }
}

// COMPLETE
WebPage* SearchEng::retrieve_page(const std::string& page_name) const
{
    // check for existence of the webpage by name
    std::map<std::string, WebPage*>::const_iterator it = filenames_.find(page_name);
    // no webpage stored by the name
    if(it == filenames_.end()) return NULL;
    else return it->second; // return a pointer to a webpage
}

// COMPLETE
void SearchEng::display_page(std::ostream& ostr, const std::string& page_name) const
{
    // check if the page_name exists in the map
    std::map<std::string, WebPage*>::const_iterator it = filenames_.find(page_name);
    if(it == filenames_.end())
    {
        throw std::invalid_argument("Page does not exist");
    }

    string ext = extract_extension(page_name);

    // find the proper parser
    std::map<std::string, PageParser*>::const_iterator parser_ptr = parsers_.find(ext);

    // in the case that the parser doesn't exist, throw logic error
    if(parser_ptr == parsers_.end())
    {
        throw std::logic_error("No parser provided for extension type");
    }

    // display text
    ifstream ifile(page_name);
    string res = parser_ptr->second->display_text(ifile);
    ostr << res;
}

// COMPLETE
WebPageSet SearchEng::search(const std::vector<std::string>& terms, WebPageSetCombiner* combiner) const
{
    WebPageSet result;
    
    // if there's not terms, return an empty WebPageSet
    if(terms.empty())
    {
        return result; // return result as it is -- empty
    }
    // assumed terms isn't empty 

    // find the first term from terms in term_sets
    std::map<std::string, WebPageSet>::const_iterator term_locator =  term_sets.find(terms[0]);
    // set result to the set associated with it
    result = term_locator->second;

    if(terms.size() == 1)
    {
        return result; // return result as is -- with the one and only set applicable to the term
    }

    for(int i = 1; i < (int)terms.size(); i++)
    {
        term_locator = term_sets.find(terms[i]);

        if(term_locator != term_sets.end()) // if the term is found in term_sets
        {
            result = combiner->combine(result, term_locator->second);
        }
    }
    return result;
}