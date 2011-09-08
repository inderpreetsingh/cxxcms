#ifndef CGI_REQUEST_HPP
#define CGI_REQUEST_HPP
#include <cgi/cgi.hpp>

namespace CGI {

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
}
#endif
