#include <common/exception.hpp>

namespace Common {

  /* Definition of the Exception Class */

  /* Type conversion functions.
   * See exception.hpp for more info
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

  Exception::Exception(std::string m, int c, unsigned int l, const char * fname) {
    setMessage(m);
    setCode(c);
    setLineNo(l);
    setFileName(fname);
  }

  Exception& Exception::setMessage(std::string m) {
    message = m;
    return *this;
  }  
  /* End Exception Definition */
}
