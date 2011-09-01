#include "url.hpp"
#include <string>
#include <list>
#include <algorithm>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>
#include <boost/lambda/construct.hpp>
#include <iostream>
#include <memory>
#include <cstring>

int main() {
  std::string q("a=b&c=d");
  URL::Parser p(q);
  URL::Dict_t& d = p.parse();
  std::cout << "PLONK" << "\n";
  for(URL::Dict_t::iterator i = d.begin(); i != d.end(); i++)
    std::cout << "Value1: " <<  i->first << "\t" << "Value2: " << i->second << "\n";    
}

/*
 * This namespace URL as said in url.hpp contains all URL-related functions and classes
 * Complying to the respective RFCs
 */

namespace URL {

  /*
   * Implementation of the Parser class
   */

  /*
   * Constructor has been defined in the header itself
   * because it should be inlined, it conatins just one function call to setQstr()
   */

  /*
   * Destructor, deallocates dicts and strings allocated, if any
   */

  Parser::~Parser() {

    /*
     * Clear memory for Dictonaries allocated
     */

    using namespace boost::lambda;

    if(Dicts.size())
      std::for_each(Dicts.begin(), Dicts.end(), bind(delete_ptr(), _1));

    /*
     * Clear memory for char*'s allocated
     */

    if(Strings.size())
      std::for_each(Strings.begin(), Strings.end(), bind(delete_array(), _1));    
  }

  void Parser::_sanitize(std::string& s, size_t n = 1) {
    size_t i;
    for (i = n; i < s.size(); i++) {
      if (s.at(i) == '&' || s.at(i) == '=') {
	if (i == 0 or i >= s.size()) break;
	if (s.at(i-1) == '&' || s.at(i-1) == '=') {
	  s.erase(i,1);
	  _sanitize(s,i);
	}
      }
    }
        
    while (s.at(0) == '=' || s.at(0) == '&')
      s.erase(0,1);
        
    while (s.at(s.size()-1) == '=' || s.at(s.size()-1) == '&')
      s.erase(s.size()-1,1);
  }

  std::string& Parser::_splitbyamp(std::string& src) {
    size_t amp_pos;
    std::auto_ptr <std::string> ret (new std::string);    
    if((amp_pos = src.find('&')) == std::string::npos)
      throw Common::Exception("& not found", QS_AMP_NOT_FOUND);
    else {
      *ret = std::string(src.substr(0, src.size() - amp_pos - 1));
      return *ret;
    }
  }

  std::string& Parser::_splitbyeq(std::string& src, unsigned int part) {
    size_t eq_pos;
    std::auto_ptr <std::string> ret (new std::string);
    if((eq_pos = src.find('=')) == std::string::npos)
      throw Common::Exception("= not found", QS_EQ_NOT_FOUND);
    else {
      if(part == 1)
	*ret = std::string(src.substr(0, src.size() - eq_pos - 1));
      else if(part == 2)
	*ret = std::string(src.substr(src.size() - eq_pos));
      else
	throw Common::Exception("Invalid Part number", QS_SPLIT_EQ_INVALID_PART, __LINE__, __FILE__);
      return *ret;
    }
  }

  Dict_t& Parser::parse() {

    std::cout << "Entered";
    
    std::string t = source;
    _sanitize(t);

    bool flag = true;
    std::string str_with_eq, part1, part2;
    char *tmpstr_to_map = NULL;
    const char *tmpstr = NULL;
    Dict_t *ret = new Dict_t;

    Dicts.push_back(ret);

    std::cout << "Added";

    do {      
      try {
	try {
	  str_with_eq = _splitbyamp(t);
	}
	catch(Common::Exception e) {
	  if(e.getCode() == QS_AMP_NOT_FOUND) {
	    str_with_eq = t;
	    flag = false;
	  }
	  else
	    throw;
	}
	part1 = _splitbyeq(str_with_eq, 1);
	part2 = _splitbyeq(str_with_eq, 2);
      }
      catch(Common::Exception e) {
	if(e.getCode() == QS_EQ_NOT_FOUND) {
	  part1 = t;
	  part2 = "";
	}
	else
	  throw;
      }
    	
      /*
       * For Part 1, before =
       */
      if(part1.size()) {
	tmpstr_to_map = new char[part1.size()];
	tmpstr = part1.c_str();
	std::strcpy(tmpstr_to_map, tmpstr);
	Strings.push_back(tmpstr_to_map);
      }

      /*
       * For Part 2, after =
       */

      if(part2.size()) {
	tmpstr_to_map = new char[part2.size()];
	tmpstr = part2.c_str();
	std::strcpy(tmpstr_to_map, tmpstr);
	std::strcpy(tmpstr_to_map, tmpstr);
	Strings.push_back(tmpstr_to_map);
      }
      ret->insert(Tuple_t(tmpstr_to_map, tmpstr_to_map));
    } while(flag);
    return *ret;
  }	
    
  const char* Parser::getQstr() const {
    if(!source.size())
      throw Common::Exception("Query string propery requested while it was never set! in URL::Parser::getQstr()", QS_NOT_SET, __LINE__, __FILE__);        
    return source.c_str();
  }
}
