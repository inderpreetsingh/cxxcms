#ifndef GLOBAL_HPP
#define GLOBAL_HPP
#include <string>
#include <list>
#include <map>
#include <memory>
#include <typeinfo>

/*
 * The global header file
 * This will contain mostly typedefs and global functions, which don't fit in any kind of module or namespace
 */


/*
 * Define the Dict and Tuple data types
 * Dict is a std::map<string, string> (two column)
 * Tuple is one single element of the map using std::pair<string, string>
 */

typedef std::map<std::string, std::string> Dict_t;
typedef std::pair<std::string, std::string> Tuple_t;
typedef std::unique_ptr <Dict_t> Dict_ptr_t;

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

#endif
