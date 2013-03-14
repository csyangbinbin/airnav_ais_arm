#include "net_tcp_srv.hpp"

  tcp::socket& tcp_connect_session::socket()
  {
    return socket_;
  }


void tcp_connect_session::start()
{
  //when this session start ,to do something 
  //TODO:
}

void tcp_connect_session::send_msg(const std::string& msg)
{
  if(msg.empty()) return  ; 

if(socket_.is_open())
  {
     boost::asio::async_write(socket_,
          boost::asio::buffer(msg.c_str() , msg.size() ),
          boost::bind(&tcp_connect_session::handle_write, 
          	shared_from_this(),
            boost::asio::placeholders::error));
   }
}

void tcp_connect_session::handle_write(const boost::system::error_code& error)
{
  if (!error)
    {
  
    }
    else
    {


#ifdef _DEBUG
    printf("tcp_connect_session::handle_write error!\r\n") ; 
    std::cout<<error.message()<<std::endl;
#endif

    server_->discard_connect(shared_from_this());
    }

}


///////////////////////////////////////////////////////////////////////////

 void ais_net_server::start_accept()
  {
    tcp_connect_session_ptr current_connect_session( 
    	new tcp_connect_session(io_service_ ,	
    	shared_from_this() ) ) ;

{
Write_Lock(rwmutex_); //lock to write 
      connect_sessions.insert(current_connect_session) ;
}


    acceptor_.async_accept(current_connect_session->socket(),
        boost::bind(&ais_net_server::handle_accept, this, current_connect_session,
          boost::asio::placeholders::error));
  }

  void ais_net_server::handle_accept(tcp_connect_session_ptr session,
      const boost::system::error_code& error)
  {
    if (!error)
    {

      #ifdef _DEBUG
    	printf("new connect come ...  \r\n") ; 
      #endif

        session->start();
    }

    start_accept();
  }

  void ais_net_server::discard_connect(tcp_connect_session_ptr connect_ptr )
  {
connect_sessions.erase(connect_ptr) ;

  }


void ais_net_server::send_msg_2_all_client(std::string msg)
 {
    Read_Lock(rwmutex_) ;  //lock to read 
        std::for_each(connect_sessions.begin(), connect_sessions.end(),
        boost::bind(&tcp_connect_session::send_msg, _1, boost::ref(msg))) ; 
}