/**
Copyright (c) 2015 Florian Evaldsson <floriane@kth.se>

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#define GLOBHERE 1

#include <openssl/err.h>
#include <openssl/dh.h>
#include <openssl/ssl.h>
#include <openssl/conf.h>
#include <openssl/engine.h>
#include "cryptocommon.h"

//the global virtual outports
cXX srvl={0},clil={0};
cXX *toSrv=&srvl,*coCli=&clil;

/**
	Function used to send data from the server
*/
void GLOB_srv_send_data(char data)
{
	while(toSrv->state==1);//{printf("1\n");}
	
	toSrv->c=data;
	toSrv->state=1;
}

/**
	Get data from the client
*/
char GLOB_srv_get_data()
{
	while(coCli->state==0);//{printf("2\n");}
	
	char retval=coCli->c;
	coCli->state=0;
	return retval;
}

/**
	Function used to send data from the client
*/
void GLOB_cli_send_data(char data)
{
	while(coCli->state==1);//{printf("3\n");}
	
	coCli->c=data;
	coCli->state=1;
}

/**
	Get data from the server
*/
char GLOB_cli_get_data()
{
	while(toSrv->state==0);//{printf("4\n");}
	
	char retval=toSrv->c;
	toSrv->state=0;
	return retval;
}

/**
	Main function
*/
int main(void)
{
	RXTX commline;
	pthread_t server,client;
	int result_code;
 	
 	//begin the crypto-process. If this would have been done in embedded systems, then put this in the "thread" at the start
 	cryptsend_begin();
 	
	//create the server and client threads, to emulate the process
	printf("init server\n");
	result_code = pthread_create(&server, NULL, main_server, (void *) &commline);
  	assert(0 == result_code);
  	printf("init client\n");
  	result_code = pthread_create(&client, NULL, main_client, (void *) &commline);
  	assert(0 == result_code);
 
	//Wait for the threads to complete
	result_code = pthread_join(server, NULL);
	printf("Server has completed\n");
	assert(0 == result_code);
	result_code = pthread_join(client, NULL);
	printf("Client has completed\n");
	assert(0 == result_code);
	
	//end the crypto-process. If this would have been done in embedded systems, then put this in the "thread" at the end
	cryptsend_end();
 
	printf("In main: All threads completed successfully\n");
	return 0;
}
