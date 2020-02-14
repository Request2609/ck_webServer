#include <iostream>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
void reversestr(char*source,char target[],unsigned int length);
void tohex(unsigned long num,char*hexStr);
void reversestr(char*source,char target[],unsigned int length)
{
	unsigned int i;
	for(i=0;i<length;i++)
		target[i]=source[length-1-i];
	target[i]=0;
}
void tohex(unsigned long num,char*hexStr)
{
	unsigned long n=num;
	char hextable[]="0123456789ABCDEF";
	char temphex[16],hex[16];
	unsigned long int i=0;
	while(n)
	{
		temphex[i++]=hextable[n%16];
		n/=16;
	}
	temphex[i]=0;
	reversestr(temphex,hex,i);
	strcpy(hexStr,hex);
}
