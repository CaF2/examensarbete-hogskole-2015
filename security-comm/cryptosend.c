/**
Copyright (c) 2015 Florian Evaldsson <floriane@kth.se>

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include "cryptosend.h"

#ifdef WIN32
#include <windows.h>
#define COM_PORT 7 //com port 8
#elif _POSIX_C_SOURCE >= 199309L
#include <time.h>   // for nanosleep
#define COM_PORT 16 // /dev/ttyUSB0
#else
#include <unistd.h> // for usleep
#endif

/**
	cross-platform sleep function
*/
void wait_cross(int milliseconds)
{
#ifdef WIN32
    Sleep(milliseconds);
#elif _POSIX_C_SOURCE >= 199309L
    struct timespec ts;
    ts.tv_sec = 0;
    ts.tv_nsec = milliseconds * 1000000;
    nanosleep(&ts, NULL);
#else
    usleep(milliseconds * 1000);
#endif
}

/**
	Send data with a function.
*/
void cryptosend_send_data(char *theData, size_t dataLen, int insegment, void *sendFunction, double delayTime)
{
	void (*typefunction)(char);
	
	typefunction=sendFunction;
	
	//with null-termination
	char *controlsekvens;
	size_t controlsekvenslen;
	
	//<1 bit kontrollsekvens><om kontrollsekvensen är 0 -> 31 (7+8*3) bitar som säger datans längd><data>
	if(insegment==0)
	{
		//4 bits + null termination
		controlsekvens=malloc(4);
		controlsekvenslen=4;
		
		size_t newcontrol=(dataLen&0x7FFFFFFF);
		
		printf("newcontrol: %d\n",newcontrol);
		
		//remember little endian/big endian
		for(int i=0;i<4;i++)
		{
			controlsekvens[i]=((newcontrol>>(3-i)*8)&0xff);
		}
	}
	
	size_t totLen=controlsekvenslen+dataLen;
	
	char *datastream=malloc(totLen);
	
	memcpy(datastream,controlsekvens,controlsekvenslen);
	
	memcpy(datastream+controlsekvenslen,theData,dataLen);
	
	for(int i=0;i<totLen;i++)
	{
		typefunction(datastream[i]);
		
		wait_cross(delayTime);
	}
}

/**
	init the cryptosend structure
*/
CryptosendDataHandler *cryptosenddatahandler_init(void)
{
	return calloc(1,sizeof(CryptosendDataHandler));
}

/**
	get data, and when finished, do a function.
*/
int cryptosend_get_data(CryptosendDataHandler *dataHandler,char *inData, size_t inDataLen, void *functionWhenRead, void *inparam)
{
	void (*typefunction)(char*,size_t,void*);
	int doneFunct=0;
	
	typefunction=functionWhenRead;
	
	for(int i=0;i<inDataLen;i++)
	{
		if(dataHandler->curpos==0)
		{	
			printf("datalen=%d\n",dataHandler->datalen);
		
			if(inData[i]&(1<<(8-1)))
			{
				//maybe later
				dataHandler->insegment=1;
			}
			else
			{
				dataHandler->insegment=0;
			}
		}
		
		if(dataHandler->insegment==0)
		{
			if(dataHandler->curpos>=0 && dataHandler->curpos<=3)
			{
				printf("indata:%d\n",inData[i]);
				dataHandler->datalen|=(dataHandler->curpos==0?(((uint8_t)inData[i]&0x7f)<<(8*3)):((uint8_t)inData[i]<<(8*(3-dataHandler->curpos))));
				
				if(dataHandler->curpos==3)
				{
					printf("datalen=%d\n",dataHandler->datalen);
					dataHandler->data=malloc(dataHandler->datalen);
				}
			}
			else
			{
				dataHandler->data[dataHandler->curpos-4]=inData[i];
				printf("data in:%c\n",dataHandler->data[dataHandler->curpos-4]);
			
				if(dataHandler->curpos-(4-1) >= dataHandler->datalen)
				{
					printf("should put out the data now...\n");
					typefunction(dataHandler->data,dataHandler->datalen,inparam);
					doneFunct=1;
					
					free(dataHandler->data);
					memset(dataHandler,0,sizeof(CryptosendDataHandler));
				}
			}
		}
		
		dataHandler->curpos++;
	}
	
	return doneFunct;
}
