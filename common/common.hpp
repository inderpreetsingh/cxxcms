#ifndef COMMON_HPP
#define COMMON_HPP
#include <string>
#include <global.hpp>

namespace Common {

  /*
   * Error codes for the common namespace
   */

  enum {
    E_CONFIG_LOAD };


  /*
   * Exception Class
   * This class will be thrown whenever an exception occurs
   * Converting this class to char* or std::string will return the message
   * Converting this class to int will return the code
   */

  /*
   * Exception class won't be used very frequently, in the sense, it will be thrown only in case of error.
   * Errors obviously are bound to occur rarely.
   * Hence, functions of this class won't be inlined to keep the binary size simple in case the compiler inlines the code after requesition.
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

  /* Configuration Manager class */

  class Config {
  private:
    Dict_t data;
  public:
    /*
     * We don't need multiple functions to parse the configuration, because it is job of pugixml
     * The things are done in the constructor
     * And we have the property getters
     */
    Config(std::string);
    const std::string operator[](std::string key) {
      return data[key];
    }
    const std::string getParam(std::string key) {
      return data[key];
    }
  };

  /* End Configuration Manager class */
    
}
#endif
