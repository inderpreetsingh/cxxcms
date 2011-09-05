#include <cgi/cgi.hpp>

namespace CGI {

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
