#
# Author: Aurelio Colosimo, 2016
# Released under MIT-Expat License.
#

OBJS = $(patsubst %.c,%.o,$(wildcard *.c))
EXE = $(patsubst %.c,psdr-%,$(wildcard *.c))

all: $(EXE)

rebuild: clean all

LFLAGS = -lm

# The list of files to be cleaned is:
# 1) any .o and in obj or in any arch; 2) each .d corresponding to its .o
CLEAN_LIST += $(EXE) $(OBJS) $(patsubst %.o,%.d,$(OBJS))

psdr-%: %.o
	gcc -o $@ $< $(LFLAGS)

%.o: %.c
	gcc -c $(CFLAGS) -MMD -o $@ $<

clean:
	rm -f $(CLEAN_LIST)

.PHONY: examples clean
