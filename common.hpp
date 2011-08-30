#ifndef COMMON_H
#define COMMON_H 1
#include <cstddef>
#include <string>
#include <list>
#include <typeinfo>

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

  /*
   * Templated functions cannot be declared somewhere and
   * defined somewhere else.
   * Hence, defining them here.
   */

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
#endif
