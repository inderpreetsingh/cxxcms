#ifndef REQUEST_HPP
#define REQUEST_HPP
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
     * environment variables
     * HTTP GET Data
     * HTTP POST Data
     */

    Dict_t env, *get, *post; // Declared as pointer because the actual parsing will take place in CGI::Parser

  public:

    enum option_t { GET, POST, COOKIE, ENV };

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

    Dict_t getData(unsigned short option = GET | POST | COOKIE | ENV);

    /*
     * This function will return the value of the parameter specified.
     * Option will be by default GET, POST and COOKIE.
     */

    std::string getParam(std::string, unsigned short option = GET | POST | COOKIE);

  };
}
#endif
