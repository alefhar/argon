PREFIX = /usr/local

SRC_DIR    = src
HEADER_DIR = include/ar
LIB_DIR    = lib

LIB_INSTALL_DIR    = $(PREFIX)/lib
HEADER_INSTALL_DIR = $(PREFIX)/$(HEADER_DIR)

HEADER_FILES = $(patsubst $(HEADER_DIR)/%, $(HEADER_INSTALL_DIR)/%, $(wildcard $(HEADER_DIR)/*.h))

default: all
all:
	@tup

install: 
	@cp -v $(LIB_DIR)/libargon.so $(LIB_INSTALL_DIR)
	@if [ ! -d "$(HEADER_INSTALL_DIR)" ]; then mkdir -p $(HEADER_INSTALL_DIR); fi
	@cp -v $(HEADER_DIR)/*.h $(HEADER_INSTALL_DIR)

uninstall:
	@rm -v $(LIB_INSTALL_DIR)/libargon.so
	@rm -v $(HEADER_FILES)
	@rmdir -v $(HEADER_INSTALL_DIR)

test: all
	@LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${PWD}/lib ./test_suite
