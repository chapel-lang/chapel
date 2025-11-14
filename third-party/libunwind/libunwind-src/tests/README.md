Testing libunwind
=================

Testing in the build tree
-------------------------

Testing in a self-hosted build environment is as simple as `make check`


Testing staged builds
---------------------

```
$ STAGINGDIR=/tmp/gh-705
$ make install DESTDIR=$STAGINGDIR
$ LIBUNWIND=$STAGINGDIR/usr/lib/libunwind.so LIBUNWIND_GENERIC=$STAGINGDIR/usr/lib/libunwind-x86_64.so LD_LIBRARY_PATH=$STAGINGDIR/usr/lib $STAGINGDIR/usr/libexec/libunwind/test-runner
```

Testing installed builds
------------------------

```
$ /usr/libexec/libunwind/test-runner
```
