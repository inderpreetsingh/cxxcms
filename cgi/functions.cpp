#include <cgi/cgi.hpp>
#include <string.h>
#include <stdio.h>

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


    
    void reverseString(char *string)
    {
        int i, j = 0;
        char rev[20];
        for (i = strlen(string) - 1; i >= 0; i--) {
            rev[j++] = string[i];
        } rev[j] = '\0';

        strcpy(string, rev);        
    }


    char* convertToHex(int number)
    {
        char hex[20];
        int rem, i = 0;

        while (number > 0) {
            rem = number % 16;
            if (rem >= 10)
                hex[i++] = ('A'-10) + rem;
            else
                hex[i++] = rem + '0';
            number /= 16;        
        } hex[i] = '\0';
        reverseString(hex);
        return hex;
    }


    std::string encodeHex(std::string s)
    {
        std::string hexEncoded;
        for (int i = 0; i < s.size(); i++)
            hexEncoded.append((std::string)convertToHex(s.at(i)));
        return hexEncoded;
    }

}
