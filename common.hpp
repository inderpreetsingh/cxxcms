#ifndef COMMON_H
#define COMMON_H 1
#include <cstddef>
#include <string>
#include <list>
#include <typeinfo>
#include <string>
#include <cassert>

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
     * The constructor (templated)
     */

    template <typename T>
    Exception(T, int = 0, unsigned int = 0,  const char* = NULL);

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

    template <typename T>
    Exception& setMessage(T);
    Exception& setCode(int);
    Exception& setLineNo(unsigned int);
    Exception& setFileName(const char*);
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

  template <typename T>
  Exception::Exception(T m, int c, unsigned int l, const char * fname) {
    setMessage(m);
    setCode(c);
    setLineNo(l);
    setFileName(fname);
  }

  template <typename T>
  Exception& Exception::setMessage(T m) {
    assert(Common::is_string(m) == true);
    message = m;
    return *this;
  }  
}
#endif
