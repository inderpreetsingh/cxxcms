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
      env[tmp.substr(0, delimiter)] = tmp.substr(delimiter + 1);
      envp++;
    }

    if(getParam("QUERY_STRING", ENV).size())
      get = *(Parser(getParam("QUERY_STRING", ENV)).parse()); // Parse the input query string into dict.      

    std::string reqmethod = getParam("REQUEST_METHOD", ENV);
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
      std::sscanf(getParam("CONTENT_LENGTH", ENV).c_str(), "%zu", &length); // %zu - size_t

      if(not length)
	throw Common::Exception("Invalid CONTENT_LENGTH", E_INVALID_CONTENT_LENGTH, __LINE__, __FILE__);
      
      char *buf = new char[length + 1];
      std::memset(buf, 0, length + 1);
      std::fread(buf, 1, length, in);
      post = *(CGI::Parser(buf).parse());
      delete [] buf;
    }

    Cookie(getParam("HTTP_COOKIE", ENV));
    
    //! \todo Use session cookie name from configuration file
    try {
      Session(getParam("sess_id", COOKIE));
    }
    catch(Common::Exception e) {
      if(e != E_PARAM_NOT_FOUND)
	throw e;
    }
  }

  Dict_ptr_t Request::getData(unsigned option) {
    Dict_t::iterator i;
    Dict_ptr_t ret (new Dict_t);
    if(option & GET)
      for(i = get.begin(); i != get.end(); i++)
	ret->insert(*i);
    if(option & POST)
      for(i = post.begin(); i != post.end(); i++)
	ret->insert(*i);
    if(option & COOKIE) {
      Dict_ptr_t ck = Cookie::getData();
      for(i = ck->begin(); i != ck->end(); i++)
	ret->insert(*i);
    }
    if(option & SESSION) {
      Dict_ptr_t ss = Session::getData();
      for(i = ss->begin(); i != ss->end(); i++)
	ret->insert(*i);
    }
    if(option & ENV)
      for(i = env.begin(); i != env.end(); i++)
	ret->insert(*i);
    return ret;
  }

  // option below is an optional parameter. See request.hpp

  std::string Request::getParam(std::string name, unsigned option) {

    // Order preference - GPCSE.
    
    Dict_t::iterator i;
    
    if((option & GET) and ((i = get.find(name)) != get.end()))
      return i->second;
    if((option & POST) and ((i = post.find(name)) != post.end()))
      return i->second;
    if(option & COOKIE)
      return Cookie::getParam(name);
    if(option & SESSION)
      return Session::getParam(name);
    if((option & ENV) and ((i = env.find(name)) != env.end()))
      return i->second;
    
    throw Common::Exception("Request parameter " + name + " not found in GET, POST data and environment variables", E_PARAM_NOT_FOUND, __LINE__, __FILE__);
  }
}
