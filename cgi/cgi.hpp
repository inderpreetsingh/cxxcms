#ifndef CGI_HPP
#define CGI_HPP
#include <global.hpp>
#include <common/common.hpp>
#include <ctime>
#include <string>

/*! \namespace CGI
  \brief The %CGI - %Common Gateway Interface module.

  The %Common Gateway Interface is the basic protocol by which a webserver communicates data to the application.
  The Query string, HTTP GET/POST, HTTP header, etc, data is available through the %CGI protocol (environment variables, stdin).
*/

namespace CGI {
  
  /*! \brief Error codes for CGI namespace.
    \todo Move error codes to global.hpp (also rename if required).
  */    
  
  enum {
    E_QS_NOT_SET, //!< Query string not set. \sa Parser::getQstr
    E_INVALID_HEX_SYMBOL, //!< Invalid hexadecimal symbol. \sa #decodeHex
    E_ENV_NOT_FOUND, //!< Environment variable not found. \sa Request::getEnv
    E_PARAM_NOT_FOUND, //!< Parameter not found. \sa Request::getParam
    E_INVALID_FILE_PTR, //!< Invalid file pointer. \sa Request::Request
    E_INVALID_CONTENT_LENGTH, //!< Invalid content length. \sa Request::Request
  };

  /*! \brief Hex decoder

     Query string often contains characters in hex format which have special meaning.
     We need the actual representation/ASCII of the same.

     \param[in] source std::string containing a single hex code - \%XX
     \return int ASCII code of the character
     \coder{Nilesh G,nileshgr}
   */

  int decodeHex(std::string source);

  /*! \brief Hex encoder

    Hex encoder encodes a character which has a special meaning in the query string to
    it's hexadecimal representation (\%XX).

    \param[in,out] source std::string&
    \return std::string& source (processed)
    \coder{Ershad K,ershus}
  */

  std::string& encodeHex(std::string& source);

  /*! \brief Query string parser

    Query string is the main source of data for HTTP applications, because most of the requests are HTTP GET.
    The data is available in enviornment variable QUERY_STRING as per the %CGI/1.1 protocol.
   */

  class Parser {
  private:

    //! The raw query string supplied to CGI::Parser::Parser or CGI::Parser::setQstr
    
    std::string source;

    /*! \brief Query string sanitizer

      Removes certain string combinations in the query string that can cause hindrance while parsing it.

      \param[in,out] s The query string
      \param[in] n Offset to start checking from
      \coder{Ershad K,ershus}
    */

    void _sanitize(std::string& s, size_t n = 1);
    
  public:

    /*! \brief Constructor (1 parameter)
      \param[in] s Query string
    */

    Parser(std::string s) {
      source = s;
    }

    //! Default constructor
    
    Parser() = default;

    /*! \brief Sets the query string
      \param[in] s Query string
      \return const Parser& for cascading
    */
    
    const Parser& setQstr(std::string s) {
      source = s;
      return *this;
    }

    /*! \return Query string in const char* form
      \throw Common::Exception if #source is empty
     */

    const char* getQstr() const {
      if(source.empty())
	throw Common::Exception("Query string propery requested while it was never set! in URL::Parser::getQstr()", E_QS_NOT_SET, __LINE__, __FILE__);
      return source.c_str();
    }

    /*! \brief Query string parser
      \return #Dict_ptr_t
     */

    Dict_ptr_t parse();
    
  };

  /*! \brief Class to manage Request data

    When a client requests a resource, the webserver feeds the parameters via environment variables and stdin (if POST)
    to the application. The methods in this class can be used to read data present in those.

    \coder{Nilesh G,nileshgr}
    \todo Add CGI::Session support
  */

  class Request {
  private:    
    Dict_t env; //!< Dictionary to hold environment variables
    Dict_t get; //!< Dictionary to hold HTTP GET data
    Dict_t post; //!< Dictionary to hold HTTP POST data
    Dict_t cookie; //!< Dictionary to hold cookie data

  public:

    /*! \brief Options for which dictionary should be used

      #getData and #getParam use this to decide which dictionary to use.
      #getParam uses bitwise operators to search for the request key, hence multiple dictionaries to search for can be specifed
    */

    enum option_t {
      OPT_GET, //!< Use only HTTP GET data present in #get
      OPT_POST, //!< Use only HTTP POST data present in #post
      OPT_COOKIE, //!< Use only cookie data present in #cookie
      OPT_ENV //!< Use only environment variables data present in #env
    };

    /*! \brief Constructor
      \param[in] env Array of C-style strings for environment variables
      \param[in] in Pointer to FILE*, defaults to stdin
      \throw Common::Exception if CONTENT_LENGTH = 0 or in = NULL
     */

    Request(char** env, FILE* in = stdin);

    /*! \brief Returns value of environment variable from #env
      \param[in] name Name of environment variable
      \return Value of environment variable
      \throw Common::Exception if the request variable is not found in #env
    */
    
    std::string getEnv(std::string name);

    /*! \brief Returns all data or combination of requested data

      All the requested data is contained in the class variables, #get, #post, #env and #cookie \n
      This function will return the requested one (#Dict_ptr_t) or if multiple ones are specified (bitwise operators)
      then the returned #Dict_t will contain combination of those.

      \param[in] option The dictionary which should be returned. Defaults to all values bitwise-or'd \sa #option_t
      \return #Dict_ptr_t for a #Dict_t containing the requested data      
     */

    Dict_ptr_t getData(unsigned option = OPT_GET | OPT_POST | OPT_COOKIE | OPT_ENV);

    /*! \brief Returns value of single request parameter

       The default order for searching dictionaries is GPC - Get, Post and Cookie. For environment variables, use #getEnv

       \param[in] name Name of the request parameter
       \param[in] option Dictionaries to search for. Defaults to all three of them (GPC). \sa #option_t
       \return Value of the request parameter
       \throw Common::Exception if requested parameter is not found in the dictionary(s).
     */

    std::string getParam(std::string name, unsigned option = OPT_GET | OPT_POST | OPT_COOKIE);

  };

  /*! \brief Class to manage sessions

    HTTP is a stateless protocol, hence we have to handle sessions on the server side.
    We track users by using a cookie for session id and storing relevant data on server side.
    
    \remark This class implements singleton design pattern. Hence constructors are private.
    \sa getInstance destroyInstance
    \note Module is not complete
    \todo Complete the module by using cookies, and database.\n
    Add support in CGI::Request.\n
    %Session destruction should be written to storage.
    \coder{Nilesh G,nileshgr}
  */

  class Session {
  private:    
    std::string id; //!< %Session id (identifer)
    Dict_t data; //!< %Session data dictionary
    time_t expire; //!< %Session expiry time

    //! Constructor to start/create a new session      
    Session();

    /*! \brief Constructor for loading existing session present in storage
      \param _id Session ID to be loaded
      \todo Throw Common::Exception if session is not found
    */
    
    Session(const std::string _id);

    //! Private default copy constructor (disabling it)
    
    Session(const Session&) = default;
    
  public:

    /*! \brief Method to return pointer to instance

      A static variable is used inside to track session object, ensuring only one instance at a time.
      If the pointer is NULL, a new instance will be created and returned, if not NULL, the same will be returned.

      \sa destroyInstance
      \param _id Session id to be loaded. Defaults to "" (empty string)
      \return Pointer to Session object
     */

    static Session* getInstance(const std::string _id = "");

    /*! \brief Method to destroy Session object

      \remark MUST be called when the object's use is over, otherwise it might lead to memory leaks.
      \param ptr Pointer to Session object obtained from #getInstance
     */

    static void destroyInstance(Session* ptr) {
      if(ptr)
	delete ptr;
    }

    //! \return %Session ID
    std::string getSessionId() {
      return id;
    }

    /*! \brief Method to retrive everything present in #data

      Returns a new dictionary, modifications won't reflect.

      \remark Overloaded function
      \sa getData(const std::string)
      \return Dict_ptr_t of a newly allocated #Dict_t containing everything from #data
    */

    Dict_ptr_t getData();

    /*! \brief Wrapper around #operator[] to add data
      \return Session& for cascading operations
     */
    
    Session& addData(const std::string key, const std::string value) {
      data[key] = value;
      return *this;
    }

    /*! \brief Method to retrive value corresponding to key
      \param key Key of the value required
      \return std::string value from #data
      \remark std::map will throw std::exception if key is not found
      \todo Handle exceptions from std::map and throw Common::Exception instead
    */
          
    std::string getData(const std::string key) {
      return data[key];
    }

    /*! \brief Load session #data from an existing dictionary
      \param _data Reference to #Dict_t from where data is to be loaded
      \return Session& for cascading operations
    */
    
    Session& loadData(Dict_t& _data) {
      data = _data;
      return *this;
    }

    /*! \brief Set the expire time

      \remark time_t is defined in time.h (POSIX)
      \param _expire Seconds since UNIX Epoch
      \return Session& for cascading operations
     */
    
    Session& setExpireTime(time_t _expire) {
      expire = _expire;
      return *this;
    }

    //! Retrieve expire time (since UNIX Epoch)

    time_t getExpireTime() {
      return expire;
    }

    /*! \brief Operator overloading for simple access to #data

      Can be used to retrieve or set pairs

      \param key Key of the value required/to be set
      \return std::string& from #data
     */
    
    std::string& operator[] (const std::string key) {
      return data[key];
    }
  };
}
#endif
