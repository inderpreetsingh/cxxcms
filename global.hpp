#ifndef GLOBAL_HPP
#define GLOBAL_HPP
#include <string>
#include <list>
#include <map>
#include <memory>
#include <typeinfo>

/*! \file global.hpp
  \brief The global header file.

  This contains mostly typedefs, etc. and code which do not fit in any module or namespace.
*/

/*! \brief Defines dictionary type Dict_t

  Define the dictionary type using std::map. The key and value are of type std::string.
  const std::string cannot be used because we need modification access (using the element access operator).
*/

typedef std::map<std::string, std::string> Dict_t;

//! Defines a single element type Tuple_t of the dictionary type Dict_t.

typedef std::pair<std::string, std::string> Tuple_t;

/*! \brief Defines a smart pointer type Dict_ptr_t to dictionary type Dict_t.
  
  Usage of smart pointers simplifies the program, because we do not need to take care of garbage collection.
  std::auto_ptr is deprecated in C++11, hence std::unique_ptr is used.
*/

typedef std::unique_ptr <Dict_t> Dict_ptr_t;

/*! \brief Templated Function to check if a given type is some form of string.

  This function can be used to check if a given type s is some form of string.
  It has not been tested properly and will be removed if it is buggy or not required.

  \tparam s is of any type T
  \return bool(true) if s is std::string, std::string&, std::string*, const std::string, const std::string&, const std::string*, char*, const char*;
  otherwise bool(false).
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
#endif
