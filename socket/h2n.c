#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
typedef unsigned int unit32_t;
typedef unsigned short unit16_t;
union
{
	char c;
	int i;
} un; 
unit32_t h2nl(uint32_t hostlong)
{
	uint32_t x = hostlong;
	un.i = 0x1;
	if(un.c)
	{      
		x = (hostlong<<24)            //0x78000000
                    | ((hostlong &0xff00)<<8) //0x00560000
                    | ((hostlong>>8)&0xff00)  //0x00003400
                    | (hostlong>>24);         //0x00000012
	}
	return x;
}
unit16_t h2ns(uint16_t hostshort)
{
	unit16_t x = hostshort;
	un.i = 0x1;
	if(un.c)
	{//0x1234
		x = (hostshort<<8)   //0x3400
                    | (hostshort>>8);//0x0012
	}
	return x;
}
unit32_t n2hl(uint32_t netlong)
{
	uint32_t x = netlong;
	un.i = 0x1;
	if(un.c)
	{
		x = (netlong<<24)
                    | ((netlong &0xff00)<<8) 
                    | ((netlong>>8)&0xff00) 
                    | (netlong>>24);
	}
	return x;
}
unit16_t n2hs(uint16_t netshort)
{
	unit16_t x = netshort;
	un.i = 0x1;
	if(un.c)
	{
		x = (netshort<<8) 
                    | (netshort>>8);
	}
	return x;
}
//////////////////////////////////
in_addr_t inet_addr_2(const char* strptr)
{
	char * p = NULL;
	char * q = NULL;
	int a[4] = {0};
	int i = 0;
	in_addr_t addr = 0;
	char buf[100] = {0};
	strcpy(buf,strptr);
	p = strtok(buf,".");
	a[i++] = atoi(p);
        while((q = strtok(NULL, ".")))
	{
		a[i++] = atoi(q);
	}
	for(i = 0; i < 4; i++)
	{
		printf("%d ",a[i]);
	}
	puts("");
        addr = a[3] |(a[2]<<8)|(a[1]<<16)|(a[0]<<24);
	return addr;
}
char* addr_inet_2(in_addr_t i)
{
	char buf[100] = {0};
	sprintf(buf+strlen(buf),"%d.",(i>>24)&0xff);
	sprintf(buf+strlen(buf),"%d.",(i>>16)&0xff);
	sprintf(buf+strlen(buf),"%d.",(i>>8)&0xff);
	sprintf(buf+strlen(buf),"%d", i&0xff);
	return buf;
}
/////////////////////////////////
int main()
{
	printf("0x%08x 0x%08x 0x%08x 0x%08x\n",h2nl(0x12345678)
,h2ns(0x1234),n2hl(0x12345678),n2hs(0x1234));
	printf("0x%08X\n",inet_addr_2("192.168.0.55"));
	printf("%s\n",addr_inet_2(0xc0a80037));
        return 0;
}
