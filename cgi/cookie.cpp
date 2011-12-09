#include <cgi/cgi.hpp>

namespace CGI {

  Dict_ptr_t Cookie::getCookies() {
    Dict_ptr_t ret (new Dict_t);
    *ret = cookies;
    return ret;
  }

  Cookie::Cookie(std::string _cookies) : response (false) {
    while(_cookies.size()) {
      size_t delimiter = std::string::npos;
      std::string key = "", value = "", extract = "";
      if((delimiter = _cookies.find(';')) != std::string::npos) {
	extract = _cookies.substr(0, delimiter);
	_cookies.erase(0, delimiter + 2); // +2 because we have a space between multiple cookies. NAME1=VALUE1; NAME2=VALUE2; NAME3=VALUE3
      }
      else {
	extract = _cookies;
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
      cookies[key] = value;
    }
  }
}
