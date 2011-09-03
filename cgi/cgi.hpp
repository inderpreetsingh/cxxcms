#ifndef CGI_HPP
#define CGI_HPP
#include <common/exception.hpp>
#include <map>
#include <memory>
#include <string>
#include <algorithm>
#include <cmath>
#include <cctype>


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

  enum err_t { QS_NOT_SET, INVALID_TYPE, INVALID_HEX_SYMBOL };

  /*
   * Hex decoder
   */

  int decodeHex(std::string);
}
