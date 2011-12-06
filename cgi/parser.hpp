#ifndef CGI_PARSER_HPP
#define CGI_PARSER_HPP
#include <cgi/cgi.hpp>

/*
 * This namespace CGI will contain all CGI-related functions and classes.
 * All according to RFCs.
 */

namespace CGI {


  /*
   * Query String Parser, according to RFC 1738
   */

  class Parser {
  private:

    /*
     * The raw query string as obtained from getenv('QUERY_STRING')
     */
    
    std::string source;

    /*
     * Query string sanitizer
     * There are some string combinations, that can produce errors, major ones!
     * Some of them are described here- https://bitbucket.org/nileshgr/cppcms/wiki/Plan
     */

    void _sanitize(std::string&, size_t);
    
  public:

    /*
     * Constructor accepting std::string
     */

    Parser(std::string s) {
      source = s;
    }

    Parser() {
    }

    /*
     * Property setter accepting std::string
     */

    const Parser& setQstr(std::string s) {
      source = s;
      return *this;
    }

    /*
     * Property retriever in type const char*
     */

    const char* getQstr() const {
      if(!source.size())
	throw Common::Exception("Query string propery requested while it was never set! in URL::Parser::getQstr()", E_QS_NOT_SET, __LINE__, __FILE__);
      return source.c_str();
    }

    /*
     * The parser, returns reference to auto_ptr'd Dict.
     */

    Dict_ptr_t parse();
    
  };
}
#endif
