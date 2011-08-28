#ifndef COMMON_H
#define COMMON_H 1
#include <cstddef>

namespace Common {

  /*
   * Exception Class
   * This class will be thrown whenever an exception occurs
   * Converting this class to char* will return the message
   * Converting this class to int will return the code
   */

  class Exception {

  private:

    char* message;
    int code;
    size_t length;

  public:

    /* The constructor
     * Accepts input for message as char *
     */

    Exception(char *, int);

    /* The destructor
     * Required to deallocate message memory
     */

    ~Exception();

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

    /* Functions to set object properties
     * These are called by the constructor and can be called directly
     */
    
    Exception& setMessage(const char *);
    Exception& setCode(int);        
  };

  /* End of Exception Class */
    
}
#endif
