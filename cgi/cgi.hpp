#ifndef CGI_HPP
#define CGI_HPP
#include <common/exception.hpp>
#include <map>
#include <memory>
#include <string>
#include <algorithm>
#include <cmath>
#include <cctype>
#include <cstdio>
#include <cstring>
#include <set>

namespace CGI {

  /*
   * Define the Dict and Tuple data types
   * Dict is a std::map<string, string> (two column)
   * Tuple is one single element of the map using std::pair<string, string>
   */

  typedef std::map<const std::string, const std::string> Dict_t;
  typedef std::pair<const std::string, const std::string> Tuple_t;
  typedef std::auto_ptr <Dict_t> Dict_ptr_t;
  
  /*
   * Error codes for our URL namespace
   */

  enum {
    E_QS_NOT_SET,
    E_INVALID_HEX_SYMBOL,
    E_ENV_NOT_FOUND,
    E_PARAM_NOT_FOUND,
    E_INVALID_FILE_PTR,
    E_INVALID_CONTENT_LENGTH };

  /*
   * Hex decoder
   */

  int decodeHex(std::string);

  /*
   * Hex encoder
   */

  std::string& encodeHex(std::string&);
}
#endif
