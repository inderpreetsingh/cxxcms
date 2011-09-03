#include "url.hpp"
#include <string>
#include <list>
#include <algorithm>
#include <iostream>
#include <memory>
#include <cstring>
#include <cstdlib>
#include <cassert>

int main() {
    std::string q("a=b&c=d");
    URL::Parser p(q);
    URL::Dict_ptr_t& d = p.parse();
    for(URL::Dict_t::iterator i = d->begin(); i != d->end(); i++) {
        std::cout << "Value1: " <<  i->first << "\t";
        if(i->second)
            std::cout << "Value2: " << i->second << "\n";
    }
}

/*
 * This namespace URL as said in url.hpp contains all URL-related functions and classes
 * Complying to the respective RFCs
 */

namespace URL {

    /*
     * Implementation of the Parser class
     */

    /*
     * Constructor has been defined in the header itself
     * because it should be inlined, it conatins just one function call to setQstr()
     */

    void Parser::_sanitize(std::string& s, size_t n = 1) {
        size_t i;
        for (i = n; i < s.size(); i++) {
            if (s.at(i) == '&' || s.at(i) == '=' || s.at(i) == ';') {
                if (i == 0 or i >= s.size()) break;
                if (s.at(i-1) == '&' || s.at(i-1) == '=' || s.at(i-1) == ';') {
                    s.erase(i,1);
                    _sanitize(s,i);
                }
            }
        }
        
        while (s.at(0) == '=' || s.at(0) == '&' || s.at(0) == ';')
            s.erase(0,1);
        
        while (s.at(s.size()-1) == '=' || s.at(s.size()-1) == '&' || s.at(s.size()-1) == ';')
            s.erase(s.size()-1,1);
    }

    Dict_ptr_t& Parser::parse() {    
        std::string copy = getQstr(), extract = "";
        _sanitize(copy);
        size_t amp_pos = 0, eq_pos = 0;
        Dict_ptr_t ret (new Dict_t);
        do {
            if((amp_pos = copy.find("&")) != std::string::npos) {
                extract = copy.substr(0, amp_pos);
                copy.erase(0, amp_pos + 1);
                std::cout << "Extract: " << extract << "\n";
            }
            else {
                extract = copy;
                copy.clear();
            }
            if((eq_pos = extract.find("=")) != std::string::npos) {
                std::cout << "Adding elements: " << extract.substr(0, eq_pos) << "\t" << extract.substr(eq_pos + 1) << "\n";
                std::cout << "Value of copy: " << copy << "\n";
                ret->insert(Tuple_t(extract.substr(0, eq_pos).c_str(), extract.substr(eq_pos + 1).c_str()));
                std::cout << "Deference: " << ret->find(extract.substr(0, eq_pos).c_str())->second << "\n";
                extract.clear();
            }
            else {
                ret->insert(Tuple_t(extract.c_str(), NULL));
                extract.clear();
            }
        } while(copy.size());
        return ret;
    }	
    
    const char* Parser::getQstr() const {
        if(!source.size())
            throw Common::Exception("Query string propery requested while it was never set! in URL::Parser::getQstr()", QS_NOT_SET, __LINE__, __FILE__);        
        return source.c_str();
    }
}
