# Getting Started with Apache Arrow and Parquet in Chapel

This document will detail how I was able to call Apache Arrow from Chapel and some issues that I ran into with tools like `c2chapel`.

## Overview

We use Apache Arrow's C (GLib) interface in tandem with Chapel's C interoperability features to call from Chapel to Arrow.

## Installing Arrow

You must install the C++ version of Arrow first before installing the C version. The commands to do that on MacOS using Homebrew are as follows:

```bash
brew install apache-arrow
brew install apache-arrow-glib
```

For information on how to do that on other platforms see [Arrow's Installation Guide](http://arrow.apache.org/install/).

## Using the C version

Once you have the C++ version going, using Arrow in a C project is just a couple of includes and a libraries away.

See `cbuild.c` which builds a simple array and prints it out.
Check out the includes used.
You can see how it is made and linked in the Makefile that accompanies it.

## Using it in Chapel

Doing the same as `cbuild.c` in Chapel.
First I declared all the functions and types using the `extern` keyword in `ArrayDecl.chpl`
The `build.chpl` does the actual work.
You must use the following command to compile it:

```bash
  chpl  build.chpl -o build  -I /usr/local/include/glib-2.0 -I /usr/local/lib/glib-2.0/include -lparquet-glib -larrow-glib -lglib-2.0 -lgobject-2.0
```

You can also simple call `make` to build both `cbuild.c` and `build.chpl`.

If both `cbuild.c` and `build.chpl` have the same output, this verifies that you have Apache Arrow working.

### NOTE

This file should be accompanied by the following files in order of mentioning in the document:

- cbuild.c
- ArrayDecl.chpl
- build.chpl
- null.c
- null.h
