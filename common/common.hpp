#ifndef COMMON_HPP
#define COMMON_HPP
#include <string>
#include <global.hpp>
#include <map>
#include <memory>
#include <algorithm>
#include <cctype>

/*! \file common.hpp
  \brief %Common utilities

  File contains declaration for various classes and functions commonly used. Template functions
  and small class methods are implemented inline
*/

/*! \namespace Common
  \brief %Common functions/classes

  This module contains functions, classes, etc. which are common to the whole program (frequently used).
*/

namespace Common {

  //! Error codes for the common namespace

  enum {
    E_CONFIG_LOAD, //!< Error while loading configuration file
    E_CONFIG_PARAM_NOT_FOUND, //!< Configuration parameter not found \sa Config::operator[]
    E_REGISTRY_ITEM_NOT_FOUND, //!< Registry item not found \sa Registry::getItem
  };


  /*! \brief Global %Exception class

    An instance of this class will be thrown whenever an exception/error occurs in the program code. This does not apply to
    exceptions thrown by STL.
    -Converting/casting this class to const char* will return the message
    -Converting/casting this class to int will return the code
    
   %Exception class won't be used very frequently, in the sense, it will be thrown only in case of error,
   and errors obviously must occur rarely. Hence, functions of this class aren't inlined to keep the binary size small
   in case the compiler inlines the code after requesition by the inline statement (or definition in the class itself).

   \todo Global exception handler
   \coder{Nilesh G,nileshgr}
   */

  class Exception {
  private:
    std::string message; //!< Message of the exception
    int code; //!< Error code of the exception
    unsigned int line; //!< Line number in the file where the exception was thrown
    std::string file; //!< File name in which exception was thrown
    
  public:

    /*! \brief Constructor
      \param[in] _message std::string - The message
      \param[in] _code int - Error code
      \param[in] _line unsigned int - Line number in file where exception was thrown
      \param[in] _file const char* - File name where exception was thrown
    */

    Exception(std::string _message, int _code = 0, unsigned int _line = 0, const char* _file = NULL);

    /*! \brief Object to const char* casting operator

      When object of Exception is casted to const char*, it returns the message value from #getCMessage

      \return const char* #getCMessage
    */
    
    operator const char*() const;

    /*! \brief Object to int casting operator

      When object of Exception is casted to int, it returns #code value from #getCode

      \return int #getCode
    */
    
    operator int() const;

    /*! \return The exception message excluding line number & file name (#message)
      \sa getCMessage
    */
    
    const char* getMessage() const;

    //! \return Error/Exception code from #code
    
    int getCode() const;

    //! \return Line number where exception occured from #line
    
    unsigned int getLineNo() const;

    //! \return File name in which exception occurred from #file
    
    const char* getFileName() const;

    //! \return Complete exception message in the format: [Error X] on line \#L of file F
    
    const char* getCMessage() const;
  };

  /*! \brief Configuration reader class

    The applications configuration will be stored in a XML file which will be parsed using pugixml into a #Dict_t.
    #operator[] gives access (read only) to each parameter.
  */

  class Config {
  private:

    //! Dicitonary to store parsed XML data
    Dict_t data;
    
  public:

    /*! \brief Constructor - the main function which does the parsing.

      The whole parsing/processing occurs in the constructor (by calling pugixml's functions).

      \remark The following structure is assumed in the XML file:
      \verbatim
      <config>
        <moduleName>
          <moduleParameter1>value1</moduleParameter1>
          <moduleParameter2>value2</moduleParameter2>
	  .
	  .
	  .
        </moduleName>
      </config>
      \endverbatim
      The configuration in #data will be stored as data[moduleName_moduleParameter1] = value1

      \param[in] filename Path to the XML file containing configuration
     */
    
    Config(std::string filename);

    /*! \brief Element access to parsed \<keys,values\> stored in #data

      We don't need write access to configuration, hence the function returns a const std::string and not a reference or pointer

      \param[in] key Element name (xml) or key name in the configuration
      \return Value of the respective key in #data
      \throw Common::Exception with #E_CONFIG_PARAM_NOT_FOUND if key is not found in #data
    */
    
    const std::string operator[](std::string key) {
      if(data.find(key) == data.end())
	throw Common::Exception("Configuration parameter " + key + " not found", E_CONFIG_PARAM_NOT_FOUND, __LINE__, __FILE__);
      return data[key];
    }

    //! \sa #operator[]
    
    const std::string getParam(std::string key) {
      return data[key];
    }
  };

  /*! \brief Registry class

    A class to store pointers to various types of objects etc with key name.
    The class supports two interfaces- singleton and the usual, multiple instances. \n
    In singleton method, once an instance is created, it will be persisted till the method to destroy the instance is called.

    \coder{Nilesh G,nileshgr}
  */

  class Registry {
  private:
    typedef std::map<std::string, void*> items_t; //!< Define items_t dictionary type
    typedef std::pair<std::string, void*> item_t; //!< Define item_t, one single item type
    items_t items; //!< Dictionary to store pointers to objects
    static std::unique_ptr<Registry> instance; //!< Unique_ptr to store pointer to singleton instance

  public:
    //! Static method to obtain instance

    static Registry& getInstance();
    
    //! Static method to destroy instance

    static void destroyInstance();

    /*! \brief Method to add item
      \param name Name of item to be added
      \param ptr Pointer to item to be stored
      \return Registry& for cascading operations
    */

    Registry& addItem(std::string name, void* ptr) {
      std::transform(name.begin(), name.end(), name.begin(), (int (*)(int))std::tolower);
      items[name] = ptr;
      return *this;
    }

    /*! \brief Templated function to retrieve item
      \param name Name of item to be returned
      \tparam objtype Type of the object required as return type. dynamic_cast will be used.
      \return Reference to dynamic_cast of objtype
      \throw Common::Exception if name is not found in items
    */
      
    template<typename objtype>
    objtype& getItem(std::string name) {
      std::transform(name.begin(), name.end(), name.begin(), (int (*)(int)) std::tolower);
      items_t::iterator i;
      if((i = items.find(name)) == items.end())
	throw Common::Exception("Item: " + name + " not found in registry", E_REGISTRY_ITEM_NOT_FOUND, __LINE__, __FILE__);
      return *(reinterpret_cast <objtype*> (i->second));
    }

    /*! \brief Method to delete item
      \param name Name of the item to be deleted
      \return Registry& for cascading operations
    */
      
    Registry& deleteItem(std::string name) {
      items.erase(name);
      return *this;
    }
  };
}
#endif
