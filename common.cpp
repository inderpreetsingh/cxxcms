#include "common.hpp"
#include <cstring>
#include <cstddef>
#include <new>
#include <sstream>

namespace Common {

  /* Definition of the Exception Class */

  /* Type conversion functions.
   * See common.hpp for more info
   */
  
  Exception::operator const char*() const {
    return getCMessage();
  }

  Exception::operator int() const {
    return getCode();
  }

  /* Functions to get object properties */

  const char* Exception::getMessage() const {
    return message.c_str();
  }

  int Exception::getCode() const {
    return code;
  }

  unsigned int Exception::getLineNo() const {
    return line;
  }

  const char* Exception::getFileName() const {
    return file.c_str();
  }

  const char* Exception::getCMessage() const {
    /*
     * [Error X] on line #L of file F
     * Using StringStream
     */
    std::stringstream s;
    s << "[" << getMessage() << "]" << " on line #" << getLineNo() << " of file " << getFileName();
    return s.str().c_str();
  }

  Exception& Exception::setFileName(const char* str) {
    if(!str) {
      file = "";
      return *this;
    }
    file  = str;
    return *this;
  }

  Exception& Exception::setCode(int c) {
    code = c;
    return *this;
  }

  Exception& Exception::setLineNo(unsigned int l) {
    line = l;
    return *this;
  }

  /* End Exception Definition */
}
