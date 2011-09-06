#ifndef COMMON_EXCEPTION_H
#define COMMON_EXCEPTION_H
#include <common/common.hpp>

namespace Common {

  /*
   * Exception Class
   * This class will be thrown whenever an exception occurs
   * Converting this class to char* or std::string will return the message
   * Converting this class to int will return the code
   */

  class Exception {

  private:

    std::string message;
    int code;
    unsigned int line;
    std::string file;

  public:

    /*
     * The constructor
     */

    Exception(std::string, int = 0, unsigned int = 0,  const char* = NULL);

    /* Define type conversion functions
     * If this object is converted into a char*, then it will return the message
     * If it is converted into a int, it will return the code
     */

    operator const char*() const;
    operator int() const;

    /* Functions to get object properties
     * These can be directly used, or type conversion functions will call these
     */
    
    const char* getMessage() const; 
    int getCode() const;
    unsigned int getLineNo() const;
    const char* getFileName() const;
    const char* getCMessage() const; // Returns the complete message of the form [Error X] on line #L of file F, called by object-char converter

    /* Functions to set object properties
     * These are called by the constructor and can be called directly
     */

    Exception& setMessage(std::string);
    Exception& setCode(int);
    Exception& setLineNo(unsigned int);
    Exception& setFileName(const char*);
  };

  /* End of Exception Class */
}
#endif
