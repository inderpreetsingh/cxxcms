#include <cgi/cgi.hpp>
#include <uuid/uuid.h>

namespace CGI {

  Session::Session() {
    uuid_t uu;
    char _id[16];
    uuid_generate(uu);
    uuid_unparse(uu, _id);
    id = _id;

    // We need to set the default expire time as per the settings in configuration file
    // For now, it's one hour (hardcoded).
    
    setExpireTime(std::time(NULL) + (time_t) 3600);
   }

  Session::Session(const std::string _id) {

    // To be implemented, first storage system has to be setup [DB]

  }

  Session* Session::getInstance(std::string _id) {

    /*
     * This statement might be a bit confusing
     * but static variables are initialized only once.
     * Over consecutive calls, they retain their value.
     */

    static Session* ptr = NULL;

    if(not ptr)
      ptr = new Session;
    if(not ptr and _id.size())
      ptr = new Session(_id);

    return ptr;
  }

  Dict_ptr_t Session::getData() {
    Dict_ptr_t ret (new Dict_t);
    *ret = data;
    return ret;
  }
}
