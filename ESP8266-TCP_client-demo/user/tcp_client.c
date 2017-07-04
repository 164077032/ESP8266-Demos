/*
 * tcp_client.c
 *
 *  Created on: 2017��7��3��
 *      Author: Administrator
 */


#include "ets_sys.h"
#include "os_type.h"
#include "osapi.h"
#include "mem.h"
#include "user_interface.h"
#include "espconn.h"

#include "tcp_client.h"

#define TCP_BUF_LEN		512
static u8 tcp_rev_buff[TCP_BUF_LEN];
static u16 rev_data_len;

/**************************************************************************/
//TCP client
static struct espconn tcp_client_conn;
struct espconn *ptcp_conn = &tcp_client_conn;


LOCAL void ICACHE_FLASH_ATTR
tcp_client_sent_cb(void *arg)
{
	os_printf("tcp_client_sent_cb\n");
}

LOCAL void ICACHE_FLASH_ATTR
tcp_client_discon_cb(void *arg)
{
	os_printf("tcp_client_discon_cb\n");
}

LOCAL void ICACHE_FLASH_ATTR
tcp_client_recv(void *arg, char *pdata, unsigned short len)
{
	os_printf("tcp_client_recv\n");

	os_printf("lenth:%d\r\n",len);
	os_printf("data:%s\r\n",pdata);

	// TODO:

	os_memset(tcp_rev_buff, 0, TCP_BUF_LEN);
	os_memcpy(tcp_rev_buff, pdata, len);
	rev_data_len = len;

}

LOCAL void ICACHE_FLASH_ATTR
tcp_client_recon_cb(void *arg)
{
    struct espconn *pesp_conn = arg;

    os_printf("tcp_client_recon_cb\r\n");
}

LOCAL void ICACHE_FLASH_ATTR
tcp_client_connect_cb(void *arg)
{
    struct espconn *pesp_conn = arg;

    espconn_regist_disconcb(pesp_conn, tcp_client_discon_cb);	//ע��TCP�ͻ��˶Ͽ��ص�����
    espconn_regist_recvcb(pesp_conn, tcp_client_recv);			//ע��TCP�ͻ��˽��ջص�����
    espconn_regist_sentcb(pesp_conn, tcp_client_sent_cb); 		//ע��TCP�ͻ��˷��ͻص�����
    //espconn_regist_reconcb(pesp_conn, tcp_client_recon_cb);

    os_printf("tcp_client_connect_cb\r\n");
}

void ICACHE_FLASH_ATTR
tcp_client_init(u8* ip, u16 port)
{
	static esp_tcp esptcp;
    u32 u32_ip = ipaddr_addr(ip);

    os_printf("tcp_client_init\n");

#if 1
    if(wifi_station_get_connect_status() != STATION_GOT_IP){
    	os_printf("no got ip\r\n");
    	//return;
    }
#endif

    tcp_client_conn.type = ESPCONN_TCP;
    tcp_client_conn.state = ESPCONN_NONE;
    tcp_client_conn.proto.tcp = &esptcp;

    os_memcpy(tcp_client_conn.proto.tcp->remote_ip, &u32_ip, 4);	//���÷�����IP
    tcp_client_conn.proto.tcp->remote_port = port;				//���÷�����PORT

    tcp_client_conn.proto.tcp->local_port = espconn_port();

    espconn_regist_connectcb(&tcp_client_conn, tcp_client_connect_cb);

    u8 rc= espconn_connect(&tcp_client_conn);

    os_printf("connect to "IPSTR":%d rc=%d\r\n",
    		IP2STR(tcp_client_conn.proto.tcp->remote_ip),
			tcp_client_conn.proto.tcp->remote_port, rc);

}

