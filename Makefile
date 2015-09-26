default: all
all:
	@tup upd

install: 
	@cp lib/libargon.so /usr/local/lib
	@cp -r include/ar /usr/local/include/

test: all
	@./test_suite
