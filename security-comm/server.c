/**
Copyright (c) 2015 Florian Evaldsson <floriane@kth.se>

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#define SERVER
#include <stdio.h>
#include <stdlib.h>
 
#include <openssl/err.h>
#include <openssl/dh.h>
#include <openssl/ssl.h>
#include <openssl/conf.h>
#include <openssl/engine.h>
#include "cryptocommon.h"

/**
	Main function for the client
*/
void *main_server(void *argument) {
	
	/* startup SSL */
	//cryptsend_begin();
 
	/* create client/server objects */
	cryptsend server;
 
	/* init server. */
	if(cryptsend_ssl_ctx_init(&server, "server") < 0) {
		exit(EXIT_FAILURE);
	}
	if(cryptsend_ssl_init(&server, 1, cryptsend_ssl_server_info_callback) < 0) {
		exit(EXIT_FAILURE);
	}
 
	printf("+ Initialized server.\n");
	
	/* kickoff handshake; initiated by client (e.g. browser) */
	//SSL_do_handshake(client.ssl);
	printf("s1 STEP\n");
	cryptsend_ssl_read(&server,1); 
	printf("s2 STEP\n");
	cryptsend_ssl_write(&server,1); 
	printf("s3 STEP\n");
	cryptsend_ssl_read(&server,1); 
	printf("s4 STEP\n");
	cryptsend_ssl_write(&server,1); 
 
	/* encrypt some data and send it to the client */
	char buf[521] = { 0 } ;
	sprintf(buf, "%s", "Hello world! Det fungerar ska du se!");
	SSL_write(server.ssl, buf, sizeof(buf));
	cryptsend_ssl_write(&server,1);
 
	cryptsend_ssl_shutdown(&server);
 
	//cryptsend_end();
	return EXIT_SUCCESS;
}
