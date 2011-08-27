#include "common.hpp"
#include <cstring>
#include <cstddef>
#include <new>

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
  
  Exception::operator const char*() {
    return getMessage();
  }

  Exception::operator int() {
    return getCode();
  }

  /* Functions to get object properties */

  const char* Exception::getMessage() {
    return message;
  }

  int Exception::getCode() {
    return code;
  }

  /* Functions to set object properties */

  Exception* Exception::setMessage(const char* str) {
    size_t len = std::strlen(str);

    /*
     * Memory must be allocated if the error message present has smaller
     * length than the given one
     */
    
    if(length < len) {
      char *tmp = new char[len];
      if(message && length > 0)
	this->~Exception();
      message = tmp;
    }
    std::strcpy(message, str);
    length = len;
    return this;
  }

  Exception* Exception::setCode(int c) {
    code = c;
    return this;
  }

  /*
   * Deallocate memory allocate for message
   */

  Exception::~Exception() {
    if(message && length)
      delete [] message;
  }

  /* End Exception Definition */

}
