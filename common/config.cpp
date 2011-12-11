#include <common/common.hpp>
#include <contrib/pugixml/pugixml.hpp>
#include <string>

/*! \file config.cpp
  \brief Implementation of Common::Config
*/

namespace Common {
  Config::Config(std::string filename) {
    pugi::xml_document doc;
    pugi::xml_parse_result res = doc.load(filename.c_str());

    if(not res)
      throw Common::Exception(std::string("Error parsing configuration file: ").append(res.description()), E_CONFIG_LOAD, __LINE__, __FILE__);

    pugi::xml_node root = doc.child("config"); // Getting the root node

    for(pugi::xml_node moduleName = root.first_child(); moduleName; moduleName = root.next_sibling()) {
      std::string key = moduleName.name();
      key += "_";
      for(pugi::xml_node moduleParameter = moduleName.first_child(); moduleParameter; moduleParameter = moduleName.next_sibling()) {
	key += moduleParameter.name();
	std::string value = moduleParameter.value();
	data[key] = value;
      }
    }
  }
}
