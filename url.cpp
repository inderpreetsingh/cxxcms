#include "url.hpp"
#include <string>
#include <list>
#include <algorithm>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>
#include <boost/lambda/construct.hpp>

/*
 * This namespace URL as said in url.hpp contains all URL-related functions and classes
 * Complying to the respective RFCs
 */

namespace URL {

  /*
   * Implementation of the Parser class
   */

  /*
   * Templated Constructor
   * Type checking using RTTI
   */

  template <typename T>
  Parser::Parser(T s) {

    /*
     * We're using `s' to set value of a string
     * Hence it must be a string compatible type
     */

    if(!Common::is_string(s))
      throw Common::Exception("Invalid type specified to URL::Parser::Parser", INVALID_TYPE);

    setQstr(s);
    
  }

  /*
   * Destructor, deallocates dicts and strings allocated, if any
   */

  Parser::~Parser() {

    /*
     * Clear memory for Dictonaries allocated
     */

    using namespace boost::lambda;
    
    std::for_each(Dicts.begin(), Dicts.end(), bind(delete_ptr(), _1));

    /*
     * Clear memory for char*'s allocated
     */
    
    std::for_each(Strings.begin(), Strings.end(), bind(delete_array(), _1));
    
  }

}
