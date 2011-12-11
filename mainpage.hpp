/*! \file mainpage.hpp
  \brief File just for general-purpose documentation (all pages)
*/

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

   If you know the specified things, just drop an email to contact@nileshgr.com\n
   You will get access to your desired repository (git or hg) and also become a member of the mailing list where discussions happen,
   located at https://groups.google.com/group/cxxcms
*/
