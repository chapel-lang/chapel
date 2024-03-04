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

There is an expectation that a different file extension will be chosen
once further work is done on the file format.

Endianness
----------

At present, '.dyno' files are assumed to be in native endianness.
However, the file format makes it possible to detect reversed endianness,
so this should be possible to address in the future if needed.

Overall Format
--------------

Overall, the file format consists of a header followed by a number of
contiguous modules sections. In this way, the file can store information
for different modules. The file format is designed so that libraries for
individual modules can be concatenated together (if the file header and
file module table is adjusted appropriately). To support that, offsets
are generally relative within the section referred to. Each module
section contains its own sections that contain details of the module.

Each section in the file is padded to start at an 8 byte alignment.

Building Block: Variable Byte Encoding
--------------------------------------

When storing a length (such as a string length) where short lengths are
common, the file format uses a variable byte encoding. This encoding
stores a variable number of bytes to represent a number and uses fewer
bytes for smaller numbers. Portions of the number are stored in a
variable number of component bytes. Each component byte has part of the
number stored in the bottom 7 bits and uses the top bit to indicate if
there are more components. The components store portions of the original
number in a little-endian way (i.e., starting with the least-significant
group of 7 bits).

For example, the variable byte encoding of 0b110110 is just 0b00110110
because it fits in 7 bits.

In contrast, to find the variable byte encoding of 0b1010111011:

  - first, divide the number into 7-bit groups: 0b101 0b0111011

  - next, store the groups in little-endian order, and set the high
    bit on each other than the last: 0b10111011 0b00000101

Signed integers can be stored in a variable-length byte encoding as well,
using the "ZigZag" encoding (following Google Protocol Buffers). In the
ZigZag encoding, the number is transformed to an unsigned number before
encoding. A positive number *i* will be encoded as *2\*i*. A negative
number will be encoded as *2\*|i|-1*.

Building Block: Storing a String
--------------------------------

Strings can be stored in 2 ways, depending on if a long-strings
table is in use.

When not using a long-strings table, a string is stored as:

 * the string length, using variable-byte encoding
 * string data

These strings are not null-terminated.

When using a long-strings table, a string is stored in two ways which
depend on the first byte:

 * if the first byte does not have the high bit set

   * the first byte stores the length
     (only possible for strings < 128 bytes)
   * followed by that number of bytes of string data

 * if the first byte does have the high bit set

   * there are 3 more bytes that, combined with the bottom 7 bits of the
     first byte, form an index into the long strings table.


File Header
-----------

The '.dyno' file format header consists of:

 * 8 bytes of magic number 0x4c50484342494c7f
   ('<7F>LIBCHPL' or the reverse of it, depending on endianness)
 * 4 bytes of library file format major version number
 * 4 bytes of library file format minor version number
 * 4 bytes of chpl major version number
 * 4 bytes of chpl minor version number
 * 4 bytes of chpl update version number
 * 4 bytes of N, the number of module sections contained herein
 * 256 bits / 32 bytes of SHA-256 hash of the '.dyno' file contents
   * the hash is computed assuming that the hash is 0
 * a module section table, consisting of N entries, each consists of:
   * 8 bytes of module section offset (pointing to a module section header)
 * 8 bytes storing the offset just after the last module

Module Section Header
---------------------

The module section header uses relative section offsets. These relative
offsets store offsets relative to the module section header. The 'end'
offsets are the offset just after the section; so the length of a section
is endOffset - startOffset.

Each module section begins with a header that consists of:

 * 8 bytes of magic number 0x4d4dd01e5ec14d4d
 * 8 bytes of reserved space for future flags
 * 8 bytes of symbol table section start offset
 * 8 bytes of symbol table section end offset
 * 8 bytes of uAST section start offset
 * 8 bytes of uAST section end offset
 * 8 bytes of long strings table start offset
 * 8 bytes of long strings table end offset
 * 8 bytes of location section start offset
 * 8 bytes of location section end offset
 * 8 bytes of generated LLVM IR section start offset
 * 8 bytes of generated LLVM IR section end offset
 * a string storing the module symbol ID
   (e.g. "TopLevelModule" or "MyModule.SubModule")
   (note that this string does not use the long strings table)
 * a string storing the file path that generated the module
   (note that this string does not use the long strings table)

Symbol Table Section
--------------------

The symbol table section has an entry for each public, top-level symbol
contained in the module, as well as the module itself. These are sorted
by symbol table ID.

For a symbol, the symbol table ID consists of:

 * "" for the module itself
 * the name of the symbol, for anything top-level to a module
 * the regular ID minus the module name, for anything else

    - Normally, symbol IDs are stored as e.g.  'MyModule.MyClass.myMethod',
      but since this entire section of the file contains things within
      'MyModule', that part is omitted; so the symbol table ID for the example
      would just be 'MyClass.myMethod'.

This section consists of:

 * 8 bytes of magic number 0x4d59531e5ec110e0

 * 4 bytes of N, the number of entries

 * 4 bytes reserved for future use

 * entries sorted by symbol table ID.  For each entry, it stores:

   * 4 byte relative offset from the uAST section start,
     pointing to the serialized uAST for this symbol

   * 4 byte relative offset from the location section start,
     pointing to the location group for this symbol

   * a byte storing flags / kind information

   * the symbol table ID, stored in a compressed form. It is formed by
     concatenating the first A bytes of the previous symbol table ID with
     the B bytes of suffix:

     * unsigned variable-byte encoded, prefix A to copy from the
       previous symbol table ID

     * unsigned variable-byte encoded, suffix size B stored here

     * B bytes of suffix

  * unsigned variable-byte encoded number, G, of code-generated versions

  * for each of the G code-generated versions

    * byte indicating 0 if it is concrete and nonzero for an
      instantiation

    * additional information TBD for instantiations

    * the name of the symbol in the generated code, also called a "cname",
      stored in a compressed form. It is formed by concatenating the first
      A bytes of the previous cname with the B bytes of suffix:

       * unsigned variable-byte encoded, prefix A to copy from the
         previous symbol's cname

       * unsigned variable-byte encoded, suffix size B stored here

       * B bytes of suffix


uAST Section
------------

The uAST section consists of serialized uAST entries in the order they
appear within the file. The top-level entry will be the uAST for the
corresponding module. Note that the symbol table can point to an
individual entry within the serialized uAST.

IDs are not stored here. They are recomputed when the uAST is read.

The uAST section consists of:

 * 8 bytes of magic number 0x5453411e5ec110e0

 * 8 bytes: the total number of uAST entries

 * the contained entries, where each entry consists of:

   * 1 byte, tag indicating which uAST element it is (e.g. Variable or Forall)
   * attribute group child num (variable-byte encoded)
   * L bytes storing the additional information for the uAST element
     (where L can be determined from the tag)
   * variable-byte encoded number of children
   * the children, where each is stored as a uAST entry as described here

Long Strings Table Section
--------------------------

Long strings might occur many times, and so space can be saved by
replacing them with an integer. Since there are a lot of short strings,
and the long string table adds indirection, this only applies to long
strings. The threshold length is not set by the file format, but it
must be less than 128.

.. comment

   Here are two breadcrumbs that might aid decision making here:

   * this data structure space overhead is 8 bytes per long string + 4
     bytes per string use

     - supposing 1 use of a given string:
        16 byte strings -> all in line gives 17*1=17 bytes
                           with table gives 16+8+4*1=28 bytes
        20 byte strings -> all in line gives 21*1=21 bytes
                           with table gives 20+8+4*1=32 bytes
     - supposing 4 uses of a given string:
        16 byte strings -> all in line gives 17*4=68 bytes
                           with table gives 16+8+4*4=40 bytes
        20 byte strings -> all in line gives 21*4=84 bytes
                           with table gives 20+8+4*4=44 bytes

   * early experiments with sample Chapel source code indicate that 50% or
     so of strings are less than 18 bytes.


The long strings table section does not store strings in any particular
order.

The long strings table consists of the following:

 * 4 bytes magic number 0x52545301

 * 4 bytes N counting the number of long strings, including two unused ones:

     * offset 0 is unused
     * the last offset is also unused
     * so, valid long string indices are in 1 <= i < N

 * relative offsets of each string, from the start of the long strings section

   * each offset is 4 bytes

 * string data


Location Section
----------------

The Location section represents source locations of uAST elements.
It is stored in the same order as the uAST elements, but it is stored
separately because it is only relevant for error messages.

The locations are stored in location groups. Each location group can be
read independently, but some basic compression is used within each group,
so that the whole group might need to be read in order to compute a
location.

The Location section consists of:

 * 8 bytes of magic number 0x434F4C075ec110e0

 * 4 bytes, the number of file paths used here

 * 4 bytes storing the number of location groups. There will be
   a location group for the module under consideration and then
   for each uAST referred to by the symbol table. However, these
   are in uAST order rather than symbol table order.

 * for each file path

   * a string storing the file path
   * 256 bits / 32 bytes of SHA-256 hash of the input file that was parsed

 * information for each location group, consisting of the following:

   * unsigned variable-byte encoded index into the file paths stored in the
     location section to indicate which file this location group came from

   * signed variable-byte encoded starting line number

   * a number of location entries, each consisting of:

     * the first line, stored as a signed variable-byte encoded
       difference from the previous entry's last line, or a difference
       from the starting line number if this is the first entry - i.e.
       the first line is prevEntry.lastLine + decodedDifference.

     * the last line, stored as a signed variable-byte encoded difference
       from this entry's first line

     * unsigned variable-byte encoded first column

     * unsigned variable-byte encoded first last column

     * variable-byte encoded M, number of additional locations associated
       with this uAST item

     * for each of the M additional locations:

       * unsigned variable-byte encoded additional location tag

       * first line, stored as a signed variable-byte encoded difference
         from the containing entry's first line

       * last line, stored as a signed variable-byte encoded difference
         from the additional location's first line

       * unsigned variable-byte encoded first column

       * unsigned variable-byte encoded first last column


Generated Code Section
----------------------

The generated code contains serialized LLVM IR for the result of
compilation for the module (with the exception of generic functions that
are not yet instantiated).

The generated code section consists of:

 * 8 bytes of magic number 0x4e4547075ec110e0

 * 8 bytes, M, the size of the serialized LLVM IR bytecode

 * M bytes of serialized LLVM IR bytecode


Types Section
-------------

TODO

Functions Section
-----------------

TODO

Module Dependencies Section
---------------------------

TODO
