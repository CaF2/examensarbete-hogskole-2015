/**
Copyright (c) 2015 Florian Evaldsson <floriane@kth.se>

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#pragma once

typedef enum CryptosendInsegment
{
	CRYPTOSEND_NORMAL_DATA=0,
	CRYPTOSEND_SPECIAL=1
}CryptosendInsegment;

typedef struct CryptosendDataHandler
{
	char *data;
	int curpos;
	size_t datalen;
	int insegment;
}CryptosendDataHandler;

void wait_cross(int milliseconds);

CryptosendDataHandler *cryptosenddatahandler_init(void);

void cryptosend_send_data(char *theData, size_t dataLen, int insegment, void *sendFunction, double delayTime);

int cryptosend_get_data(CryptosendDataHandler *dataHandler,char *inData, size_t inDataLen, void *functionWhenRead, void *inparam);
