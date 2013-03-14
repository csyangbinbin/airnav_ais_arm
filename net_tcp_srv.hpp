/*
net_tcp_srv.hpp
*/

#include <algorithm>
#include <cstdlib>
#include <deque>
#include <set>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/thread/shared_mutex.hpp> //for shared_mutex
#include <boost/thread/locks.hpp>
#include <boost/asio.hpp>
#include <string>
#include "config.hpp"

using boost::asio::ip::tcp;


//define read write lock 
typedef boost::shared_mutex rwmutex; 
typedef boost::unique_lock<boost::shared_mutex> wlock ; //write lock 
typedef boost::shared_lock<boost::shared_mutex> rlock ; //read lock ; 
#define Write_Lock(p) wlock(p)
#define Read_Lock(p)  rlock(p)




class ais_net_server ; 
typedef boost::shared_ptr<ais_net_server> ais_net_server_ptr ; 


class tcp_connect_session:
    public boost::enable_shared_from_this< tcp_connect_session >

{
public:
  tcp_connect_session(
  	boost::asio::io_service& io_service , 
  	ais_net_server_ptr server )
    : socket_(io_service) ,  server_(server)
  {
  }

public:
   tcp::socket& socket() ;  //get this session's socket 
   void start() ;  
   void send_msg(const std::string& msg) ; 

 private:
   void handle_write(const boost::system::error_code& error) ; 

private:
  tcp::socket socket_;
  ais_net_server_ptr server_  ;
};

typedef boost::shared_ptr<tcp_connect_session> tcp_connect_session_ptr ; 



////////////////////////////////////////////////////////////////////////

class ais_net_server
  :public boost::enable_shared_from_this< ais_net_server >
{
public:
  ais_net_server(boost::asio::io_service& io_service,
      const tcp::endpoint& endpoint)
    : io_service_(io_service), acceptor_(io_service, endpoint)
  {
  }

public:
  void start_accept() ; 
  void discard_connect(tcp_connect_session_ptr connect_ptr ) ; 
  void send_msg_2_all_client(std::string msg) ; 

private:
    void handle_accept(tcp_connect_session_ptr session,
                      const boost::system::error_code& error) ; 

private:
  boost::asio::io_service& io_service_;
  tcp::acceptor acceptor_;
  std::set<tcp_connect_session_ptr> connect_sessions;
  rwmutex rwmutex_  ; // mutx for  connect_sessions 
};





