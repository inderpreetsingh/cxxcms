/*! \mainpage CXXCMS: A content management system written in C++

  \section what What is CXXCMS?
  The title surprises you? Yet another content management system? Yes. That is because, we have millions of CMSes out there,
  but all in Python, PHP, Java, etc. which are mainly scripting or interpreted languages and hence are slow. The slowness really
  takes a toll when the number of hits are high. You have ultimately to increase your server capacity and even after that, again after sometime
  you will face the problem.\n\n
  CXXCMS is a content management system written in C++. C++, is an efficient language; well, not good for writing operating systems as Linus Torvalds says,
  but definitely a good one for writing applications. We chose C++ because it is easier to glue up the things with the object oriented features C++ offers
  and with little overhead. If we chose C, which is used in performance critical applications and many others, we would nearly have to reinvent the wheel by
  writing object interface patterns and what not which is bound to have much more overhead than C++ itself and will be slow, defeating the whole purpose of a
  fast and light application.

 \section why Why CXXCMS?
  When web was built it revolved around web pages. As the time passed, webpages has evolved into web applications but the tools that we use for web developement
  are still page oriented. The boundary between a web application and desktop applications is getting blurred. Now web applications are as good as (if not better) 
  than desktop applications. So we should start thinking about using application oriented languages such as c++ for making web applications. CXXCMS makes it easier 
  to use C++ for making web applications. 

  \section how How is it done?
  Okay, so you came down reading here means you are really interested. Good. We use OpenMarket's FastCGI library to interface with the webserver. So essentially
  it reduces to running a FastCGI application on the server. Running a FastCGI application means, you either need to have your own server, virtual or real, because
  there are barely any shared hosting providers who let you run binaries as FastCGI application.

  \section license Is this project open source?
  Yes! We are open source software lovers and this is as open source as well. The project is licensed under GNU GPL v3.

  \section status What is the status of the project?
  As of now, the project is under development, just in the preliminary stages, the basic layout and framework.

  \section contribute How can I contribute?
  Contributors are always welcome.
  \subsection things_to_know Things you must know before contributing
  - C++
   - Your concepts should be strong enough, especially STL
  - Version Control
   - You should know either Git or Mercurial. The Mercurial repository is the default located at https://bitbucket.org/nileshgr/cxxcms \n
   and a Git repository, a mirror is located at https://github.com/nileshgr/cxxcms
   - Both the repository hosting services have comprehensive documentation to learn those, do read them if you wish to.
  - Doxygen
   - Doxygen is the documentation system we use. In fact, this page itself is generated using doxygen (see footer)
   - For more information on doxygen, see http://www.doxygen.org

   \section bugtrack Bug tracking and Todo lists
   We have a bug tracker at https://tracker.itech7.com and to do list at \ref todo\n
   If you want to post a bug, you should register at the tracker and post a bug. You can also email the bugs to cxxcms-issues@itech7.com.
   You will be asked to confirm your account.

   If you know the specified things, fork either of the repositories and start working. You can then make pull requests, we'll take it into account.\n\n
   Discussions happen at the mailing list- https://groups.google.com/group/cxxcms\n
   You should request an invite there.
*/
