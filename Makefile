#
# Author: Aurelio Colosimo, 2016
# Released under MIT-Expat License.
#

# In config dir, some typical configurations are stored.
# Default is unix32, a 32bit unix system
CONFIG ?= unix32
include config/$(CONFIG)_cfg.mk

ifeq ($(UINTP),)
	UINTP = u$(INTP)
endif

LIBPSDRPATH := $(shell pwd)

# Cross compiling configuration
CC              = $(CROSS_COMPILE)gcc
AR              = $(CROSS_COMPILE)ar

TARGET_LIB := libpsdr.a
TARGET_MK := generated/$(patsubst %.a,%.mk,$(TARGET_LIB))

export LIBPSDRPATH
export CONFIG
export ARCH

# Targets begin

all: dirs tools lib examples

tools:
	make --no-print-directory -C tools

examples: libmk lib tools
	make --no-print-directory -C examples

rebuild: clean all

lib: $(TARGET_LIB)

libmk: dirs
	@echo "# Created on $(shell date --rfc-3339=seconds)" > $(TARGET_MK)
	@echo INTP=$(INTP) >> $(TARGET_MK)
	@echo UINTP=$(UINTP) >> $(TARGET_MK)
	@echo CFLAGS=$(CFLAGS) >> $(TARGET_MK)
	@echo LFLAGS=$(LFLAGS) >> $(TARGET_MK)
	@echo INCFLAGS=$(INCFLAGS) >> $(TARGET_MK)
	@echo ARCH=$(ARCH) >> $(TARGET_MK)
	@echo CROSS_COMPILE=$(CROSS_COMPILE) >> $(TARGET_MK)

dirs:
	mkdir -p generated

# Find out obj list by looking for any .c file under src/ and arch/$(ARCH)
OBJS += $(patsubst %.c,%.o,$(wildcard src/*.c)) \
        $(patsubst %.c,%.o,$(wildcard arch/$(ARCH)/*.c))

CFLAGS += -DINTP="$(INTP)" -DUINTP="$(UINTP)"

DEPS := $(OBJS:.o=.d)

-include $(DEPS)
include arch/$(ARCH)/arch-$(ARCH).mk

INCFLAGS += -Iarch/$(ARCH)/include -Iinclude -Igenerated

# The list of files to be cleaned is:
# 1) any .o and in obj or in any arch; 2) each .d corresponding to its .o;
# 3) everything in generated/ directory
CLEAN_LIST += $(TARGET_LIB) $(OBJS) $(patsubst %.o,%.d,$(OBJS)) \
    $(wildcard $(patsubst %,%/*.o,$(wildcard arch/*))) \
    $(wildcard $(patsubst %,%/*.d,$(wildcard arch/*))) \
    $(wildcard generated/*) \

$(TARGET_LIB): $(OBJS)
	$(AR) cr $(TARGET_LIB) $(OBJS)

%.o: %.c
	$(CC) -c $(CFLAGS) $(INCFLAGS) -MMD -o $@ $<

clean:
	rm -f $(CLEAN_LIST)
	make --no-print-directory -C tools clean
	make --no-print-directory -C examples clean

.PHONY: lib libmk clean tools examples
