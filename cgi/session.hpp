#include <cgi/cgi.hpp>
#include <cgi/parser.hpp>

namespace CGI {

  class Session {
  private:
    std::string id;
    Dict_t data;
    time_t expire;

    /*
     * We cannot have more than one instance of this class.
     * This static variable will keep track of number of instances and
     * prohibit creation of new instances if it is already 1 by throwing exception (constructor will throw exception)
     */
    
    static unsigned int instances;
    
  public:

    /*
     * The first constructor will start a new session
     * The second one will load an exisiting session
     */
    
    Session();
    Session(const std::string&);

    /*
     * This function will return the session id of the class     
     */

    std::string getSessionId();

    /*
     * Returns the whole dictionary
     */
    
    Dict_ptr_t getAll();

    /*
     * Functions to add/get session data
     */
    
    void addData(const std::string, const std::string);
    std::string getData(const std::string);

    /*
     * This will load the session dictionary from the one passed as a parameter
     */
    
    void loadData(Dict_t&);

    /*
     * Sets the expire time. time_t is defined in time.h (POSIX)
     */
    
    void setExpireTime(time_t);

    /*
     * Returns the expire time
     */

    time_t getExpireTime();

    /*
     * Operator overloading for simple access to data
     */
    
    std::string& operator[] (const std::string);
  };

  Session::instances = 0; // initial value
  
}
