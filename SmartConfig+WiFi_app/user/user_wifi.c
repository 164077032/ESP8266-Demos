/*
 * user_wifi.c
 *
 *  Created on: Dec 30, 2014
 *      Author: yannaxiu
 */


#include "user_interface.h"
#include "osapi.h"
#include "espconn.h"
#include "os_type.h"
#include "mem.h"

#include "user_smart_config.h"

/*************************************************************/

#define HUMITURE_WIFI_LED_IO_MUX 	PERIPHS_IO_MUX_GPIO0_U
#define HUMITURE_WIFI_LED_IO_NUM 	0
#define HUMITURE_WIFI_LED_IO_FUNC 	FUNC_GPIO0

void ICACHE_FLASH_ATTR
wifi_status_len_init(void)
{
	// ʹ�� GPIO0 ��Ϊ WiFi ״̬ LED
	wifi_status_led_install(HUMITURE_WIFI_LED_IO_NUM,
			HUMITURE_WIFI_LED_IO_MUX,
			HUMITURE_WIFI_LED_IO_FUNC
		);
}

/*************************************************************/

static os_timer_t g_wifi_timer;
static os_timer_t g_smartconig_led_timer;

/*
 * ������smartconfig_led_timer_cb
 * ˵����smartconfig״̬�ƶ�ʱ���ص�����
 */
static void ICACHE_FLASH_ATTR
smartconfig_led_timer_cb(void *arg)
{
	u8 led = GPIO_INPUT_GET(0);
	GPIO_OUTPUT_SET(0, 1-led);		// ȡ��
}

/*
 * ������user_smartconfig_init
 * ˵����smartconfig��ʼ��
 */
static void ICACHE_FLASH_ATTR
user_smartconfig_init(void)
{
	//esptouch_set_timeout(30);
	smartconfig_set_type(SC_TYPE_ESPTOUCH_AIRKISS); //SC_TYPE_ESPTOUCH,SC_TYPE_AIRKISS,SC_TYPE_ESPTOUCH_AIRKISS
	smartconfig_start(smartconfig_done);
	os_printf("[INFO] smartconfig start!\r\n");

	os_timer_disarm(&g_smartconig_led_timer);
	os_timer_setfn(&g_smartconig_led_timer, (os_timer_func_t *)smartconfig_led_timer_cb, NULL);
	os_timer_arm(&g_smartconig_led_timer, 1000, 1);
}

/*
 * ������wifi_status_change_cb
 * ˵����WiFi״̬�ı�ص�����
 */
static void ICACHE_FLASH_ATTR
wifi_status_change_cb(void)
{
	u8 status = wifi_station_get_connect_status();
	os_printf("[INFO] wifi_connect_cb\r\n");

	if(status == STATION_GOT_IP){
		// TODO:
	} else {
		// TODO:
	}

}

/*
 * ������wifi_start_timer_cb
 * ˵����Wi-Fi�״����Ӷ�ʱ���ص�����
 */
static void ICACHE_FLASH_ATTR
wifi_start_timer_cb(void *arg)
{
	static u8 s_flag = 0;
	static u8 s_wifi_led_init_flag = 0;
	struct ip_info ipConfig;

	os_timer_disarm(&g_wifi_timer);	// ֹͣ�ö�ʱ��
	wifi_get_ip_info(STATION_IF, &ipConfig);
	u8 wifi_status = wifi_station_get_connect_status();

	if(0==s_wifi_led_init_flag){
		s_wifi_led_init_flag = 1;
		// WiFi״̬�Ƴ�ʼ��
		wifi_status_len_init();
	}

	// ��WiFi���ӳɹ���
	if (STATION_GOT_IP == wifi_status && ipConfig.ip.addr != 0){
		wifi_station_set_reconnect_policy(TRUE);	// ����AP�Ͽ����Զ�����
		wifi_station_set_auto_connect(TRUE);		// �����Զ�����
		os_printf("[INFO] Wi-Fi Connected!\r\n");
	}else{
		if(0==s_flag){	// û�л�ȡ��smartconfig
			os_printf("[INFO] smartconfig stop!\r\n");
			smartconfig_stop();
			os_timer_disarm(&g_smartconig_led_timer);
			s_flag = 1;		// ��־���ٽ���ô���
		}else{
			os_printf("[ERROR] Wi-Fi Connect Fail!\r\n");
		}

		wifi_station_disconnect();
		wifi_station_connect();		// ���Խ�������

		os_timer_setfn(&g_wifi_timer, (os_timer_func_t *)wifi_start_timer_cb, NULL);
		os_timer_arm(&g_wifi_timer, 10000, 0);	// 10������һ�λص�
	}
}


void ICACHE_FLASH_ATTR
wifi_handle_event_cb(System_Event_t *evt)
{
	os_printf("event %x\n", evt->event);
	switch (evt->event) {
		case EVENT_STAMODE_CONNECTED:
			os_printf("connect to ssid %s, channel %d\n",
			evt->event_info.connected.ssid,
			evt->event_info.connected.channel);
		break;
		case EVENT_STAMODE_DISCONNECTED:
			os_printf("disconnect from ssid %s, reason %d\n",
			evt->event_info.disconnected.ssid,
			evt->event_info.disconnected.reason);
		break;
		case EVENT_STAMODE_AUTHMODE_CHANGE:
			os_printf("mode: %d -> %d\n",
			evt->event_info.auth_change.old_mode,
			evt->event_info.auth_change.new_mode);
		break;
		case EVENT_STAMODE_GOT_IP:
		os_printf("ip:" IPSTR ",mask:" IPSTR ",gw:" IPSTR,
			IP2STR(&evt->event_info.got_ip.ip),
			IP2STR(&evt->event_info.got_ip.mask),
			IP2STR(&evt->event_info.got_ip.gw));
			os_printf("\n");

			// TODO:


		break;
			case EVENT_SOFTAPMODE_STACONNECTED:
			os_printf("station: " MACSTR "join, AID = %d\n",
			MAC2STR(evt->event_info.sta_connected.mac),
			evt->event_info.sta_connected.aid);
		break;
		case EVENT_SOFTAPMODE_STADISCONNECTED:
			os_printf("station: " MACSTR "leave, AID = %d\n",
			MAC2STR(evt->event_info.sta_disconnected.mac),
			evt->event_info.sta_disconnected.aid);
		break;
		default:
		break;
	}

	wifi_status_change_cb();
}


void ICACHE_FLASH_ATTR
wifi_connect(void)
{
	wifi_set_opmode_current(STATION_MODE);		// ����ΪSTATIONģʽ
	wifi_station_disconnect();
	wifi_station_set_reconnect_policy(FALSE);	// ���� AP ʧ�ܻ�Ͽ����Ƿ�������
	wifi_station_set_auto_connect(FALSE);		// �ر��Զ�����

	// ��������SmartConfig
	user_smartconfig_init();

	// һ��ʱ�����Wi-Fi����״̬
	os_timer_disarm(&g_wifi_timer);
	os_timer_setfn(&g_wifi_timer, (os_timer_func_t *)wifi_start_timer_cb, NULL);
	os_timer_arm(&g_wifi_timer, 20000, 0);	// 20������һ�λص�

	// ע�� Wi-Fi �¼�
	wifi_set_event_handler_cb(wifi_handle_event_cb);
}


