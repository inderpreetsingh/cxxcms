#ifndef COMMON_H
#define COMMON_H 1
#include <string>

namespace Common {

  /*
   * Exception Class
   * This class will be thrown whenever an exception occurs
   * Converting this class to std::string or char* will return the message
   * Converting this class to int will return the code
   */

  class Exception {

  private:

    std::string message;
    int code;

  public:

    /* The constructor
     * Accepts input for message as std::string or char *
     */

    Exception(std::string, int);
    Exception(char *, int);

    /* Define type conversion functions
     * If this object is converted into a string or char*, then it will return the message
     * If it is converted into a int, it will return the code
     */

    operator std::string();
    operator const char*();
    operator int();

    /* Functions to get object properties
     * These can be directly used, or type conversion functions will call these
     */
    
    std::string getMessage();
    //const char* getMessage();
    int getCode();

    /* Functions to set object properties
     * These are called by the constructor and can be called directly
     */
    
    void setMessage(std::string);
    void setMessage(char *);
    void setCode(int);        
  };

  /* End of Exception Class */
    
}
#endif
