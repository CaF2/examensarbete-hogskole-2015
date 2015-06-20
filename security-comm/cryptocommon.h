/**
Copyright (c) 2015 Florian Evaldsson <floriane@kth.se>

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

/* this file is largely based on: http://www.roxlu.com/2014/042/using-openssl-with-memory-bios */

#pragma once

#include "include.h"
/* SSL debug */
#define SSL_WHERE_INFO(ssl, w, flag, msg) {								\
		if(w & flag) {																				 \
			printf("+ %s: ", name);															\
			printf("%20.20s", msg);															\
			printf(" - %30.30s ", SSL_state_string_long(ssl));	 \
			printf(" - %5.10s ", SSL_state_string(ssl));				 \
			printf("\n");																				\
		}																											\
	}
 
typedef void(*info_callback)();
 
typedef struct {
	SSL_CTX* ctx;												/* main ssl context */
	SSL* ssl;													/* the SSL* which represents a "connection" */
	BIO* in_bio;												/* we use memory read bios */
	BIO* out_bio;												/* we use memory write bios */
	char name[512];
} cryptsend;
 
void cryptsend_begin();																																		 /* initialize SSL */
void cryptsend_end();																																			 /* shutdown SSL */
int cryptsend_ssl_ctx_init(cryptsend* k, const char* keyname);																		/* initialize the SSL_CTX */
int cryptsend_ssl_init(cryptsend* k, int isserver, info_callback cb);														 /* init the SSL* (the "connection"). we use the `isserver` to tell SSL that it should either use the server or client protocol */
int cryptsend_ssl_shutdown(cryptsend* k);																												 /* cleanup SSL allocated mem */
int cryptsend_ssl_verify_peer(int ok, X509_STORE_CTX* ctx);																 /* we set the SSL_VERIFY_PEER option on the SSL_CTX, so that the server will request the client certificate. We can use the certificate to get/verify the fingerprint */

int cryptsend_ssl_write(cryptsend* from, int isServer);
void cryptsend_ssl_read_done(char *text,size_t textlen,void *param);
int cryptsend_ssl_read(cryptsend* to, int isServer);
 
/* some debug info */
void cryptsend_ssl_server_info_callback(const SSL* ssl, int where, int ret);								/* purely for debug purposes; logs server info. */
void cryptsend_ssl_client_info_callback(const SSL* ssl, int where, int ret);								/* client info callback */
void cryptsend_ssl_info_callback(const SSL* ssl, int where, int ret, const char* name);		 /* generic info callback */
