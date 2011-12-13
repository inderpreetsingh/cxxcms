#ifndef CGI_HPP
#define CGI_HPP
#include <global.hpp>
#include <common/common.hpp>
#include <ctime>
#include <string>
#include <memory>

/*! \file cgi.hpp
  \brief %CGI namespace definition

  File contains definition of various classes and functions for the %CGI module
*/

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
    E_PARAM_NOT_FOUND, //!< Parameter not found. \sa Request::getParam Session::getParam Cookie::getParam
    E_INVALID_FILE_PTR, //!< Invalid file pointer. \sa Request::Request
    E_INVALID_CONTENT_LENGTH, //!< Invalid content length. \sa Request::Request
    E_COOKIE_REQUEST, //!< Cookie is in request mode. \sa Cookie::response
    E_SESSION_REQUEST, //!< Session is in request mode. \sa Session::response
    E_POST_BINARY, //!< POST data is binary. \sa Request::getData Request::getParam
    E_POST_NOT_BINARY, //!< POST data is not binary. \sa Request::getBinPost
    E_RESPONSE_BINARY, //!< Response is binary. \sa Response::getCompleteBody Response::getContentBody
    E_RESPONSE_NOT_BINARY, //!< Response is not binary. \sa Response::getBinaryData
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

    void _sanitize(std::string& s, size_t n = 1) const;
    
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

    Dict_ptr_t parse() const;
    
  };

  /*! \brief Class to manage sessions

    HTTP is a stateless protocol, hence we have to handle sessions on the server side.
    We track users by using a cookie for session id and storing relevant data on server side.

    \remark The session id, data, expire and response is common to all instances (static) and hence, over multiple instances, the methods
    will work on the same piece of %data instead of having their own copy.
    \note Module is not complete
    \todo Complete the module using database as storage.\n
    %Session destruction should be written to storage.
    \coder{Nilesh G,nileshgr}
  */

  class Session {
  private:
    static std::string id; //!< %Session identifier (id)
    static Dict_t data; //!< %Session data dictionary
    static time_t expire; //!< %Session expiry time
    static bool response; //!< Variable to track if it is in response mode or request mode
    
  public:

    /*! \brief Constructor to start/create a new session

      This class is inherited by Request, so we need to provide a default constructor.\n
      We will assume by default response mode here and if the other constructor Session(const std::string) gets called
      we will switch to request mode, hence prohibiting insertion/modification
    */

    Session();

    /*! \brief Constructor for loading existing session present in storage (request mode)
      \param[in] _id Session ID to be loaded
      \todo Throw Common::Exception if session is not found
    */
    
    Session(std::string _id);

    //! \return %Session ID
    const std::string getSessionId() const {
      return Session::id;
    }

    /*! \brief Method to retrive everything present in #data
      \remark Returns a new dictionary, modifications won't reflect.
      \sa getParam(const std::string)
      \return Dict_ptr_t of a newly allocated #Dict_t containing everything from #data
    */

    virtual Dict_ptr_t getData() const;

    /*! \brief Method to set parameter

      This is a virtual method because Response will have a wrapper.
      
      \throw Common::Exception with E_SESSION_REQUEST if class is in request mode
      \param[in] name Name of the session parameter
      \param[in] value Value of the session parameter
      \return const Session& for cascading operations
     */
    
    virtual const Session& setParam(std::string name, std::string value) {
      if(not response)
	throw Common::Exception("You are not allowed to set a session parameter in a request", E_SESSION_REQUEST, __LINE__, __FILE__);
      Session::data[name] = value;
      return *this;
    }

    /*! Method to retrieve a parameter

      This is a virtual method because Request has a wrapper.
      
      \param[in] name Name of the session parameter
      \return const std::string value from #data
      \throw Common::Exception with #E_PARAM_NOT_FOUND if key is not found in #data
    */
          
    virtual const std::string getParam(std::string name) const {
      Dict_t::iterator i;
      if((i = Session::data.find(name)) == Session::data.end())
	throw Common::Exception("Session parameter `" + name + "` was not found", E_PARAM_NOT_FOUND, __LINE__, __FILE__);
      return i->second;
    }

    /*! \brief Load session #data from an existing dictionary
      \remark Method might be removed after everything is glued properly and is not used.
      \param[in] _data Reference to #Dict_t from where data is to be loaded
      \return const Session& for cascading operations
    */
    
    const Session& loadData(const Dict_t& _data) {
      Session::data = _data;
      return *this;
    }

    /*! \brief Set the expire time
      \remark time_t is defined in time.h
      \param[in] _expire Seconds since UNIX Epoch
      \return Session& for cascading operations
     */
    
    Session& setExpireTime(time_t _expire) {
      Session::expire = _expire;
      return *this;
    }

    //! Retrieve expire time (since UNIX Epoch)

    time_t getExpireTime() const {
      return Session::expire;
    }
  };

  /*! \brief Structure to store cookies value and other attributes

    std::map will be used with key as name and value as this structure to store cookies
  */

  struct cookie_t {
    std::string value; //!< Value of cookie
    std::string path; //!< Path of cookie
    std::string domain; //!< Domain name (must start with a dot) of cookie
    time_t expire; //!< Expiration time since UNIX Epoch
    bool secure; //!< Cookie is HTTPS only?
    bool httponly; //!< Cookie only for HTTP requests

    // value, path and domain are initialized to empty string by string's constructor
    // We initialize expire, secure and httponly to 0, false, true by default
    cookie_t() : expire(0), secure(false), httponly(true) {}
  };

  /*! \brief Class to manage %Cookie data

    Cookies can be used for storage of data on the client side.\n
    This is basically a cookie-jar.

    \coder{Nilesh G,nileshgr}
  */

  class Cookie {
  public:
    typedef std::map<std::string, cookie_t> cookie_dict_t; //!< Type definition for cookie dictionary
    typedef std::pair<std::string, cookie_t> cookie_tuple_t; //!< Type definition for cookie pair
  private:
    cookie_dict_t cookies; //!< Dictionary to store cookies, consisting of name & cookie_t
    /*! \brief Type of jar - will be true if jar is for response

      If jar is in request mode, then only value will be used in cookie_t.
    */
    bool response; 
    
  public:
    
    /*! \brief Constructor - Cookie Jar for response

      By default we assume response mode. If Cookie::Cookie(std::string) is called, we switch to request mode.
    */

    Cookie() : response (true) {}

    /*! \brief Constructor- cookie parser

      The constructor parses the string into #cookies

      \param[in] _cookies string containing cookies.\n
      Format: NAME1=VALUE1; NAME2=VALUE2; (see RFC 3875)
      \remark This sets #response to false \sa Cookie(bool)
    */
    
    Cookie(std::string _cookies);

    /*! \brief Returns a copy of cookie_t
      \param[in] name Name of the cookie
      \throw Common::Exception with #E_PARAM_NOT_FOUND if name is not found in #cookies
      \return cookie_t copy present in #cookies
    */

    cookie_t getCookie(std::string name) {
      cookie_dict_t::iterator i;
      if((i = cookies.find(name)) == cookies.end())
	throw Common::Exception("Cookie named `" + name + "` was not found", E_PARAM_NOT_FOUND, __LINE__, __FILE__);
      return i->second;
    }

    /*! \brief Sets a cookie
      \param[in] name string containing name of cookie
      \param[in] data cookie_t containing value and other parameters
      \return Cookie& for cascading operations
      \throw Common::Exception with E_COOKIE_REQUEST if response is false
    */

    Cookie& setCookie(std::string name, cookie_t data) {
      cookie_dict_t::iterator i;
      if(not response)
	throw Common::Exception("You are not allowed to set a cookie in request mode", E_COOKIE_REQUEST, __LINE__, __FILE__);
      cookies[name] = data;
    }

    /*! \brief Returns all cookies
      \return Copy of #cookies
    */

    cookie_dict_t getCookies() {
      return cookies;
    }
  };

  /*! \brief Class to manage HTTP %Request data

    When a client requests a resource, the webserver feeds the parameters via HTTP headers which are translated to environment variables
    (and stdin if POST) to the application. The methods in this class can be used to read data present in those.

    \coder{Nilesh G,nileshgr}
  */

  class Request : public Cookie, public Session {
  private:    
    Dict_t env; //!< Dictionary to hold environment variables
    Dict_t get; //!< Dictionary to hold HTTP GET data
    Dict_t post; //!< Dictionary to hold HTTP POST data
    bool rawpostdata; //!< Variable to check if the POST data received was ASCII or binary (file upload)
    char *postBuffer; //!< If rawpostdata is true, then we cannot use post to store data, we need to use buffer
    char *postBuffer_fncall; //!< Pointer to the memory created and returned by #getBinPost, so that it can be deleted[]d on class destruction
    
  public:

    /*! \brief Options for which dictionary should be used

      #getData and #getParam use this to decide which dictionary to use.
      #getParam uses bitwise operators to search for the request key, hence multiple dictionaries to search for can be specifed
    */

    enum option_t {
      GET, //!< Use only HTTP GET data present in #get
      POST, //!< Use only HTTP POST data present in #post
      SESSION, //!< Use only CGI::Session
      ENV, //!< Use only environment variables data present in #env
    };

    /*! \brief Constructor
      \param[in] env Array of C-style strings for environment variables
      \throw Common::Exception with #E_INVALID_CONTENT_LENGTH if request mode is #POST and CONTENT_LENGTH = 0
     */

    Request(char** env);

    /*! \brief Returns all data or combination of requested data

      All the requested data is contained in the class variables, #get, #post, #env and data available from CGI::Session \n
      This function will return the requested one (#Dict_ptr_t) or if multiple ones are specified (bitwise operators)
      then the returned #Dict_t will contain combination of those.

      \sa Cookie::getCookies
      \param[in] option The dictionary which should be returned. Defaults to all values bitwise-or'd \sa #option_t
      \throw Common::Exception with #E_POST_BINARY if option has POST and #rawpostdata is true.
      \return #Dict_ptr_t for a #Dict_t containing the requested data      
     */

    Dict_ptr_t getData(unsigned option = GET | POST | SESSION | ENV);

    /*! \brief Returns value of single request parameter

       The default order for searching is GPSE - Get, Post, %Session and Environment variables

       \sa Cookie::getCookie
       \param[in] name Name of the request parameter
       \param[in] option Dictionaries to search for. Defaults to all five of them (GPSE). \sa #option_t
       \return Value of the request parameter
       \throw Common::Exception with #E_PARAM_NOT_FOUND if requested parameter is not found in the specified dictionaries.
       \throw Common::Exception with #E_POST_BINARY if option has #POST and #rawpostdata is true
     */

    std::string getParam(std::string name, unsigned option = GET | POST | SESSION | ENV);

    /*! \brief Returns post data if it is binary (file upload)

      We cannot use getParam or getData if HTTP POST data is binary

      \remark Do not delete[] the pointer returned. It is take care of by ~Request
      \throw Common::Exception with #E_POST_NOT_BINARY if #rawpostdata = false
      \return char* #postBuffer_fncall
    */
    
    char* getBinPost();

    //! Destructor, to deallocate memory in #postBuffer and #postBuffer_fncall (if present)

    ~Request();
  };

  /*! \brief Class to manage response data

    Response body will be generated by this class depending on the parameters fed via methods.
    In this class, Cookie & Session will be used in write-mode.
    
    \coder{Nilesh G,nileshgr}
  */

  class Response : public Cookie, public Session {
  private:
    Dict_t headers; //!< Headers are sent before body and even Cookie is present in HTTP header.
    std::string completeBody; //!< The complete response body (includes headers)
    std::string contentBody; //!< Content body (response body excluding headers)
    /*! \brief Binary mode

      Binary mode should be used when we need to emit binary files, for instance generate image and emit it
      #binary is used to know if we should use binary mode or not
      \sa #binaryData
    */
    bool binary;
    std::unique_ptr<char[]> binaryData; //!< Holds pointer to binary data
    size_t binaryLength; //!< Holds length of binary data
    std::string headerString; //!< String to store headers (parsed)

    /*! \brief Parses #headers into #headerString

      Headers are sent as Name-value pairs separated by colon and CRLF.
      We translate #headers into this format and store it in #headerString, so that
      it can be directly prepended to the output
    */
    void setupHeaders();

  public:

    //! \brief Options for where data should be added when #setParam is called
    
    enum option_t {
      SESSION, //!< Set a session parameter
      HEADER, //!< Set a response header
    };

    /*! \brief Constructor, sets up intial values for various parameters

      The constructor sets the session cookie, and sets the default headers to the following values:\n
      Content-Encoding: utf-8
      Content-Type: text/html
    */

    Response();

    /*! \brief Sets parameters in the specified context

      If #HEADER is used as option then the parameter will be added to #headers
      
      \param name Name of parameter
      \param value Value of parameter
      \param option Context of parameter (#option_t)
      \return Response& for cascading operation
    */
    
    Response& setParam(std::string name, std::string value, option_t option) {
      if(option == HEADER)
	headers[name] = value;
      if(option == SESSION)
	Session::setParam(name, value);
    }

    /*! \brief Returns the specified parameter from the context

      If #HEADER is used as option then the paramter will be obtained from #headers

      \param name Name of parameter
      \param option Context of parameter (#option_t)
      \throw Common::Exception with #E_PARAM_NOT_FOUND if parameter is not found in the context
      \return std::string Value of parameter
    */

    std::string getParam(std::string name, option_t option);

    /*! \brief Appends data to #contentBody
      \param data Data to be appended      
      \return Response& for cascading operations
    */

    Response& appendBody(std::string data) {
      if(binary)
	throw Common::Exception("Response is binary", E_RESPONSE_BINARY, __LINE__, __FILE__);
      contentBody += data;
    }

    /*! \brief Clears #contentBody, #completeBody and if #binary is true, #binaryData (deallocates memory)
      \return Response& for cascading operations
    */

    Response& clearBody();

    /*! \brief Returns content body
      \remark Reference is returned because body data might be large
      \throw Common::Exception with #E_RESPONSE_BINARY if #binary is true
      \return std::string& #contentBody
    */

    std::string& getContentBody() {
      if(binary)
	throw Common::Exception("Response is binary", E_RESPONSE_BINARY, __LINE__, __FILE__);
      return contentBody;
    }

    /*! \brief Returns complete body

      The processing happens here- conversion of cookies into HTTP headers and other headers
      present in #headers into the form in which them must be emitted
      
      \remark Reference is returned because the data might be large
      \throw Common::Exception with #E_RESPONSE_BINARY if #binary is true
      \return std::string& #completeBody
    */

    std::string& getCompleteBody();

    /*! \brief Add binary body
      \remark
      -# If this is used, content added via #appendBody will be discarded
      -# Sets #binary to true
      \param _binaryData std::unique_ptr holding pointer to block of data
      \param _binaryLength Length/size of data (bytes)
      \return Response& for cascading operations
    */
    
    Response& setBinaryBody(std::unique_ptr<char[]> _binaryData, size_t _binaryLength) {
      binary = true;
      binaryData = std::move(_binaryData);
      binaryLength = _binaryLength;
    }

    /*! \brief Returns binary body
      \throw Common::Exception with #E_RESPONSE_NOT_BINARY if #binary is false
      \return std::unique_ptr<char[]> of memory location containing the parsed header string and binary data.
    */

    std::unique_ptr<char[]> getBinaryBody();
  };
}
#endif
