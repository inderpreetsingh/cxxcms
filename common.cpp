#include "common.hpp"
#include <cstring>
#include <cstddef>
#include <new>
#include <list>
#include <string>
#include <typeinfo>

namespace Common {

  /* Definition of the Exception Class */

  Exception::Exception(char * str, int code = 0) {
    message = NULL;
    length = 0;
    setMessage(str);
    setCode(code);
  }

  /* Type conversion functions.
   * See common.hpp for more info
   */
  
  Exception::operator const char*() const {
    return getMessage();
  }

  Exception::operator int() const {
    return getCode();
  }

  /* Functions to get object properties */

  const char* Exception::getMessage() const {
    return message;
  }

  int Exception::getCode() const {
    return code;
  }

  /* Functions to set object properties */

  Exception& Exception::setMessage(const char* str) {
    size_t len = std::strlen(str);

    /*
     * Memory must be allocated if the error message present has smaller
     * length than the given one
     */
    
    if(length < len) {
      char *tmp = new char[len];
      if(message and length > 0)
	this->~Exception();
      message = tmp;
    }
    std::strcpy(message, str);
    length = len;
    return *this;
  }

  Exception& Exception::setCode(int c) {
    code = c;
    return *this;
  }

  /*
   * Deallocate memory allocate for message
   */

  Exception::~Exception() {
    if(message and length)
      delete [] message;
  }

  /* End Exception Definition */

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
