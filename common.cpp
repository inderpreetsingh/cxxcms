#include "common.hpp"

namespace Common {

  /* Definition of the Exception Class */

  Exception::Exception(std::string str, int code = 0) {
    this->setMessage(str);
    this->setCode(code);
  }

  Exception::Exception(char * str, int code = 0) {
    this->setMessage(str);
    this->setCode(code);
  }

  /* Type conversion functions.
   * See common.hpp for more info
   */
  
  Exception::operator std::string() {
    return this->getMessage();
  }

  Exception::operator const char*() {
    return this->getMessage().c_str();
  }

  Exception::operator int() {
    return this->getCode();
  }

  /* Functions to get object properties */

  std::string Exception::getMessage() {
    return this->message;
  }

  /*const char* Exception::getMessage() {
    return (this->message).c_str();
    }*/

  int Exception::getCode() {
    return this->code;
  }

  /* Functions to set object properties */

  void Exception::setMessage(std::string str) {
    this->message = str;
  }

  void Exception::setMessage(char* str) {
    this->message = str;
  }

  void Exception::setCode(int code) {
    this->code = code;
  }

  /* End Exception Definition */

}
  
