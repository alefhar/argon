# libargon
A library for reading/writing PNM (PBM, PGM, PPM) and PFM images (and hopefully
more...)

## Building libargon
This project relies on `tup` for builds, you can get `tup` either through your
distributions package manager or directly from GitHub by cloning the
repository [gittup/tup](https://github.com/gittup/tup).

To build the tests, additionaly GoogleTest is required. Usually, your
distribution should include this package. Alternatively, you can get
the latest version from [google/googletest](https://github.com/google/googletest).

To build the project type

    $ tup init
    $ tup

into your terminal. `tup` then initializes its database for managing builds
and then builds the library. The library is build to the directory `lib`. To
install, simply copy the library and all headers to a location of your choice.
You can also use the `Makefile` to install everything. The install path for
the headers is `/usr/local/include`, the library is copied to `/usr/local/lib`.

### Requirements
* tup for building
* make for installing (optional)
* a C++14 compliant compiler like GCC 4.9 or clang 3.5

## Using libargon
If you installed the headers and library to their default directories you include
the headers via

    #include <ar/argon.h>

To link against the library simply add it to your compile command, e.g.:

    $ g++ -std=c++14 -o main main.cc -largon

### Notes
When `libargon` is installed to `/usr/local/lib` the linker and the loader
might not find the library. There are ways to change that. When compiling
and linking, change the compile command to

    $ g++ -std=c++14 -o main main.cc -largon -L/usr/local/lib

When executing `./main` you have two possibilities: Change the library
path to contain the installation directory and execute your `./main`:

    $ export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib
    $ ./main

Alternatively, you can set the library path only for a single invocation:

    $ LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib ./main

A third possibility is to configure the linker and loader to contain your
installation directory. Execute with administrator privileges:

    $ echo /usr/local/lib > /etc/ld.so.conf.d/99local.conf
    $ ldconfig

This creates a configuration file telling linker and loader where to find
additional libraries, in this case in `/usr/local/lib`. To take effect,
linker and loader have to be configured.
Now it is possible to just call

    $ ./main
