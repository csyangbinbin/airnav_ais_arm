/*
serport.hpp
*/
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <string>

//when ais serial port recv data will call this function 
typedef  boost::function<void (const std::string&) > read_data_callback ; 


//serial port read control
class ais_serial_port
{
public:
	ais_serial_port(boost::asio::io_service& io_service) ; 

int open(const std::string& port_name , 
		  unsigned int rate) ;

void start_read(read_data_callback fn) ; 

private:
	void handle_read( unsigned char* data ,
                                   boost::system::error_code err ,  
	                              std::size_t bytes_transferred) ; 

    void default_read_data_callback_fn(const std::string& msg);

private:
	typedef boost::asio::serial_port   serial_port_crt ;
private:
	enum { max_buffer_size = 256 } ;  // recv buffer size 
	unsigned char recv_buffer[max_buffer_size] ;  
	serial_port_crt sp ;  //serial port read control 
	read_data_callback fn_ ;  // call back function 
} ; 