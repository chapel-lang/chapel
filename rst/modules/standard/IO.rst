.. default-domain:: chpl

.. module:: IO
   :synopsis: Support for a variety of kinds of input and output.

IO
==
**Usage**

.. code-block:: chapel

   use IO;

**Submodules**

.. toctree::
   :maxdepth: 1
   :glob:

   IO/*




Support for a variety of kinds of input and output.

.. note:: All Chapel programs automatically include :proc:`~ChapelIO.write`,
          :proc:`~ChapelIO.writeln` and :proc:`~ChapelIO.writef`.

Input/output (I/O) facilities in Chapel include the types :record:`file` and
:record:`channel`; the constants :record:`stdin`, :record:`stdout` and
:record:`stderr`; the functions :proc:`open`, :proc:`file.close`,
:proc:`file.reader`, :proc:`file.writer`, :proc:`channel.read`,
:proc:`channel.write`, and many others.

.. _about-io-overview:

I/O Overview
------------

A :record:`file` in Chapel identifies a file in the underlying operating
system.  Reads and writes to a file are done via one or more channels
associated with the file.  Each :record:`channel` uses a buffer to provide
sequential read or write access to its file, optionally starting at an offset.

For example, the following program opens a file and writes an integer to it:

.. code-block:: chapel

  // open the file "test-file.txt" for writing, creating it if
  // it does not exist yet.
  var myFile = open("test-file.txt", iomode.cw);

  // create a writing channel starting at file offset 0
  // (start and end offsets can be specified when creating the
  // channel)
  var myWritingChannel = myFile.writer();

  var x: int = 17;

  // This function will write the human-readable text version of x;
  // binary I/O is also possible.
  myWritingChannel.write(x);

  // Now test-file.txt contains:
  // 17

Then, the following program can be used to read the integer:

.. code-block:: chapel

  // open the file "test-file.txt" for reading only
  var myFile = open("test-file.txt", iomode.r);

  // create a reading channel starting at file offset 0
  // (start and end offsets can be specified when creating the
  // channel)
  var myReadingChannel = myFile.reader();

  var x: int;

  // Now read a textual integer. Note that the
  // channel.read function returns a bool to indicate
  // if it read something or if the end of the file
  // was reached before something could be read.
  var readSomething = myReadingChannel.read(x);

  writeln("Read integer ", x);
  // prints out:
  // 17

Design Rationale
----------------

Since channels operate independently, concurrent I/O to the same open file is
possible without contending for locks.  Furthermore, since the channel (and not
the file) stores the current file offset, it is straightforward to create
programs that access the same open file in parallel. Note that such parallel
access is not possible in C when multiple threads are using the same ``FILE*``
to write to different regions of a file because of the race condition between
``fseek`` and ``fwrite``. Because of these issues, Chapel programmers wishing
to perform I/O will need to know how to open files as well as create channels.


.. _about-io-styles:

I/O Styles
----------

Reading and writing of Chapel's basic types is regulated by an applicable
:record:`iostyle`.  In particular, the I/O style controls whether binary or
text I/O should be performed. For binary I/O it specifies, for example, byte
order and string encoding. For text I/O it specifies string representation; the
base, field width and precision for numeric types; and so on.  Each channel has
an associated I/O style.  It applies to all read/write operations on that
channel, except when the program specifies explicitly an I/O style for a
particular read or write.

See the definition for the :record:`iostyle` type. This type represents I/O
styles and provides details on formatting and other representation choices.

The default value of the :record:`iostyle` type is undefined.  However, the
compiler-generated constructor is available.  It can be used to generate the
default I/O style, with or without modifications. In addition, the function
:proc:`defaultIOStyle` will return the default I/O style just as ``new
iostyle()`` will.

The I/O style for an I/O operation can be provided through an optional
``style=`` argument in a variety of places:

 * when performing the I/O, e.g. in calls to :proc:`channel.write` or
   :proc:`channel.read`
 * when creating the channel with :proc:`file.reader` or :proc:`file.writer`
 * or when creating the file with e.g. :proc:`open`

Note that :proc:`file.reader`, or :proc:`file.writer` will copy the file's I/O
style if a ``style=`` argument is not provided. Also note that I/O functions on
channels will by default use the I/O style stored with that channel.

A channel's I/O style may be retrieved using :proc:`channel._style` and set
using :proc:`channel._set_style`. These functions should only be called while
the channel lock is held, however. See :ref:`about-io-channel-synchronization`
for more information on channel locks.

.. note::

  :record:`iostyle` is work in progress: the fields and/or their types may
  change. Among other changes, we expect to be replacing the types of some
  multiple-choice fields from integral to enums.

As an example for specifying an I/O style, the code below specifies the minimum width for writing numbers so array elements are aligned in the output:

.. code-block:: chapel

  stdout.writeln(MyArray, new iostyle(min_width=10));


I/O facilities in Chapel also include several other ways to control I/O
formatting. There is support for :ref:`formatted I/O <about-io-formatted-io>`
with :proc:`FormattedIO.channel.readf` and :proc:`FormattedIO.channel.writef`.
Also note that record or class implementations can provide custom functions
implementing read or write operations for that type (see
:ref:`readThis-writeThis-readWriteThis`).

.. _about-io-files:

Files
-----

There are several functions that open a file and return a :record:`file` including :proc:`open`, :proc:`opentmp`, :proc:`openmem`, :proc:`openfd`, and :proc:`openfp`.

Once a file is open, it is necessary to create associated channel(s) - see
:proc:`file.reader` and :proc:`file.writer` - to write to and/or read from the
file.

Use the :proc:`file.fsync` function to explicitly synchronize the file to
ensure that file data is committed to the file's underlying device for
persistence.

To release any resources associated with a file, it is necessary to first close
any channels using that file (with :proc:`channel.close`) and then the file
itself (with :proc:`file.close`).

.. _about-io-channel-creation:

Functions for Channel Creation
------------------------------

:proc:`file.writer` creates a channel for writing to a file, and
:proc:`file.reader` create a channel for reading from a file.

.. _about-io-channel-synchronization:

Synchronization of Channel Data and Avoiding Data Races
-------------------------------------------------------

Channels (and files) contain locks in order to keep their operation safe for
multiple tasks. When creating a channel, it is possible to disable the lock
(for performance reasons) by passing ``locking=false`` to e.g.  file.writer().
Some channel methods - in particular those beginning with the underscore -
should only be called on locked channels.  With these methods, it is possible
to get or set the channel style, or perform I/O "transactions" (see
:proc:`channel.mark` and :proc:`channel._mark`). To use these methods,
first lock the channel with :proc:`channel.lock`, call the methods you need,
then unlock the channel with :proc:`channel.unlock`.
Note that in the future, we may move to alternative ways of calling
these functions that guarantee that they are not called on a channel
without the appropriate locking.

Besides data races that can occur if locking is not used in channels when it
should be, it is also possible for there to be data races on file data that is
buffered simultaneously in multiple channels.  The main way to avoid such data
races is the :proc:`channel.flush` synchronization operation.
:proc:`channel.flush` will make all writes to the channel, if any, available to
concurrent viewers of its associated file, such as other channels or other
applications accessing this file concurrently. See the note below for
more details on the situation in which this kind of data race can occur.

.. note::

  Since channels can buffer data until :proc:`channel.flush` is called, it is
  possible to write programs that have undefined behaviour because of race
  conditions on channel buffers. In particular, the problem comes up for
  programs that make:

   * concurrent operations on multiple channels that operate on overlapping
     regions of a file
   * where at least one of the overlapping channels is a writing channel
   * and where data could be stored more than one of the overlapping channel's
     buffers at the same time (ie, write and read ordering are not enforced
     through :proc:`channel.flush` and other mean such as sync variables).

  Note that it is possible in some cases to create a :record:`file` that does
  not allow multiple channels at different offsets. Channels created on such
  files will not change the file's position based on a ``start=`` offset
  arguments. Instead, each read or write operation will use the file
  descriptor's current position. Therefore, only one channel should be created
  for files created in the following situations:

    * with :proc:`openfp`
    * with :proc:`openfd` when provided a non-seekable system file descriptor


Performing I/O with Channels
----------------------------

Channels contain read and write methods, which are generic methods that can
read or write anything, and can also take optional arguments such as I/O style
or. These functions generally take any number of arguments and `throw`
if there was an error. See:

 * :proc:`channel.write`
 * :proc:`channel.writeln`
 * :proc:`channel.writebits`
 * :proc:`FormattedIO.channel.writef` (see also :ref:`about-io-formatted-io`)
 * :proc:`channel.read`
 * :proc:`channel.readln`
 * :proc:`channel.readbits`
 * :proc:`FormattedIO.channel.readf` (see also :ref:`about-io-formatted-io`)

In addition, there are several convenient synonyms for :proc:`channel.write` and
:proc:`channel.read`:

 * :proc:`channel.readwrite`
 * the `<~> operator`_

Sometimes it's important to flush the buffer in a channel - to do that, use the
.flush() method. Flushing the buffer will make all writes available to other
applications or other views of the file (ie, it will call e.g. the OS call
pwrite).  It is also possible to close a channel, which will implicitly
flush it and release any buffer memory used by the channel.  Note that if you
need to ensure that data from a channel is on disk, you'll have to call
:proc:`channel.flush` or :proc:`channel.close` and then :proc:`file.fsync` on
the related file.

.. _about-io-closing-channels:

Functions for Closing Channels
------------------------------

A channel must be closed in order to free the resources allocated for it,
to ensure that data written to it is visible to other channels,
or to allow the associated file to be closed.

See :proc:`channel.close`.

It is an error to perform any I/O operations on a channel
that has been closed.
It is an error to close a file when it has channels that
have not been closed.

Files and channels are reference counted. Each file and channel is
closed automatically when no references to it remain. For example, if
a local variable is the only reference to a channel, the channel will
be closed when that variable goes out of scope.  Programs may also
close a file or channel explicitly.

.. _about-io-style:

The ``stdin``, ``stdout``, and ``stderr`` Channels
--------------------------------------------------

Chapel provides the predefined channels :var:`stdin`, :var:`stdout`, and
:var:`stderr` to access the corresponding operating system streams standard
input, standard output, and standard error.

:var:`stdin` supports reading;
:var:`stdout` and :var:`stderr` support writing.

All three channels are safe to use concurrently.
Their types' ``kind`` argument is ``dynamic``

.. _about-io-error-handling:

Error Handling
--------------

Most I/O routines throw a :class:`SysError.SystemError`, and can be handled
appropriately with ``try`` and ``catch``. For legacy reasons most I/O routines
can also can accept an optional `error=` argument.
In this documentation, `SystemError` may include both the
:class:`SysError.SystemError` class proper and its subclasses.

Some of these subclasses commonly used within the I/O implementation include:

 * :class:`SysError.EOFError` - the end of file was reached
 * :class:`SysError.UnexpectedEOFError` - a read or write only returned
                                          part of the requested data
 * :class:`SysError.BadFormatError` - data read did not adhere to the
                                      requested format

Some of the legacy error codes used include:

 * :const:`SysBasic.EILSEQ` - illegal multibyte sequence (e.g. there was a
   UTF-8 format error)
 * :const:`SysBasic.EOVERFLOW` - data read did not fit into requested type
   (e.g. reading 1000 into a `uint(8)`).

An error code can be converted to a string using the function
:proc:`~SysError.errorToString`.

.. _about-io-ensuring-successful-io:

Ensuring Successful I/O
-----------------------

It is possible - in some situations - for I/O to fail without returning an
error. In cases where a programmer wants to be sure that there was no error
writing the data to disk, it is important to call :proc:`file.fsync` to make
sure that data has arrived on disk without an error. Many errors can be
reported with typical operation, but some errors can only be reported by the
system during :proc:`file.close` or even :proc:`file.fsync`.

When a file (or channel) is closed, data written to that file will be written
to disk eventually by the operating system. If an application needs to be sure
that the data is immediately written to persistent storage, it should use
:proc:`file.fsync` prior to closing the file.

Correspondence to C I/O
-----------------------

It is not possible to seek, read, or write to a file directly.
Create a channel to proceed.

:proc:`channel.flush` in Chapel has the same conceptual meaning as fflush() in
C.  However, fflush() is not necessarily called in channel.flush().  Unlike
fsync(), which is actually called in file.fsync() in Chapel.

The iomode constants in Chapel have the same meaning as the following
strings passed to fopen() in C:

  * iomode.r   "r"
  * iomode.rw  "r+"
  * iomode.cw  "w"
  * iomode.cwr "w+"

However, open() in Chapel does not necessarily invoke fopen().

IO Functions and Types
----------------------

 

.. enum:: enum iomode { r = 1, cw = 2, rw = 3, cwr = 4 }

   
   
   The :type:`iomode` type is an enum.
   When used as arguments when opening files,
   its constants have the following meaning:
   
   * ``iomode.r`` - open an existing file for reading.
   * ``iomode.rw`` - open an existing file for reading and writing.
   * ``iomode.cw`` - create a new file for writing.
     If the file already exists, its contents are removed
     when the file is opened in this mode.
   * ``iomode.cwr`` - as with ``iomode.cw`` but reading from the
     file is also allowed.
   


.. enum:: enum iokind { dynamic = 0, native = 1, big = 2, little = 3 }

   
   
   The :type:`iokind` type is an enum. When used as arguments to the
   :record:`channel` type, its constants have the following meaning:
   
   * ``iokind.big`` means binary I/O with big-endian byte order is performed
     when writing/reading basic types from the channel.
   
   * ``iokind.little`` means binary I/O with little-endian byte order
     (similar to ``iokind.big`` but with little-endian byte order).
   
   * ``iokind.native`` means binary I/O in native byte order
     (similar to ``iokind.big`` but with the byte order that is native
     to the target platform).
   
   * ``iokind.dynamic`` means that the applicable I/O style has full effect
     and as a result the kind varies at runtime.
   
   In the case of ``iokind.big``, ``iokind.little``, and
   ``iokind.native`` the applicable :record:`iostyle` is consulted when
   writing/reading strings, but not for other basic types.
   
   There are synonyms available for these values:
   
   * :proc:`iodynamic` = ``iokind.dynamic``
   * :proc:`ionative` = ``iokind.native``
   * :proc:`iobig` = ``iokind.big``
   * :proc:`iolittle` = ``iokind.little``
   


.. data:: param iodynamic = iokind.dynamic

   A synonym for ``iokind.dynamic``; see :type:`iokind` 

.. data:: param ionative = iokind.native

   A synonym for ``iokind.native``; see :type:`iokind` 

.. data:: param iobig = iokind.big

   A synonym for ``iokind.big``; see :type:`iokind` 

.. data:: param iolittle = iokind.little

   A synonym for ``iokind.little``; see :type:`iokind` 

.. enum:: enum iostringstyle { len1b_data = -1, len2b_data = -2, len4b_data = -4, len8b_data = -8, lenVb_data = -10, data_toeof = -65280, data_null = -256 }

   
   
   This enum contains values used to control binary I/O with strings
   via the ``str_style`` field in :record:`iostyle`.
   
   * ``iostringstyle.len1b_data`` indicates a string format of 1 byte of
     length followed by length bytes of string data.
   * ``iostringstyle.len2b_data`` indicates a string format of 2 bytes of
     length followed by length bytes of string data.
   * ``iostringstyle.len4b_data`` indicates a string format of 4 bytes of
     length followed by length bytes of string data.
   * ``iostringstyle.len8b_data`` indicates a string format of 8 bytes of
     length followed by length bytes of string data.
   * ``iostringstyle.lenVb_data`` indicates a string format of a variable
     number of bytes of length, encoded with high-bit meaning more bytes
     of length follow, and where the 7-bits of length from each byte store
     the 7-bit portions of the length in order from least-significant to
     most-significant. This way of encoding a variable-byte length  matches
     `Google Protocol Buffers <https://github.com/protocolbuffers/protobuf>`_.
   * ``iostringstyle.data_toeof`` indicates a string format that contains
     only the string data without any length or terminator. When reading,
     this format will read a string until the end of the file is reached.
   * ``iostringstyle.data_null`` indicates a string that is terminated
     by a zero byte. It can be combined with other numeric
     values to indicate a string terminated by a particular byte. For example,
     to indicate a string terminated by ``$`` (which in ASCII has byte value 0x24),
     one would use the value ``iostringstyle.data_null|0x24``.
   * A positive and nonzero value indicates that a string of exactly that many
     bytes should be read or written.
   


.. enum:: enum iostringformat { word = 0, basic = 1, chpl = 2, json = 3, toend = 4, toeof = 5 }

   
   
   This enum contains values used to control text I/O with strings
   via the ``string_format`` field in :record:`iostyle`.
   
     * ``iostringformat.word`` means string is as-is;
       reading reads until whitespace. This is the default.
     * ``iostringformat.basic`` means only escape *string_end* and ``\``
       with ``\``
     * ``iostringformat.chpl`` means  escape *string_end*
       ``\`` ``'`` ``"`` ``\n`` with ``\`` and
       nonprinting characters ``c = 0xXY`` with ``\xXY``
     * ``iostringformat.json`` means  escape *string_end* ``"`` and ``\``
       with ``\``, and nonprinting characters ``c = \uABCD``
     * ``iostringformat.toend`` means string is as-is; reading reads until
       *string_end*
     * ``iostringformat.toeof`` means string is as-is; reading reads until
       end of file


.. function:: proc stringStyleTerminated(terminator: uint(8))

   
   
   This method returns the appropriate :record:`iostyle` ``str_style`` value
   to indicate a string format where strings are terminated by a
   particular byte.
   
   :arg terminator: a byte value that the strings will be terminated by
   :returns: a value that indicates a string format where strings
             are terminated by the terminator byte. This value is appropriate
             to store in :var:`iostyle.str_style`.
   

.. function:: proc stringStyleNullTerminated()

   
   This method returns the appropriate :record:`iostyle` ``str_style`` value
   to indicate a string format where strings are terminated by a
   zero byte.
   

.. function:: proc stringStyleWithLength(lengthBytes: int) throws

   
   Return the appropriate :record:`iostyle` ``str_style`` value
   to indicate a string format where string data is preceded by a
   `lengthBytes` of length. Only lengths of 1, 2, 4, or 8 are supported.
   When `lengthBytes` is 0, the returned value indicates variable-byte length.
   
   :throws SystemError: Thrown for an unsupported value of `lengthBytes`.
   

.. data:: const IOHINT_NONE = 0: c_int

   IOHINT_NONE means normal operation, nothing special
   to hint. Expect to use NONE most of the time.
   The other hints can be bitwise-ORed in.
   

.. data:: const IOHINT_RANDOM = QIO_HINT_RANDOM

   IOHINT_RANDOM means we expect random access to a file 

.. data:: const IOHINT_SEQUENTIAL = QIO_HINT_SEQUENTIAL

   IOHINT_SEQUENTIAL means expect sequential access. On
   Linux, this should double the readahead.
   

.. data:: const IOHINT_CACHED = QIO_HINT_CACHED

   IOHINT_CACHED means we expect the entire file
   to be cached and/or we pull it in all at
   once. May request readahead on the entire file.
   

.. data:: const IOHINT_PARALLEL = QIO_HINT_PARALLEL

   IOHINT_PARALLEL means that we expect to have many
   channels working with this file in parallel.
   It might change the reading/writing implementation
   to something more efficient in that scenario.
   

.. record:: iostyle

   
   
   The :record:`iostyle` type represents I/O styles
   defining how Chapel's basic types should be read or written.
   
   See :ref:`about-io-style`.
   


   .. attribute:: var binary: uint(8) = 0

      Perform binary I/O? 1 - yes, 0 - no.
      This field is ignored for :type:`iokind` values other than ``dynamic``.
      

   .. attribute:: var byteorder: uint(8) = iokind.native: uint(8)

      What byte order should we use when performing binary I/O?
      This field is ignored for :type:`iokind` values other than ``dynamic``.
      It should be set to a value in :type:`iokind`.
      

   .. attribute:: var str_style: int(64) = iostringstyle.data_toeof: int(64)

      
      What string format should we use when writing strings
      in binary mode? See :type:`iostringstyle` for more information
      on what the values of ``str_style`` mean.
      

   .. attribute:: var min_width_columns: uint(32) = 0

      When performing text I/O, pad out to this many columns 

   .. attribute:: var max_width_columns: uint(32) = max(uint(32))

      When performing text I/O, do not use more than this many columns 

   .. attribute:: var max_width_characters: uint(32) = max(uint(32))

      When performing text I/O, do not use more than this many characters 

   .. attribute:: var max_width_bytes: uint(32) = max(uint(32))

      When performing text I/O, do not use more than this many bytes 

   .. attribute:: var string_start: style_char_t = 34

      What character do we start strings with, when appropriate? Default is " 

   .. attribute:: var string_end: style_char_t = 34

      What character do we end strings with, when appropriate? Default is " 

   .. attribute:: var string_format: uint(8) = iostringformat.word: uint(8)

      How should we format strings when performing text I/O?
      See :type:`iostringstyle` for more information
      on what the values of ``str_style`` mean.
      

   .. attribute:: var bytes_prefix: style_char_t = 98

      What character do we start bytes with, when appropriate? Default is " 

   .. attribute:: var base: uint(8) = 0

      When reading or writing a numeric value in a text mode channel,
      what base should be used for the number? Default of 0 means decimal.
      Bases 2, 8, 10, 16 are supported for integers. Bases 10 and 16
      are supported for real values.

   .. attribute:: var point_char: style_char_t = 46

      When reading or writing a numeric value in a text mode channel,
      how is the integer portion separated from the fractional portion?
      Default is '.' 

   .. attribute:: var exponent_char: style_char_t = 101

      When reading or writing a numeric value in a text mode channel,
      how is the exponent written? Default is 'e' 

   .. attribute:: var other_exponent_char: style_char_t = 112

      When reading or writing a numeric value in a text mode channel,
      when base is > 10, how is the exponent written? Default is 'e' 

   .. attribute:: var positive_char: style_char_t = 43

      What character denotes a positive number? Default is '+' 

   .. attribute:: var negative_char: style_char_t = 45

      What character denotes a negative number? Default is '-' 

   .. attribute:: var i_char: style_char_t = 105

      What character follows an the imaginary number? Default is 'i' 

   .. attribute:: var prefix_base: uint(8) = 1

      When writing in a base other than 10, should the prefix be used?
      (e.g. hexadecimal numbers are prefixed with 0x) 

   .. attribute:: var pad_char: style_char_t = 32

      When padding with spaces, which pad character to use? Default is ' ' 

   .. attribute:: var showplus: uint(8) = 0

      When printing a positive numeric value, should the + be shown? 

   .. attribute:: var uppercase: uint(8) = 0

      When printing a numeric value in hexadecimal, should it be
      uppercase? 

   .. attribute:: var leftjustify: uint(8) = 0

      When printing a numeric value in a field of specified width, should
      the number be on the left (that is padded on the right?). The default
      is to right-justify the number. 

   .. attribute:: var showpoint: uint(8) = 0

      When printing an integral value using a real format, should a trailing
      decimal point be included? If so, the value 0 will be written as '0.' 

   .. attribute:: var showpointzero: uint(8) = 1

      When printing an integral value using a real format, should a trailing
      decimal point and zero be included? If so, the value 0 will be written
      as '0.0' 

   .. attribute:: var precision: int(32) = -1

      Specifies the precision for real format conversions. See the description
      of realfmt below. 

   .. attribute:: var realfmt: uint(8) = 0

      
      Formatting of real numbers:
      
        * 0 means  print out 'precision' number of significant digits
          (%g in printf)
        * 1 means  print out 'precision' number of digits after the decimal point
          (%f)
        * 2 means  always use exponential and 'precision' number of digits (%e)
      

   .. attribute:: var complex_style: uint(8) = 0

   .. attribute:: var array_style: uint(8) = 0

   .. attribute:: var aggregate_style: uint(8) = 0

   .. attribute:: var tuple_style: uint(8) = 0

.. function:: proc defaultIOStyle(): iostyle

   
   :returns: the default I/O style. See :record:`iostyle`
             and :ref:`about-io-styles`
   
   

.. method:: proc iostyle.native(str_style: int(64) = stringStyleWithVariableLength()): iostyle

   Get an I/O style indicating binary I/O in native byte order.
   
   :arg str_style: see :type:`iostringstyle` - which format to use when reading
                   or writing strings. Defaults to variable-byte length.
   :returns: the requested :record:`iostyle`
   

.. method:: proc iostyle.big(str_style: int(64) = stringStyleWithVariableLength()): iostyle

   Get an I/O style indicating binary I/O in big-endian byte order.
   
   :arg str_style: see :type:`iostringstyle` - which format to use when reading
                   or writing strings. Defaults to variable-byte length.
   :returns: the requested :record:`iostyle`
   

.. method:: proc iostyle.little(str_style: int(64) = stringStyleWithVariableLength()): iostyle

   Get an I/O style indicating binary I/O in little-endian byte order.
   
   :arg str_style: see :type:`iostringstyle` - which format to use when reading
                   or writing strings. Defaults to variable-byte length.
   :returns: the requested :record:`iostyle`
   

.. method:: proc iostyle.text(): iostyle

.. type:: type iohints = c_int

.. record:: file

   
   
   The :record:`file` type is implementation-defined.  A value of the
   :record:`file` type refers to the state that is used by the implementation to
   identify and interact with the OS file.
   
   When a :record:`file` formal argument has default intent, the
   actual is copied to the formal upon a function call and
   the formal cannot be assigned within the function.
   
   The default value of the :record:`file` type does not represent any OS file.
   It is illegal to perform any I/O operations on the default value.
   


   .. method:: proc init()

.. method:: proc file.init=(x: file)

.. method:: proc file.check() throws

   Throw an error if `this` is not a valid representation of an OS file.
   
   :throws SystemError: Indicates that `this` does not represent an OS file.

.. method:: proc file.close() throws

   Close a file.
   
   In order to free the resources allocated for a file, it
   must be closed using this method.
   
   Closing a file does not guarantee immediate persistence of the performed
   updates, if any. In cases where immediate persistence is important,
   :proc:`file.fsync` should be used for that purpose prior to closing the file.
   In particular, even though closing the file might complete without errors,
   the data written might not persist in the event of a severe error like
   running out of storage space or power loss. See also
   :ref:`about-io-ensuring-successful-io`.
   
   Files are automatically closed when the file variable
   goes out of scope and all channels using that file are closed. Programs
   may also explicitly close a file using this method.
   
   It is an error to perform any I/O operations on a file that has been closed.
   It is an error to close a file when it has channels that have not been closed.
   
   :throws SystemError: Thrown if the file could not be closed.
   

.. method:: proc file.fsync() throws

   
   
   Sync a file to disk.
   
   Commits file data to the device associated with this file.
   Data written to the file by a channel will be committed
   only if the channel has been closed or flushed.
   
   This function will typically call the ``fsync`` system call.
   
   :throws SystemError: Thrown if the file could not be synced.
    

.. method:: proc file.path: string throws

   
   
   Get the path to an open file.
   
   Note that not all files have a path (e.g. files opened with :proc:`openmem`),
   and that this function may not work on all operating systems.
   
   The function :proc:`Path.file.realPath` is an alternative way
   to get the path to a file.
   
   :throws SystemError: Thrown if the path could not be retrieved.
    

.. method:: proc file.tryGetPath(): string

   
   
   Get the path to an open file, or return "unknown" if there was
   a problem getting the path to the open file.
   

.. method:: proc file.size: int(64) throws

   
   
   Get the current size of an open file. Note that the size can always
   change if other channels, tasks or programs are writing to the file.
   
   :returns: the current file size
   
   :throws SystemError: Thrown if the size could not be retrieved.

.. method:: proc file.length(): int(64) throws

   
   Deprecated - please use :proc:`file.size`.

.. function:: proc open(path: string, mode: iomode, hints: iohints = IOHINT_NONE, style: iostyle = defaultIOStyle()): file throws

   
   
   Open a file on a filesystem. Note that once the
   file is open, you will need to use a :proc:`file.reader` or :proc:`file.writer`
   to create a channel to actually perform I/O operations
   
   :arg path: which file to open (for example, "some/file.txt").
   :arg mode: specify whether to open the file for reading or writing and
                whether or not to create the file if it doesn't exist.
                See :type:`iomode`.
   :arg hints: optional argument to specify any hints to the I/O system about
               this file. See :type:`iohints`.
   :arg style: optional argument to specify I/O style associated with this file.
               The provided style will be the default for any channels created for
               on this file, and that in turn will be the default for all I/O
               operations performed with those channels.
   :returns: an open file to the requested resource.
   
   :throws SystemError: Thrown if the file could not be opened.

.. function:: proc openfd(fd: fd_t, hints: iohints = IOHINT_NONE, style: iostyle = defaultIOStyle()): file throws

   
   
   Create a Chapel file that works with a system file descriptor  Note that once
   the file is open, you will need to use a :proc:`file.reader` or
   :proc:`file.writer` to create a channel to actually perform I/O operations
   
   The system file descriptor will be closed when the Chapel file is closed.
   
   .. note::
   
     This function can be used to create Chapel files that refer to system file
     descriptors that do not support the ``seek`` functionality. For example, file
     descriptors that represent pipes or open socket connections have this
     property. In that case, the resulting file value should only be used with one
     :record:`channel` at a time.
     The I/O system will ignore the channel offsets when reading or writing
     to files backed by non-seekable file descriptors.
   
   
   :arg fd: a system file descriptor (obtained with :proc:`Sys.sys_open` or
            :proc:`Sys.sys_connect` for example).
   :arg hints: optional argument to specify any hints to the I/O system about
               this file. See :type:`iohints`.
   :arg style: optional argument to specify I/O style associated with this file.
               The provided style will be the default for any channels created for
               on this file, and that in turn will be the default for all I/O
               operations performed with those channels.
   :returns: an open :record:`file` using the specified file descriptor.
   
   :throws SystemError: Thrown if the file descriptor could not be retrieved.
   
    

.. function:: proc openfp(fp: _file, hints: iohints = IOHINT_NONE, style: iostyle = defaultIOStyle()): file throws

   
   
   Create a Chapel file that works with an open C file (ie a ``FILE*``).  Note
   that once the file is open, you will need to use a :proc:`file.reader` or
   :proc:`file.writer` to create a channel to actually perform I/O operations
   
   .. note::
   
     The resulting file value should only be used with one :record:`channel` at a
     time. The I/O system will ignore the channel offsets when reading or writing
     to a file opened with :proc:`openfp`.
   
   
   :arg fp: a C ``FILE*`` to work with
   :arg hints: optional argument to specify any hints to the I/O system about
               this file. See :type:`iohints`.
   :arg style: optional argument to specify I/O style associated with this file.
               The provided style will be the default for any channels created for
               on this file, and that in turn will be the default for all I/O
               operations performed with those channels.
   :returns: an open :record:`file` that uses the underlying FILE* argument.
   
   :throws SystemError: Thrown if the C file could not be retrieved.
    

.. function:: proc opentmp(hints: iohints = IOHINT_NONE, style: iostyle = defaultIOStyle()): file throws

   
   
   Open a temporary file. Note that once the file is open, you will need to use a
   :proc:`file.reader` or :proc:`file.writer` to create a channel to actually
   perform I/O operations.
   
   The temporary file will be created in an OS-dependent temporary directory,
   for example "/tmp" is the typical location. The temporary file will be
   deleted upon closing.
   
   Temporary files are always opened with :type:`iomode` ``iomode.cwr``;
   that is, a new file is created that supports both writing and reading.
   
   :arg hints: optional argument to specify any hints to the I/O system about
               this file. See :type:`iohints`.
   :arg style: optional argument to specify I/O style associated with this file.
               The provided style will be the default for any channels created for
               on this file, and that in turn will be the default for all I/O
               operations performed with those channels.
   :returns: an open temporary file.
   
   :throws SystemError: Thrown if the temporary file could not be opened.
    

.. function:: proc openmem(style: iostyle = defaultIOStyle()): file throws

   
   
   Open a file that is backed by a buffer in memory that will not persist when the
   file is closed.  Note that once the file is open, you will need to use a
   :proc:`file.reader` or :proc:`file.writer` to create a channel to actually
   perform I/O operations.
   
   The resulting file supports both reading and writing.
   
   :arg style: optional argument to specify I/O style associated with this file.
               The provided style will be the default for any channels created for
               on this file, and that in turn will be the default for all I/O
               operations performed with those channels.
   :returns: an open memory file.
   
   :throws SystemError: Thrown if the memory buffered file could not be opened.
    

.. record:: channel

   
   
   A channel supports either sequential reading or sequential writing to an
   underlying :record:`file` object. A channel can buffer data. Read operations
   on the channel might return old data. Write operations might not have an
   immediate effect. Use :proc:`channel.flush` to control this buffering.
   
   The :record:`channel` type is implementation-defined.
   A value of the :record:`channel` type refers to the state that is used
   to implement the channel operations.
   
   When a :record:`channel` formal has default intent, the
   actual is copied to the formal upon a function call and
   the formal cannot be assigned within the function.
   
   The default value of the :record:`channel` type is not associated
   with any file and so cannot be used to perform I/O.
   
   The :record:`channel` type is generic.
   
    


   .. attribute:: param writing: bool

      
      writing is a boolean indicating whether the channels of this type
      support writing (when `true`) or reading (when `false`).
      

   .. attribute:: param kind: iokind

      
      kind is an enum :type:`iokind` that allows narrowing
      this channel's I/O style for more efficient binary I/O.
      

   .. attribute:: param locking: bool

      
      locking is a boolean indicating whether it is safe to use this
      channel concurrently (when `true`).
      

.. method:: proc channel.init=(x: this.type )

.. record:: ioChar

   
   
   Represents a Unicode codepoint. I/O routines (such as :proc:`channel.read`
   and :proc:`channel.write`) can use arguments of this type in order to read or
   write a single Unicode codepoint.
   
    


   .. attribute:: var ch: int(32)

      The codepoint value 

.. record:: ioNewline

   
   
   Represents a newline character or character sequence (ie ``\n``). I/O routines
   (such as :proc:`channel.read` and :proc:`channel.write`) can use arguments of
   this type in order to read or write a newline. This is different from '\n'
   because an ioNewline always produces an actual newline, but in some cases
   writing ``\n`` will produce an escaped string (such as ``"\n"``).
   
   When reading an ioNewline, read routines will skip any character sequence
   (including e.g. letters and numbers) to get to the newline character unless
   ``skipWhitespaceOnly`` is set to true.
   
    


   .. attribute:: var skipWhitespaceOnly: bool = false

      
      Normally, we will skip anything at all to get to a \n,
      but if skipWhitespaceOnly is set, it will be an error
      if we run into non-space characters other than \n.
      

.. record:: ioLiteral

   
   
   Used to represent a constant string we want to read or write.
   
   When writing, the ioLiteral is output without any quoting or escaping.
   
   When reading, the ioLiteral must be matched exactly - or else the read call
   will return an error with code :data:`SysBasic.EFORMAT`.
   


   .. attribute:: var val: string

      The value of the literal 

   .. attribute:: var ignoreWhiteSpace: bool = true

      Should read operations using this literal ignore and consume
      whitespace before the literal?
      

   .. method:: proc writeThis(f) throws

.. record:: ioBits

   
   
   Represents a value with a particular bit length that we want to read or write.
   The I/O will always be done in binary mode.
   


   .. attribute:: var v: uint(64)

      The bottom ``nbits`` of v will be read or written 

   .. attribute:: var nbits: int(8)

      How many of the low-order bits of ``v`` should we read or write? 

.. method:: proc channel.lock() throws

   
   Acquire a channel's lock.
   
   :throws SystemError: Thrown if the lock could not be acquired.
   

.. method:: proc channel.unlock()

   
   Release a channel's lock.
   

.. method:: proc channel.offset(): int(64)

   
   Return the current offset of a channel.
   
   .. warning::
   
      If the channel can be used by multiple tasks, take care
      when doing operations that rely on the channel's current offset.
      To prevent race conditions, first lock the channel with
      :proc:`channel.lock`, do the operations, then unlock it
      with :proc:`channel.unlock`.
      While holding the lock, use :proc:`channel._offset` instead.
   
   :returns: the current offset of the channel
   

.. method:: proc channel.advance(amount: int(64)) throws

   
   Move a channel offset forward.
   
   For a reading channel, this function will consume the next ``amount``
   bytes. If EOF is reached, the channel position may be left at the
   EOF.
   
   For a writing channel, this function will write ``amount`` zeros - or some
   other data if it is stored in the channel's buffer, for example with
   :proc:`channel._mark` and :proc:`channel._revert`.
   
   :throws SystemError: Throws if the channel offset was not moved.
   

.. method:: proc channel.advancePastByte(byte: uint(8)) throws

   
   Reads until ``byte`` is found and then leave the channel offset
   just after it.
   
   :throws EOFError: if the requested `byte` could not be found.
   :throws SystemError: if another error occurred.
   

.. method:: proc channel.mark() throws

   
    *mark* a channel - that is, save the current offset of the channel
    on its *mark stack*. This function can only be called on a channel
    with ``locking==false``.
   
    The *mark stack* stores several channel offsets. For any channel offset that
    is between the minimum and maximum value in the *mark stack*, I/O operations
    on the channel will keep that region of the file buffered in memory so that
    those operations can be un-done. As a result, it is possible to perform *I/O
    transactions* on a channel. The basic steps for an *I/O transaction* are:
   
     * *mark* the current position with :proc:`channel.mark`
     * do something speculative (e.g. try to read 200 bytes of anything followed
       by a 'B')
     * if the speculative operation was successful,  commit the changes by
       calling :proc:`channel.commit`
     * if the speculative operation was not successful, go back to the *mark* by
       calling :proc:`channel.revert`. Subsequent I/O operations will work
       as though nothing happened.
   
   .. note::
   
     Note that it is possible to request an entire file be buffered in memory
     using this feature, for example by *marking* at offset=0 and then
     advancing to the end of the file. It is important to be aware of these
     memory space requirements.
   
   :returns: The offset that was marked
   :throws: SystemError: if marking the channel failed
   

.. method:: proc channel.revert()

   
   Abort an *I/O transaction*. See :proc:`channel.mark`. This function
   will pop the last element from the *mark stack* and then leave the
   previous channel offset unchanged.  This function can only be
   called on a channel with ``locking==false``.

.. method:: proc channel.commit()

   
   Commit an *I/O transaction*. See :proc:`channel.mark`.  This
   function will pop the last element from the *mark stack* and then
   set the channel offset to the popped offset.  This function can
   only be called on a channel with ``locking==false``.
   

.. method:: proc channel.seek(start: int, end: int = max(int)) throws

   
   Reset a channel to point to a new part of a file.
   This function allows one to jump to a different part of a
   file without creating a new channel. It can only be called
   on a channel with ``locking==false``.
   
   Besides setting a new start position, this function allows
   one to specify a new end position. Specifying the end position
   is usually not necessary for correct behavior but it might be
   an important performance optimization since the channel will not
   try to read data outside of the start..end region.
   
   This function will, in most cases, discard the channel's buffer.
   When writing, the data will be saved to the file before discarding.
   
   :arg start: the new start offset, measured in bytes and counting from 0
   :arg end: optionally, a new end offset, measured in bytes and counting from 0
   :throws: SystemError: if seeking failed. Possible reasons include
                         that the file is not seekable, or that the
                         channel is marked.
   

.. method:: proc channel._offset(): int(64)

   
   For a channel locked with :proc:`channel.lock`, return the offset
   of that channel.
   

.. method:: proc channel._mark() throws

   
    This routine is identical to :proc:`channel.mark` except that it
    can be called on channels with ``locking==true`` and should be
    called only once the channel has been locked with
    :proc:`channel.lock`.  The channel should not be unlocked with
    :proc:`channel.unlock` until after the mark has been committed with
    :proc:`channel._commit` or reverted with :proc:`channel._revert`.
   
    See :proc:`channel.mark` for details other than the locking
    discipline.
   
   :returns: The offset that was marked
   :throws: SystemError: if marking the channel failed
   

.. method:: proc channel._revert()

   
   Abort an *I/O transaction*. See :proc:`channel._mark`.  This
   function will pop the last element from the *mark stack* and then
   leave the previous channel offset unchanged.  This function should
   only be called on a channel that has already been locked and
   marked.

.. method:: proc channel._commit()

   
   Commit an *I/O transaction*. See :proc:`channel._mark`.  This
   function will pop the last element from the *mark stack* and then
   set the channel offset to the popped offset.  This function should
   only be called on a channel that has already been locked and
   marked.

.. method:: proc channel._style(): iostyle

.. method:: proc channel._set_style(style: iostyle)

   
   
   Set the style associated with a channel. This function should only
   be called on a locked channel.
   
   

.. method:: proc channel.readWriteThisFromLocale()

   
   
   Return the locale on which an ongoing I/O was started with a channel.
   This method will return nil unless it is called on a channel that is
   the formal argument to a `readThis`, `writeThis`, or `readWriteThis` method.
   
   

.. function:: proc openreader(path: string, param kind = iokind.dynamic, param locking = true, start: int(64) = 0, end: int(64) = max(int(64)), hints: iohints = IOHINT_NONE, style: iostyle = defaultIOStyle()): channel(false, kind, locking) throws

.. function:: proc openwriter(path: string, param kind = iokind.dynamic, param locking = true, start: int(64) = 0, end: int(64) = max(int(64)), hints: iohints = IOHINT_NONE, style: iostyle = defaultIOStyle()): channel(true, kind, locking) throws

   
   
   Open a file at a particular path and return a writing channel for it.
   This function is equivalent to calling :proc:`open` with ``iomode.cwr`` and then
   :proc:`file.writer` on the resulting file.
   
   :arg path: which file to open (for example, "some/file.txt").
   :arg kind: :type:`iokind` compile-time argument to determine the
              corresponding parameter of the :record:`channel` type. Defaults
              to ``iokind.dynamic``, meaning that the associated
              :record:`iostyle` controls the formatting choices.
   :arg locking: compile-time argument to determine whether or not the
                 channel should use locking; sets the
                 corresponding parameter of the :record:`channel` type.
                 Defaults to true, but when safe, setting it to false
                 can improve performance.
   :arg start: zero-based byte offset indicating where in the file the
               channel should start writing. Defaults to 0.
   :arg end: zero-based byte offset indicating where in the file the
             channel should no longer be allowed to write. Defaults
             to a ``max(int)`` - meaning no end point.
   :arg hints: optional argument to specify any hints to the I/O system about
               this file. See :type:`iohints`.
   :returns: an open writing channel to the requested resource.
   
   :throws SystemError: Thrown if a writing channel could not be returned.

.. method:: proc file.reader(param kind = iokind.dynamic, param locking = true, start: int(64) = 0, end: int(64) = max(int(64)), hints: iohints = IOHINT_NONE, style: iostyle = this._style): channel(false, kind, locking) throws

.. method:: proc file.lines(param locking: bool = true, start: int(64) = 0, end: int(64) = max(int(64)), hints: iohints = IOHINT_NONE, in local_style: iostyle = this._style) throws

   Iterate over all of the lines in a file.
   
   :returns: an object which yields strings read from the file
   
   :throws SystemError: Thrown if an ItemReader could not be returned.
   

.. method:: proc file.writer(param kind = iokind.dynamic, param locking = true, start: int(64) = 0, end: int(64) = max(int(64)), hints: c_int = 0, style: iostyle = this._style): channel(true, kind, locking) throws

.. method:: proc channel.readwrite(const x) throws

   
   For a writing channel, writes as with :proc:`channel.write`.
   For a reading channel, reads as with :proc:`channel.read`.
   Stores any error encountered in the channel. Does not return anything.
   
   :throws SystemError: When an IO error has occurred.
   

.. function:: proc <~>(const ref ch: channel, x) const ref throws

   
   
   The _`<~> operator`
   
   This `<~>` operator is the same as calling :proc:`channel.readwrite`,
   except that it returns the channel so that multiple operator
   calls can be chained together.
   
   :returns: ch
   :throws SystemError: When an IO error has occurred.
   

.. function:: proc <~>(const ref r: channel, lit: ioLiteral) const ref throws

   Overload to support reading an :type:`IO.ioLiteral` without
   passing ioLiterals by reference, so that
   
   .. code-block:: chapel
   
     reader <~> new ioLiteral("=")
   
   works without requiring an explicit temporary value to store
   the ioLiteral.
   

.. function:: proc <~>(const ref r: channel, nl: ioNewline) const ref throws

   Overload to support reading an :type:`IO.ioNewline` without
   passing ioNewline by reference, so that
   
   .. code-block:: chapel
   
     reader <~> new ioNewline()
   
   works without requiring an explicit temporary value to store
   the ioNewline.
   

.. method:: proc channel.readWriteLiteral(lit: string, ignoreWhiteSpace = true) throws

   Explicit call for reading or writing a literal as an
   alternative to using :type:`IO.ioLiteral`.
   

.. method:: proc channel.readWriteNewline() throws

   Explicit call for reading or writing a newline as an
   alternative to using :type:`IO.ioNewline`.
   

.. method:: proc channel.binary(): bool

   Returns `true` if this channel is configured for binary I/O.
   

.. method:: proc channel.error(): syserr

   
   Return any saved error code.
   

.. method:: proc channel.setError(e: syserr)

   
   Save an error code.
   

.. method:: proc channel.clearError()

   
   Clear any saved error code.
   

.. method:: proc channel.writeBytes(x, len: ssize_t): bool throws

   
   Write a sequence of bytes.
   
   :throws SystemError: Thrown if the byte sequence could not be written.
   

.. itermethod:: iter channel.lines()

   
   Iterate over all of the lines ending in ``\n`` in a channel - the channel
   lock will be held while iterating over the lines.
   
   Only serial iteration is supported.
   
   .. warning::
   
     This iterator executes on the current locale. This may impact multilocale
     performance if the current locale is not the same locale on which the
     channel was created.
   
   :yields: lines ending in ``\n`` in channel
   

.. function:: proc stringify(const args ...?k): string

   
   Creates a string representing the result of writing the arguments.
   
   Writes each argument, possibly using a `writeThis` method,
   to a string and returns the result.
   

.. method:: proc channel.read(ref args ...?k): bool throws

.. method:: proc channel.read(ref args ...?k, style: iostyle): bool throws

   
   
   Read values from a channel. The input will be consumed atomically - the
   channel lock will be held while reading all of the passed values.
   
   :arg args: a list of arguments to read. Basic types are handled
              internally, but for other types this function will call
              value.readThis() with a ``Reader`` argument as described
              in :ref:`readThis-writeThis-readWriteThis`.
   :arg style: optional argument to provide an :type:`iostyle` for this read.
               If this argument is not provided, use the current style
               associated with this channel.
   :returns: `true` if the read succeeded, and `false` on end of file.
   
   :throws SystemError: Thrown if the channel could not be read.
   

.. method:: proc channel.readline(arg: [] uint(8), out numRead: int, start = arg.domain.low, amount = arg.domain.high-start+1): bool throws

   
   Read a line into a Chapel array of bytes. Reads until a ``\n`` is reached.
   The ``\n`` is returned in the array.
   
   Throws a SystemError if a line could not be read from the channel.
   
   :arg arg: A 1D DefaultRectangular array which must have at least 1 element.
   :arg numRead: The number of bytes read.
   :arg start: Index to begin reading into.
   :arg amount: The maximum amount of bytes to read.
   :returns: true if the bytes were read without error.

.. method:: proc channel.readline(ref arg: ?t): bool throws

   
   Read a line into a Chapel string or bytes. Reads until a ``\n`` is reached.
   The ``\n`` is included in the resulting value.
   
   :arg arg: a string or bytes to receive the line
   :returns: `true` if a line was read without error, `false` upon EOF
   
   :throws SystemError: Thrown if data could not be read from the channel.

.. method:: proc channel.readstring(ref str_out: string, len: int(64) = -1): bool throws

   read a given number of bytes from a channel
   
   :arg str_out: The string to be read into
   :arg len: Read up to len bytes from the channel, up until EOF
             (or some kind of I/O error). If the default value of -1
             is provided, read until EOF starting from the channel's
             current offset.
   :returns: `true` if we read something, `false` upon EOF
   
   :throws SystemError: Thrown if the bytes could not be read from the channel.
   

.. method:: proc channel.readbytes(ref bytes_out: bytes, len: int(64) = -1): bool throws

   read a given number of bytes from a channel
   
   :arg bytes_out: The bytes to be read into
   :arg len: Read up to len bytes from the channel, up until EOF
             (or some kind of I/O error). If the default value of -1
             is provided, read until EOF starting from the channel's
             current offset.
   :returns: `true` if we read something, `false` upon EOF
   
   :throws SystemError: Thrown if the bytes could not be read from the channel.
   

.. method:: proc channel.readbits(out v: integral, nbits: integral): bool throws

   
   Read bits with binary I/O
   
   :arg v: where to store the read bits. This value will have its *nbits*
           least-significant bits set.
   :arg nbits: how many bits to read
   :returns: `true` if the bits were read without error, `false` upon EOF
   
   :throws SystemError: Thrown if the bits could not be read from the channel.
   

.. method:: proc channel.writebits(v: integral, nbits: integral): bool throws

   
   Write bits with binary I/O
   
   :arg v: a value containing *nbits* bits to write the least-significant bits
   :arg nbits: how many bits to write
   :returns: `true` if the bits were written without error, `false` on error
   
   :throws IllegalArgumentError: Thrown if writing more bits than fit into `v`.
   :throws SystemError: Thrown if the bits could not be written to the channel.
   

.. method:: proc channel.readln(ref args ...?k, style: iostyle): bool throws

   
   
   Read values from a channel and then consume any bytes until
   newline is reached. The input will be consumed atomically - the
   channel lock will be held while reading all of the passed values.
   
   :arg args: a list of arguments to read. This routine can be called
              with zero or more such arguments. Basic types are handled
              internally, but for other types this function will call
              value.readThis() with a ``Reader`` argument as described
              in :ref:`readThis-writeThis-readWriteThis`.
   :arg style: optional argument to provide an :type:`iostyle` for this read.
               If this argument is not provided, use the current style
               associated with this channel.
   :returns: `true` if the read succeeded, and `false` upon end of file.
   
   :throws SystemError: Thrown if a line could not be read from the channel.
   

.. method:: proc channel.read(type t) throws

   
   Read a value of passed type.
   
   .. note::
   
     It is difficult to handle errors or to handle reaching the end of
     the file with this function. If such cases are important please use
     the :proc:`channel.read` returning the values read in arguments instead.
   
   For example, the following line of code reads a value of type `int`
   from :var:`stdin` and uses it to initialize a variable ``x``:
   
   .. code-block:: chapel
   
     var x = stdin.read(int)
   
   
   :arg t: the type to read
   :returns: the value read
   
   :throws SystemError: Thrown if the type could not be read from the channel.
   

.. method:: proc channel.readln(type t) throws

   
   Read a value of passed type followed by a newline.
   
   .. note::
   
     It is difficult to handle errors or to handle reaching the end of
     the file with this function. If such cases are important please use
     :proc:`channel.readln` instead.
   
   
   :arg t: the type to read
   :returns: the value read
   
   :throws SystemError: Thrown if the type could not be read from the channel.
   

.. method:: proc channel.readln(type t ...?numTypes) throws

   
   Read values of passed types followed by a newline
   and return a tuple containing the read values.
   
   :arg t: more than one type to read
   :returns: a tuple of the read values
   
   :throws SystemError: Thrown if the types could not be read from the channel.
   

.. method:: proc channel.read(type t ...?numTypes) throws

   
   Read values of passed types and return a tuple containing the read values.
   
   :arg t: more than one type to read
   :returns: a tuple of the read values
   
   :throws SystemError: Thrown if the types could not be read from the channel.
   

.. method:: proc channel.write(const args ...?k, style: iostyle): bool throws

   
   Write values to a channel. The output will be produced atomically -
   the channel lock will be held while writing all of the passed
   values.
   
   :arg args: a list of arguments to write. Basic types are handled
              internally, but for other types this function will call
              value.writeThis() with the channel as an argument.
   :arg style: optional argument to provide an :type:`iostyle` for this write.
               If this argument is not provided, use the current style
               associated with this channel.
   :returns: `true` if the write succeeded
   
   :throws SystemError: Thrown if the values could not be written to the channel.
   

.. method:: proc channel.writeln(const args ...?k, style: iostyle): bool throws

   
   
   Write values to a channel followed by a newline.  The output will be
   produced atomically - the channel lock will be held while writing all of the
   passed values.
   
   :arg args: a variable number of arguments to write. This method can be
              called with zero or more arguments. Basic types are handled
              internally, but for other types this function will call
              value.writeThis() with the channel as an argument.
   :arg style: optional argument to provide an :type:`iostyle` for this write.
               If this argument is not provided, use the current style
               associated with this channel.
   :returns: `true` if the write succeeded
   
   :throws SystemError: Thrown if the values could not be written to the channel.
   

.. method:: proc channel.flush() throws

   
   
   Makes all writes to the channel, if any, available to concurrent viewers
   of its associated file, such as other channels or other applications
   accessing this file concurrently.
   Unlike :proc:`file.fsync`, this does not commit the written data
   to the file's device.
   
   :throws SystemError: Thrown if the flush fails.

.. method:: proc channel.assertEOF(errStr: string = "- Not at EOF")

   Assert that a channel has reached end-of-file and that there was no error
   doing the read.
   

.. method:: proc channel.close() throws

   
   Close a channel. Implicitly performs the :proc:`channel.flush` operation
   (see :ref:`about-io-channel-synchronization`).
   
   :throws SystemError: Thrown if the channel is not successfully closed.

.. method:: proc channel.isclosed()

   
   Return `true` if a channel is currently closed.
   

.. record:: ItemReader

   Wrapper class on a channel to make it only read values
   of a single type. Also supports an iterator yielding
   the read values.
   


   .. attribute:: type ItemType

      What type do we read and yield? 

   .. attribute:: param kind: iokind

      the kind field for our channel 

   .. attribute:: param locking: bool

      the locking field for our channel 

   .. attribute:: var ch: channel(false, kind, locking)

      our channel 

   .. method:: proc read(out arg: ItemType): bool throws

      read a single item, throwing on error 

   .. itermethod:: iter these()

      iterate through all items of that type read from the channel 

.. method:: proc channel.itemReader(type ItemType, param kind: iokind = iokind.dynamic)

   Create and return an :record:`ItemReader` that can yield read values of
   a single type.
   

.. record:: ItemWriter

   .. attribute:: type ItemType

      What type do we write? 

   .. attribute:: param kind: iokind

      the kind field for our channel 

   .. attribute:: param locking: bool

      the locking field for our channel 

   .. attribute:: var ch: channel(true, kind, locking)

      our channel 

   .. method:: proc write(arg: ItemType): bool throws

      write a single item, throwing on error 

.. method:: proc channel.itemWriter(type ItemType, param kind: iokind = iokind.dynamic)

   Create and return an :record:`ItemWriter` that can write values of
   a single type.
   

.. data:: const stdin: channel(false, iokind.dynamic, true) = stdinInit()

   standard input, otherwise known as file descriptor 0 

.. data:: const stdout: channel(true, iokind.dynamic, true) = stdoutInit()

   standard output, otherwise known as file descriptor 1 

.. data:: const stderr: channel(true, iokind.dynamic, true) = stderrInit()

   standard error, otherwise known as file descriptor 2 

.. function:: proc read(ref args ...?n): bool throws

   Equivalent to ``stdin.read``. See :proc:`channel.read` 

.. function:: proc readln(ref args ...?n): bool throws

   Equivalent to ``stdin.readln``. See :proc:`channel.readln` 

.. function:: proc readln(type t ...?numTypes) throws

   Equivalent to ``stdin.readln``. See :proc:`channel.readln` for types 

.. function:: proc read(type t ...?numTypes) throws

   Equivalent to ``stdin.read``. See :proc:`channel.read` for types 

.. function:: proc unlink(path: string) throws

.. function:: proc unicodeSupported(): bool

   
   :returns: `true` if this version of the Chapel runtime supports UTF-8 output.
   

.. method:: proc file.getchunk(start: int(64) = 0, end: int(64) = max(int(64))): (int(64), int(64)) throws

   
   Returns (chunk start, chunk end) for the first chunk in the file
   containing data in the region start..end-1. Note that the returned
   chunk might not cover all of the region in question.
   
   Returns (0,0) if no such value exists.
   
   :arg start: the file offset (starting from 0) where the region begins
   :arg end: the file offset just after the region
   :returns: a tuple of (chunkStart, chunkEnd) so that the bytes
             in chunkStart..chunkEnd-1 are stored in a manner that makes
             reading that chunk at a time most efficient
   
   :throws SystemError: Thrown if the chunk is not attained.
   

.. method:: proc file.localesForRegion(start: int(64), end: int(64))

   
   
   Returns the 'best' locale to run something working with the region
   of the file in start..end-1.
   
   This *must* return the same result when called from different locales.
   Returns a domain of locales that are "best" for the given region. If no
   locales are "best" we return a domain containing all locales.
   
   :arg start: the file offset (starting from 0) where the region begins
   :arg end: the file offset just after the region
   :returns: a set of locales that are best for working with this region
   :rtype: domain(locale)
   

