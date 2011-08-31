#include "common.hpp"
#include <cstring>
#include <cstddef>
#include <new>
#include <sstream>
#include <iostream>

int main() {
  try {
    throw Common::Exception("MOO", 11, __LINE__, __FILE__);
  } catch(Common::Exception e) {
    std::cout << e.getCMessage() << "\n";
  }
}
    

namespace Common {

  /* Definition of the Exception Class */

  Exception::Exception(char *str, int code = 0, unsigned int line_no = 0, char *fname = NULL) {
    message = NULL;
    length_msg = length_file = line = 0;
    setMessage(str);
    setCode(code);
    setLineNo(line_no);
    setFileName(fname);
  }

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
    return message;
  }

  int Exception::getCode() const {
    return code;
  }

  unsigned int Exception::getLineNo() const {
    return line;
  }

  const char* Exception::getFileName() const {
    return file;
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

  /* Functions to set object properties */

  Exception& Exception::setMessage(const char* str) {
    size_t len = std::strlen(str);

    /*
     * Memory must be allocated if the error message present has smaller
     * length than the given one
     */
    
    if(length_msg < len) {
      char *tmp = new char[len];
      if(message and length_msg > 0)
	this->~Exception();
      message = tmp;
    }
    std::strcpy(message, str);
    length_msg = len;
    return *this;
  }

  Exception& Exception::setFileName(const char* str) {
    size_t len = std::strlen(str);

    /*
     * Memory must be allocated if the error message present has smaller
     * length than the given one
     */
    
    if(length_file < len) {
      char *tmp = new char[len];
      if(file and length_file > 0)
	this->~Exception();
      file = tmp;
    }
    std::strcpy(file, str);
    length_file = len;
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

  /*
   * Deallocate memory allocate for message
   */

  Exception::~Exception() {
    if(message and length_msg)
      delete [] message;
    if(file and length_file)
      delete [] file;
  }

  /* End Exception Definition */
}
