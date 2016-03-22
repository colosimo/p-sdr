#
# Author: Aurelio Colosimo, 2016
# Released under MIT-Expat License.
#

LIBPSDRPATH ?= $(shell pwd)/../..

-include $(LIBPSDRPATH)/generated/libpsdr.mk

CFLAGS += $(patsubst -I%,-I$(LIBPSDRPATH)/%,$(INCFLAGS))

CC=$(CROSS_COMPILE)gcc

EXE = psdr_enum
LFLAGS += -lusb-1.0
OBJS += main.o

$(EXE): $(OBJS)
	$(CC) $(OBJS) $(LIBPSDRPATH)/libpsdr.a $(LFLAGS) -o $@

clean:
	rm -f *.o $(EXE)

.PHONY: clean