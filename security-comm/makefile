###################################
# Copyright (c) 2015 Florian Evaldsson <floriane@kth.se>
#
# Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
###################################

###################
# typical parameters
###################

NAME:=cryptotest

CC:=gcc

###################
# cflags and ldflags
###################

CFLAGS:= -g -std=gnu99

LDFLAGS:= -lssl -lcrypto -ldl -pthread

###################
# all the c files
###################

FILES:=$(wildcard ./*.c)
OBJS:=$(patsubst %.c,%.o,$(FILES))

###################
# compilation options
###################

all:$(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $(NAME)

.c.o:
	$(CC) $(CFLAGS) -c -o $@ $<

val: all
	valgrind  --track-origins=yes --tool=memcheck ./$(NAME)
	
run: all
	./$(NAME)
	
cert:
	openssl req -x509 -newkey rsa:2048 -days 3650 -nodes -keyout client-key.pem -out client-cert.pem
	openssl req -x509 -newkey rsa:2048 -days 3650 -nodes -keyout server-key.pem -out server-cert.pem
 
clean:
	rm -f *.o
	rm $(NAME)
