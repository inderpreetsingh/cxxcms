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
	throw Common::Exception("Invalid HEX symbol found in Common::decodeHex", E_INVALID_HEX_SYMBOL);
    return result;
  }

  std::string& encodeHex(std::string& src) {
    /*
     * Characters to be encoded:
     * 0-31, 128-255
     * 36, 38, 43, 44, 47, 58, 59, 61, 63, 64, 127
     * 32, 34, 60, 62, 35. 37, 123, 125, 124, 92
     * 91, 93, 96, 126, 94
     * See http://www.blooberry.com/indexdot/html/topics/urlencoding.htm
     */
    size_t i;
    int array[] = {43, 44, 47, 63, 64, 96};
    std::set <int> toBeReplaced (array);
    for(i = 0; i < src.size(); i++) {
      /*
       * Encode the first set of characters, stated above
       */
      char current = src.at(i);
      if(current <= 32 or
	 (current >= 34 and current <= 38) or
	 (current >= 58 and current <= 61) or
	 (current >= 91 and current <= 94) or
	 current >= 123 or
	 toBeReplaced.count(current)) {
	  char *tmp = new char[4]; // %XX + NUL
	  std::sprintf(tmp, "%X", (int) current);
	  src.replace(i, 1, tmp);
	  delete [] tmp;
      }
    }
    return src;
  }
}
