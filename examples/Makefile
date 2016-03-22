#
# Author: Aurelio Colosimo, 2016
# Released under MIT-Expat License.
#

EXAMPLES := $(wildcard */)
EXAMPLES-CLEAN := $(patsubst %,clean-%,$(EXAMPLES))

all: $(EXAMPLES)

clean: $(EXAMPLES-CLEAN)

$(EXAMPLES):
	make --no-print-directory -C $@

$(EXAMPLES-CLEAN):
	make --no-print-directory -C $(patsubst clean-%,%,$@) clean

.PHONY: all $(EXAMPLES) $(EXAMPLES-CLEAN)
