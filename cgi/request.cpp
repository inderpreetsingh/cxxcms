#include <cgi/request.hpp>
#include <cgi/parser.hpp>

namespace CGI {

  Request::Request(char **envp, FILE* in) {

    env.reset(new Dict_t);
    cookie.reset(new Dict_t);
    
    /*
     * Parse **env into dict_t env (private variable)
     */

    while(*envp) {
      std::string tmp = *envp;
      size_t pos = tmp.find('=');
      Tuple_t t (tmp.substr(0, pos), tmp.substr(pos + 1));
      env->insert(t);
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
      buf = NULL;
    }

    std::string cookies = getEnv("HTTP_COOKIE");

    /*    if(cookies.size()) {
      size_t pos;
      do {
      if((pos = cookies.find(';')) != std::string::npos) {*/
	  
      
  }

  std::string Request::getEnv(std::string name) {
    if(!env->count(name))
      throw Common::Exception("Environment variable " + name + "not found", E_ENV_NOT_FOUND, __LINE__, __FILE__);
    return env->find(name)->second;
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
    /*    if(opiton & COOKIE)
      for(i = cookie.begin(); i != cookie.end(); i++)
      ret->insert(*i);*/
    return ret;
  }

  // option below is an optional parameter. See request.hpp

  std::string Request::getParam(std::string name, unsigned short option) {
    std::string result = "";

    // What order preference should we use? GPCSE? Or something else?

    //    if(option & GET)
  }
}
