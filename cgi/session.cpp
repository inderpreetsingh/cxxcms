#include <cgi/session.hpp>
#include <uuid/uuid.h>

namespace CGI {

  Session::Session() {
    uuid_t uu;
    char sid[16];
    uuid_generate(uu);
    uuid_unparse(uu, sid);
    id = sid;

    // We need to set the default expire time as per the settings in configuration file
    // For now, it's one hour (hardcoded).
    
    setExpireTime(std::time(NULL) + (time_t) 3600);
   }

  Session::Session(const std::string &sid) {

    // To be implemented, first storage system has to be setup [DB]

  }

  Session* Session::getInstance(std::string sid) {

    /*
     * This statement might be a bit confusing
     * but static variables are initialized only once.
     * Over consecutive calls, they retain their value.
     */

    static Session* ptr = NULL;

    if(!ptr)
      ptr = new Session;
    if(!ptr and sid.size())
      ptr = new Session(sid);

    return ptr;
  }

  Dict_ptr_t Session::getAll() {
    Dict_ptr_t ret (new Dict_t);
    *ret = data;
    return ret;
  }
}
