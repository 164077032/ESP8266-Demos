/*
 * lwip_demo.c
 *
 *  Created on: 2018��1��3��
 *      Author: Administrator
 *
 *  ����lwIP��raw/callback API���
 */

#include "lwip/ip_addr.h"
#include "lwip/udp.h"

#define UDP_ECHO_PORT	7

/*
 * function: udp_demo_cb
 * parameter: void *arg -
 *            struct udp_pcb *upcb -
 *            struct pbuf *p       - recvive data�����յ������ݰ�
 *            struct ip_addr *addr - remote ip���������ݵ�����ip
 *            u16 port             - remote port���������ݵ�����port
 * description: udp recvive callback function
 *              UDP���ջص�����
 */
static void ICACHE_FLASH_ATTR
udp_demo_cb(void *arg, struct udp_pcb *upcb,
		struct pbuf *p, struct ip_addr* addr, u16 port)
{
	unsigned char* temp = (unsigned char*)addr;
	udp_sendto(upcb, p, addr, port);
	pbuf_free(p);	// �ǵ�Ҫ�ֶ����
}

void ICACHE_FLASH_ATTR
udp_demo_init(void)
{
	struct udp_pcb *upcb;
	upcb = udp_new();
	udp_bind(upcb, IP_ADDR_ANY, UDP_ECHO_PORT);
	udp_recv(upcb, udp_demo_cb, NULL);
}
