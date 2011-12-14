/*! \file registry.cpp
  \brief Static method implementation of Registry

  Static data members cannot be defined in the header, because it causes multiple definition error
*/

#include <cgi/cgi.hpp>

namespace Common {
  std::unique_ptr<Registry> Registry::instance; // Definition of static data member

  Registry& Registry::getInstance() {
    if(!instance.get())
      instance.reset (new Registry);
    return *instance;
  }

  void Registry::destroyInstance() {
    instance.reset();
  }
}
