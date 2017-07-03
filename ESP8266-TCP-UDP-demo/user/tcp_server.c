/*
 * tcp_server.c
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

#include "tcp_server.h"


#define TCP_BUFF_SIZE		128
static u8 tcp_buff[TCP_BUFF_SIZE];

static struct espconn tcp_server;
static esp_tcp esptcp;

/*
 * ������tcp_server_sent_cb
 * ������void *arg - ���ӽṹ��
 * ���أ�void
 * ˵����TCP Server���ͻص�
 */
static void ICACHE_FLASH_ATTR
tcp_server_sent_cb(void *arg)
{
	os_printf("TCP Client Send Call Back\n");
}

/*
 * ������tcp_server_discon_cb
 * ������void *arg - ���ӽṹ��
 * ���أ�void
 * ˵����TCP Server�Ͽ��ص�
 */
static void ICACHE_FLASH_ATTR
tcp_server_discon_cb(void *arg)
{
	os_printf("TCP Client discon\n");
}

/*
 * ������tcp_server_recv
 * ������void *arg - ���ӽṹ��
 * 	   char *pdata - ���������׵�ַ
 * 	   unsigned short len - �������ݳ���
 * ���أ�void
 * ˵����TCP Server���ջص�
 */
static void ICACHE_FLASH_ATTR
tcp_server_recv(void *arg, char *pdata, unsigned short len)
{
	struct espconn *pesp_conn = arg;

	static char temp[64];

	os_printf("lenth:%d\r\ndata:%s\r\n",len, pdata);

  // TODO:

  espconn_send(pesp_conn, tcp_buff, os_strlen(tcp_buff));
}

/*
 * ������tcp_server_listen
 * ������void *arg - ���ӽṹ��
 * ���أ�void
 * ˵����TCP Server����
 */
static void ICACHE_FLASH_ATTR
tcp_server_listen(void *arg)
{
    struct espconn *pesp_conn = arg;

    espconn_regist_disconcb(pesp_conn, tcp_server_discon_cb);
    espconn_regist_recvcb(pesp_conn, tcp_server_recv);
    espconn_regist_sentcb(pesp_conn, tcp_server_sent_cb);
    //espconn_regist_reconcb(pesp_conn, client_recon_cb);

}

/*
 * ������tcp_server_init
 * ������void *arg - ���ӽṹ��
 * ���أ�void
 * ˵����TCP Server��ʼ��
 */
void ICACHE_FLASH_ATTR
tcp_server_init(uint32 port)
{
	tcp_server.type = ESPCONN_TCP;
	tcp_server.state = ESPCONN_NONE;
	tcp_server.proto.tcp = &esptcp;
	tcp_server.proto.tcp->local_port = port;
    espconn_regist_connectcb(&tcp_server, tcp_server_listen);

    espconn_accept(&tcp_server);

    os_printf("tcp_server_init\r\n");
}
