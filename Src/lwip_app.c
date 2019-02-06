#include "lwip.h"
#include "t_shell.h"
#include "string.h"
#include "socket.h"
#include "t_str.h"
/*
dhcp start
dhcp stop
dhcp sta
*/
int32_t DhcpCommand(p_shell_context_t context, int32_t argc, char **argv)
{
	char buf[32];
	if(argc==2)
	{
		if( 0==strcmp(argv[1],"start"))
		{
				dhcp_start(&gnetif);			
				context->printf_data_func("DHCP BEGIN\r\n");
		}
		else if( 0==strcmp(argv[1],"stop"))
		{
				dhcp_stop(&gnetif);			
				context->printf_data_func("DHCP STOP\r\n");
		}
		else if( 0==strcmp(argv[1],"sta"))
		{
			get_dhcp_state(&gnetif,buf);
			context->printf_data_func(buf);
		}
		else
		{
			context->printf_data_func("UNKNOW!\r\n");
			context->printf_data_func("USED like this : dhcp []; []=start,stop,sta\r\n");
		}
	}
	
	return 0;
}

const shell_command_context_t xDhcpCommand = {"dhcp", "\r\n\"dhcp\":[start,stop,sta]\r\n",DhcpCommand, 1};


struct netif *netif = &gnetif;
int32_t IfconfigCommand(p_shell_context_t context, int32_t argc, char **argv)
{
	context->printf_data_func("\r\n IPv4 Address     : %u.%u.%u.%u\r\n", ((u8_t *)&netif->ip_addr.addr)[0],
		((u8_t *)&netif->ip_addr.addr)[1], ((u8_t *)&netif->ip_addr.addr)[2],
		((u8_t *)&netif->ip_addr.addr)[3]);
	context->printf_data_func(" IPv4 Subnet mask : %u.%u.%u.%u\r\n", ((u8_t *)&netif->netmask.addr)[0],
		((u8_t *)&netif->netmask.addr)[1], ((u8_t *)&netif->netmask.addr)[2],
		((u8_t *)&netif->netmask.addr)[3]);
	context->printf_data_func(" IPv4 Gateway     : %u.%u.%u.%u\r\n\r\n", ((u8_t *)&netif->gw.addr)[0],
		((u8_t *)&netif->gw.addr)[1], ((u8_t *)&netif->gw.addr)[2], 
		((u8_t *)&netif->gw.addr)[3]);
	return 0;
}
const shell_command_context_t xIfconfigCommand = {"ifconfig", "\r\n\"ifconfig\": show IPv4 Address,Subnet mask and Gateway\r\n",IfconfigCommand, 0};



/*
netif sta
netif down
netif up
netif restart
*/
int32_t NetifCommand(p_shell_context_t context, int32_t argc, char **argv)
{
	
	//STA
	if( 0==strcmp(argv[1],"sta"))
	{
		if(netif_is_link_up(&gnetif))
		{
			context->printf_data_func("netif_is_link_up OK\r\n");
		}
		else
		{
			context->printf_data_func("netif_is_link_up DOWN\r\n");
		}
	}
	//DOWN
	else if( 0==strcmp(argv[1],"down"))
	{	
		netif_set_down(&gnetif);	
	}
	//UP
	else if( 0==strcmp(argv[1],"up"))
	{	
		netif_set_up(&gnetif);	
	}
	//RESTART
	else if( 0==strcmp(argv[1],"restart"))
	{
		netif_set_down(&gnetif);
		netif_set_up(&gnetif);
		
	}
	else
	{
		context->printf_data_func("err\r\n");
	}
	return 0;
}
const shell_command_context_t xNetifCommand = {"netif", "\r\n\"netif\": [sta,down,up,restart]\r\n",NetifCommand, 1};

/*
ip set 192.168.0.102,255.255.255.0,192.168.0.1
*/
int32_t IpCommand(p_shell_context_t context, int32_t argc, char **argv)
{
	_stringList *strlist=NULL;
	if( 0==strcmp(argv[1],"set"))
	{
		strlist=mtStrDiv(argv[2],",");
		if(strlist==NULL)
		{
			context->printf_data_func("err:strlist=NULL\r\n");
		}
		else if(strlist->num==3)
		{
			netif_set_down(&gnetif);
			
			ipaddr.addr = inet_addr(strlist->d[0]);
			netmask.addr = inet_addr(strlist->d[1]);
			gw.addr = inet_addr(strlist->d[2]);
			
			netif_set_addr(&gnetif, &ipaddr, &netmask, &gw);
			
			netif_set_up(&gnetif);
		}
		else
		{
			context->printf_data_func("config err\r\n");
		}
	}
	return 0;
}
const shell_command_context_t xIpCommand = {"ip", "\r\n\"ip\": [set]\r\n",IpCommand, 2};

/*
socket creat 			tcp 
socket connect 		0,192.168.0.106,3200
socket send 			0,asdfget
socket close 			0
*/
#define SOCKET_MAX_NUM		5
static uint8_t socket_num=0;
static int sock[SOCKET_MAX_NUM];
struct sockaddr_in addr[SOCKET_MAX_NUM];
int32_t SocketCommand(p_shell_context_t context, int32_t argc, char **argv)
{
	int port=-1;
	int type=-1;
	int t_sock=-1;
	int re=-1;
	_stringList *strlist=NULL;
	struct sockaddr_in *t_addr=NULL;
	//CREAT
	if(0==strcmp(argv[1],"creat"))
	{
		if(socket_num>=5)
		{
			context->printf_data_func("err:socket_num<=5\r\n");
		}
		else if(0==strcmp("tcp",argv[2]))
		{
			type = SOCK_STREAM;
		}
		else if(0==strcmp("udp",argv[2]))
		{
			type = SOCK_DGRAM;
		}
		else if(0==strcmp("raw",argv[2]))
		{
			type = SOCK_RAW;
		}
		else
		{
			context->printf_data_func("parameter(s) err\r\n");
		}
		if(type)
		{
			sock[socket_num] = socket(AF_INET, type, 0);	
			if(sock[socket_num]>=0)context->printf_data_func("creat ok,socket_id=%d\r\n",socket_num++);
			else context->printf_data_func("creat err\r\n");
		}
	}
	//CONNECT
	else if(0==strcmp(argv[1],"connect"))
	{
		strlist=mtStrDiv(argv[2],",");
		if(strlist==NULL)
		{
			context->printf_data_func("err:strlist=NULL\r\n");
		}
		else if(strlist->num!=3)
		{
			context->printf_data_func("err:strlist->num!=3\r\n");
		}
		else 
		{
			sscanf(strlist->d[0],"%d",&t_sock);
			if(t_sock>=0 && t_sock<socket_num)
			{
				sscanf(strlist->d[2],"%u",&port);
				if(port>0&&port<65536)
				{
					t_addr = &(addr[t_sock]);
					memset(&addr[t_sock], 0, sizeof(addr));
					t_addr->sin_family      = AF_INET;
					t_addr->sin_port        = htons(port);
					t_addr->sin_addr.s_addr = inet_addr(strlist->d[1]);
					//	bind(sock, (struct sockaddr *)&addr, sizeof(addr));						
					re = connect(sock[t_sock],(struct sockaddr *)t_addr,sizeof(t_addr[0]));
					if(re==0)
					{
						context->printf_data_func("connect %s:%s ok\r\n",strlist->d[1],strlist->d[2]);
					}
					else
					{
						context->printf_data_func("connect %s:%s err\r\n",strlist->d[1],strlist->d[2]);
					}
				}
				else
				{
					context->printf_data_func("port err\r\n");
				}
				
			}
			else
			{
				context->printf_data_func("sock num err\r\n");
			}
		}
		FreeStringList(strlist);
		
	}
	//SNED
	else if(0==strcmp(argv[1],"send"))
	{
		strlist=mtStrDiv(argv[2],",");
		if(strlist==NULL)
		{
			context->printf_data_func("err:strlist=NULL\r\n");
		}
		else if(strlist->num!=2)
		{
			context->printf_data_func("err:strlist->num!=2\r\n");
		}
		else
		{
			sscanf(strlist->d[0],"%d",&t_sock);
			if(t_sock>=0 && t_sock<socket_num)
			{
				re = send(sock[t_sock],strlist->d[1],strlen(strlist->d[1]),0);
				if(re>=0)
				{
					context->printf_data_func("sock send %d,%s ok\r\n",sock[t_sock],strlist->d[1]);
				}
				else
				{
					context->printf_data_func("sock send %d,%s err\r\n",sock[t_sock],strlist->d[1]);
				}
			}
			else
			{
				context->printf_data_func("sock num err\r\n");
			}
		}
		FreeStringList(strlist);
	}
	//CLOSE
	else if(0==strcmp(argv[1],"close"))
	{	
		sscanf(argv[2],"%d",&t_sock);
		if(t_sock>=0 && t_sock<socket_num)
		{
			re = closesocket(sock[t_sock]);
			if(re>=0)context->printf_data_func("close ok\r\n");
			else context->printf_data_func("close err\r\n");
		}
		else
		{
			context->printf_data_func("sock num err\r\n");
		}
	}
	else
	{
		context->printf_data_func("err\r\n");
	}
	return 0;
}
const shell_command_context_t xSocketCommand = {"socket", "\r\n\"socket\": [cerat,connect,send,close]\r\n",SocketCommand, 2};

