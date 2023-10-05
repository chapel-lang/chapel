.. default-domain:: cpp

.. _Chapter-dyno-file-format:

File Format for '.dyno' Files
=============================

What is a .dyno file?
---------------------

'.dyno' files are experimental work that is building towards separate
compilation. These files have some similarities to a C compiler's
precompiled header files, but we are working towards making them have
some of the capabilities of a C compiler's .o (object file). 

The .dyno file format is a binary format.

Endianness
----------

At present, '.dyno' files are assumed to be in native endianness.
However, the file format makes it possible to detect reversed endianness,
so this should be possible to address in the future if needed.

Header
------

The '.dyno' file format header consists of:

 * 8 bytes of magic number 0x4C5048434550487F
   ('<7F>HPECHPL' or the reverse of it, dependeng on endianness)
 * 4 bytes of library major version number
 * 4 bytes of library minor version number
 * 4 bytes of chpl major version number
 * 4 bytes of chpl minor version number
 * 4 bytes of chpl update version number
