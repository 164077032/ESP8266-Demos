/*
 * tcp_server.c
 *
 *  Created on: 2017��7��3��
 *      Author: Administrator
 */

#include "ets_sys.h"
#include "osapi.h"
#include "user_interface.h"
#include "mem.h"
#include "espconn.h"

#include "tcp_server.h"

#define TCP_BUFF_SIZE		128
static u8 g_tcp_buff[TCP_BUFF_SIZE];



/*
 * ������tcp_server_sent_cb
 * ������void *arg - ���ӽṹ��
 * ���أ�void
 * ˵����TCP Server���ͻص�
 */
static void ICACHE_FLASH_ATTR
tcp_server_sent_cb(void *arg)
{
	// TODO:
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
	// TODO:
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
	u16 send_data_len = 0;

	// TODO:

	espconn_send(pesp_conn, g_tcp_buff, send_data_len);
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
	static struct espconn s_tcp_server;
	static esp_tcp s_esptcp;

	s_tcp_server.type = ESPCONN_TCP;
	s_tcp_server.state = ESPCONN_NONE;
	s_tcp_server.proto.tcp = &s_esptcp;
	s_tcp_server.proto.tcp->local_port = port;
    espconn_regist_connectcb(&s_tcp_server, tcp_server_listen);

    espconn_accept(&s_tcp_server);

    os_printf("tcp_server_init\r\n");
}
