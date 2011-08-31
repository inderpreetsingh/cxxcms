#include "url.hpp"
#include <string>
#include <list>
#include <algorithm>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>
#include <boost/lambda/construct.hpp>
#include <cmath>

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

  /*
   * Destructor, deallocates dicts and strings allocated, if any
   */

  Parser::~Parser() {

    /*
     * Clear memory for Dictonaries allocated
     */

    using namespace boost::lambda;

    if(Dicts.size())
      std::for_each(Dicts.begin(), Dicts.end(), bind(delete_ptr(), _1));

    /*
     * Clear memory for char*'s allocated
     */

    if(Strings.size())
      std::for_each(Strings.begin(), Strings.end(), bind(delete_array(), _1));    
  }

    void Parser::_sanitize(std::string& s, unsigned int n) {
        unsigned int i;
        for (i = n; i < s.size(); i++) {
            if (s.at(i) == '&' || s.at(i) == '=') {
                if (i == 0 or i >= s.size()) break;
                if (s.at(i-1) == '&' || s.at(i-1) == '=') {
                    s.erase(i,1);
                    _sanitize(s,i);
                }
            }
        }
        while (s.at(0) == '=' || s.at(0) == '&')
            s.erase(0,1);
        
        while (s.at(s.size()-1) == '=' || s.at(s.size()-1) == '&')
            s.erase(s.size()-1,1);
    }
    
    const char* Parser::getQstr() const {
        if(!source.size())
            throw Common::Exception("Query string propery requested while it was never set! in URL::Parser::getQstr()", QS_NOT_SET);
        
        return source.c_str();
    }
}

