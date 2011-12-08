#include <common/common.hpp>
#include <sstream>

namespace Common {

  // Refer to the header common.hpp for documentation

  Exception::operator const char*() const {
    return getCMessage();
  }

  Exception::operator int() const {
    return getCode();
  }

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
    // Format: [Error X] on line #L of file F
    std::stringstream s;
    s << "[" << getMessage() << "]" << " on line #" << getLineNo() << " of file " << getFileName();
    return s.str().c_str();
  }

  Exception::Exception(std::string _message, int _code, unsigned int _line, const char *_file) {
    message = _message;
    code = _code;
    line = _line;
    file = _file;
  }
}
