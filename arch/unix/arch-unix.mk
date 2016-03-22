#
# Author: Aurelio Colosimo, 2016
# Released under MIT-Expat License.
#

LFLAGS += $(shell pkg-config --libs libusb-1.0)
CFLAGS += $(shell pkg-config --cflags libusb-1.0)
