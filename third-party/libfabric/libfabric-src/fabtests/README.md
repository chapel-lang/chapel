# fabtests

Fabtests provides a set of examples that uses
[libfabric](http://libfabric.org) -- a high-performance fabric
software library.

## Notes

Note that the fabtests suite is released paired with a specific
version of libfabric.  For example, libfabric v1.4 and fabtests v1.4
were released together.

Using these paired versions is the best way to test a given version of
libfabric.  Using version-mismatched libfabric/fabtests pairs may
produce unexpected results.

## Building fabtests

Distribution tarballs are available from the Github
[releases](https://github.com/ofiwg/fabtests/releases) tab.

If you are building Fabtests from a developer Git clone, you must
first run the `autogen.sh` script. This will invoke the GNU Autotools
to bootstrap Fabtests' configuration and build mechanisms. If you are
building Fabtests from an official distribution tarball, there is no
need to run `autogen.sh`; Fabtests distribution tarballs are already
bootstrapped for you.

Fabtests relies on being able to find an installed version of
Libfabric. In some cases, Libfabric may be in default compiler /
linker search paths, and you don't need to tell Fabtests where to find
it. In other cases, you may need to tell Fabtests where to find the
installed Libfabric's header and library files using the
`--with-libfabric=<directory>` option, described below.

### Configure options

The `configure` script has many built in options (see `./configure
--help`). Some useful options are:

```
--prefix=<directory>
```

By default `make install` will place the files in the `/usr` tree.
The `--prefix` option specifies that the Fabtests files should be
installed into the tree specified by named `<directory>`. The
executables will be located at `<directory>/bin`.

```
--with-libfabric=<directory>
```

Specify the directory where the Libfabric library and header files are
located.  This is necessary if Libfabric was installed in a location
where the compiler and linker will not search by default.  The
Libfabric library will be searched for in `<directory>/lib`, and
headers will be searched for in `<directory>/include`.

```
--with-valgrind=<directory>
```

Directory where valgrind is installed.  If valgrind is found, then
valgrind annotations are enabled. This may incur a performance
penalty.

### Examples

Consider the following example:

```
$ ./configure --with-libfabric=/opt/libfabric --prefix=/opt/fabtests && make -j 32 && sudo make install
```

This will tell the Fabtests to look for Libfabric libraries in the
`/opt/libfabric` tree, and to install the Fabtests in the
`/opt/fabtests` tree.

Alternatively:

```
$ ./configure --prefix=/opt/fabtests && make -j 32 && sudo make install
```

Tells the Fabtests that it should be able to find the Libfabric header
files and libraries in default compiler / linker search paths
(configure will abort if it is not able to find them), and to install
Fabtests in `/opt/fabtests`.
