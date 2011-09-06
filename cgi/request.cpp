#include <cgi/request.hpp>
#include <common/exception.hpp>

namespace CGI {

  Request::Request(char **envp, FILE* in) {

    /*
     * Parse **env into dict_t env (private variable)
     */

    while(*envp) {
      std::string tmp = *envp;
      size_t pos = tmp.find('=');
      Tuple_t t (tmp.substr(0, pos), tmp.substr(pos + 1));
      env.insert(t);
      envp++;
    }

  }

  std::string Request::getEnv(std::string name) {
    if(!env.count(name))
      throw Common::Exception("Environment variable " + name + "not found", ENV_NOT_FOUND, __LINE__, __FILE__);
    return env.find(name)->second;
  }

}
      
