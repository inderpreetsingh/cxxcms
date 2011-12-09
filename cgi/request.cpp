#include <cgi/cgi.hpp>
#include <cstdio>
#include <algorithm>
#include <cstring>

namespace CGI {

  Request::Request(char **envp, FILE* in) {    

    //Parse **env into dict_t env (private variable)

    while(*envp) {
      std::string tmp = *envp;
      size_t delimiter = tmp.find('=');
      env.insert(Tuple_t(tmp.substr(0, delimiter), tmp.substr(delimiter + 1)));
      envp++;
    }

    if(getEnv("QUERY_STRING").size())
      get = *(Parser(getEnv("QUERY_STRING")).parse()); // Parse the input query string into dict.      

    std::string reqmethod = getEnv("REQUEST_METHOD");
    std::transform(reqmethod.begin(), reqmethod.end(), reqmethod.begin(), (int (*)(int)) std::toupper); // Convert to uppercase

    if(reqmethod == "POST") {

      /*
       * As per CGI specifications, HTTP POST data is available in stdin.
       * Since we're using FastCGI, streams may be changed by the library.
       * Hence, we accept the stream as argument.
       * We need to read data from the stream, whose length is defined in the environment variable CONTENT_LENGTH
       */

      if(in == NULL)
	throw Common::Exception("Invalid FILE in pointer", E_INVALID_FILE_PTR, __LINE__, __FILE__);
      
      size_t length = 0;
      std::sscanf(getEnv("CONTENT_LENGTH").c_str(), "%zu", &length); // %zu - size_t

      if(!length)
	throw Common::Exception("Invalid CONTENT_LENGTH", E_INVALID_CONTENT_LENGTH, __LINE__, __FILE__);
      
      char *buf = new char[length + 1];
      std::memset(buf, 0, length + 1);
      std::fread(buf, 1, length, in);
      post = *(CGI::Parser(buf).parse());
      delete [] buf;
    }

    Cookie(getEnv("HTTP_COOKIE"));

    //! \todo Use session cookie name from configuration file
    try {
      std::string sid = getCookie("sess_id");
      session = Session::getInstance(sid);
    }
    catch(Common::Exception e) {
      if(e == E_COOKIE_NOT_FOUND)
	session = Session::getInstance();
      else
	throw e;
    }
  }

  std::string Request::getEnv(std::string name) {
    Dict_t::iterator i;
    if((i = env.find(name)) != env.end())
      return i->second;
    /*
     * We have return statement above. If code has come here, it means none was found, so we throw exception.
     */
    throw Common::Exception("Environment variable " + name + "not found", E_ENV_NOT_FOUND, __LINE__, __FILE__);
  }

  Dict_ptr_t Request::getData(unsigned option) {
    Dict_t::iterator i;
    Dict_ptr_t ret (new Dict_t);
    if(option & OPT_GET)
      for(i = get.begin(); i != get.end(); i++)
	ret->insert(*i);
    if(option & OPT_POST)
      for(i = post.begin(); i != post.end(); i++)
	ret->insert(*i);
    if(option & OPT_ENV)
      for(i = env.begin(); i != env.end(); i++)
	ret->insert(*i);
    return ret;
  }

  // option below is an optional parameter. See request.hpp

  std::string Request::getParam(std::string name, unsigned option) {

    // Order preference - GP. For environment variables, use getEnv() for cookies use inherited method getCookie()

    Dict_t::iterator i;

    if((option & OPT_GET) and ((i = get.find(name)) != get.end()))
      return i->second;
    if((option & OPT_POST) and ((i = post.find(name)) != post.end()))
      return i->second;
    
    throw Common::Exception("Request parameter " + name + " not found in GET, POST and COOKIE data", E_PARAM_NOT_FOUND, __LINE__, __FILE__);
  }
}
