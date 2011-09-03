#ifndef COMMON_H
#define COMMON_H
#include <list>
#include <string>
#include <typeinfo>
#include <cstddef>
#include <sstream>

namespace Common {

  /*
   * Function returns true if the given type is some form of string
   * C-style strings, const strings, etc, etc
   */

  template <typename T>
  bool is_string(T s) {    

    std::string tn[] = {
      typeid(std::string).name(),
      typeid(std::string&).name(),
      typeid(std::string*).name(),
      typeid(const std::string).name(),
      typeid(const std::string&).name(),
      typeid(const std::string*).name(),
      typeid(char*).name(),
      typeid(const char*).name()
    };

    std::list <std::string> tnames (tn, tn + sizeof(tn) / sizeof(std::string));
    std::string source_name = typeid(s).name();

    for(std::list <std::string>::iterator i = tnames.begin(); i != tnames.end(); i++)
      if(i->compare(source_name) == 0)
	return true;
    return false;    
  }
}
#endif
