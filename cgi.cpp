#include "cgi.hpp"
#include <string>
#include <list>
#include <algorithm>
#include <memory>
#include <cstring>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>
#include <boost/lambda/construct.hpp>
#include <iostream>

int main() {
  std::string q("a=b&c=d&e=f&g&=h==");
  CGI::Dict_ptr_t p = CGI::Parser(q).parse();
  for(CGI::Dict_t::iterator i = p->begin(); i != p->end(); i++) {
    std::cout << "Value 1: " << i->first << "\t";
    if(i->second)
      std::cout << "Value 2: " << i->second;
    std::cout << "\n";
  }
  CGI::Dict_t d = *p.get();
  std::cout << "\n\n\n" << d.find("a")->second;
}

/*
 * This namespace CGI as said in cgi.hpp contains all CGI-related functions and classes
 * Complying to the respective RFCs
 */

namespace CGI {

  /*
   * Implementation of the Parser class
   */

  /*
   * Constructor has been defined in the header itself
   * because it should be inlined, it conatins just one function call to setQstr()
   */

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

  Dict_ptr_t Parser::parse() {    
    std::string copy = getQstr(), extract = "";
    _sanitize(copy);

    size_t amp_pos = 0, eq_pos = 0;
    Dict_ptr_t ret (new Dict_t);
    Tuple_t t;
    char *out1 = NULL;
    char *out2 = NULL;
    std::string tmp;

    do {
      if((amp_pos = copy.find("&")) != std::string::npos) {
	extract = copy.substr(0, amp_pos);
	copy.erase(0, amp_pos + 1);
      }
      else {
	extract = copy;
	copy.clear();
      }
      if((eq_pos = extract.find("=")) != std::string::npos) {
	tmp = extract.substr(0, eq_pos);
	out1 = new char[tmp.size()];
	std::strcpy(out1, tmp.c_str());
	tmp = extract.substr(eq_pos + 1);
	out2 = new char[tmp.size()];
	std::strcpy(out2, tmp.c_str());
	ret->insert(Tuple_t(out1, out2));
	extract.clear();
	/*Strings.push_back(out1);
	  Strings.push_back(out2);*/
      }
      else {
	out1 = new char[extract.size()];
	std::strcpy(out1, extract.c_str());
	ret->insert(Tuple_t(out1, NULL));
	extract.clear();
	//	Strings.push_back(out1);
      }
    } while(copy.size());
    return ret;
  }	
    
  const char* Parser::getQstr() const {
    if(!source.size())
      throw Common::Exception("Query string propery requested while it was never set! in URL::Parser::getQstr()", QS_NOT_SET, __LINE__, __FILE__);        
    return source.c_str();
  }

  Parser::~Parser() {
    using namespace boost::lambda;

    if(Strings.size())
      std::for_each(Strings.begin(), Strings.end(), bind(delete_array(), _1));
  }    
}
