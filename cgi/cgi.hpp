#ifndef CGI_HPP
#define CGI_HPP
#include <global.hpp>
#include <common/common.hpp>
#include <ctime>
#include <string>

namespace CGI {
  
  /*
   * Error codes for CGI namespace
   */

  enum {
    E_QS_NOT_SET,
    E_INVALID_HEX_SYMBOL,
    E_ENV_NOT_FOUND,
    E_PARAM_NOT_FOUND,
    E_INVALID_FILE_PTR,
    E_INVALID_CONTENT_LENGTH,
    E_MORE_THAN_ONE_INSTANCE };

  /*
   * Hex decoder
   */

  int decodeHex(std::string);

  /*
   * Hex encoder
   */

  std::string& encodeHex(std::string&);

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
     */

    Parser(std::string s) {
      source = s;
    }

    Parser() {
    }

    /*
     * Property setter accepting std::string
     */

    const Parser& setQstr(std::string s) {
      source = s;
      return *this;
    }

    /*
     * Property retriever in type const char*
     */

    const char* getQstr() const {
      if(!source.size())
	throw Common::Exception("Query string propery requested while it was never set! in URL::Parser::getQstr()", E_QS_NOT_SET, __LINE__, __FILE__);
      return source.c_str();
    }

    /*
     * The parser, returns reference to auto_ptr'd Dict.
     */

    Dict_ptr_t parse();
    
  };

  /*
   * The request class
   * Accepts array of c-strings in constructor
   * And sets up properties, that can be used to check and obtain various parameters
   * of the request.
   * This is according to RFC 3875 (Only for HTTP(S)).
   */

  class Request {
  private:

    /*
     * Three dictionaries to hold data related to
     * Environment variables
     * HTTP GET Data
     * HTTP POST Data
     * HTTP COOKIE Data
     */

    Dict_ptr_t env, get, post, cookie;

  public:

    enum {
      OPT_GET,
      OPT_POST,
      OPT_COOKIE,
      OPT_ENV };

    /*
     * Constructor, accepts array of strings (for env and optionally pointer to file- stdin)
     */

    Request(char** env, FILE* in = stdin);

    /*
     * This function will return the value of specific environment variable, if specified and it is present in the dict.
     * If absent, exception will be thrown.
     */
    
    std::string getEnv(std::string);

    /*
     * This function will return the dict as specified by the option.
     * Basically, it contains all the request data.
     */

    Dict_ptr_t getData(unsigned short option = OPT_GET | OPT_POST | OPT_COOKIE | OPT_ENV);

    /*
     * This function will return the value of the parameter specified.
     * Option will be by default GET, POST and COOKIE.
     */

    std::string getParam(std::string, unsigned short option = OPT_GET | OPT_POST | OPT_COOKIE);

  };

  class Session {
  private:    
    std::string id;
    Dict_t data;
    time_t expire;

    /*
     * We have private constructors and copy-constructor
     * because this class implements singleton pattern
     */

    Session();
    Session(const std::string&);
    Session(const Session&) = default;
    
  public:

    /*
     * Method to return pointer to instance
     * This function will create a new object of this class if instance does not exist
     */

    static Session* getInstance(const std::string = "");

    /*
     * Method to destroy the instance specified as argument
     * This method MUST be called when the object's use is over, otherwise it might lead to memory leaks.
     */

    static void destroyInstance(Session* ptr) {
      if(ptr)
	delete ptr;
    }

    /*
     * This function will return the session id of the class     
     */

    std::string getSessionId() {
      return id;
    }

    /*
     * Returns the whole dictionary
     */
    
    Dict_ptr_t getAll();

    /*
     * Functions to add/get session data
     */
    
    void addData(const std::string key, const std::string value) {
      data[key] = value;
    }
    
    std::string getData(const std::string key) {
      return data[key];
    }

    /*
     * This will load the session dictionary from the one passed as a parameter
     */
    
    void loadData(Dict_t& sdata) {
      data = sdata;
    }

    /*
     * Sets the expire time. time_t is defined in time.h (POSIX)
     */
    
    void setExpireTime(time_t exp) {
      expire = exp;
    }

    /*
     * Returns the expire time
     */

    time_t getExpireTime() {
      return expire;
    }

    /*
     * Operator overloading for simple access to data
     */
    
    std::string& operator[] (const std::string key) {
      return data[key];
    }
  };
}
#endif
