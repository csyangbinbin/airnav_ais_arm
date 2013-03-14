/*
serport.cc
*/
#include  "serport.hpp"
#include  "config.hpp"

ais_serial_port::ais_serial_port(boost::asio::io_service& io_service)
:sp(io_service) , fn_(0)
{

}

int ais_serial_port::open(const std::string& port_name ,   unsigned int rate) 
{

boost::system::error_code err ; 
sp.open(port_name , err) ; //open serial port 
if(err)
{
	#ifdef _DEBUG
	std::cout<<"open "<<port_name<<" error " <<err.message() <<std::endl ; 
	#endif 

	return -1 ; 	
}

//set serial port parameters 
try
 {
sp.set_option(serial_port_crt::baud_rate(rate)) ; 
sp.set_option(serial_port_crt::flow_control(serial_port_crt::flow_control::none)) ; 
sp.set_option(serial_port_crt::parity(serial_port_crt::parity::none)) ;
sp.set_option(serial_port_crt::stop_bits(serial_port_crt::stop_bits::one));
sp.set_option(serial_port_crt::character_size(8)) ;
  }
  catch (std::exception& e)
  {
    std::cerr << "set serial port parameters exception: " << e.what() << "\n";
    return -1 ; 
  }
  return  0 ;
}


    void ais_serial_port::default_read_data_callback_fn(const std::string& msg)
    {
//nothing to do 
    }


void ais_serial_port::start_read(read_data_callback fn)
{
//if fn is empty , then use default function 
	if(fn)
		fn_ = fn ;
	else
		fn_ =  boost::bind(&ais_serial_port::default_read_data_callback_fn  , this , _1) ; 

   if(sp.is_open())
   {
   	//start  read some data not must is max_buffer_size 
    sp.async_read_some(boost::asio::buffer(recv_buffer , max_buffer_size) ,
	boost::bind(&ais_serial_port::handle_read , this ,recv_buffer ,  _1,_2) ) ;

   }
}

void ais_serial_port::handle_read( unsigned char* data ,
                                   boost::system::error_code err , 
	                               std::size_t bytes_transferred)
{

if(err)
{
#ifdef _DEBUG
       printf("read ais serial port data error ! app will exit ......\r\n") ;
#endif
   exit(-1);  // read com data error , app exit !

return ; 
}
else
{
	#ifdef _DEBUG
	printf("\r\n[%d]\r\n" ,bytes_transferred );
	for(int i=0; i< bytes_transferred ; i++)  printf("%c",data[i]);
	#endif

	std::string ais_data((const char*)data ,bytes_transferred ) ;

if(fn_)
	fn_(ais_data) ;  //call back

    sp.async_read_some(boost::asio::buffer(recv_buffer , max_buffer_size) ,
	boost::bind(&ais_serial_port::handle_read , this ,recv_buffer ,  _1,_2) ) ;
}

}