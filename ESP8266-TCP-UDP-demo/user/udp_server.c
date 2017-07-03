/*
 * udp_server.c
 *
 *  Created on: 2017��7��3��
 *      Author: Administrator
 */


#include "ets_sys.h"
#include "ip_addr.h"
#include "espconn.h"
#include "osapi.h"
#include "user_interface.h"
#include "mem.h"

#include "driver/uart.h"

#include "udp_server.h"

#define UDP_BUFF_SIZE		128
static u8 udp_buff[UDP_BUFF_SIZE];

static struct espconn udp_server;
static esp_udp espudp;


/*
 * ����: udp_get_peer_conn
 * ˵������ȡ�Զ˵�UDP��Ϣ
 */
static u8 ICACHE_FLASH_ATTR
udp_get_peer_conn(void *arg)
{
	struct espconn *pesp_conn = arg;
	remot_info *premot = NULL;

	if (espconn_get_connection_info(pesp_conn, &premot, 0) == ESPCONN_OK){
		pesp_conn->proto.udp->remote_port = premot->remote_port;
		pesp_conn->proto.udp->remote_ip[0] = premot->remote_ip[0];
		pesp_conn->proto.udp->remote_ip[1] = premot->remote_ip[1];
		pesp_conn->proto.udp->remote_ip[2] = premot->remote_ip[2];
		pesp_conn->proto.udp->remote_ip[3] = premot->remote_ip[3];
		//espconn_sent(pesp_conn, pusrdata, os_strlen(pusrdata));
		return 1;
	}
	return 0;
}

/*
 * ������udp_print_conn
 * ˵������ӡUDP�������ӵ���Ϣ
 */
static void ICACHE_FLASH_ATTR
udp_print_conn(void *arg)
{
	struct espconn *pesp_conn = arg;

    os_printf("UDP recv from ip:%d.%d.%d.%d:%d\n",
    		pesp_conn->proto.udp->remote_ip[0],pesp_conn->proto.udp->remote_ip[1],
			pesp_conn->proto.udp->remote_ip[2],pesp_conn->proto.udp->remote_ip[3],
			pesp_conn->proto.udp->remote_port);
}

static void ICACHE_FLASH_ATTR
udp_server_sent_cb(void *arg)
{
	os_printf("udp_server_sent_cb\r\n");

	// TODO:
}

/*
 * ������udp_server_recv
 * ˵����UDP Server���ջص�
 */
static void ICACHE_FLASH_ATTR
udp_server_recv(void *arg, char *pdata, unsigned short len)
{
	struct espconn *pesp_conn = arg;
	u8 *start, *end;
	u8 length = 0;
	static u8 temp[64];

	udp_get_peer_conn(arg);
	udp_print_conn(arg);

	os_printf("lenth:%d\r\ndata:%s\r\n", len, pdata);

  // TODO:

	espconn_send(pesp_conn, udp_buff, os_strlen(udp_buff));
}


/*
 * ������udp_server_init
 * ˵����UDP Server ��ʼ��
 */
void ICACHE_FLASH_ATTR
udp_server_init(uint32 port)
{
	udp_server.type = ESPCONN_UDP;
	udp_server.state = ESPCONN_NONE;
	udp_server.proto.udp = &espudp;
	udp_server.proto.udp->local_port = port;

  espconn_regist_recvcb(&udp_server, udp_server_recv);		//ע��UDP���ջص�����
  espconn_regist_sentcb(&udp_server, udp_server_sent_cb); 	//ע��UDP���ͻص�����

  u8 ret = espconn_create(&udp_server);

  os_printf("udp_server_init\r\n");
}
