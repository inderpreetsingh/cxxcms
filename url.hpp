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

  enum err_t { QS_NOT_SET, INVALID_TYPE };

  /*
   * Query String Parser, according to RFC 1738
   */

  class Parser {
    
  private:

    /*
     * We're using const char* in the dictionary, hence it is important to free memory
     * after the object goes out of scope.
     * std::auto_ptr doesn't work with arrays, here const char* is an array,
     * So, this variable will keep a track of all dicts allocated till now, considering that
     * one object of this class might be used multiple times.
     * Memory will be freed in the destructor.
     * We have a count variable as well, to know how many dicts were allocated (see below)
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
     * String type checked with Common::is_string()
     */

    template <typename T>
    Parser(T);

    /*
     * The destructor, which will free all memory allocated for dicts, etc.
     */

    ~Parser();

    /*
     * Templated property setter accepting some kind of string
     * String type checked with Common::is_string()
     */

    template <typename T>
    void setQstr(T);

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
   * Templated function to decode() HEX (%FF) in Query Strings
   * This function will be used by Parser::parse()
   * Returns char, accepts char* or std::string.
   * Type will be checked in the function using RTTI
   */

  template <typename T>
  char decode(T);

}
