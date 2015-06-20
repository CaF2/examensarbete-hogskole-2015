##############################
# Copyright (c) 2015 Florian Evaldsson <floriane@kth.se>
#
# Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
##############################

##############################
# COMMON STUFF
##############################

NAME := Biscuit

COMPONENT := CC2540EB

CC := sdcc

OUTDIR := ./.build/

##############################
# INCLUDES
##############################

INCLUDES := ./Projects/ble/Biscuit/Source
INCLUDES += ./Projects/ble/common/cc2540
INCLUDES += ./Projects/ble/Profiles/TXRX/
INCLUDES += ./Projects/ble/util/UBL/soc_8051/usb_msd/app/
#check if these dirs are correct
INCLUDES += ./Components/hal/target/$(COMPONENT)
INCLUDES += ./Components/osal/include
INCLUDES += ./Components/hal/include
INCLUDES += ./Components/ble/controller/include
INCLUDES += ./Components/ble/include
INCLUDES += ./Components/ble/hci
INCLUDES += ./Projects/ble/Include
INCLUDES += ./Projects/ble/Profiles/Roles
INCLUDES += ./
INCLUDES += ./Projects/ble/Profiles/DevInfo
INCLUDES += ./Projects/ble/common/npi/npi_np
INCLUDES += ./Components/ble/host
INCLUDES += ./Projects/ble/util/UBL/soc_8051/usb_msd/hal/usb/class_msd

##############################
# SRCS
##############################

SRCS := $(wildcard ./Projects/ble/Biscuit/Source/*.c)
SRCS += $(wildcard ./Projects/ble/common/cc2540/*.c)
SRCS += $(wildcard ./Projects/ble/Profiles/TXRX/*.c)
SRCS += $(wildcard ./Projects/ble/util/UBL/soc_8051/usb_msd/app/*.c)

#check if these dirs are correct
SRCS += $(wildcard ./Components/hal/common/*.c)
SRCS += $(wildcard ./Components/osal/common/*.c)
SRCS += $(wildcard ./Components/hal/target/$(COMPONENT)/*.c)
SRCS += $(wildcard ./Projects/ble/Profiles/Roles/*.c)
SRCS += $(wildcard ./Projects/ble/Profiles/DevInfo/*.c)
SRCS += $(wildcard ./Projects/ble/common/npi/npi_np/*.c)
SRCS += $(wildcard ./Components/ble/host/*.c)

##############################
# CFLAGS
##############################

#some flags to improve stuff
CFLAGS := --std-sdcc99 -mmcs51
#CFLAGS += -Wall
CFLAGS += $(addprefix -I,$(INCLUDES))

##############################
# some definitions
##############################

#taken from IARs workbench
CFLAGS += -DHOST_CONFIG=PERIPHERAL_CFG -DGAP_PRIVACY_RECONNECT
CFLAGS += -DUSE_SDCC

#information from sdcc:s howto
CFLAGS += -D__near_func=__near
CFLAGS += -D__no_init=

##############################
# temporary
##############################

#CFLAGS += -Duint8=
#CFLAGS += -Duint16=

#CFLAGS += -DSLEEPCMD="(*((int*)(0xBE)))"
#CFLAGS += -DSLEEPSTA="(*((int*)(0x9D)))"
#CFLAGS += -DCLKCONCMD="(*((int*)(0xC6)))"
#CFLAGS += -DCLKCONSTA="(*((int*)(0x9E)))"
#CFLAGS += -DFCTL="(*((int*)(0x6270)))"
##fixa
#CFLAGS += -DIEN0="(*((int*)(0x6270)))"
#CFLAGS += -DEA="(IEN0)"

##############################
# LDFLAGS
##############################

LDFLAGS :=

##############################
# OTHER
##############################

OBJS := $(addprefix $(OUTDIR),$(patsubst %.s,%.o,$(patsubst %.c,%.o,$(SRCS))))

##############################
# COMPILATION OPTIONS
##############################

all: $(NAME)

$(NAME): $(OBJS) 
	$(CC) $(LDFLAGS) -o $(NAME) $^
	
$(OUTDIR)%.o: %.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f -r ./.build
