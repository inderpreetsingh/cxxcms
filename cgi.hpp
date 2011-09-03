#ifndef CGI_HPP
#define CGI_HPP 1
#include "common.hpp"
#include <map>
#include <string>
#include <memory>

/*
 * This namespace CGI will contain all CGI-related functions and classes.
 * All according to RFCs.
 */

namespace CGI {

  /*
   * Define the Dict and Tuple data types
   * Dict is a std::map<string, string> (two column)
   * Tuple is one single element of the map using std::pair<string, string>
   */

  typedef std::map<const std::string, const std::string> Dict_t;
  typedef std::pair<const std::string, const std::string> Tuple_t;
  typedef std::auto_ptr <Dict_t> Dict_ptr_t;
  
  /*
   * Error codes for our URL namespace
   */

  enum err_t { QS_NOT_SET, INVALID_TYPE, INVALID_HEX_SYMBOL };

  /*
   * Query String Parser, according to RFC 1738
   */

  class Parser {
  private:

    /*
     * The raw query string as obtained from getenv('QUERY_STRING')
     */
    
    std::string source;

    /*
     * Query string sanitizer
     * There are some string combinations, that can produce errors, major ones!
     * Some of them are described here- https://bitbucket.org/nileshgr/cppcms/wiki/Plan
     */

    void _sanitize(std::string&, size_t);
    
  public:

    /*
     * Constructor accepting std::string
     * Calls setQstr()
     */

    Parser(std::string s) {
      setQstr(s);
    }

    Parser() {
    }

    /*
     * Property setter accepting std::string
     */

    const Parser& setQstr(std::string);

    /*
     * Property retriever in type const char*
     */

    const char* getQstr() const;

    /*
     * The parser, returns reference to auto_ptr'd Dict.
     */

    Dict_ptr_t parse();

    /*
     * Clear function. Calls the destructor to clear up everything allocated so far
     * and the object is like a new instance.
     */

    void clear() {
      this->~Parser();
    }
  };

  /*
   * Hex decoder
   */

  int decodeHex(std::string);  
}
#endif
