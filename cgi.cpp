#include "cgi.hpp"
#include <string>
#include <algorithm>
#include <memory>
#include <cmath>
#include <cctype>

/*
 * This namespace CGI as said in cgi.hpp contains all CGI-related functions and classes
 * Complying to the respective RFCs
 */

namespace CGI {

    /*
     * Implementation of the Parser class
     */

    /*
     * Constructor has been defined in the header itself
     * because it should be inlined, it conatins just one function call to setQstr()
     */

    void Parser::_sanitize(std::string& s, size_t n = 1) {
        size_t i;
        for (i = n; i < s.size(); i++) {
            if (s.at(i) == '&' || s.at(i) == '=' || s.at(i) == ';') {
                if (i == 0 or i >= s.size()) break;
                if (s.at(i-1) == '&' || s.at(i-1) == '=' || s.at(i-1) == ';') {
                    s.erase(i,1);
                    _sanitize(s,i);
                }
            }
        }
        
        while (s.at(0) == '=' || s.at(0) == '&' || s.at(0) == ';')
            s.erase(0,1);
      
        while (s.at(s.size()-1) == '=' || s.at(s.size()-1) == '&' || s.at(s.size()-1) == ';')
            s.erase(s.size()-1,1);
    }

    Dict_ptr_t Parser::parse() {    
        std::string copy = getQstr(), extract = "";
        _sanitize(copy);

	size_t amp_pos = std::string::npos, eq_pos = std::string::npos, percent_pos = std::string::npos;
	Dict_ptr_t ret (new Dict_t);
	std::string key = "", value = "";

    do {
      if((amp_pos = copy.find('&')) != std::string::npos) {
	extract = copy.substr(0, amp_pos);
	copy.erase(0, amp_pos + 1);
      }
      else {
	extract = copy;
	copy.clear();
      }
      if((eq_pos = extract.find('=')) != std::string::npos) {
	key  = extract.substr(0, eq_pos);
	value = extract.substr(eq_pos + 1);
	extract.clear();
      }
      else {
	key = extract;
	value = "";
	extract.clear();
      }
      if((percent_pos = key.find('%')) != std::string::npos)
	key.replace(percent_pos, percent_pos + 3, 1, decodeHex(key.substr(percent_pos, percent_pos + 3)));

      /*
       * For some strange reason value.find() returns pos + 1 for position for %
       * Hence a hack has been added here. If troublesome, remove -1 and change 4 to 3.
       */
      
      if((percent_pos = value.find('%') != std::string::npos))
	value.replace(percent_pos-1, percent_pos + 4, 1, decodeHex(value.substr(percent_pos-1, percent_pos + 4)));
      
      ret->insert(Tuple_t(key, value));
    }
    while(copy.size());
    return ret;
  }	
    
    const char* Parser::getQstr() const {
        if(!source.size())
            throw Common::Exception("Query string propery requested while it was never set! in URL::Parser::getQstr()", QS_NOT_SET, __LINE__, __FILE__);        
        return source.c_str();
    }

  const Parser& Parser::setQstr(std::string s) {
    source = s;
    return *this;    
  }

  /*
   * Function decodeHex()
   * Input: HEX String (with the % symbol, for example %AA)
   * Output: Integer
   */

  int decodeHex(std::string source) {
    source.erase(0, 1); // Removing % from %XX
    
    unsigned int i = 0;
    unsigned int j = source.size() - 1;
    int result = 0;

    /*
     * Convert the string to upper case
     */
    
    std::transform(source.begin(), source.end(), source.begin(), (int (*)(int)) std::toupper);
    // (int (*) (int)) added so that the proper toupper() in cctype is chosen instead of the one in locale.

    for (i = 0; i < source.size(); i++, j--)
        if (source.at(j) >= '0' and source.at(j) <= '9')
            result += (source.at(j) - '0') * std::pow(16, i);
        else if (source.at(j) >= 'A' and source.at(j) <= 'F')
	  result += ((source.at(j) - 'A') + 10) * std::pow(16, i); // eg: ('B' - 'A') + 10 = (66 - 65) + 10 = 11
	else
	  throw Common::Exception("Invalid HEX symbol found in Common::decodeHex", INVALID_HEX_SYMBOL);       
    return result;
  }
}
