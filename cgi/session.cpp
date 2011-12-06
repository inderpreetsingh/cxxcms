#include <cgi/session.hpp>
#include <uuid/uuid.h>

namespace CGI {

  Session::Session() {
    if(Session::instances == 1)
      throw Common::Exception("More than one instance of the session class is not permitted", E_MORE_THAN_ONE_INSTANCE, __LINE__, __FILE__);
    Session::instances = 1;
    uuid_t uu;
    char sid[16];
    uuid_generate(uu);
    uuid_unparse(uu, sid);
    id(sid);

    // We need to set the default expire time as per the settings in configuration file
    // For now, it's one hour (hardcoded).
    
    setExpireTime(time() + 3600);
  }

  Session::Session(const std::string &sid) {

    // To be implemented, first storage system has to be setup [DB]

  }

  std::string Session::getSessionId() {
    return id;
  }

  Dict_ptr_t Session::getAll() {
    Dict_ptr_t ret (new Dict_t);
    *ret = data;
    return ret;
  }

  void Session::addData(const std::string key, const std::string value) {
    id[key] = value;
  }

  std::string Session::getData(const std::string key) {
    return id[key];
  }

  void Session::loadData(Dict_t& sdata) {
    data = sdata;
  }

  void Session::setExpireTime(time_t exp) {
    expire = exp;
  }

  time_t Session::getExpireTime() {
    return expire;
  }

  std::string& Session::operator[] (const std::string key) {
    return data[key];
  }

}
