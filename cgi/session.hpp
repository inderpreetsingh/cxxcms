#include <cgi/cgi.hpp>
#include <cgi/parser.hpp>

namespace CGI {

  class Session {
  private:
    std::string session_id;
    Dict_ptr_t session_data;
  public:
    Session();
    std::string getSessionId();
    bool loadSession(std::string);
    Dict_ptr_t getAll();    
    void addKey(Tuple_t&);
    void loadData(Dict_t&);
    void setExpireTime(size_t);
  };
}
