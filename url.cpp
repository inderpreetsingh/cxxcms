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

  template <typename T>
  const Parser& Parser::setQstr(T s) {

    /*
     * We're using `s' to set value of a string
     * Hence it must be a string compatible type
     */

    if(!Common::is_string(s))
      throw Common::Exception("Invalid type specified to URL::Parser::Parser", INVALID_TYPE);

    source_string = s;
    return *this;    
  }

  const char* Parser::getQstr() const {
    if(!source_string.size())
      throw Common::Exception("Query string propery requested while it was never set! in URL::Parser::getQstr()", QS_NOT_SET);

    return source_string.c_str();
  }

  /*
   * Templated function decodeHex()
   * Input: HEX String (with the % symbol, for example %AA)
   * Output: The ASCII character
   * Note that type of string must be one of those defined in Common::is_string()
   */

  template <typename T>
  char decodeHex(T s) {
    if(!Common::is_string(s))
      throw Common::Exception("Invalid type passed to URL::decodeHex()", INVALID_TYPE);

    std::string source = s;
    unsigned int result = 0;
    unsigned int i = 0;
    
    source.erase(0, 1); // Removing % from %XX

    /* This is incomplete!!!! */

    /*    for(i = 0; i <= source.size(); i++) {
     if(source.at(i) >= '0' and source.at(i) <= '9')
      result += source.at(i) * pow(16, i + 1);*/

  }
}
