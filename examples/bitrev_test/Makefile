#
# Author: Aurelio Colosimo, 2016
# Released under MIT-Expat License.
#

LIBPSDRPATH ?= $(shell pwd)/../..

-include $(LIBPSDRPATH)/generated/libpsdr.mk

CFLAGS += $(patsubst -I%,-I$(LIBPSDRPATH)/%,$(INCFLAGS))

CC=$(CROSS_COMPILE)gcc

EXE = bitrev_test
OBJS += main.o bitrev16.o


$(EXE): $(OBJS)
	$(CC) $(OBJS) $(LIBPSDRPATH)/libpsdr.a $(LFLAGS) -o $@

main.o: bitrev16.h

bitrev16.c bitrev16.h:
	$(LIBPSDRPATH)/tools/psdr-bitreverse 16 bitrev16.h bitrev16.c

clean:
	rm -f *.o $(EXE) bitrev16.h bitrev16.c

.PHONY: clean
