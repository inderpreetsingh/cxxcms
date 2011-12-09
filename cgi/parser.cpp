#include <cgi/cgi.hpp>

namespace CGI {

  /*
   * Implementation of the Parser class
   */

  /*
   * Constructor has been defined in the header itself
   * because it should be inlined, it conatins just one function call to setQstr()
   */

  void Parser::_sanitize(std::string& s, size_t n) {
    size_t i;
    for (i = n; i < s.size(); i++) {
      if (s.at(i) == '&' or s.at(i) == '=' or s.at(i) == ';') {
	if (i == 0) break;
	if (s.at(i-1) == '&' or s.at(i-1) == '=' or s.at(i-1) == ';') {
	  s.erase(i,1);
	  _sanitize(s,i);
	}
      }
    }
        
    while (s.at(0) == '=' or s.at(0) == '&' or s.at(0) == ';')
      s.erase(0,1);
      
    while (s.at(s.size()-1) == '=' or s.at(s.size()-1) == '&' or s.at(s.size()-1) == ';')
      s.erase(s.size()-1,1);
  }

  Dict_ptr_t Parser::parse() {    
    std::string copy = getQstr(), extract = "";
    _sanitize(copy);

    size_t delimiter = std::string::npos;
    Dict_ptr_t ret (new Dict_t);
    std::string key = "", value = "";

    while(copy.size()) {
      if((delimiter = copy.find('&')) != std::string::npos or
	 (delimiter = copy.find(';')) != std::string::npos) {
	extract = copy.substr(0, delimiter);
	copy.erase(0, delimiter + 1);
      }
      else {
	extract = copy;
	copy.clear();
      }
      if((delimiter = extract.find('=')) != std::string::npos) {
	key  = extract.substr(0, delimiter);
	value = extract.substr(delimiter + 1);
	extract.clear();
      }
      else {
	key = extract;
	value = "";
	extract.clear();
      }
      if((delimiter = key.find('%')) != std::string::npos)
	key.replace(delimiter, delimiter + 3, 1, decodeHex(key.substr(delimiter, delimiter + 3)));

      /*
       * For some strange reason value.find() returns pos + 1 for position for %
       * Hence a hack has been added here. If troublesome, remove -1 and change 4 to 3.
       */
      
      if((delimiter = value.find('%') != std::string::npos))
	value.replace(delimiter-1, delimiter + 4, 1, decodeHex(value.substr(delimiter-1, delimiter + 4)));
      
      ret->insert(Tuple_t(key, value));
    }
    return ret;
  }
  
}
