/*
AirNav AIS
*/
#include "net_tcp_srv.hpp"
#include "serport.hpp"
#include "xml_cfg.hpp"
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/stat.h>


//app config file path
#define APP_CONFIG_FILE_NAME "/etc/airnav_ais.cfg"


//demon
void init_daemon(void)
{
  int pid;
  int i;
  if(pid=fork())
    exit(0);
  else if(pid<0)
    exit(1);
  //是第一子进程
    setsid();//第一子进程成为新的会话组长和进程组长, 并与控制终端分离
    if(pid=fork())
      exit(0);//结束第一子进程
      else if(pid< 0)
  exit(1);
      //第二子进程不再是会话组长
  for(i=0;i< NOFILE;++i)//关闭打开的文件描述符
close(i);
chdir("/");//改变工作目录到/
umask(0);//重设文件创建掩模
return;
}


//main
int main(int argc, char* argv[])
{
  init_daemon() ; 
  try
  {

ConfigManager xml_config ;          //XML config file reader 
boost::asio::io_service io_service;  // io service

ais_serial_port sp(io_service) ;      //serial port 


//read app config 
bool load_cfg_success = xml_config.LoadConfigFile(APP_CONFIG_FILE_NAME) ;
if(!load_cfg_success)
{
#ifdef _DEBUG
  printf("load app airnav_ais config file error!\r\n");
#endif

return -1 ; 
}

// net tcp listen port 
std::string net_tcp_listen_port_str = get_node_text(xml_config , "net_config" , "listen_port") ;
if(net_tcp_listen_port_str.empty())
{
  #ifdef _DEBUG
  printf("read app config net_config->listen_port  error!\r\n") ;
  #endif
  return -1 ; 
}
int net_tcp_listen_port = atoi(net_tcp_listen_port_str.c_str()) ;

 #ifdef _DEBUG
 printf("net tcp listen port = %d\r\n" , net_tcp_listen_port) ;
 #endif



//AIS COM config
std::string ais_com_str = get_node_text(xml_config , "ais_com_config" , "com") ;
if(ais_com_str.empty())
{
#ifdef _DEBUG
printf("read app config ais_com_comfig->com error!\r\n");
#endif
return -1 ; 
}

std::string ais_com_rate_str = get_node_text(xml_config , "ais_com_config" , "rate") ;
if(ais_com_rate_str.empty())
{
#ifdef _DEBUG
  printf("read app config ais_com_config->rate error!\r\n");
#endif
return -1 ; 
}

int ais_com_rate = atoi(ais_com_rate_str.c_str()) ;

#ifdef _DEBUG
printf("AIS COM :[%s:%d]" , ais_com_str.c_str() , ais_com_rate);
#endif 



int result = sp.open(ais_com_str , ais_com_rate) ;
if(result)
{
printf("open serial port error1\r\n");
return -1 ; 

}


//read ais serial port and send by net 

      tcp::endpoint endpoint(tcp::v4(), net_tcp_listen_port);

      ais_net_server_ptr server(new ais_net_server(io_service, endpoint));
      server->start_accept() ;  // tcp port start listen ... 
    
      //serial port start to read  ais data  ... 
      sp.start_read(boost::bind(&ais_net_server::send_msg_2_all_client , server , _1 )); 

 
     io_service.run();  // loop start 

  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }
}
