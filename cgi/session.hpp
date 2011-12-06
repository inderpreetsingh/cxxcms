#include <cgi/cgi.hpp>
#include <cgi/parser.hpp>

namespace CGI {

  class Session {
  private:    
    std::string id;
    Dict_t data;
    time_t expire;

    /*
     * We have private constructors and copy-constructor
     * because this class implements singleton pattern
     */

    Session();
    Session(const std::string&);
    Session(const Session&);
    
  public:

    /*
     * Method to return pointer to instance
     * This function will create a new object of this class if instance does not exist
     */

    static Session* getInstance(const std::string = "");

    /*
     * Method to destroy the instance specified as argument
     * This method MUST be called when the object's use is over, otherwise it might lead to memory leaks.
     */

    static void destroyInstance(Session*);

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

}
