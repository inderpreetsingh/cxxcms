#include <cgi/cgi.hpp>
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <fcgi_stdio.h>

namespace CGI {

  Request::Request(char **envp) : postBuffer(NULL), postBuffer_fncall(NULL), rawpostdata(false) {    

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

      // As per CGI specifications, HTTP POST data is available in stdin.

      size_t length = 0;
      std::sscanf(getParam("CONTENT_LENGTH", ENV).c_str(), "%zu", &length); // %zu - size_t

      if(not length)
	throw Common::Exception("Invalid CONTENT_LENGTH", E_INVALID_CONTENT_LENGTH, __LINE__, __FILE__);
      
      char *buf = new char[length];

      for(size_t c = 0; c < length; c++)
	buf[c] = getchar();
      try {
	if(getParam("CONTENT_TYPE", ENV).compare("application/x-www-form-urlencoded") != 0) { // Submitted data is binary
	  postBuffer = buf;
	  rawpostdata = true;
	}
      } catch(Common::Exception e) {
	if(e.getCode() != E_PARAM_NOT_FOUND)
	  throw e;
	else
	  post = *(CGI::Parser(buf).parse());
      }
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
    if(option & POST) {
      if(rawpostdata)
	throw Common::Exception("Error: POST data is binary, use getbinPost()", E_POST_BINARY, __LINE__, __FILE__);
      for(i = post.begin(); i != post.end(); i++)
	ret->insert(*i);
    }
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
    if(option & POST) {
      if(rawpostdata)
	throw Common::Exception("Error: POST data is binary", E_POST_BINARY, __LINE__, __FILE__);
      return i->second;
    }
    if(option & COOKIE)
      return Cookie::getParam(name);
    if(option & SESSION)
      return Session::getParam(name);
    if((option & ENV) and ((i = env.find(name)) != env.end()))
      return i->second;
    
    throw Common::Exception("Request parameter " + name + " not found in GET, POST data and environment variables", E_PARAM_NOT_FOUND, __LINE__, __FILE__);
    }

  Request::~Request() {
    if(postBuffer)
      delete[] postBuffer;
    if(postBuffer_fncall)
      delete[] postBuffer_fncall;
    postBuffer = postBuffer_fncall = NULL;
  }

  char* Request::getBinPost() {
    if(not rawpostdata)
      throw Common::Exception("Error: POST data is not binary", E_POST_NOT_BINARY, __LINE__, __FILE__);
    size_t len = 0;
    std::sscanf(getParam("CONTENT_LENGTH", ENV).c_str(), "%zu", &len);
    postBuffer_fncall = new char[len];
    std::memcpy(postBuffer_fncall, postBuffer, len);
    return postBuffer_fncall;
  }
}
