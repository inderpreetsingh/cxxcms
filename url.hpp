#include "common.hpp"
#include <map>
#include <string>
#include <memory>
#include <list>

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

  enum err_t { QS_NOT_SET, INVALID_TYPE, MORE_THAN_ONE_HEX };

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
   * Templated function to decodeHex() HEX (%FF) in Query Strings
   * This function will be used by Parser::parse()
   * Returns char, accepts char* or std::string.
   * Type will be checked in the function using RTTI
   */

  template <typename T>
  char decodeHex(T);
}
