# cJSON_app

��ESP8266��ʹ��cJSON��ĿǰcJSON���°汾��1.5.3��

����ESP8266��֧�ָ��������޸���cJSON���й�double�ĺ�����

Ч��ͼ

![ESP826-cJSON-demo](https://github.com/AngelLiang/ESP8266-Demos/blob/master/img/cJSON.jpg)

��ز��ͣ�http://blog.csdn.net/yannanxiu/article/details/52713746

�ַ���������JSONʾ��

```
u8* pdata = "{\"hello\":\"world\"}";
cJSON *root = cJSON_Parse(pdata);
print_preallocated(root);
cJSON *hello = cJSON_GetObjectItemCaseSensitive(root, "hello");
if (cJSON_IsString(hello)){
 ?os_printf("hello %s!\r\n", hello->valuestring); ? // ��ӡ��hello world!��
}
cJSON_Delete(root);
```

����JSONʾ����ο�cJSON_text.c�ļ���Ĵ��롣