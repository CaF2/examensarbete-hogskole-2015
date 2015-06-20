/**
Copyright (c) 2015 Florian Evaldsson <floriane@kth.se>

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

/* this file is largely based on: http://www.roxlu.com/2014/042/using-openssl-with-memory-bios */

#include <stdio.h>
#include <stdlib.h>
 
#include <openssl/err.h>
#include <openssl/dh.h>
#include <openssl/ssl.h>
#include <openssl/conf.h>
#include <openssl/engine.h>
#include "cryptocommon.h"
#include "cryptosend.h"

/**
	begin the process
*/
void cryptsend_begin() {
	SSL_library_init();
	SSL_load_error_strings();
	ERR_load_BIO_strings();
	OpenSSL_add_all_algorithms();
}

/**
	end the process
*/
void cryptsend_end() {
	ERR_remove_state(0);
	ENGINE_cleanup();
	CONF_modules_unload(1);
	ERR_free_strings();
	EVP_cleanup();
	sk_SSL_COMP_free(SSL_COMP_get_compression_methods());
	CRYPTO_cleanup_all_ex_data();
}

/**
	init ctx
*/
int cryptsend_ssl_ctx_init(cryptsend* k, const char* keyname) {
 
	int r = 0;
 
	/* create a new context using DTLS */
	k->ctx = SSL_CTX_new(DTLSv1_method());
	if(!k->ctx) {
		printf("Error: cannot create SSL_CTX.\n");
		ERR_print_errors_fp(stderr);
		return -1;
	}
 
	/* set our supported ciphers */
	r = SSL_CTX_set_cipher_list(k->ctx, "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH");
	if(r != 1) {
		printf("Error: cannot set the cipher list.\n");
		ERR_print_errors_fp(stderr);
		return -2;
	}
 
	/* the client doesn't have to send it's certificate */
	SSL_CTX_set_verify(k->ctx, SSL_VERIFY_PEER, cryptsend_ssl_verify_peer);
 
	/* enable srtp */
	r = SSL_CTX_set_tlsext_use_srtp(k->ctx, "SRTP_AES128_CM_SHA1_80");
	if(r != 0) {
		printf("Error: cannot setup srtp.\n");
		ERR_print_errors_fp(stderr);
		return -3;
	}
 
	/* load key and certificate */
	char certfile[1024];
	char keyfile[1024];
	sprintf(certfile, "./%s-cert.pem", keyname);
	sprintf(keyfile, "./%s-key.pem", keyname);
 
	/* certificate file; contains also the public key */
	r = SSL_CTX_use_certificate_file(k->ctx, certfile, SSL_FILETYPE_PEM);
	if(r != 1) {
		printf("Error: cannot load certificate file.\n");
		ERR_print_errors_fp(stderr);
		return -4;
	}
 
	/* load private key */
	r = SSL_CTX_use_PrivateKey_file(k->ctx, keyfile, SSL_FILETYPE_PEM);
	if(r != 1) {
		printf("Error: cannot load private key file.\n");
		ERR_print_errors_fp(stderr);
		return -5;
	}
 
	/* check if the private key is valid */
	r = SSL_CTX_check_private_key(k->ctx);
	if(r != 1) {
		printf("Error: checking the private key failed. \n");
		ERR_print_errors_fp(stderr);
		return -6;
	}
 
	sprintf(k->name, "+ %s", keyname);
 
	return 0;
}

int cryptsend_ssl_verify_peer(int ok, X509_STORE_CTX* ctx) {
	return 1;
}
 
/** 
	this sets up the SSL
*/
int cryptsend_ssl_init(cryptsend* k, int isserver, info_callback cb) {
 
	/* create SSL* */
	k->ssl = SSL_new(k->ctx);
	if(!k->ssl) {
		printf("Error: cannot create new SSL*.\n");
		return -1;
	}
 
	/* info callback */
	SSL_set_info_callback(k->ssl, cb);
 
	/* bios */
	k->in_bio = BIO_new(BIO_s_mem());
	if(k->in_bio == NULL) {
		printf("Error: cannot allocate read bio.\n");
		return -2;
	}
 
	BIO_set_mem_eof_return(k->in_bio, -1); /* see: https://www.openssl.org/docs/crypto/BIO_s_mem.html */
 
	k->out_bio = BIO_new(BIO_s_mem());
	if(k->out_bio == NULL) {
		printf("Error: cannot allocate write bio.\n");
		return -3;
	}
 
	BIO_set_mem_eof_return(k->out_bio, -1); /* see: https://www.openssl.org/docs/crypto/BIO_s_mem.html */
 
	SSL_set_bio(k->ssl, k->in_bio, k->out_bio);
 
	/* either use the server or client part of the protocol */
	if(isserver == 1) {
		SSL_set_accept_state(k->ssl);
	}
	else {
		SSL_set_connect_state(k->ssl);
	}
 
	return 0;
}
 
void cryptsend_ssl_server_info_callback(const SSL* ssl, int where, int ret) {
	cryptsend_ssl_info_callback(ssl, where, ret, "server");
}
void cryptsend_ssl_client_info_callback(const SSL* ssl, int where, int ret) {
	cryptsend_ssl_info_callback(ssl, where, ret, "client");
}

void cryptsend_ssl_info_callback(const SSL* ssl, int where, int ret, const char* name) {
 
	if(ret == 0) {
		printf("-- cryptsend_ssl_info_callback: error occured.\n");
		return;
	}
 
	SSL_WHERE_INFO(ssl, where, SSL_CB_LOOP, "LOOP");
	SSL_WHERE_INFO(ssl, where, SSL_CB_HANDSHAKE_START, "HANDSHAKE START");
	SSL_WHERE_INFO(ssl, where, SSL_CB_HANDSHAKE_DONE, "HANDSHAKE DONE");
}

/**
	convert all '\0' to '0' in a string
*/
char *to_no_null(char *buf,int len)
{
	char *newbuf=malloc(len+1);

	for(int i=0;i<len;i++)
	{
		if(buf[i]=='\0')
		{
			newbuf[i]='0';
		}
		else
		{
			newbuf[i]=buf[i];
		}
	}
	
	newbuf[len]='\0';

	return newbuf;
}

/**
	write data
	
	@param isServer
		says if it is the server that reads, or if false then its the client
*/
int cryptsend_ssl_write(cryptsend* from,int isServer)
{
//, cryptsend* to
	// Did SSL write something into the out buffer
	//outbuf is what we send
	char outbuf[4096]; 
	int written = 0;
	int read = 0;
	
	int pending = BIO_ctrl_pending(from->out_bio);
 
	if(pending > 0) {
		read = BIO_read(from->out_bio, outbuf, sizeof(outbuf));
	}

	char *newstr=to_no_null(outbuf,read);

	printf("%s Pending %d, and read: %d %s\n", from->name, pending, read, newstr);

	free(newstr);
	
	if(read > 0)
	{
		if(!isServer)
			cryptosend_send_data(outbuf, read, CRYPTOSEND_NORMAL_DATA, GLOB_cli_send_data, 5);
		else
			cryptosend_send_data(outbuf, read, CRYPTOSEND_NORMAL_DATA, GLOB_srv_send_data, 5);
	}
 
	return 0;
}

/**
	when finished with the reading
*/
void cryptsend_ssl_read_done(char *text,size_t textlen,void *param)
{
	cryptsend *to=param;

	char outbuf[4096];
	int written = 0;
		
	if(textlen > 0) {
		written = BIO_write(to->in_bio, text, textlen);
	}
 
	if(written > 0) {
		if(!SSL_is_init_finished(to->ssl)) {
			SSL_do_handshake(to->ssl);
		}
		else {
			textlen = SSL_read(to->ssl, outbuf, sizeof(outbuf));
			printf("%s read: %s\n", to->name, outbuf);
		}
	}
}

/**
	read data
	
	@param isServer
		says if it is the server that reads, or if false then its the client
*/
int cryptsend_ssl_read(cryptsend* to,int isServer)
{
	CryptosendDataHandler *datahandl=cryptosenddatahandler_init();
	
	int functDone=0;
	
	while(!functDone)
	{
		char out;
		if(!isServer)
			out=GLOB_cli_get_data();
		else
			out=GLOB_srv_get_data();
	
		functDone=cryptosend_get_data(datahandl, &out, 1, cryptsend_ssl_read_done, (void*)to);
	}
 
	return 0;
}
 
/**
	shutdown the process
*/
int cryptsend_ssl_shutdown(cryptsend* k)
{
	if(!k) {
		return -1;
	}
 
	if(k->ctx) { 
		SSL_CTX_free(k->ctx);
		k->ctx = NULL;
	}
 
	if(k->ssl) {
		SSL_free(k->ssl);
		k->ssl = NULL;
	}
 
	return 0;
}
