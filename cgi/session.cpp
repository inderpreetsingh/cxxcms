#include <cgi/cgi.hpp>
#include <uuid/uuid.h>

/*! \file session.cpp
  \brief Implementation of CGI::Session
*/

namespace CGI {
  /* We have static members in the class, we need to initialize them, so that the methods in the class can reference them using FQN
     This works only for static members (even for private members) */
  std::string Session::id;
  time_t Session::expire;
  Dict_t Session::data;
  bool Session::response; // See constructor definition in the above class

  Session::Session() {
    uuid_t uu;
    char _id[16];
    uuid_generate(uu);
    uuid_unparse(uu, _id);
    id = _id;

    //! \todo We need to set the default expire time as per the settings in configuration file. For now, it's one hour (hardcoded).
    
    setExpireTime(std::time(NULL) + (time_t) 3600);
   }

  Session::Session(std::string _id) {

    // To be implemented, first storage system has to be setup [DB]

  }

  Dict_ptr_t Session::getData() const {
    Dict_ptr_t ret (new Dict_t);
    *ret = data;
    return ret;
  }
}
