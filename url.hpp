#ifndef URL_HPP
#define URL_HPP 1
#include "common.hpp"
#include <map>
#include <string>
#include <memory>
#include <list>
#include <cmath>
#include <algorithm>

/*
 * This namespace URL will contain all URL-related functions and classes.
 * All according to RFCs.
 */

namespace URL {

  /*
   * Define the Dict and Tuple data types
   * Dict is a std::map<char*, char*> (two column)
   * Tuple is one single element of the map using std::pair<char*, char*>
   */

  typedef std::map<const char*, const char*> Dict_t;
  typedef std::pair<const char*, const char*> Tuple_t;
  
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
     * We are using const char* in the dictionary.
     * const char* is an array and hence std::auto_ptr cannot be used here, to automatically deallocate memory when the object goes out of scope
     * Hence we need tracking, for dictionaries allocated and strings (char*'s) allocated
     * so that we can free them when either clear() is called or object of this class is destroyed.
     * One object of this class might be used multiple times, and we can't gurantee that the older dictionaries are in use or not, hence this.
     */

    std::list<Dict_t*> Dicts;
    std::list<char*> Strings; // char* used here because we need modification access
    Tuple_t Tuple;

    /*
     * The raw query string as obtained from getenv('QUERY_STRING')
     */
    
    std::string source;

    /*
     * Query string sanitizer
     * There are some string combinations, that can produce errors, major ones!
     * Some of them are described here- https://bitbucket.org/nileshgr/cppcms/wiki/Plan
     */

    void _sanitize(std::string&);
    
  public:

    /*
     * Templated constructor accepting some kind of string
     * Calls setQstr(T)
     */

    template <typename T>
    Parser(T s) {
      setQstr(s);
    }

    /*
     * The destructor, which will free all memory allocated for dicts, etc.
     */

    ~Parser();

    /*
     * Templated property setter accepting some kind of string
     * String type checked with Common::is_string()
     */

    template <typename T>
    const Parser& setQstr(T);

    /*
     * Property retriever in type const char*
     */

    const char* getQstr() const;

    /*
     * The parser, returns reference to Dict 
     */

    Dict_t& parse();

    /*
     * Clear function. Calls the destructor to clear up everything allocated so far
     * and the object is like a new instance.
     */

    void clear() {
      this->~Parser();
    }

  };

  /*
   * Templated functions CANNOT be declared somewhere and defined somewhere else
   * Hence, defining them here.
   */

  template <typename T>
  const Parser& Parser::setQstr(T s) {

    /*
     * We're using `s' to set value of a string
     * Hence it must be a string compatible type
     */

    if(!Common::is_string(s))
      throw Common::Exception("Invalid type specified to URL::Parser::Parser", INVALID_TYPE);

    source = s;
    return *this;    
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
    source.erase(0, 1); // Removing % from %XX
    
    unsigned int i = 0;
    unsigned int j = source.size() - 1;
    unsigned int result = 0;

    /*
     * Convert the string to upper case
     */
    
    std::transform(source.begin(), source.end(), source.begin(), (int (*)(int)) std::toupper);
    // (int (*) (int)) added so that the proper toupper() in cctype is chosen instead of the one in locale.

    for (i = 0; i < source.size(); i++, j--)
        if (source.at(j) >= '0' and source.at(j) <= '9')
            result += (source.at(j) - '0') * std::pow(16, i);
        else if (source.at(j) >= 'A' and source.at(j) <= 'F')
	  result += ((source.at(j) - 'A') + 10) * std::pow(16, i); // eg: ('B' - 'A') + 10 = (66 - 65) + 10 = 11
	else
	  throw Common::Exception("Invalid HEX symbol found in Common::decodeHex", INVALID_HEX_SYMBOL);       
    return result;
  }
}
#endif
