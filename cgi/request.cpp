#include <cgi/cgi.hpp>
#include <cstdio>
#include <algorithm>
#include <cstring>

namespace CGI {

  Request::Request(char **envp, FILE* in) {

    env.reset(new Dict_t);
    cookie.reset(new Dict_t);
    
    /*
     * Parse **env into dict_t env (private variable)
     */

    while(*envp) {
      std::string tmp = *envp;
      size_t delimiter = tmp.find('=');
      env->insert(Tuple_t(tmp.substr(0, delimiter), tmp.substr(delimiter + 1)));
      envp++;
    }

    if(getEnv("QUERY_STRING").size())
      get = Parser(getEnv("QUERY_STRING")).parse(); // Parse the input query string into dict.      

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
      post = CGI::Parser(buf).parse();
      delete [] buf;
    }

    std::string cookies = getEnv("HTTP_COOKIE");

    while(cookies.size()) {
      size_t delimiter = std::string::npos;
      std::string key = "", value = "", extract = "";
      if((delimiter = cookies.find(';')) != std::string::npos) {
	extract = cookies.substr(0, delimiter);
	cookies.erase(0, delimiter + 2); // +2 because we have a space between multiple cookies. NAME1=VALUE1; NAME2=VALUE2; NAME3=VALUE3
      }
      else {
	extract = cookies;
	cookies.clear();
      }
      if((delimiter = extract.find('=')) != std::string::npos) {
	key = extract.substr(0, delimiter);
	value = extract.substr(delimiter + 1);
      }
      else {
	key = extract;
	value = "";
      }
      cookie->insert(Tuple_t(key, value));
    }
  }

  std::string Request::getEnv(std::string name) {
    Dict_t::iterator i;
    if((i = env->find(name)) != env->end())
      return i->second;
    /*
     * We have return statement above. If code has come here, it means none was found, so we throw exception.
     */
    throw Common::Exception("Environment variable " + name + "not found", E_ENV_NOT_FOUND, __LINE__, __FILE__);
  }

  // option below, is an optional parameter. See request.hpp

  Dict_ptr_t Request::getData(unsigned short option) {
    Dict_ptr_t ret (new Dict_t);
    Dict_t::iterator i;

    if(option & OPT_GET)
      for(i = get->begin(); i != get->end(); i++)
	ret->insert(*i);
    if(option & OPT_POST)
      for(i = post->begin(); i != post->end(); i++)
	ret->insert(*i);
    if(option & OPT_ENV)
      for(i = env->begin(); i != env->end(); i++)
	ret->insert(*i);
    if(option & OPT_COOKIE)
      for(i = cookie->begin(); i != cookie->end(); i++)
	ret->insert(*i);
    return ret;
  }

  // option below is an optional parameter. See request.hpp

  std::string Request::getParam(std::string name, unsigned short option) {

    // Order preference - GPC. For environment variables, use getEnv()

    Dict_t::iterator i;

    if(option & OPT_GET)
      if((i = get->find(name)) != get->end())
	return i->second;
    if(option & OPT_POST)
      if((i = post->find(name)) != post->end())
	return i->second;
    if(option & OPT_COOKIE)
      if((i = cookie->find(name)) != cookie->end())
	return i->second;
    /*
     * We have return statements above. If code has come here, it means none was found, so we throw exception.
     */
    throw Common::Exception("Request parameter " + name + " not found in GET, POST and COOKIE data", E_PARAM_NOT_FOUND, __LINE__, __FILE__);
  }
}
