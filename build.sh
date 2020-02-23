#! /bin/sh -e
g++ -std=c++14 -O3 -g -Wall -Wextra -pedantic -Iinclude/ -fPIC -c -o build/endian.o src/endian.cc
g++ -std=c++14 -O3 -g -Wall -Wextra -pedantic -Iinclude/ -fPIC -c -o build/image.o src/image.cc
g++ -std=c++14 -O3 -g -Wall -Wextra -pedantic -Iinclude/ -fPIC -c -o build/image_io.o src/image_io.cc
g++ -std=c++14 -O3 -g -Wall -Wextra -pedantic -Iinclude/ -fPIC -c -o build/pnm_header_parser.o src/pnm_header_parser.cc
g++ -std=c++14 -O3 -g -Wall -Wextra -pedantic -Iinclude/ -fPIC -c -o build/pnm_types.o src/pnm_types.cc
g++ -std=c++14 -O3 -g -Wall -Wextra -pedantic -Iinclude/ -c -o build/test/test_runner.o src/test/test_runner.cc -lgtest
g++ -std=c++14 -O3 -g -Wall -Wextra -pedantic -Iinclude/ -shared -o lib/libargon.so build/endian.o build/image.o build/image_io.o build/pnm_header_parser.o build/pnm_types.o
ar rvs lib/libargon.a build/endian.o build/image.o build/image_io.o build/pnm_header_parser.o build/pnm_types.o
g++ -std=c++14 -O3 -g -Wall -Wextra -pedantic -Iinclude/ -o test_suite build/test/test_runner.o lib/libargon.so -lgtest -largon -Llib
