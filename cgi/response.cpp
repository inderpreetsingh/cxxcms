/*! \file response.cpp
  \brief CGI::Response class implementation
*/

#include <cgi/cgi.hpp>
#include <ctime>
#include <clocale>
#include <cstring>

namespace CGI {
  std::unique_ptr<char[]> Response::getBinaryBody() {
    if(!binary)
      throw Common::Exception("Response is not binary", E_RESPONSE_NOT_BINARY, __LINE__, __FILE__);
    setupHeaders();
    std::unique_ptr<char[]> ret (new char[headerString.size() + binaryLength]);
    std::strcpy(ret.get(), headerString.c_str());
    std::strcat(ret.get(), binaryData.get());
    return ret;
  }

  std::string& Response::getCompleteBody() {
    if(binary)
      throw Common::Exception("Response is binary", E_RESPONSE_BINARY, __LINE__, __FILE__);      
    setupHeaders();
    completeBody = headerString + contentBody;
    return completeBody;
  }

  std::string Response::getParam(std::string name, option_t option) {    
    Dict_t::iterator i;
    if(option == HEADER) {
      i = headers.find(name);
      if(i == headers.end())
	throw Common::Exception("Parameter: " + name + " not found in headers", E_PARAM_NOT_FOUND, __LINE__, __FILE__);
      return i->second;
    }
    else if(option == SESSION)
      return Session::getParam(name);
  }

  Response& Response::clearBody()  {
    contentBody.clear();
    completeBody.clear();
    if(binary)
      binaryData.reset();
  }

  void Response::setupHeaders() {
    cookie_t c;
    c.value = getSessionId();

    /*
      Cookie should have secure parameter if protocol is HTTPS
      Check for environment variable HTTP_HTTPS or HTTPS with value on, ON or 1
      and set secure property
    */

    std::string https;
    Common::Registry &reg = Common::Registry::getInstance();
    CGI::Request &req = reg.getItem<CGI::Request>("request");

    try {
      https = req.getParam("HTTPS", Request::ENV);
    }
    catch(Common::Exception e) {
      if(e != E_PARAM_NOT_FOUND)
	throw e;      
      try {
	https = req.getParam("HTTP_HTTPS", Request::ENV);
      }
      catch(Common::Exception e) {
	if(e != E_PARAM_NOT_FOUND)
	  throw e;
      }
    }

    if(https == "on" or https == "1" or https == "ON")
      c.secure = true;
    else
      c.secure = false;

    c.path = "/"; // Session cookies don't make sense in subdiretories

    try {
      c.domain = req.getParam("HTTP_HOST", Request::ENV);
    }
    catch(Common::Exception e) {
      if(e != E_PARAM_NOT_FOUND)
	throw e;
      c.domain = req.getParam("SERVER_NAME", Request::ENV);
    }

    // Cookie domain cannot be set for complete domain name, they must be valid across all subdomains

    c.domain = "." + c.domain; // suppose example1.example.com is the servername/httphost, then cookie will be valid for .example1.example.com, which is the default anyways if it is not set.
    
    cookie_dict_t cookies = getCookies();
    cookie_dict_t::iterator cookieIterator;
    Dict_t::iterator dictIterator;
    for(dictIterator = headers.begin(); dictIterator != headers.end(); dictIterator++)
      headerString += dictIterator->first + ": " + dictIterator->second + "\r\n";
    for(cookieIterator = cookies.begin(); cookieIterator != cookies.end(); cookieIterator++) {
      cookie_t &ck = cookieIterator->second;
      char expire[29]; // Wdy, dd-mm-yyyy hh:mm:ss GMT + NUL
      std::setlocale(LC_TIME, "en_US");
      std::strftime(expire, 29, "%a, %d-%m-%Y %H:%M:%S GMT", std::gmtime(&ck.expire));
      headerString += "Set-Cookie: " + cookieIterator->first + "=" + ck.value + "; Expires=" + expire + "; ";
      headerString += "Domain=" + ck.domain + "; Path=" + ck.path + "; \r\n";
    }
    headerString += "\r\n\r\n";
  }
  Response::Response() {
    setParam("Content-Type", "text/html", HEADER);
    setParam("Content-Encoding", "utf-8", HEADER);
  }
}
