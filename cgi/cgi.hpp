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
  
  //! Error codes for CGI namespace.  
  enum {
    E_QS_NOT_SET, //!< Query string not set. \sa Parser::getQstr
    E_INVALID_HEX_SYMBOL, //!< Invalid hexadecimal symbol. \sa #decodeHex
    E_ENV_NOT_FOUND, //!< Environment variable not found. \sa Request::getEnv
    E_PARAM_NOT_FOUND, //!< Parameter not found. \sa Request::getParam Session::operator[]
    E_INVALID_FILE_PTR, //!< Invalid file pointer. \sa Request::Request
    E_INVALID_CONTENT_LENGTH, //!< Invalid content length. \sa Request::Request
    E_COOKIE_NOT_FOUND, //!< Cookie not found. \sa Cookie::getCookie Cookie::operator[]
    E_COOKIE_ALREADY_PRESENT, //!< Cookie already present. \sa Cookie::operator[]
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
      \throw Common::Exception with #E_QS_NOT_SET if #source is empty
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

  /*! \brief Class to manage sessions

    HTTP is a stateless protocol, hence we have to handle sessions on the server side.
    We track users by using a cookie for session id and storing relevant data on server side.
    
    \remark This class implements singleton design pattern. Hence constructors are private.
    \sa getInstance destroyInstance
    \note Module is not complete
    \todo Complete the module using database as storage.\n
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
      \param[in] _id Session ID to be loaded
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
      \param[in] _id Session id to be loaded. Defaults to "" (empty string)
      \return Pointer to Session object
     */

    static Session* getInstance(const std::string _id = "");

    /*! \brief Method to destroy Session object

      \remark MUST be called when the object's use is over, otherwise it might lead to memory leaks.
      \param[in] ptr Pointer to Session object obtained from #getInstance
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

    /*! \remark Overloaded function
      \sa #operator[] #getData
    */
          
    std::string getData(const std::string key) {
      return data[key];
    }

    /*! \brief Load session #data from an existing dictionary
      \remark Method might be removed after everything is glued properly and is not used.
      \param[in] _data Reference to #Dict_t from where data is to be loaded
      \return Session& for cascading operations
    */
    
    Session& loadData(Dict_t& _data) {
      data = _data;
      return *this;
    }

    /*! \brief Set the expire time

      \remark time_t is defined in time.h (POSIX)
      \param[in] _expire Seconds since UNIX Epoch
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

      \param[in] key Key of the value required/to be set
      \return std::string& from #data
      \throw Common::Exception with #E_PARAM_NOT_FOUND if key is not found in #data
     */
    
    std::string& operator[] (const std::string key) {
      if(data.find(key) == data.end())
	throw Common::Exception("Session parameter `" + key + "` not found", E_PARAM_NOT_FOUND, __LINE__, __FILE__);
      return data[key];
    }
  };

  /*! \brief Class to manage %Cookie data

    Cookies can be used for storage of data on the client side.\n
    This is basically a cookie-jar.

    \coder{Nilesh G,nileshgr}
  */

  class Cookie {
  private:
    Dict_t cookies; //!< Dictionary to store cookies
    bool response; //!< Type of jar - will be true if jar is for response
    
  public:

    /*! \brief Constructor - Cookie Jar for response

      This cookie jar can be used for two purposes- for request data (means CGI::Request will use it)
      and for request data (means CGI::Response and others will use it).

      \param[in] _response If jar is for response, insertion/modification functions will be allowed \n
      otherwise it will not be allowed. Defaults to true (response).
    */

    Cookie(bool _response = true) : response (_response) {}

    /*! \brief Constructor- cookie parser

      The constructor parses the string into #cookies

      \param[in] _cookies string containing cookies.\n
      Format: NAME1=VALUE1; NAME2=VALUE2; (see RFC 3875)
      \remark This sets #response to false \sa Cookie(bool)
    */
    
    Cookie(std::string _cookies);

    /*! \brief Operator overloading for simple access to #cookies
      \param[in] name Name of the cookie
      \throw Common::Exception with #E_COOKIE_NOT_FOUND if named cookie is not found in #cookies and #response is false
      \throw Common::Exception with #E_COOKIE_ALREADY_PRESENT if named cookie is found in #cookies and #response is true
      \return std::string& to cookie's value
    */

    std::string& operator[](const std::string name) {
      bool found = cookies.find(name) == cookies.end();
      if(not response and found) // If response is false then check if cookie is found
	throw Common::Exception("Cookie with name: " + name + " was not found in the dictionary", E_COOKIE_NOT_FOUND, __LINE__, __FILE__);
      else if(response and not found) // If response is true then check if cookie is already present
	throw Common::Exception("Cookie with name: " + name + " is already present in the jar", E_COOKIE_ALREADY_PRESENT, __LINE__, __FILE__);
      return cookies[name];
    }

    /*! \brief Returns a copy of the cookie value
      \sa operator[]
    */

    std::string getCookie(const std::string name) {
      return cookies[name];
    }

    /*! \brief Sets a cookie

      This cookie jar can be used for both input & output. If Cookie::Cookie(bool) is used, then the class lets one modify contents
      else it doesn't. The class can be made to act for input only 
      
      \sa operator[]
      \param[in] name string containing name of cookie
      \param[in] value string containing value of cookie
    */

    void setCookie(const std::string name, const std::string value) {
      cookies[name] = value;
    }

    /*! \brief Returns all cookies
      \return #Dict_ptr_t of a #Dict_t which has everything from #cookies
    */
    
    Dict_ptr_t getCookies();
  };
  
  /*! \brief Class to manage Request data

    When a client requests a resource, the webserver feeds the parameters via environment variables and stdin (if POST)
    to the application. The methods in this class can be used to read data present in those.

    \todo Implement session processing using session-jar like cookies, using inheritance
    \coder{Nilesh G,nileshgr}
  */

  class Request : public Cookie {
  private:    
    Dict_t env; //!< Dictionary to hold environment variables
    Dict_t get; //!< Dictionary to hold HTTP GET data
    Dict_t post; //!< Dictionary to hold HTTP POST data

    Session *session; //!< Session class pointer.
    
  public:

    /*! \brief Options for which dictionary should be used

      #getData and #getParam use this to decide which dictionary to use.
      #getParam uses bitwise operators to search for the request key, hence multiple dictionaries to search for can be specifed
    */

    enum option_t {
      OPT_GET, //!< Use only HTTP GET data present in #get
      OPT_POST, //!< Use only HTTP POST data present in #post
      OPT_SESSION, //!< Use only session data available from CGI::Session
      OPT_ENV, //!< Use only environment variables data present in #env
    };

    /*! \brief Constructor
      \param[in] env Array of C-style strings for environment variables
      \param[in] in Pointer to FILE*, defaults to stdin
      \throw Common::Exception with #E_INVALID_FILE_PTR if in = NULL
      \throw Common::Exception with #E_INVALID_CONTENT_LENGTH if CONTENT_LENGTH = 0
     */

    Request(char** env, FILE* in = stdin);

    /*! \brief Returns value of environment variable from #env
      \param[in] name Name of environment variable
      \return Value of environment variable
      \throw Common::Exception with #E_PARAM_NOT_FOUND if the request variable is not found in #env
    */
    
    std::string getEnv(std::string name);

    /*! \brief Returns all data or combination of requested data

      All the requested data is contained in the class variables, #get, #post, #env and data available from CGI::Session \n
      This function will return the requested one (#Dict_ptr_t) or if multiple ones are specified (bitwise operators)
      then the returned #Dict_t will contain combination of those.

      \remark For cookie data, use inherited method #getCookies
      \param[in] option The dictionary which should be returned. Defaults to all values bitwise-or'd \sa #option_t
      \return #Dict_ptr_t for a #Dict_t containing the requested data      
     */

    Dict_ptr_t getData(unsigned option = OPT_GET | OPT_POST | OPT_SESSION | OPT_ENV);

    /*! \brief Returns value of single request parameter

       The default order for searching dictionaries is GPS - Get, Post and %Session.

       \remark For environment variables, use #getEnv and for cookies use inherited method #getCookie
       \param[in] name Name of the request parameter
       \param[in] option Dictionaries to search for. Defaults to all three of them (GPCS). \sa #option_t
       \return Value of the request parameter
       \throw Common::Exception with #E_PARAM_NOT_FOUND if requested parameter is not found in the specified dictionaries.
     */

    std::string getParam(std::string name, unsigned option = OPT_GET | OPT_POST | OPT_SESSION);

  };
}
#endif
