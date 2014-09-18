=========================================================
This README supplements the Chapel Language Specification
with details on the Cray Chapel implementation of I/O.
=========================================================


Predefined Constants of the Type 'syserr'
-----------------------------------------

  The following constants indicating error conditions are provided:

    EEOF       unexpected end of file
    ESHORT     short read or write
    EFORMAT    bad format
    EILSEQ     illegal multibyte sequence
    EOVERFLOW  overflow

  In addition, the constants defined for 'errno' in C are also available.


Ensuring Successful I/O
-----------------------

  It is possible - in some situations - for I/O to fail without
  returning an error. In cases where a programmer wants to be sure
  that there was no error writing the data to disk, it is important
  to call file.fsync() to make sure that data has arrived on disk without
  an error. Many errors can be reported with typical operation, but
  some errors can only be reported by the system during file.close
  or even file.fsync.

  When a file (or channel) is closed, data written to that file will
  be written to disk eventually by the operating system. If an
  application needs to be sure that the data is written to persistent
  storage, it should use fsync prior to closing the file.


Miscellanea
-----------

  It is not possible to seek, read, or write to a file directly.
  Create a channel to proceed.

  channel.flush() in Chapel has the same meaning as fflush() in C.
  However, fflush() is not necessarily called in channel.flush().
  Unlike fsync(), which is actually called in file.fsync() in Chapel.

  The iomode constants in Chapel have the same meaning as the following
  strings passed to fopen() in C:
    iomode.r   "r"
    iomode.rw  "r+"
    iomode.cw  "w"
    iomode.cwr "w+"
  However, open() in Chapel does not necessarily invoke fopen().

  A channel's I/O style may be retrieved using the following method:
    proc channel._style():iostyle;


Using Channels
--------------

  Channels contain read and write methods, which are generic
  methods that can read or write anything, and can also take
  optional arguments (style, error)

  In addition, channels have read() and write() methods that take any
  number of arguments, and will halt if there was an error.

  Sometimes it's important to flush the buffer in a channel -
  to do that, use the .flush() method. Flushing the buffer will
  make all writes available to other applications or other views
  of the file (ie, it will call e.g. the OS call pwrite).

  It is also possible to close a channel, which will flush
  it and release any buffer memory used by the channel.

  Note that if you need to ensure that data from a channel is on disk,
  you'll have to call channel.flush() or channel.close() and then
  fsync on the related file.


The 'iostyle' Type
------------------

  'iostyle' is a record type. In Cray's Chapel Implementation,
  its fields and their default values are as follows:

  Perform binary I/O: 1 - yes, 0 - no.
    var binary:uint(8) = 0;

  --- binary I/O ---

  Byte order:
    var byteorder:uint(8) = iokind.native:uint(8);

  String format:
     -1    1 byte of string length, followed by the string
     -2    2 bytes  "  "
     -4    4  "     "  "
     -8    8  "     "  "
     -10   variable length (in bytes) before the string
           length encoding: little endian; hi-bit 1 means continue to next byte
     -0x01XX  - the string is terminated by the byte XX
     +N (nonzero positive number)  - read exactly this length.
    var str_style:int(64) = -10;

  --- text I/O ---

  General formatting:
    var min_width:uint(32) = 1;
    var max_width:uint(32) = max(uint(32));
    var string_start:style_char_t = 0x22;  // double quote
    var string_end:style_char_t = 0x22;    // double quote

  String format:
      QIO_STRING_FORMAT_WORD  string is as-is; reading reads until whitespace.
      QIO_STRING_FORMAT_BASIC only escape string_end and \ with \
      QIO_STRING_FORMAT_CHPL  escape string_end \ ' " \n with \
                              and nonprinting characters c = 0xXY with \xXY
      QIO_STRING_FORMAT_JSON  escape string_end " and \ with \,
                              and nonprinting characters c = \uABCD
      QIO_STRING_FORMAT_TOEND string is as-is; reading reads until string_end
    var string_format:uint(8) = 0;

  Numeric formatting:
    var base:uint(8) = 0;
    var point_char:style_char_t = 0x2e;          // .
    var exponent_char:style_char_t = 0x65;       // e
    var other_exponent_char:style_char_t = 0x70; // p
    var positive_char:style_char_t = 0x2b;       // +
    var negative_char:style_char_t = 0x2d;       // -
    var prefix_base:uint(8) = 1;
  //primarily for printing
    var pad_char:style_char_t = 0x20; // ' '
    var showplus:uint(8) = 0;
    var uppercase:uint(8) = 0;
    var leftjustify:uint(8) = 0;
    var showpoint:uint(8) = 0;
    var showpointzero:uint(8) = 1;
    var precision:int(32) = -1;
    var significant_digits:int(32) = -1;

  Formatting of real numbers:
      0  print out 'significant_digits' number of significant digits (%g in printf)
      1  print out 'precision' number of digits after the decimal point (%f)
      2  always use exponential and 'precision' number of digits (%e)
    var realfmt:uint(8) = 0;

  Formatting of complex numbers:
    var complex_style:uint(8) = 0;


  The default value of the 'iostyle' type is undefined.
  However, the compiler-generated constructor is available.
  It can be used to generate the default I/O style,
  with or without modifications.

  Example: specifying the minimum width for writing numbers
  so array elements are aligned:

    stdout.writeln(MyArray, new iostyle(min_width=10));

  Useful functions:

  generate the default I/O style, same as 'new iostyle()':
    proc defaultIOStyle():iostyle;

  retrieve the channel's I/O style:
    proc channel._style():iostyle;

  'iostyle' is work in progress: the fields and/or their types
  may change. Among other changes, we will be replacing the types
  of some multiple-choice fields from integral to enums.


Locking
-------

  Channels (and files) contain locks in order to keep their operation
  thread-safe. When creating a channel, it is possible to disable the
  lock (for performance reasons) by passing locking=false to e.g.
  file.writer(). Some channel methods - in particular those beginning
  with the underscore - should only be called on locked channels.
  With these methods, it is possible to get or set the channel style,
  or perform I/O "transactions". To use these methods, first lock the
  channel with channel.lock(), call the methods you need, and then
  unlock the channel with channel.unlock(). Note that in the future,
  we may move to alternative ways of calling these functions that
  guarantee that they are not called on a channel without the appropriate
  locking.


Bytes Type
----------

  A bytes object is just some data in memory along with a size. Bytes
  objects are reference counted, and the memory will be freed when
  nothing refers to the bytes object any more.


Buffers
-------

  A buffer stores some number subsections of bytes objects. It is efficient
  to go to a particular offset in a buffer, and to push or pop bytes
  objects from the beginning or end of a buffer.

  Buffers are used internally in each channel.


