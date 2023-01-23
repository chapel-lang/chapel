/*
 * Copyright 2020-2023 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
 * Other additional copyright holders may be indicated within.
 *
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 *
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*


Support for a variety of kinds of input and output.

.. note:: All Chapel programs automatically include :proc:`~ChapelIO.write`,
          :proc:`~ChapelIO.writeln` and :proc:`~ChapelIO.writef`.

Input/output (I/O) facilities in Chapel include the types :record:`file` and
:record:`channel`; the constants :record:`stdin`, :record:`stdout` and
:record:`stderr`; the functions :proc:`open`, :proc:`file.close`,
:proc:`file.reader`, :proc:`file.writer`, :proc:`channel.read`,
:proc:`channel.write`, and many others.

.. warning::
  Please be aware, the IO Module documentation is under development and
  currently contains some minor inconsistencies.

  For example, the :record:`channel` type has been replaced with the
  :record:`fileWriter` and :record:`fileReader` types; however, not all
  references to ``channel`` have been removed from the docs. As such, note that
  writing methods on the ``channel`` type (such as :proc:`channel.writeln`) are
  intended to be called on a ``fileWriter`` and reading methods are intended to
  be called on a ``fileReader``.

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

The :proc:`~IO.read` functions allow one to read values into variables as
the following example demonstrates. It shows three ways to read values into
a pair of variables ``x`` and ``y``.

.. code-block:: chapel

  var x: int;
  var y: real;
  /* reading into variable expressions, returning
     true if the values were read, false on EOF */
  var ok:bool = read(x, y);

  /* reading via a single type argument */
  x = read(int);
  y = read(real);

  /* reading via multiple type arguments */
  (x, y) = read(int, real);

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

.. warning::

   :record:`iostyle` is now unstable.
   We are working on creating a full-featured replacement for it
   but in the meantime the :ref:`about-io-formatted-io` facilities are still
   available to control formatting.

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

As an example for specifying an I/O style, the code below specifies the minimum width for writing numbers so array elements are aligned in the output:

.. code-block:: chapel

  stdout.writeln(MyArray, new iostyle(min_width=10));


I/O facilities in Chapel also include several other ways to control I/O
formatting. There is support for :ref:`formatted I/O <about-io-formatted-io>`
with :proc:`FormattedIO.channel.readf` and :proc:`FormattedIO.channel.writef`.
Also note that record or class implementations can provide custom functions
implementing read or write operations for that type (see
:ref:`readThis-writeThis`).

.. _about-io-files:

Files
-----

There are several functions that open a file and return a :record:`file`
including :proc:`open`, :proc:`openTempFile`, :proc:`openmem`, :proc:`openfd`,
and :proc:`openfp`.

Once a file is open, it is necessary to create associated channel(s) - see
:proc:`file.reader` and :proc:`file.writer` - to write to and/or read from the
file.

Use the :proc:`file.fsync` function to explicitly synchronize the file to
ensure that file data is committed to the file's underlying device for
persistence.

To release any resources associated with a file, it is necessary to first close
any channels using that file (with :proc:`channel.close`) and then the file
itself (with :proc:`file.close`).

 .. note::

    :ref:`Escaped strings <string.nonunicode>` can be used for paths on systems
    where UTF-8 file names are not enforced.


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
   * and where data could be stored in more than one of the overlapping
     channel's buffers at the same time (i.e., write and read ordering are
     not enforced through :proc:`channel.flush` and other means such as
     sync variables).

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

Sometimes it's important to flush the buffer in a channel - to do that, use the
:proc:`channel.flush()` method. Flushing the buffer will make all writes available
to other applications or other views of the file (e.g., it will call the OS call
``pwrite()``).  It is also possible to close a channel, which will implicitly
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

.. _stdin-stdout-stderr:

The ``stdin``, ``stdout``, and ``stderr`` Channels
--------------------------------------------------

Chapel provides the predefined channels :var:`stdin`, :var:`stdout`, and
:var:`stderr` to access the corresponding operating system streams standard
input, standard output, and standard error.

:var:`stdin` supports reading;
:var:`stdout` and :var:`stderr` support writing.

All three channels are safe to use concurrently.
Their types' ``kind`` argument is ``dynamic``.

.. _about-io-error-handling:

Error Handling
--------------

Most I/O routines throw a :class:`OS.SystemError`, and can be handled
appropriately with ``try`` and ``catch``.

Additionally, some subclasses of :class:`Errors.Error` are commonly used within
the I/O implementation. These are:

 * :class:`OS.EofError` - the end of file was reached
 * :class:`OS.UnexpectedEofError` - a read or write only returned part of the requested data
 * :class:`OS.BadFormatError` - data read did not adhere to the requested format

An error code can be converted to a string using the function
:proc:`OS.errorToString()`.

.. _about-io-ensuring-successful-io:

Ensuring Successful I/O
-----------------------

It is possible - in some situations - for I/O to fail without returning an
error. In cases where a programmer wants to be sure that there was no error
writing the data to disk, it is important to call :proc:`file.fsync` to make
sure that data has arrived on disk without an error. Many errors can be
reported with a typical operation, but some errors can only be reported by the
system during :proc:`file.close` or even :proc:`file.fsync`.

When a file (or channel) is closed, data written to that file will be written
to disk eventually by the operating system. If an application needs to be sure
that the data is immediately written to persistent storage, it should use
:proc:`file.fsync` prior to closing the file.

Correspondence with C I/O
-------------------------

It is not possible to seek, read, or write to a file directly; channels must be
created and used.

:proc:`channel.flush` in Chapel has the same conceptual meaning as ``fflush()``
in C.  However, ``fflush()`` is not necessarily called in :proc:`channel.flush()`,
unlike ``fsync()``, which is actually called by :proc:`file.fsync()` in Chapel.

IO Functions and Types
----------------------

 */
module IO {
/* "channel" I/O contributed by Michael Ferguson

   Future Work:
    - We would like to have a 'serialization' system, including allowing
      the writing of data structures with circular references, and
      encoding the types of classes.
    - Currently, the union tag numbers are per-compile; you could not read
      binary union data written in one Chapel program with another. This problem
      will also apply to class tag numbers once we start encoding types.
    - The Chapel compiler does not currently allow RAII/reference counting
      to work correctly in all cases (bug report 'records containing pointers'
      and test files in test/users/ferguson/{byvalue.chpl,refcnt.chpl};
      as a result, current code might need to close channels and files
      explicitly and/or sharing of channels and files between variables might
      not work correctly.
    - Channels have a lock, and some methods on a channel only make sense
      when the lock is held. In the future (again, once RAII makes sense),
      we might replace this with 'get locked channel' that returns a different
      type that has the methods that can only be used on a locked channel.
    - The error handling strategy here is a bit awkward for the library-writer
      (not so much for the user). A broader exceptions strategy would help
      quite a bit - and some of these interfaces will change when that happens.
    - Fancy features, like adding a bytes or buffer object to a channel
      (so that the channel just refers to it and does not copy it) are
      implemented but not well tested.
    - It would be nice if ioBits:string printed itself in binary instead of
      decimal.
    - Cleaning up to reduce the number of exported symbols, and using enums for
      all constants once 'use enum' is available and we have a way to get
      C constants into Chapel enums.
    - Support for libevent/qthreads system call thread so that we have
      something suitable for writing (in a straightforward manner) a
      multithreaded webserver
    - Doing something reasonable with a file that moves from one node to
      another - namely, when a channel is created with a file that resides
      remotely, if that same file is accessible locally (e.g. with Lustre or
      NFS), we should open a local copy of that file and use that in the
      channel. (not sure how to avoid opening # channels copies of these files
      -- seems that we'd want some way to cache that...).
*/

import OS.POSIX.{ENOENT, ENOSYS, EINVAL, EILSEQ, EIO, ERANGE};
import OS.POSIX.{EBADF};
import OS.{errorCode};
use CTypes;
public use OS;


/*

The :type:`iomode` type is an enum. When used as arguments when opening files, its
constants have the same meaning as the following strings passed to ``fopen()`` in C:

.. list-table::
   :widths: 8 8 64
   :header-rows: 1

   * - :type:`iomode`
     - ``fopen()`` argument
     - Description
   * - ``iomode.r``
     - ``"r"``
     - open an existing file for reading.
   * - ``iomode.rw``
     - ``"r+"``
     - open an existing file for reading and writing.
   * - ``iomode.cw``
     - ``"w"``
     - create a new file for writing. If the file already exists, its contents are truncated.
   * - ``iomode.cwr``
     - ``"w+"``
     - same as ``iomode.cw``, but reading from the file is also allowed.

.. TODO: Support append / create-exclusive modes:
   * - ``iomode.a``
     - ``"a"``
     - open a file for appending, creating it if it does not exist.
   * - ``iomode.ar``
     - ``"a+"``
     - same as ``iomode.a``, but reading from the file is also allowed.
   * - ``iomode.cwx``
     - ``"wx"``
     - open a file for writing, throwing an error if it already exists. (The test for file's existence and the file's creation are atomic on POSIX.)
   * - ``iomode.cwrx``
     - ``"w+x"``
     - same as ``iomode.cwx``, but reading from the file is also allowed.

However, :proc:`open()` in Chapel does not necessarily invoke ``fopen()`` in C.
*/

enum iomode {
  r = 1,
  cw = 2,
  rw = 3,
  cwr = 4,
}

/*

The :type:`iokind` type is an enum. When used as arguments to the
:record:`channel` type, its constants have the following meaning:

* ``iokind.dynamic`` means that the applicable I/O style has full effect
  and as a result the kind varies at runtime.

* ``iokind.native`` means binary I/O in native byte order
  (similar to ``iokind.big`` but with the byte order that is native
  to the target platform).

* ``iokind.big`` means binary I/O with big-endian byte order is performed
  when writing/reading basic types from the channel.

* ``iokind.little`` means binary I/O with little-endian byte order
  (similar to ``iokind.big`` but with little-endian byte order).

In the case of ``iokind.big``, ``iokind.little``, and
``iokind.native`` the applicable :record:`iostyle` is consulted when
writing/reading strings, but not for other basic types.

There are synonyms available for these values:

* :proc:`iodynamic` = ``iokind.dynamic``
* :proc:`ionative` = ``iokind.native``
* :proc:`iobig` = ``iokind.big``
* :proc:`iolittle` = ``iokind.little``

*/
enum iokind {
  // don't change these without updating qio_style.h QIO_NATIVE, etc
  // a default of 0 is always reasonable, but you can avoid some
  // branches to get faster I/O by setting this to native, big, or little.
  // In that case, the style is only consulted for text or string I/O.
  dynamic = 0, // look in iostyle
  native = 1,
  big = 2, // aka "network"
  little = 3
}

/* A synonym for ``iokind.dynamic``; see :type:`iokind` */
param iodynamic = iokind.dynamic;
/* A synonym for ``iokind.native``; see :type:`iokind` */
param ionative = iokind.native;
/* A synonym for ``iokind.big``; see :type:`iokind` */
param iobig = iokind.big;
/* A synonym for ``iokind.little``; see :type:`iokind` */
param iolittle = iokind.little;

/*

The :type:`ioendian` type is an enum. When used as an argument to the
:record:`channel` methods, its constants have the following meanings:

* ``ioendian.big`` means binary I/O is performed in big-endian byte order.

* ``ioendian.little`` means binary I/O is performed in little-endian byte order.

* ``ioendian.native`` means binary I/O is performed in the byte order that is native
  to the target platform.

*/

enum ioendian {
  native = 0,
  big = 1,
  little = 2
}


/*

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

*/
@unstable
"iostringstyle is unstable and will eventually be replaced"
enum iostringstyle {
  len1b_data = -1,
  len2b_data = -2,
  len4b_data = -4,
  len8b_data = -8,
  lenVb_data = -10,
  data_toeof = -0xff00,
  data_null = -0x0100,
}

/* Internal version of iostringstyle for interim use */
pragma "no doc"
enum iostringstyleInternal {
  len1b_data = -1,
  len2b_data = -2,
  len4b_data = -4,
  len8b_data = -8,
  lenVb_data = -10,
  data_toeof = -0xff00,
  data_null = -0x0100,
}

/*

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
*/
@unstable
"iostringformat is unstable and will eventually be replaced"
enum iostringformat {
  word = 0,
  basic = 1,
  chpl = 2,
  json = 3,
  toend = 4,
  toeof = 5,
}

/* Internal version of iostringformat for interim use */
pragma "no doc"
enum iostringformatInternal {
  word = 0,
  basic = 1,
  chpl = 2,
  json = 3,
  toend = 4,
  toeof = 5,
}


/*

  This method returns the appropriate :record:`iostyle` ``str_style`` value
  to indicate a string format where strings are terminated by a
  particular byte.

  :arg terminator: a byte value that the strings will be terminated by
  :returns: a value that indicates a string format where strings
            are terminated by the terminator byte. This value is appropriate
            to store in iostyle.str_style.
 */
@unstable
"stringStyleTerminated is unstable because it supports the unstable type 'iostyle'"
proc stringStyleTerminated(terminator:uint(8)) {
  return -(terminator - iostringstyleInternal.data_null:int(64));
}

/*
  This method returns the appropriate :record:`iostyle` ``str_style`` value
  to indicate a string format where strings are terminated by a
  zero byte.
 */

@unstable
"stringStyleNullTerminated is unstable because it supports the unstable type 'iostyle'"
proc stringStyleNullTerminated() {
  return iostringstyleInternal.data_null;
}

/*
  This method returns the appropriate :record:`iostyle` ``str_style`` value
  to indicate a string format where strings have an exact length.
 */
pragma "no doc"
@unstable
"stringStyleExactLen is unstable because it supports the unstable type 'iostyle'"
proc stringStyleExactLen(len:int(64)) {
  return len;
}

/*
  This method returns the appropriate :record:`iostyle` ``str_style`` value
  to indicate a string format where string data is preceded by a variable-byte
  length as described in :type:`iostringstyle`.
 */
pragma "no doc"
@unstable
"stringStyleWithVariableLength is unstable because it supports the unstable type 'iostyle'"
proc stringStyleWithVariableLength() {
  return iostringstyleInternal.lenVb_data: int(64);
}

/*
  Return the appropriate :record:`iostyle` ``str_style`` value
  to indicate a string format where string data is preceded by a
  `lengthBytes` of length. Only lengths of 1, 2, 4, or 8 are supported.
  When `lengthBytes` is 0, the returned value indicates variable-byte length.

  :throws SystemError: Thrown for an unsupported value of `lengthBytes`.
 */
@unstable
"stringStyleWithLength is unstable because it supports the unstable type 'iostyle'"
proc stringStyleWithLength(lengthBytes:int) throws {
  return stringStyleWithLengthInternal(lengthBytes);
}

// Replacement for stringStyleWithLength, though it shouldn't be relied upon by
// users as it will likely be replaced in the future
pragma "no doc"
proc stringStyleWithLengthInternal(lengthBytes:int) throws {
  var x = iostringstyleInternal.lenVb_data;
  select lengthBytes {
    when 0 do x = iostringstyleInternal.lenVb_data;
    when 1 do x = iostringstyleInternal.len1b_data;
    when 2 do x = iostringstyleInternal.len2b_data;
    when 4 do x = iostringstyleInternal.len4b_data;
    when 8 do x = iostringstyleInternal.len8b_data;
    otherwise
      throw createSystemError(EINVAL,
                              "Invalid string length prefix " +
                              lengthBytes:string);
  }
  return x;
}

pragma "no doc"
extern const QIO_FDFLAG_UNK:c_int;
pragma "no doc"
extern const QIO_FDFLAG_READABLE:c_int;
pragma "no doc"
extern const QIO_FDFLAG_WRITEABLE:c_int;
pragma "no doc"
extern const QIO_FDFLAG_SEEKABLE:c_int;

pragma "no doc"
extern const QIO_CH_ALWAYS_UNBUFFERED:c_int;
pragma "no doc"
extern const QIO_CH_ALWAYS_BUFFERED:c_int;
pragma "no doc"
extern const QIO_CH_BUFFERED:c_int;

pragma "no doc"
extern const QIO_METHOD_DEFAULT:c_int;
pragma "no doc"
extern const QIO_METHOD_READWRITE:c_int;
pragma "no doc"
extern const QIO_METHOD_PREADPWRITE:c_int;
pragma "no doc"
extern const QIO_METHOD_FREADFWRITE:c_int;
pragma "no doc"
extern const QIO_METHOD_MMAP:c_int;
pragma "no doc"
extern const QIO_METHODMASK:c_int;
pragma "no doc"
extern const QIO_HINT_RANDOM:c_int;
pragma "no doc"
extern const QIO_HINT_SEQUENTIAL:c_int;
pragma "no doc"
extern const QIO_HINT_LATENCY:c_int;
pragma "no doc"
extern const QIO_HINT_BANDWIDTH:c_int;
pragma "no doc"
extern const QIO_HINT_CACHED:c_int;
pragma "no doc"
extern const QIO_HINT_PARALLEL:c_int;
pragma "no doc"
extern const QIO_HINT_DIRECT:c_int;
pragma "no doc"
extern const QIO_HINT_NOREUSE:c_int;
pragma "no doc"
extern const QIO_HINT_OWNED:c_int;

pragma "no doc"
extern type qio_file_ptr_t;
private extern const QIO_FILE_PTR_NULL:qio_file_ptr_t;


pragma "no doc"
extern record qiovec_t {
  var iov_base: c_void_ptr;
  var iov_len: c_size_t;
}

pragma "no doc"
extern type qio_channel_ptr_t;
private extern const QIO_CHANNEL_PTR_NULL:qio_channel_ptr_t;

// also the type for a buffer for qio_file_open_mem.
pragma "no doc"
extern type qbuffer_ptr_t;
pragma "no doc"
extern const QBUFFER_PTR_NULL:qbuffer_ptr_t;

pragma "no doc"
extern type style_char_t = uint(8);

pragma "no doc"
extern const QIO_STRING_FORMAT_WORD:uint(8);
pragma "no doc"
extern const QIO_STRING_FORMAT_BASIC:uint(8);
pragma "no doc"
extern const QIO_STRING_FORMAT_CHPL:uint(8);
pragma "no doc"
extern const QIO_STRING_FORMAT_JSON:uint(8);
pragma "no doc"
extern const QIO_STRING_FORMAT_TOEND:uint(8);
pragma "no doc"
extern const QIO_STRING_FORMAT_TOEOF:uint(8);

/*

The :record:`iostyleInternal` type represents I/O styles
defining how Chapel's basic types should be read or written.

It replaces the now unstable `iostyle` type, and will eventually
be migrated into a new strategy, likely involving encoders/decoders
*/
pragma "no doc"
extern record iostyleInternal { // aka qio_style_t
  /* Perform binary I/O? 1 - yes, 0 - no.
     This field is ignored for :type:`iokind` values other than ``dynamic``.
   */
  var binary:uint(8) = 0;
  // binary style choices
  /* What byte order should we use when performing binary I/O?
     This field is ignored for :type:`iokind` values other than ``dynamic``.
     It should be set to a value in :type:`iokind`.
   */
  var byteorder:uint(8) = iokind.native:uint(8);

  /*
     What string format should we use when writing strings
     in binary mode? See :type:`iostringstyle` for more information
     on what the values of ``str_style`` mean.
   */
  var str_style:int(64) = iostringstyleInternal.data_toeof: int(64);

  // text style choices
  /* When performing text I/O, pad out to this many columns. */
  var min_width_columns:uint(32) = 0;
  /* When performing text I/O, do not use more than this many columns. */
  var max_width_columns:uint(32) = max(uint(32));
  /* When performing text I/O, do not use more than this many characters. */
  var max_width_characters:uint(32) = max(uint(32));
  /* When performing text I/O, do not use more than this many bytes. */
  var max_width_bytes:uint(32) = max(uint(32));

  /* What character do we start strings with, when appropriate? Default is ``"``. */
  var string_start:style_char_t = 0x22; // "
  /* What character do we end strings with, when appropriate? Default is ``"``. */
  var string_end:style_char_t = 0x22; // "

  /* How should we format strings when performing text I/O?
     See :type:`iostringstyleInternal` for more information
     on what the values of ``str_style`` mean.
   */
  var string_format:uint(8) = iostringformatInternal.word:uint(8);

  /* What character do we start bytes with, when appropriate? Default is ``"``. */
  var bytes_prefix:style_char_t = 0x62; // b

  // numeric scanning/printing choices
  /* When reading or writing a numeric value in a text mode channel,
     what base should be used for the number? Default of 0 means decimal.
     Bases 2, 8, 10, 16 are supported for integers. Bases 10 and 16
     are supported for real values.*/
  var base:uint(8) = 0;
  /* When reading or writing a numeric value in a text mode channel,
     how is the integer portion separated from the fractional portion?
     Default is ``.``. */
  var point_char:style_char_t = 0x2e; // .
  /* When reading or writing a numeric value in a text mode channel,
     how is the exponent written? Default is ``e``. */
  var exponent_char:style_char_t = 0x65; // e
  /* When reading or writing a numeric value in a text mode channel,
     when base is > 10, how is the exponent written? Default is ``p``. */
  var other_exponent_char:style_char_t = 0x70; // p
  /* What character denotes a positive number? Default is ``+``. */
  var positive_char:style_char_t = 0x2b; // +;
  /* What character denotes a negative number? Default is ``-``. */
  var negative_char:style_char_t = 0x2d; // -;
  /* What character follows an the imaginary number? Default is ``i``. */
  var i_char:style_char_t = 0x69; // i
  /* When writing in a base other than 10, should the prefix be used
     (e.g. hexadecimal numbers are prefixed with 0x)? */
  var prefix_base:uint(8) = 1;
  // numeric printing choices
  /* When padding with spaces, which pad character to use? Default is ' '. */
  var pad_char:style_char_t = 0x20; // ' '
  /* When printing a positive numeric value, should the ``+`` be shown? */
  var showplus:uint(8) = 0;
  /* When printing a numeric value in hexadecimal, should it be
     uppercase? */
  var uppercase:uint(8) = 0;
  /* When printing a numeric value in a field of specified width, should
     the number be on the left (that is padded on the right?). The default
     is to right-justify the number. */
  var leftjustify:uint(8) = 0;
  /* When printing an integral value using a real format, should a trailing
     decimal point be included? If so, the value 0 will be written as ``0.`` */
  var showpoint:uint(8) = 0;
  /* When printing an integral value using a real format, should a trailing
     decimal point and zero be included? If so, the value 0 will be written
     as ``0.0`` */
  var showpointzero:uint(8) = 1;
  /* Specifies the precision for real format conversions. See the description
     of realfmt below. */
  var precision:int(32) = -1;

  /*
     Formatting of real numbers:

       * 0 means  print out 'precision' number of significant digits
         (%g in printf)
       * 1 means  print out 'precision' number of digits after the decimal point
         (%f)
       * 2 means  always use exponential and 'precision' number of digits (%e)
   */
  var realfmt:uint(8) = 0;

  var complex_style:uint(8) = 0;
  var array_style:uint(8) = 0;
  var aggregate_style:uint(8) = 0;
  var tuple_style:uint(8) = 0;
}

@unstable "iostyle is unstable, a new way of controlling channel output is planned"
type iostyle = iostyleInternal;

// This class helps in implementing runtime calls.
// It represents a file as a pointer. C code can call Chapel
// code working with this file through the export procs below,
// e.g. chpl_qio_read_atleast. These work by casting to
// this type and then invoking the method with virtual dispatch.

pragma "no doc"
class QioPluginFile {
  // Assume instance has a link to filesystem if needed

  // TODO: would like these to throw instead of returning QIO errors,
  // but that will require changing the error representation within QIO.

  /* Create a new plugin channel that will work with the passed
     qio channel. */
  proc setupChannel(out pluginChannel:unmanaged QioPluginChannel?,
                    start:int(64), end:int(64),
                    qioChannelPtr:qio_channel_ptr_t):errorCode {
    return ENOSYS;
  }

  /* Returns the length of an open file. */
  proc filelength(out length:int(64)):errorCode {
    return ENOSYS;
  }

  /* Returns the path to an open file.
     The caller has the responsibility to free the returned c_string.
   */
  // TODO: use Chapel strings for this, one day
  proc getpath(out path:c_string, out len:int(64)):errorCode {
    return ENOSYS;
  }

  /* Write the file data to persistent storage. */
  proc fsync():errorCode {
    return ENOSYS;
  }
  /* Get the optimal I/O chunk size for the file. */
  proc getChunk(out length:int(64)):errorCode {
    return ENOSYS;
  }
  /* Returns a locale best at working with each chunk within a region in a
     file */
  // TODO: This is not currently implemented or used anywhere.
  proc getLocalesForRegion(start:int(64), end:int(64), out
      localeNames:c_ptr(c_string), ref nLocales:int(64)):errorCode {
    return ENOSYS;
  }

  /* Close the file. */
  proc close():errorCode {
    return ENOSYS;
  }
}

// This class helps with C runtime I/O plugins. It represents additional
// information for a channel.
pragma "no doc"
class QioPluginChannel {
  /* Read at least ``amt`` bytes and store these in the related channel. */
  proc readAtLeast(amt:int(64)):errorCode {
    return ENOSYS;
  }
  /* Write up to ``amt`` bytes from the channel. */
  proc write(amt:int(64)):errorCode {
    return ENOSYS;
  }
  /* Close the channel. */
  proc close():errorCode {
    return ENOSYS;
  }
}

// These functions let the C QIO code call the plugins
// TODO: Move more of the QIO code to be pure Chapel
pragma "no doc"
export proc chpl_qio_setup_plugin_channel(file:c_void_ptr, ref plugin_ch:c_void_ptr, start:int(64), end:int(64), qio_ch:qio_channel_ptr_t):errorCode {
  var f=(file:unmanaged QioPluginFile?)!;
  var pluginChannel:unmanaged QioPluginChannel? = nil;
  var ret = f.setupChannel(pluginChannel, start, end, qio_ch);
  plugin_ch = pluginChannel:c_void_ptr;
  return ret;
}

pragma "no doc"
export proc chpl_qio_read_atleast(ch_plugin:c_void_ptr, amt:int(64)) {
  var c=(ch_plugin:unmanaged QioPluginChannel?)!;
  return c.readAtLeast(amt);
}
pragma "no doc"
export proc chpl_qio_write(ch_plugin:c_void_ptr, amt:int(64)) {
  var c=(ch_plugin:unmanaged QioPluginChannel?)!;
  return c.write(amt);
}
pragma "no doc"
export proc chpl_qio_channel_close(ch:c_void_ptr):errorCode {
  var c=(ch:unmanaged QioPluginChannel?)!;
  var err = c.close();
  delete c;
  return err;
}

pragma "no doc"
export proc chpl_qio_filelength(file:c_void_ptr, ref length:int(64)):errorCode {
  var f=(file:unmanaged QioPluginFile?)!;
  return f.filelength(length);
}
pragma "no doc"
export proc chpl_qio_getpath(file:c_void_ptr, ref str:c_string, ref len:int(64)):errorCode {
  var f=(file:unmanaged QioPluginFile?)!;
  return f.getpath(str, len);
}
pragma "no doc"
export proc chpl_qio_fsync(file:c_void_ptr):errorCode {
  var f=(file:unmanaged QioPluginFile?)!;
  return f.fsync();
}
pragma "no doc"
export proc chpl_qio_get_chunk(file:c_void_ptr, ref length:int(64)):errorCode {
  var f=(file:unmanaged QioPluginFile?)!;
  return f.getChunk(length);
}
pragma "no doc"
export proc chpl_qio_get_locales_for_region(file:c_void_ptr, start:int(64),
    end:int(64), ref localeNames:c_void_ptr, ref nLocales:int(64)):errorCode {
  var strPtr:c_ptr(c_string);
  var f=(file:unmanaged QioPluginFile?)!;
  var ret = f.getLocalesForRegion(start, end, strPtr, nLocales);
  localeNames = strPtr:c_void_ptr;
  return ret;
}
pragma "no doc"
export proc chpl_qio_file_close(file:c_void_ptr):errorCode {
  var f = (file:unmanaged QioPluginFile?)!;
  var err = f.close();
  delete f;
  return err;
}

// Extern functions
// TODO -- move these declarations to where they are used or into
// a helper module to reduce namespace noise.

private extern proc qio_style_init_default(ref s: iostyleInternal);

private extern proc qio_file_retain(f:qio_file_ptr_t);
private extern proc qio_file_release(f:qio_file_ptr_t);

private extern proc qio_file_init(ref file_out:qio_file_ptr_t, fp: c_FILE, fd:c_int, iohints:c_int, const ref style:iostyleInternal, usefilestar:c_int):errorCode;
private extern proc qio_file_open_access(ref file_out:qio_file_ptr_t, path:c_string, access:c_string, iohints:c_int, const ref style:iostyleInternal):errorCode;
private extern proc qio_file_open_tmp(ref file_out:qio_file_ptr_t, iohints:c_int, const ref style:iostyleInternal):errorCode;
private extern proc qio_file_open_mem(ref file_out:qio_file_ptr_t, buf:qbuffer_ptr_t, const ref style:iostyleInternal):errorCode;

pragma "no doc"
extern proc qio_file_close(f:qio_file_ptr_t):errorCode;


private extern proc qio_file_lock(f:qio_file_ptr_t):errorCode;
private extern proc qio_file_unlock(f:qio_file_ptr_t);
private extern proc qio_file_isopen(f:qio_file_ptr_t):bool;

/* The general way to make sure data is written without error */
private extern proc qio_file_sync(f:qio_file_ptr_t):errorCode;

private extern proc qio_channel_end_offset_unlocked(ch:qio_channel_ptr_t):int(64);
private extern proc qio_file_get_style(f:qio_file_ptr_t, ref style:iostyleInternal);
private extern proc qio_file_get_plugin(f:qio_file_ptr_t):c_void_ptr;
private extern proc qio_channel_get_plugin(ch:qio_channel_ptr_t):c_void_ptr;
private extern proc qio_file_length(f:qio_file_ptr_t, ref len:int(64)):errorCode;

private extern proc qio_channel_create(ref ch:qio_channel_ptr_t, file:qio_file_ptr_t, hints:c_int, readable:c_int, writeable:c_int, start:int(64), end:int(64), const ref style:iostyleInternal, bufIoMax:int(64)):errorCode;

private extern proc qio_channel_path_offset(threadsafe:c_int, ch:qio_channel_ptr_t, ref path:c_string, ref offset:int(64)):errorCode;

private extern proc qio_channel_retain(ch:qio_channel_ptr_t);
private extern proc qio_channel_release(ch:qio_channel_ptr_t);

private extern proc qio_channel_clear_error(ch:qio_channel_ptr_t);
private extern proc _qio_channel_set_error_unlocked(ch:qio_channel_ptr_t, err:errorCode);
private extern proc qio_channel_error(ch:qio_channel_ptr_t):errorCode;


private extern proc qio_channel_lock(ch:qio_channel_ptr_t):errorCode;
private extern proc qio_channel_unlock(ch:qio_channel_ptr_t);

private extern proc qio_channel_get_style(ch:qio_channel_ptr_t, ref style:iostyleInternal);
private extern proc qio_channel_set_style(ch:qio_channel_ptr_t, const ref style:iostyleInternal);

private extern proc qio_channel_get_size(ch: qio_channel_ptr_t):int(64);

private extern proc qio_channel_binary(ch:qio_channel_ptr_t):uint(8);
private extern proc qio_channel_byteorder(ch:qio_channel_ptr_t):uint(8);
private extern proc qio_channel_str_style(ch:qio_channel_ptr_t):int(64);
private extern proc qio_channel_style_element(ch:qio_channel_ptr_t, element:int(64)):int(64);

private extern proc qio_channel_flush(threadsafe:c_int, ch:qio_channel_ptr_t):errorCode;
private extern proc qio_channel_close(threadsafe:c_int, ch:qio_channel_ptr_t):errorCode;
pragma "no doc"
extern proc qio_channel_isclosed(threadsafe:c_int, ch:qio_channel_ptr_t):bool;

private extern proc qio_channel_read(threadsafe:c_int, ch:qio_channel_ptr_t, ref ptr, len:c_ssize_t, ref amt_read:c_ssize_t):errorCode;
private extern proc qio_channel_read_amt(threadsafe:c_int, ch:qio_channel_ptr_t, ref ptr, len:c_ssize_t):errorCode;
pragma "no doc"
// A specialization is needed for _ddata as the value is the pointer its memory
private extern proc qio_channel_read_amt(threadsafe:c_int, ch:qio_channel_ptr_t, ptr:_ddata, len:c_ssize_t):errorCode;
// and for c_ptr
private extern proc qio_channel_read_amt(threadsafe:c_int, ch:qio_channel_ptr_t, ptr:c_ptr, len:c_ssize_t):errorCode;
private extern proc qio_channel_read_byte(threadsafe:c_int, ch:qio_channel_ptr_t):int(32);

private extern proc qio_channel_write(threadsafe:c_int, ch:qio_channel_ptr_t, const ref ptr, len:c_ssize_t, ref amt_written:c_ssize_t):errorCode;
private extern proc qio_channel_write_amt(threadsafe:c_int, ch:qio_channel_ptr_t, const ref ptr, len:c_ssize_t):errorCode;
pragma "no doc"
// A specialization is needed for _ddata as the value is the pointer its memory
private extern proc qio_channel_write_amt(threadsafe:c_int, ch:qio_channel_ptr_t, const ptr:_ddata, len:c_ssize_t):errorCode;
private extern proc qio_channel_write_byte(threadsafe:c_int, ch:qio_channel_ptr_t, byte:uint(8)):errorCode;

private extern proc qio_channel_offset_unlocked(ch:qio_channel_ptr_t):int(64);
private extern proc qio_channel_advance(threadsafe:c_int, ch:qio_channel_ptr_t, nbytes:int(64)):errorCode;
private extern proc qio_channel_advance_past_byte(threadsafe:c_int, ch:qio_channel_ptr_t, byte:c_int):errorCode;

private extern proc qio_channel_mark(threadsafe:c_int, ch:qio_channel_ptr_t):errorCode;
private extern proc qio_channel_revert_unlocked(ch:qio_channel_ptr_t);
private extern proc qio_channel_commit_unlocked(ch:qio_channel_ptr_t);

private extern proc qio_channel_seek(ch:qio_channel_ptr_t, start:int(64), end:int(64)):errorCode;

private extern proc qio_channel_write_bits(threadsafe:c_int, ch:qio_channel_ptr_t, v:uint(64), nbits:int(8)):errorCode;
private extern proc qio_channel_flush_bits(threadsafe:c_int, ch:qio_channel_ptr_t):errorCode;
private extern proc qio_channel_read_bits(threadsafe:c_int, ch:qio_channel_ptr_t, ref v:uint(64), nbits:int(8)):errorCode;

private extern proc qio_locales_for_region(fl:qio_file_ptr_t,
                                   start:int(64), end:int(64),
                                   loc_names_out:c_void_ptr,
                                   ref num_locs_out:c_int):errorCode;
private extern proc qio_get_chunk(fl:qio_file_ptr_t, ref len:int(64)):errorCode;
private extern proc qio_get_fs_type(fl:qio_file_ptr_t, ref tp:c_int):errorCode;

private extern proc qio_file_path_for_fd(fd:c_int, ref path:c_string):errorCode;
private extern proc qio_file_path_for_fp(fp:c_FILE, ref path:c_string):errorCode;
private extern proc qio_file_path(f:qio_file_ptr_t, ref path:c_string):errorCode;
private extern proc qio_shortest_path(fl: qio_file_ptr_t, ref path_out:c_string, path_in:c_string):errorCode;

// we don't use qio_channel_read_int/write_int since the code there is pretty
// much a dispatch based on type and that fits better in Chapel. Doing it
// this way also happens to avoid an Intel compiler bug.

private extern proc qio_channel_read_int8(threadsafe:c_int, ch:qio_channel_ptr_t, ref ptr:int(8)):errorCode;
private extern proc qio_channel_write_int8(threadsafe:c_int, ch:qio_channel_ptr_t, x:int(8)):errorCode;

private extern proc qio_channel_read_uint8(threadsafe:c_int, ch:qio_channel_ptr_t, ref ptr:uint(8)):errorCode;
private extern proc qio_channel_write_uint8(threadsafe:c_int, ch:qio_channel_ptr_t, x:uint(8)):errorCode;


private extern proc qio_channel_read_int16(threadsafe:c_int, byteorder:c_int, ch:qio_channel_ptr_t, ref ptr:int(16)):errorCode;
private extern proc qio_channel_write_int16(threadsafe:c_int, byteorder:c_int, ch:qio_channel_ptr_t, x:int(16)):errorCode;

private extern proc qio_channel_read_uint16(threadsafe:c_int, byteorder:c_int, ch:qio_channel_ptr_t, ref ptr:uint(16)):errorCode;
private extern proc qio_channel_write_uint16(threadsafe:c_int, byteorder:c_int, ch:qio_channel_ptr_t, x:uint(16)):errorCode;

private extern proc qio_channel_read_int32(threadsafe:c_int, byteorder:c_int, ch:qio_channel_ptr_t, ref ptr:int(32)):errorCode;
private extern proc qio_channel_write_int32(threadsafe:c_int, byteorder:c_int, ch:qio_channel_ptr_t, x:int(32)):errorCode;

private extern proc qio_channel_read_uint32(threadsafe:c_int, byteorder:c_int, ch:qio_channel_ptr_t, ref ptr:uint(32)):errorCode;
private extern proc qio_channel_write_uint32(threadsafe:c_int, byteorder:c_int, ch:qio_channel_ptr_t, x:uint(32)):errorCode;

private extern proc qio_channel_read_int64(threadsafe:c_int, byteorder:c_int, ch:qio_channel_ptr_t, ref ptr:int(64)):errorCode;
private extern proc qio_channel_write_int64(threadsafe:c_int, byteorder:c_int, ch:qio_channel_ptr_t, x:int(64)):errorCode;

private extern proc qio_channel_read_uint64(threadsafe:c_int, byteorder:c_int, ch:qio_channel_ptr_t, ref ptr:uint(64)):errorCode;
private extern proc qio_channel_write_uint64(threadsafe:c_int, byteorder:c_int, ch:qio_channel_ptr_t, x:uint(64)):errorCode;

private extern proc qio_channel_read_float32(threadsafe:c_int, byteorder:c_int, ch:qio_channel_ptr_t, ref ptr:real(32)):errorCode;
private extern proc qio_channel_write_float32(threadsafe:c_int, byteorder:c_int, ch:qio_channel_ptr_t, x:real(32)):errorCode;

private extern proc qio_channel_read_float32(threadsafe:c_int, byteorder:c_int, ch:qio_channel_ptr_t, ref ptr:imag(32)):errorCode;
private extern proc qio_channel_write_float32(threadsafe:c_int, byteorder:c_int, ch:qio_channel_ptr_t, x:imag(32)):errorCode;

private extern proc qio_channel_read_float64(threadsafe:c_int, byteorder:c_int, ch:qio_channel_ptr_t, ref ptr:real(64)):errorCode;
private extern proc qio_channel_write_float64(threadsafe:c_int, byteorder:c_int, ch:qio_channel_ptr_t, x:real(64)):errorCode;

private extern proc qio_channel_read_float64(threadsafe:c_int, byteorder:c_int, ch:qio_channel_ptr_t, ref ptr:imag(64)):errorCode;
private extern proc qio_channel_write_float64(threadsafe:c_int, byteorder:c_int, ch:qio_channel_ptr_t, x:imag(64)):errorCode;

private extern proc qio_channel_read_string(threadsafe:c_int, byteorder:c_int, str_style:int(64), ch:qio_channel_ptr_t, ref s:c_string, ref len:int(64), maxlen:c_ssize_t):errorCode;
private extern proc qio_channel_write_string(threadsafe:c_int, byteorder:c_int, str_style:int(64), ch:qio_channel_ptr_t, const s:c_string, len:c_ssize_t):errorCode;

private extern proc qio_channel_scan_int(threadsafe:c_int, ch:qio_channel_ptr_t, ref ptr, len:c_size_t, issigned:c_int):errorCode;
private extern proc qio_channel_print_int(threadsafe:c_int, ch:qio_channel_ptr_t, const ref ptr, len:c_size_t, issigned:c_int):errorCode;

private extern proc qio_channel_scan_float(threadsafe:c_int, ch:qio_channel_ptr_t, ref ptr, len:c_size_t):errorCode;
private extern proc qio_channel_print_float(threadsafe:c_int, ch:qio_channel_ptr_t, const ref ptr, len:c_size_t):errorCode;

// These are the same as scan/print float but they assume an 'i' afterwards.
private extern proc qio_channel_scan_imag(threadsafe:c_int, ch:qio_channel_ptr_t, ref ptr, len:c_size_t):errorCode;
private extern proc qio_channel_print_imag(threadsafe:c_int, ch:qio_channel_ptr_t, const ref ptr, len:c_size_t):errorCode;


private extern proc qio_channel_scan_complex(threadsafe:c_int, ch:qio_channel_ptr_t, ref re_ptr, ref im_ptr, len:c_size_t):errorCode;
private extern proc qio_channel_print_complex(threadsafe:c_int, ch:qio_channel_ptr_t, const ref re_ptr, const ref im_ptr, len:c_size_t):errorCode;


private extern proc qio_channel_read_char(threadsafe:c_int, ch:qio_channel_ptr_t, ref char:int(32)):errorCode;

private extern proc qio_nbytes_char(chr:int(32)):c_int;
private extern proc qio_encode_to_string(chr:int(32)):c_string;
private extern proc qio_decode_char_buf(ref chr:int(32), ref nbytes:c_int, buf:c_string, buflen:c_ssize_t):errorCode;

private extern proc qio_channel_write_char(threadsafe:c_int, ch:qio_channel_ptr_t, char:int(32)):errorCode;
private extern proc qio_channel_skip_past_newline(threadsafe:c_int, ch:qio_channel_ptr_t, skipOnlyWs:c_int):errorCode;
private extern proc qio_channel_write_newline(threadsafe:c_int, ch:qio_channel_ptr_t):errorCode;

private extern proc qio_channel_scan_string(threadsafe:c_int, ch:qio_channel_ptr_t, ref ptr:c_string, ref len:int(64), maxlen:c_ssize_t):errorCode;
private extern proc qio_channel_scan_bytes(threadsafe:c_int, ch:qio_channel_ptr_t, ref ptr:c_string, ref len:int(64), maxlen:c_ssize_t):errorCode;
private extern proc qio_channel_print_bytes(threadsafe:c_int, ch:qio_channel_ptr_t, const ptr:c_string, len:c_ssize_t):errorCode;
private extern proc qio_channel_print_string(threadsafe:c_int, ch:qio_channel_ptr_t, const ptr:c_string, len:c_ssize_t):errorCode;

private extern proc qio_channel_scan_literal(threadsafe:c_int, ch:qio_channel_ptr_t, const match:c_string, len:c_ssize_t, skipwsbefore:c_int):errorCode;
private extern proc qio_channel_scan_literal_2(threadsafe:c_int, ch:qio_channel_ptr_t, match:c_void_ptr, len:c_ssize_t, skipwsbefore:c_int):errorCode;
private extern proc qio_channel_print_literal(threadsafe:c_int, ch:qio_channel_ptr_t, const match:c_string, len:c_ssize_t):errorCode;
private extern proc qio_channel_print_literal_2(threadsafe:c_int, ch:qio_channel_ptr_t, match:c_void_ptr, len:c_ssize_t):errorCode;

private extern proc qio_channel_skip_json_field(threadsafe:c_int, ch:qio_channel_ptr_t):errorCode;

pragma "no doc"
extern record qio_conv_t {
  var preArg1:uint(8);
  var preArg2:uint(8);
  var preArg3:uint(8);
  var argType:uint(8);
  var literal_is_whitespace:uint(8);
  var literal_length:uint(32);
  var literal:c_void_ptr;
  var regex_length:uint(32);
  var regex:c_void_ptr;
  var regex_flags_length:uint(32);
  var regex_flags:c_void_ptr;
}

private extern const QIO_CONV_UNK:c_int;

private extern const QIO_CONV_ARG_TYPE_NUMERIC:c_int;
private extern const QIO_CONV_ARG_TYPE_SIGNED:c_int;
private extern const QIO_CONV_ARG_TYPE_BINARY_SIGNED:c_int;
private extern const QIO_CONV_ARG_TYPE_UNSIGNED:c_int;
private extern const QIO_CONV_ARG_TYPE_BINARY_UNSIGNED:c_int;
private extern const QIO_CONV_ARG_TYPE_REAL:c_int;
private extern const QIO_CONV_ARG_TYPE_BINARY_REAL:c_int;
private extern const QIO_CONV_ARG_TYPE_IMAG:c_int;
private extern const QIO_CONV_ARG_TYPE_BINARY_IMAG:c_int;
private extern const QIO_CONV_ARG_TYPE_COMPLEX:c_int;
private extern const QIO_CONV_ARG_TYPE_BINARY_COMPLEX:c_int;

private extern const QIO_CONV_ARG_TYPE_CHAR:c_int;
private extern const QIO_CONV_ARG_TYPE_STRING:c_int;
private extern const QIO_CONV_ARG_TYPE_BINARY_STRING:c_int;
private extern const QIO_CONV_ARG_TYPE_REPR:c_int;
private extern const QIO_CONV_ARG_TYPE_REGEX:c_int;
private extern const QIO_CONV_ARG_TYPE_NONE_REGEX_LITERAL:c_int;
private extern const QIO_CONV_ARG_TYPE_NONE_LITERAL:c_int;

private extern const QIO_CONV_SET_MIN_WIDTH_COLS:c_int;
private extern const QIO_CONV_SET_MAX_WIDTH_COLS:c_int;
private extern const QIO_CONV_SET_MAX_WIDTH_CHARS:c_int;
private extern const QIO_CONV_SET_MAX_WIDTH_BYTES:c_int;
private extern const QIO_CONV_SET_PRECISION:c_int;
private extern const QIO_CONV_SET_STRINGLEN:c_int;
private extern const QIO_CONV_SET_TERMINATOR:c_int;
private extern const QIO_CONV_SET_STRINGSTART:c_int;
private extern const QIO_CONV_SET_STRINGSTARTEND:c_int;
private extern const QIO_CONV_SET_STRINGEND:c_int;
private extern const QIO_CONV_SET_CAPTURE:c_int;
private extern const QIO_CONV_SET_DONE:c_int;

pragma "insert line file info"
private extern proc qio_conv_parse(const fmt:c_string, start:c_size_t, ref end:uint(64), scanning:c_int, ref spec:qio_conv_t, ref style:iostyleInternal):errorCode;

private extern proc qio_format_error_too_many_args():errorCode;
private extern proc qio_format_error_too_few_args():errorCode;
private extern proc qio_format_error_arg_mismatch(arg:int):errorCode;
pragma "no doc"
extern proc qio_format_error_bad_regex():errorCode;
private extern proc qio_format_error_write_regex():errorCode;

/*
   :returns: the default I/O style. See :record:`iostyle`
             and :ref:`about-io-styles`

 */
@unstable
"defaultIOStyle is unstable due to returning an unstable type"
proc defaultIOStyle():iostyle {
  return defaultIOStyleInternal() : iostyle;
}

pragma "no doc"
proc defaultIOStyleInternal(): iostyleInternal {
  var ret:iostyleInternal;
  qio_style_init_default(ret);
  return ret;
}

/* Get an iostyleInternal indicating binary I/O in native byte order. */
pragma "no doc"
proc iostyleInternal.native(str_style:int(64)=stringStyleWithVariableLength()):iostyleInternal {
  var ret = this;
  ret.binary = 1;
  ret.byteorder = iokind.native:uint(8);
  ret.str_style = str_style;
  return ret;
}

/* Get an iostyleInternal indicating binary I/O in big-endian byte order.*/
pragma "no doc"
proc iostyleInternal.big(str_style:int(64)=stringStyleWithVariableLength()):iostyleInternal {
  var ret = this;
  ret.binary = 1;
  ret.byteorder = iokind.big:uint(8);
  ret.str_style = str_style;
  return ret;
}

/* Get an iostyleInternal indicating binary I/O in little-endian byte order. */
pragma "no doc"
proc iostyleInternal.little(str_style:int(64)=stringStyleWithVariableLength()):iostyleInternal {
  var ret = this;
  ret.binary = 1;
  ret.byteorder = iokind.little:uint(8);
  ret.str_style = str_style;
  return ret;
}

// TODO -- add arguments to this function
/* Get an iostyleInternal indicating text I/O. */
pragma "no doc"
proc iostyleInternal.text(/* args coming later */):iostyleInternal {
  var ret = this;
  ret.binary = 0;
  return ret;
}



/* fdflag_t specifies how a file can be used. It can be:
  QIO_FDFLAG_UNK,
  QIO_FDFLAG_READABLE,
  QIO_FDFLAG_WRITEABLE,
  QIO_FDFLAG_SEEKABLE
*/
pragma "no doc"
extern type fdflag_t = c_int;

//  note - QIO supports
//  QIO_METHOD_READWRITE,
//  QIO_METHOD_P_READWRITE,
//  QIO_METHOD_MMAP,
//  QIO_HINT_RANDOM,
//  QIO_HINT_SEQUENTIAL,
//  QIO_HINT_LATENCY,
//  QIO_HINT_BANDWIDTH,
//  QIO_HINT_CACHED,
//  QIO_HINT_NOREUSE

private const IOHINTS_NONE:        c_int = 0;
private const IOHINTS_SEQUENTIAL:  c_int = QIO_HINT_SEQUENTIAL;
private const IOHINTS_RANDOM:      c_int = QIO_HINT_RANDOM;
private const IOHINTS_PREFETCH:    c_int = QIO_HINT_CACHED;
private const IOHINTS_MMAP:        c_int = QIO_METHOD_MMAP;
private const IOHINTS_NOMMAP:      c_int = QIO_METHOD_PREADPWRITE;

/* A value of the :record:`ioHintSet` type defines a set of hints about
  the I/O that the file or channel will perform.  These hints may be used
  by the implementation to select optimized versions of the I/O operations.

  Most hints have POSIX equivalents associated with posix_fadvise() and
  posix_madvise().

  .. code-block:: chapel

    use IO;

    // define a set of hints using a union operation
    var hints = ioHintSet::sequential | ioHintSet::prefetch;

    // open a file using the hints
    var f: file;
    try! {
      f = open("path/to/my/file.txt", iomode.r, hints=hints);
    }
*/
record ioHintSet {
  pragma "no doc"
  var _internal : c_int;

  /* Defines an empty set, which provides no hints.
  Corresponds to 'POSIX_*_NORMAL'.
  */
  proc type empty { return new ioHintSet(IOHINTS_NONE); }

 /* Suggests that the file will be accessed sequentially.
  Corresponds to 'POSIX_*_SEQUENTIAL'
  */
  proc type sequential { return new ioHintSet(IOHINTS_SEQUENTIAL); }

  /* Suggests that the file will be accessed randomly.
  Corresponds to 'POSIX_*_RANDOM'.
  */
  proc type random { return new ioHintSet(IOHINTS_RANDOM); }

  /* Suggests that the runtime/OS should immediately begin prefetching the file contents.
  Corresponds to 'POSIX_*_WILLNEED'.
  */
  proc type prefetch { return new ioHintSet(IOHINTS_PREFETCH); }

  /* Suggests that 'mmap' should be used to access the file contents
  */
  proc type mmap { return new ioHintSet(IOHINTS_MMAP); }

  /* Suggests that 'mmap' should not be used to access the file contents.
  Instead, pread/pwrite are used.
  */
  proc type noMmap { return new ioHintSet(IOHINTS_NOMMAP); }

  pragma "no doc"
  proc type fromFlag(flag: c_int) { return new ioHintSet(flag); }
}

/* Compute the union of two ioHintSets
*/
operator ioHintSet.|(lhs: ioHintSet, rhs: ioHintSet) {
  return new ioHintSet(lhs._internal | rhs._internal);
}

/* Compute the intersection of two ioHintSets
*/
operator ioHintSet.&(lhs: ioHintSet, rhs: ioHintSet) {
  return new ioHintSet(lhs._internal & rhs._internal);
}

/* Compare two ioHintSets for equality
*/
operator ioHintSet.==(lhs: ioHintSet, rhs: ioHintSet) {
  return lhs._internal == rhs._internal;
}

/* Compare two ioHintSets for inequality
*/
operator ioHintSet.!=(lhs: ioHintSet, rhs: ioHintSet) {
  return !(lhs == rhs);
}

/*
The :record:`file` type is implementation-defined.  A value of the
:record:`file` type refers to the state that is used by the implementation to
identify and interact with the OS file.

When a :record:`file` formal argument has default intent, the actual is passed
by ``const ref`` to the formal upon a function call, and the formal
cannot be assigned within the function.

The default value of the :record:`file` type does not represent any OS file.
It is illegal to perform any I/O operations on the default value.

*/
pragma "ignore noinit"
record file {
  pragma "no doc"
  var _home: locale = here;
  pragma "no doc"
  var _file_internal:qio_file_ptr_t = QIO_FILE_PTR_NULL;

  // INIT TODO: This would be a useful case for requesting a default initializer
  // be built even when handwritten initializers (copy init) exist.
  pragma "no doc"
  proc init() {
  }
}

// TODO -- shouldn't have to write this this way!
pragma "no doc"
proc file.init(x: file) {
  this._home = x._home;
  this._file_internal = x._file_internal;
  this.complete();
  on this._home {
    qio_file_retain(_file_internal);
  }
}

pragma "no doc"
proc file.init=(x: file) {
  this.init(x);
}

pragma "no doc"
operator file.=(ref ret:file, x:file) {
  // retain -- release
  on x._home {
    qio_file_retain(x._file_internal);
  }

  on ret._home {
    qio_file_release(ret._file_internal);
  }

  // compiler will do this copy.
  ret._home = x._home;
  ret._file_internal = x._file_internal;
}

pragma "no doc"
proc file.checkAssumingLocal() throws {
  if is_c_nil(_file_internal) then
    throw createSystemError(EBADF, "Operation attempted on an invalid file");
  if !qio_file_isopen(_file_internal) then
    throw createSystemError(EBADF, "Operation attempted on closed file");
}

/* Throw an error if `this` is not a valid representation of an OS file.

   :throws SystemError: Indicates that `this` does not represent an OS file.
*/
proc file.check() throws {
  on this._home {
    this.checkAssumingLocal();
  }
}

pragma "no doc"
proc ref file.deinit() {
  on this._home {
    qio_file_release(_file_internal);
    this._file_internal = QIO_FILE_PTR_NULL;
  }
}

/*
   We could support file locking and unlocking, but
   at the moment I don't see any use case in which
   it would make sense.
proc file.lock() {
  on this._home {
    seterr(nil, qio_file_lock(_file_internal));
  }
}
proc file.unlock() {
  on this._home {
    qio_file_unlock(_file_internal);
  }
}
*/

pragma "no doc"
proc file.filePlugin() : QioPluginFile? {
  return qio_file_get_plugin(this._channel_internal);
}

// File style cannot be modified after the file is created;
// this prevents race conditions;
// channel style is protected by channel lock, can be modified.
pragma "no doc"
proc file._style:iostyleInternal throws {
  var ret:iostyleInternal;
  on this._home {
    try this.checkAssumingLocal();
    var local_style:iostyleInternal;
    qio_file_get_style(_file_internal, local_style);
    ret = local_style;
  }
  return ret;
}

/* Close a file.

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
 */
proc file.close() throws {
  if is_c_nil(_file_internal) then
    throw createSystemError(EBADF, "Operation attempted on an invalid file");

  var err:errorCode = 0;
  on this._home {
    err = qio_file_close(_file_internal);
  }
  if err then try ioerror(err, "in file.close", this._tryGetPath());
}

/*

Sync a file to disk.

Commits file data to the device associated with this file.
Data written to the file by a channel will only be guaranteed
committed if the channel has been closed or flushed.

This function will typically call the ``fsync`` system call.

:throws SystemError: Thrown if the file could not be synced.
 */
proc file.fsync() throws {
  var err:errorCode = 0;
  on this._home {
    try this.checkAssumingLocal();
    err = qio_file_sync(_file_internal);
  }
  if err then try ioerror(err, "in file.fsync", this._tryGetPath());
}

/*
  A compile-time parameter to control the behavior of :proc:`file.path`

  When 'false', the deprecated behavior is used (i.e., return the shortest of
  the relative path and the absolute path)

  When 'true', the new behavior is used (i.e., always return the absolute path)
*/
config param filePathAbsolute = false;

deprecated "The variant of `file.path` that can return a relative path is deprecated; please compile with `-sfilePathAbsolute=true` to use the strictly absolute variant"
proc file.path: string throws where !filePathAbsolute {
  return fileRelPathHelper(this);
}

/*

Get the absolute path to an open file.

Note that not all files have a path (e.g. files opened with :proc:`openmem`),
and that this function may not work on all operating systems.

The function :proc:`Path.realPath` is an alternative way
to get the path to a file.

:returns: the absolute path to the file
:rtype: ``string``

:throws SystemError: Thrown if the path could not be retrieved.
 */
proc file.path : string throws where filePathAbsolute {
  return this._abspath;
}

// helper for relative-path deprecation
pragma "no doc"
proc file._abspath: string throws {
  var ret: string;
  var err:errorCode = 0;
  on this._home {
    try this.checkAssumingLocal();
    var tmp:c_string;
    err = qio_file_path(_file_internal, tmp);
    if !err {
      ret = createStringWithNewBuffer(tmp,
                                      policy=decodePolicy.escape);
    }
    chpl_free_c_string(tmp);
  }
  if err then try ioerror(err, "in file.path");
  return ret;
}

// internal version of 'file.path' used to generate error messages in other IO methods
// produces a relative path when available
pragma "no doc"
proc file._tryGetPath() : string {
  try {
    return fileRelPathHelper(this);
  } catch {
    return "unknown";
  }
}

private proc fileRelPathHelper(f: file): string throws {
  var ret: string;
  var err:errorCode = 0;
  on f._home {
    try f.checkAssumingLocal();
    var tmp:c_string;
    var tmp2:c_string;
    err = qio_file_path(f._file_internal, tmp);
    if !err {
      err = qio_shortest_path(f._file_internal, tmp2, tmp);
    }
    chpl_free_c_string(tmp);
    if !err {
      ret = createStringWithNewBuffer(tmp2,
                                      policy=decodePolicy.escape);
    }
    chpl_free_c_string(tmp2);
  }
  if err then try ioerror(err, "in file.path");
  return ret;
}

/*

Get the current size of an open file. Note that the size can always
change if other channels, tasks or programs are writing to the file.

:returns: the current file size

:throws SystemError: Thrown if the size could not be retrieved.
*/
proc file.size: int throws {
  var err:errorCode = 0;
  var len:int(64) = 0;
  on this._home {
    err = qio_file_length(this._file_internal, len);
  }
  if err then try ioerror(err, "in file.size");
  return len;
}

// these strings are here (vs in _modestring)
// in an attempt to avoid string copies, leaks,
// and unnecessary allocations.
private param _r = "r";
private param _rw  = "r+";
private param _cw = "w";
private param _cwr = "w+";

pragma "no doc"
proc _modestring(mode:iomode) {
  import HaltWrappers;
  select mode {
    when iomode.r do return _r;
    when iomode.rw do return _rw;
    when iomode.cw do return _cw;
    when iomode.cwr do return _cwr;
    otherwise do HaltWrappers.exhaustiveSelectHalt("Invalid iomode");
  }
}

@unstable "open with a style argument is unstable"
proc open(path:string, mode:iomode, hints=ioHintSet.empty,
          style:iostyle): file throws {
  return openHelper(path, mode, hints, style:iostyleInternal);
}

/*

Open a file on a filesystem. Note that once the
file is open, you will need to use a :proc:`file.reader` or :proc:`file.writer`
to create a channel to actually perform I/O operations

:arg path: which file to open (for example, "some/file.txt").
:arg mode: specify whether to open the file for reading or writing and
             whether or not to create the file if it doesn't exist.
             See :type:`iomode`.
:arg hints: optional argument to specify any hints to the I/O system about
            this file. See :record:`ioHintSet`.
:returns: an open file to the requested resource.

:throws FileNotFoundError: Thrown if part of the provided path did not exist
:throws PermissionError: Thrown if part of the provided path had inappropriate
                         permissions
:throws NotADirectoryError: Thrown if part of the provided path was expected to
                            be a directory but was not
:throws SystemError: Thrown if the file could not be opened.
*/
proc open(path:string, mode:iomode, hints=ioHintSet.empty): file throws {
  return openHelper(path, mode, hints);
}

private proc openHelper(path:string, mode:iomode, hints=ioHintSet.empty,
                        style:iostyleInternal = defaultIOStyleInternal()): file throws {

  var local_style = style;
  var error: errorCode = 0;
  var ret: file;
  ret._home = here;

  if (path == "") then
    try ioerror(ENOENT:errorCode, "in open: path is the empty string");

  error = qio_file_open_access(ret._file_internal,
                               path.encode(policy=encodePolicy.unescape).c_str(),
                               _modestring(mode).c_str(), hints._internal, local_style);
  if error then
    try ioerror(error, "in open", path);

  return ret;
}

pragma "no doc"
proc openplugin(pluginFile: QioPluginFile, mode:iomode,
                seekable:bool, style:iostyleInternal) throws {
  import HaltWrappers;

  extern proc qio_file_init_plugin(ref file_out:qio_file_ptr_t,
                                   file_info:c_void_ptr, flags:c_int,
                                   const ref style:iostyleInternal):errorCode;

  var local_style = style;
  var ret:file;
  ret._home = here;

  var flags:c_int = 0;
  select mode {
    when iomode.r {
      flags |= QIO_FDFLAG_READABLE;
    }
    when iomode.rw {
      flags |= QIO_FDFLAG_READABLE;
      flags |= QIO_FDFLAG_WRITEABLE;
    }
    when iomode.cw {
      flags |= QIO_FDFLAG_WRITEABLE;
    }
    when iomode.cwr {
      flags |= QIO_FDFLAG_READABLE;
      flags |= QIO_FDFLAG_WRITEABLE;
    }
    otherwise do HaltWrappers.exhaustiveSelectHalt("Invalid iomode");
  }

  if seekable then
    flags |= QIO_FDFLAG_SEEKABLE;

  var err = qio_file_init_plugin(ret._file_internal, pluginFile:c_void_ptr, flags, style);
  if err {
    var path:string = "unknown";
    if pluginFile {
      var str:c_string = nil;
      var len:int;
      var path_err = pluginFile.getpath(str, len);
      if path_err {
        path = "unknown";
      } else {
        // doesn't throw with decodePolicy.replace
        path = createStringWithNewBuffer(str, len,
                                         policy=decodePolicy.replace);
      }
    }

    try ioerror(err, "in openplugin", path);
  }

  return ret;
}

@unstable "openfd with a style argument is unstable"
proc openfd(fd: c_int, hints=ioHintSet.empty, style:iostyle):file throws {
  return openfdHelper(fd, hints, style: iostyleInternal);
}

/*

Create a Chapel file that works with a system file descriptor.  Note that once
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


:arg fd: a system file descriptor.
:arg hints: optional argument to specify any hints to the I/O system about
            this file. See :record:`ioHintSet`.
:returns: an open :record:`file` using the specified file descriptor.

:throws SystemError: Thrown if the file descriptor could not be retrieved.
*/
proc openfd(fd: c_int, hints = ioHintSet.empty):file throws {
  return openfdHelper(fd, hints);
}

private proc openfdHelper(fd: c_int, hints = ioHintSet.empty,
                          style:iostyleInternal = defaultIOStyleInternal()):file throws {
  var local_style = style;
  var ret:file;
  ret._home = here;
  extern proc chpl_cnullfile():c_FILE;
  var err = qio_file_init(ret._file_internal, chpl_cnullfile(), fd, hints._internal, local_style, 0);

  // On return, either ret._file_internal.ref_cnt == 1, or ret._file_internal is NULL.
  // err should be nonzero in the latter case.
  if err {
    var path_cs:c_string;
    var path_err = qio_file_path_for_fd(fd, path_cs);
    var path = if path_err then "unknown"
                           else createStringWithNewBuffer(path_cs,
                                                          policy=decodePolicy.replace);
    try ioerror(err, "in openfd", path);
  }
  return ret;
}

@unstable "openfp with a style argument is unstable"
proc openfp(fp: c_FILE, hints=ioHintSet.empty, style:iostyle):file throws {
  return openfpHelper(fp, hints, style: iostyleInternal);
}

/*

Create a Chapel :record:`file` that wraps around an open C file. A pointer to
a C ``FILE`` object can be obtained via Chapel's
:ref:`C Interoperability <primers-C-interop-using-C>` functionality.

Once the Chapel file is created, you will need to use a :proc:`file.reader` or
:proc:`file.writer` to create a channel to perform I/O operations on the C file.

.. note::

  The resulting file value should only be used with one :record:`channel` at a
  time. The I/O system will ignore the channel offsets when reading or writing
  to a file opened with :proc:`openfp`.


:arg fp: a pointer to a C ``FILE``. See :type:`~CTypes.c_FILE`.
:arg hints: optional argument to specify any hints to the I/O system about
            this file. See :record:`ioHintSet`.
:returns: an open :record:`file` corresponding to the C file.

:throws SystemError: Thrown if the C file could not be retrieved.
 */
proc openfp(fp: c_FILE, hints=ioHintSet.empty):file throws {
  return openfpHelper(fp, hints);
}

pragma "last resort"
deprecated "'_file' is deprecated; use the variant of 'openfp' that takes a 'c_FILE' instead"
proc openfp(fp: _file, hints=ioHintSet.empty):file throws {
  return openfpHelper(fp, hints);
}

private proc openfpHelper(fp: c_FILE, hints=ioHintSet.empty,
                          style:iostyleInternal = defaultIOStyleInternal()):file throws {
  var local_style = style;
  var ret:file;
  ret._home = here;
  var err = qio_file_init(ret._file_internal, fp, -1, hints._internal, local_style, 1);

  // On return either ret._file_internal.ref_cnt == 1, or ret._file_internal is NULL.
  // error should be nonzero in the latter case.
  if err {
    var path_cs:c_string;
    var path_err = qio_file_path_for_fp(fp, path_cs);
    var path = if path_err then "unknown"
                           else createStringWithNewBuffer(path_cs,
                                                          policy=decodePolicy.replace);
    chpl_free_c_string(path_cs);
    try ioerror(err, "in openfp", path);
  }
  return ret;
}

@unstable "openTempFile with a style argument is unstable"
proc openTempFile(hints=ioHintSet.empty, style:iostyle):file throws {
  return opentmpHelper(hints, style: iostyleInternal);
}

deprecated "opentmp is deprecated, please use :proc:`openTempFile` instead"
proc opentmp(hints=ioHintSet.empty, style:iostyle):file throws {
  return opentmpHelper(hints, style: iostyleInternal);
}

/*

Open a temporary file. Note that once the file is open, you will need to use a
:proc:`file.reader` or :proc:`file.writer` to create a channel to actually
perform I/O operations.

The temporary file will be created in an OS-dependent temporary directory,
for example "/tmp" is the typical location. The temporary file will be
deleted upon closing.

Temporary files are opened with :type:`iomode` ``iomode.cwr``; that is, a new
file is created that supports both writing and reading.  When possible, it may
be opened using OS support for temporary files in order to make sure that a new
file is created only for use by the current application.

:arg hints: optional argument to specify any hints to the I/O system about
            this file. See :record:`ioHintSet`.
:returns: an open temporary file.

:throws SystemError: Thrown if the temporary file could not be opened.
*/
proc openTempFile(hints=ioHintSet.empty):file throws {
  return opentmpHelper(hints);
}

deprecated "opentmp is deprecated, please use :proc:`openTempFile` instead"
proc opentmp(hints=ioHintSet.empty):file throws {
  return opentmpHelper(hints);
}

private proc opentmpHelper(hints=ioHintSet.empty,
                           style:iostyleInternal = defaultIOStyleInternal()):file throws {
  var local_style = style;
  var ret:file;
  ret._home = here;

  // On return ret._file_internal.ref_cnt == 1.
  var err = qio_file_open_tmp(ret._file_internal, hints._internal, local_style);
  if err then try ioerror(err, "in opentmp");
  return ret;
}

@unstable "openmem with a style argument is unstable"
proc openmem(style:iostyle):file throws {
  return openmemHelper(style: iostyleInternal);
}
/*

Open a file that is backed by a buffer in memory that will not persist when the
file is closed.  Note that once the file is open, you will need to use a
:proc:`file.reader` or :proc:`file.writer` to create a channel to actually
perform I/O operations.

The resulting file supports both reading and writing.

:returns: an open memory file.

:throws SystemError: Thrown if the memory buffered file could not be opened.
*/
proc openmem():file throws {
  return openmemHelper();
}

private
proc openmemHelper(style:iostyleInternal = defaultIOStyleInternal()):file throws {
  var local_style = style;
  var ret:file;
  ret._home = here;

  // On return ret._file_internal.ref_cnt == 1.
  var err = qio_file_open_mem(ret._file_internal, QBUFFER_PTR_NULL, local_style);
  if err then try ioerror(err, "in openmem");
  return ret;
}

pragma "ignore noinit"
pragma "no doc"
record _channel {
  /*
     writing is a boolean indicating whether the channels of this type
     support writing (when `true`) or reading (when `false`).
   */
  param writing:bool;
  /*
     kind is an enum :type:`iokind` that allows narrowing
     this channel's I/O style for more efficient binary I/O.
   */
  param kind:iokind;
  /*
     locking is a boolean indicating whether it is safe to use this
     channel concurrently (when `true`).
   */
  param locking:bool;

  pragma "no doc"
  var _home:locale = here;
  pragma "no doc"
  var _channel_internal:qio_channel_ptr_t = QIO_CHANNEL_PTR_NULL;

  // The member variable _readWriteThisFromLocale is used to support
  // writeThis needing to know where the I/O started. It is a member
  // variable on channel so that calls to writeln etc within writeThis
  // can preserve this information. Not used outside of
  // calling writeThis/readThis. If _readWriteThisFromLocale != nil, then
  // we are working on a channel created for running writeThis/readThis.
  // Therefore further locking by the same task is not necessary.
  pragma "no doc"
  var _readWriteThisFromLocale = nilLocale;
}

/*

A ``fileReader`` supports sequential reading from an underlying :record:`file`
object. It can buffer data. Read operations on it might return old data. Use
:proc:`channel.flush` to control this buffering.

The :record:`fileReader` type is implementation-defined.
A value of the :record:`fileReader` type refers to the state that is used
to implement the reading operations.

When a :record:`fileReader` formal argument has default intent, the actual is
passed by ``const ref`` to the formal upon a function call, and the formal
cannot be assigned within the function.

The default value of the :record:`fileReader` type is not associated
with any file, and so cannot be used to perform I/O.

The :record:`fileReader` type is generic.

The :record:`fileReader` type supports 3 fields:

 * ``param writing: bool`` which is always ``false`` for ``fileReader``

 * ``param kind:iokind``:
   kind is an enum :type:`iokind` that allows narrowing
   this channel's I/O style for more efficient binary I/O.

 * ``param locking:bool``:
   locking is a boolean indicating whether it is safe to use this
   channel concurrently (when `true`).
*/
type fileReader;
fileReader = _channel(writing=false, ?);

/*

A ``fileWriter`` supports sequential writing to an underlying :record:`file`
object. A ``fileWriter`` can buffer data. Write operations might not have an
immediate effect. Use :proc:`channel.flush` to control this buffering.

The :record:`fileWriter` type is implementation-defined.
A value of the :record:`fileWriter` type refers to the state that is used
to implement the writing operations.

When a :record:`fileWriter` formal argument has default intent, the actual is
passed by ``const ref`` to the formal upon a function call, and the formal
cannot be assigned within the function.

The default value of the :record:`fileWriter` type is not associated with any
file, and so cannot be used to perform I/O.

The :record:`fileWriter` type is generic.

The :record:`fileWriter` type supports 3 fields:

 * ``param writing: bool`` which is always ``true`` for ``fileWriter``

 * ``param kind:iokind``:
   kind is an enum :type:`iokind` that allows narrowing
   this channel's I/O style for more efficient binary I/O.

 * ``param locking:bool``:
   locking is a boolean indicating whether it is safe to use this
   channel concurrently (when `true`).
 */
type fileWriter;
fileWriter = _channel(writing=true, ?);

/*

A channel supports either sequential reading or sequential writing to an
underlying :record:`file` object. A channel can buffer data. Read operations
on the channel might return old data. Write operations might not have an
immediate effect. Use :proc:`channel.flush` to control this buffering.

The :record:`channel` type is implementation-defined.
A value of the :record:`channel` type refers to the state that is used
to implement the channel operations.

When a :record:`channel` formal argument has default intent, the actual is passed
by ``const ref`` to the formal upon a function call, and the formal
cannot be assigned within the function.

The default value of the :record:`channel` type is not associated
with any file, and so cannot be used to perform I/O.

The :record:`channel` type is generic.

The :record:`channel` type supports 3 fields:

 * ``param writing: bool``:
   writing is a boolean indicating whether the channels of this type
   support writing (when `true`) or reading (when `false`).

 * ``param kind:iokind``:
   kind is an enum :type:`iokind` that allows narrowing
   this channel's I/O style for more efficient binary I/O.

 * ``param locking:bool``:
   locking is a boolean indicating whether it is safe to use this
   channel concurrently (when `true`).

.. note::

  The ``channel`` type is deprecated. Please use ``fileReader`` or
  ``fileWriter`` instead.
 */
deprecated "channel type is deprecated - use fileReader or fileWriter instead"
type channel = _channel;

pragma "no doc"
operator _channel.=(ref lhs:_channel, rhs:_channel) {
  if lhs.writing==true && rhs.writing==false {
    compilerError("cannot assign writing channel to reading channel");
  } else if lhs.writing==false && rhs.writing == true {
    compilerError("cannot assign reading channel to writing channel");
  }

  // retain -- release
  on rhs._home {
    qio_channel_retain(rhs._channel_internal);
  }

  on lhs._home {
    qio_channel_release(lhs._channel_internal);
  }

  lhs._home = rhs._home;
  lhs._channel_internal = rhs._channel_internal;
}

pragma "no doc"
proc _channel.init(param writing:bool, param kind:iokind, param locking:bool) {
  this.writing = writing;
  this.kind = kind;
  this.locking = locking;
}

pragma "no doc"
proc _channel.init(x: _channel) {
  compilerWarning("new _channel(otherChannel) is deprecated");
  this.writing = x.writing;
  this.kind = x.kind;
  this.locking = x.locking;
  this._home = x._home;
  this._channel_internal = x._channel_internal;
  this._readWriteThisFromLocale = x._readWriteThisFromLocale;
  this.complete();
  on x._home {
    qio_channel_retain(x._channel_internal);
  }
}

pragma "no doc"
proc _channel.init=(x: _channel) {
  if this.type.writing != ? {
    if this.type.writing==true && x.writing==false {
      compilerError("cannot init writing channel from reading channel");
    } else if this.type.writing==false && x.writing==true {
      compilerError("cannot init reading channel from writing channel");
    }
  }
  this.writing = x.writing;

  // allow the kind and locking fields to be modified in initialization
  this.kind = if this.type.kind != ? then this.type.kind else x.kind;
  this.locking = if this.type.locking != ?
                 then this.type.locking
                 else x.locking;

  this._home = x._home;
  this._channel_internal = x._channel_internal;
  this._readWriteThisFromLocale = x._readWriteThisFromLocale;
  this.complete();
  on x._home {
    qio_channel_retain(x._channel_internal);
  }
}

pragma "no doc"
operator :(rhs: _channel, type t: _channel) {
  var tmp: t = rhs; // just run init=
  return tmp;
}

//
// Note that this is effectively the initializer that the compiler
// would typically provide and that, by providing the next initializer
// below, we have to write it out manually...  A good case for having
// a means to "opt-in" to including the compiler-provided initializer?
//
pragma "no doc"
proc _channel.init(param writing:bool, param kind:iokind, param locking:bool,
                  home: locale, _channel_internal:qio_channel_ptr_t,
                  _readWriteThisFromLocale: locale) {
  this.writing = writing;
  this.kind = kind;
  this.locking = locking;
  this._home = home;
  this._channel_internal = _channel_internal;
  this._readWriteThisFromLocale = _readWriteThisFromLocale;
}

pragma "no doc"
proc _channel.init(param writing:bool, param kind:iokind, param locking:bool, f:file, out error:errorCode, hints: ioHintSet, start:int(64), end:int(64), in local_style:iostyleInternal) {
  this.init(writing, kind, locking);
  on f._home {
    this._home = f._home;
    if kind != iokind.dynamic {
      local_style.binary = true;
      local_style.byteorder = kind:uint(8);
    }
    error = qio_channel_create(this._channel_internal, f._file_internal, hints._internal, !writing, writing, start, end, local_style, 64*1024);
    // On return this._channel_internal.ref_cnt == 1.
    // Failure to check the error return code may result in a double-deletion error.
  }
}

pragma "no doc"
proc ref _channel.deinit() {
  on this._home {
    qio_channel_release(_channel_internal);
    this._channel_internal = QIO_CHANNEL_PTR_NULL;
  }
}

/*

Represents a Unicode codepoint. I/O routines (such as :proc:`channel.read`
and :proc:`channel.write`) can use arguments of this type in order to read or
write a single Unicode codepoint.

 */
record ioChar {
  /* The codepoint value */
  var ch:int(32);
  pragma "no doc"
  proc writeThis(f) throws {
    // ioChar.writeThis should not be called;
    // I/O routines should handle ioChar directly
    assert(false);
  }
}

pragma "no doc"
inline operator :(x: ioChar, type t:string) {
  var csc: c_string =  qio_encode_to_string(x.ch);
  // The caller has responsibility for freeing the returned string.
  try! {
    return createStringWithOwnedBuffer(csc);
  }
}


/*

Represents a newline character or character sequence (ie ``\n``). I/O routines
(such as :proc:`channel.read` and :proc:`channel.write`) can use arguments of
this type in order to read or write a newline. This is different from ``\n``
because an ioNewline always produces an actual newline, but in some cases
writing ``\n`` will produce an escaped string (such as ``"\n"``).

When reading an ioNewline, read routines will skip any character sequence
(including, e.g., letters and numbers) to get to the newline character unless
``skipWhitespaceOnly`` is set to true.

 */
record ioNewline {
  /*
    Normally, we will skip anything at all to get to a ``\n``,
    but if skipWhitespaceOnly is set, it will be an error
    if we run into non-space characters other than ``\n``.
   */
  var skipWhitespaceOnly: bool = false;
  pragma "no doc"
  proc writeThis(f) throws {
    // Normally this is handled explicitly in read/write.
    f.write("\n");
  }
}

pragma "no doc"
inline operator :(x: ioNewline, type t:string) {
  return "\n";
}


/*

Used to represent a constant string we want to read or write.

When writing, the ``ioLiteral`` is output without any quoting or escaping.

When reading, the ``ioLiteral`` must be matched exactly - or else the read call
will return an error for incorrectly formatted input

*/
record ioLiteral {
  /* The value of the literal */
  var val: string;
  /* Should read operations using this literal ignore and consume
     whitespace before the literal?
   */
  var ignoreWhiteSpace: bool = true;
  proc writeThis(f) throws {
    // Normally this is handled explicitly in read/write.
    f.write(val);
  }
}

pragma "no doc"
inline operator :(x: ioLiteral, type t:string) {
  return x.val;
}

/*

Represents a value with a particular bit length that we want to read or write.
The I/O will always be done in binary mode.

*/
record ioBits {
  /* The bottom ``nbits`` of v will be read or written */
  var v:uint(64);
  /* How many of the low-order bits of ``v`` should we read or write? */
  var nbits:int(8);
  pragma "no doc"
  proc writeThis(f) throws {
    // Normally this is handled explicitly in read/write.
    f.write(v);
  }
}

pragma "no doc"
inline operator :(x: ioBits, type t:string) {
  const ret = "ioBits(v=" + x.v:string + ", nbits=" + x.nbits:string + ")";
  return ret;
}

/*
 EEOF, ESHORT, and EFORMAT are internal, Chapel-specific IO error codes.
 */

private extern proc chpl_macro_int_EEOF():c_int;
/* An error code indicating the end of file has been reached (Chapel specific)
 */
pragma "no doc"
private inline proc EEOF return chpl_macro_int_EEOF():c_int;

private extern proc chpl_macro_int_ESHORT():c_int;
/* An error code indicating that the end of file or the end of the
   input was reached before the requested amount of data could be read.
   (Chapel specific)
  */
pragma "no doc"
private inline proc ESHORT return chpl_macro_int_ESHORT():c_int;

private extern proc chpl_macro_int_EFORMAT():c_int;
/* An error code indicating a format error; for example when reading a quoted
   string literal, this would be returned if we never encountered the
   opening quote. (Chapel specific)
  */
pragma "no doc"
private inline proc EFORMAT return chpl_macro_int_EFORMAT():c_int;


pragma "no doc"
proc _channel._ch_ioerror(error:errorCode, msg:string) throws {
  var path:string = "unknown";
  var offset:int(64) = -1;
  on this._home {
    var tmp_path:c_string;
    var tmp_offset:int(64);
    var err:errorCode = 0;
    err = qio_channel_path_offset(locking, _channel_internal, tmp_path, tmp_offset);
    if !err {
      // shouldn't throw
      path = createStringWithNewBuffer(tmp_path,
                                       policy=decodePolicy.replace);
      chpl_free_c_string(tmp_path);
    } else {
      tmp_offset = qio_channel_offset_unlocked(_channel_internal);
    }
    offset = tmp_offset;
  }
  try ioerror(error, msg, path, offset);
}

pragma "no doc"
proc _channel._ch_ioerror(errstr:string, msg:string) throws {
  var path:string = "unknown";
  var offset:int(64) = -1;
  on this._home {
    var tmp_path:c_string;
    var tmp_offset:int(64);
    var err:errorCode = 0;
    err = qio_channel_path_offset(locking, _channel_internal, tmp_path, tmp_offset);
    if !err {
      // shouldn't throw
      path = createStringWithNewBuffer(tmp_path,
                                       policy=decodePolicy.replace);
      chpl_free_c_string(tmp_path);
    } else {
      tmp_offset = qio_channel_offset_unlocked(_channel_internal);
    }
    offset = tmp_offset;
  }
  try ioerror(errstr, msg, path, offset);
}

/*
   Acquire a channel's lock.

   :throws SystemError: Thrown if the lock could not be acquired.
 */
inline proc _channel.lock() throws {
  var err:errorCode = 0;

  if is_c_nil(_channel_internal) then
    throw createSystemError(EINVAL, "Operation attempted on an invalid channel");

  if locking {
    on this._home {
      err = qio_channel_lock(_channel_internal);
    }
  }
  if err then try this._ch_ioerror(err, "in lock");
}

/*
   Release a channel's lock.
 */
inline proc _channel.unlock() {
  if locking {
    on this._home {
      qio_channel_unlock(_channel_internal);
    }
  }
}

/*
   Return the current offset of a channel.

   .. warning::

      If the channel can be used by multiple tasks, take care
      when doing operations that rely on the channel's current offset.
      To prevent race conditions, first lock the channel with
      :proc:`channel.lock`, do the operations with :proc:`channel._offset`
      instead, then unlock it with :proc:`channel.unlock`.

   :returns: the current offset of the channel
 */
proc _channel.offset():int(64) {
  var ret:int(64);
  on this._home {
    try! this.lock();
    ret = qio_channel_offset_unlocked(_channel_internal);
    this.unlock();
  }
  return ret;
}

/*
   Move a channel offset forward.

   For a reading channel, this function will consume the next ``amount``
   bytes. If EOF is reached, the channel position may be left at the
   EOF.

   For a writing channel, this function will write ``amount`` zeros - or some
   other data if it is stored in the channel's buffer, for example with
   :proc:`channel._mark` and :proc:`channel._revert`.

   :throws EofError: If EOF is reached before the requested number of bytes can be consumed.
   :throws SystemError: For other failures, for which channel offset is not moved.
 */
proc _channel.advance(amount:int(64)) throws {
  var err:errorCode = 0;
  on this._home {
    try this.lock(); defer { this.unlock(); }
    err = qio_channel_advance(false, _channel_internal, amount);
  }
  if err then try this._ch_ioerror(err, "in advance");
}

/*
   Reads until ``byte`` is found and then leave the channel offset
   just after it.

   :throws UnexpectedEofError: if the requested `byte` could not be found.
   :throws SystemError: if another error occurred.
 */
proc _channel.advancePastByte(byte:uint(8)) throws {
  var err:errorCode = 0;
  on this._home {
    try this.lock(); defer { this.unlock(); }
    err = qio_channel_advance_past_byte(false, _channel_internal, byte:c_int);
  }
  if err then try this._ch_ioerror(err, "in advanceToByte");
}

/*
   *Mark* a channel - that is, save the current offset of the channel
   on its *mark stack*. This function can only be called on a channel
   with ``locking==false``.

   The *mark stack* stores several channel offsets. For any channel offset that
   is between the minimum and maximum value in the *mark stack*, I/O operations
   on the channel will keep that region of the file buffered in memory so that
   those operations can be undone. As a result, it is possible to perform *I/O
   transactions* on a channel. The basic steps for an *I/O transaction* are:

    * *mark* the current position with :proc:`channel.mark`
    * do something speculative (e.g. try to read 200 bytes of anything followed
      by a 'B')
    * if the speculative operation was successful, commit the changes by
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
 */
proc _channel.mark() throws where this.locking == false {
  const offset = this.offset();
  const err = qio_channel_mark(false, _channel_internal);

  if err then
    throw createSystemError(err);

  return offset;
}

/*
   Abort an *I/O transaction*. See :proc:`channel.mark`. This function
   will pop the last element from the *mark stack* and then leave the
   previous channel offset unchanged.  This function can only be
   called on a channel with ``locking==false``.
*/
inline proc _channel.revert() where this.locking == false {
  qio_channel_revert_unlocked(_channel_internal);
}

/*
   Commit an *I/O transaction*. See :proc:`channel.mark`.  This
   function will pop the last element from the *mark stack* and then
   set the channel offset to the popped offset.  This function can
   only be called on a channel with ``locking==false``.

*/
inline proc _channel.commit() where this.locking == false {
  qio_channel_commit_unlocked(_channel_internal);
}

/* Used to control the behavior of the region argument for
   :proc:`channel.seek`.  When set to ``true``, the region argument will fully
   specify the bounds of the seek.  When set to ``false``, the region argument
   will exclude the high bound.  Defaults to ``false``, the original behavior.
 */
config param useNewSeekRegionBounds = false;

/*
   Reset a channel to point to a new part of a file.
   This function allows one to jump to a different part of a
   file without creating a new channel. It can only be called
   on a channel with ``locking==false``.

   The region specified must have a lower bound, but does not need to have
   a high bound for correctness.  Specifying the high bound might be
   an important performance optimization since the channel will not
   try to read data outside of the region.

   This function will, in most cases, discard the channel's buffer.
   When writing, the data will be saved to the file before discarding.

   :arg region: the new region, measured in bytes and counting from 0

   .. warning::

      The region argument will ignore any specified stride other than 1.

   :throws SystemError: if seeking failed. Possible reasons include
                         that the file is not seekable, or that the
                         channel is marked.
   :throws IllegalArgumentError: if region argument did not have a lower bound
 */
proc _channel.seek(region: range(?)) throws where (!region.hasHighBound() ||
                                                   useNewSeekRegionBounds) {

  if this.locking then
    compilerError("Cannot seek on a locking channel");

  if (!region.hasLowBound()) {
    throw new IllegalArgumentError("region", "must have a lower bound");

  } else {
    if (region.hasHighBound()) {
      const err = qio_channel_seek(_channel_internal, region.low,
                                   region.high + 1);

      if err then
        throw createSystemError(err);

    } else {
      const err = qio_channel_seek(_channel_internal, region.low, max(int(64)));

      if err then
        throw createSystemError(err);
    }
  }
}

deprecated "Currently the region argument's high bound specifies the first location in the file that is not included.  This behavior is deprecated, please compile your program with `-suseNewSeekRegionBounds=true` to have the region argument specify the entire segment of the file covered, inclusive."
proc _channel.seek(region: range(?)) throws where (region.hasHighBound() &&
                                                   !useNewSeekRegionBounds) {
  if (!region.hasLowBound()) {
    throw new IllegalArgumentError("region", "must have a lower bound");

  } else {
    const err = qio_channel_seek(_channel_internal, region.low, region.high);

    if err then
      throw createSystemOrChplError(err);
  }
}


// These begin with an _ to indicated that
// you should have a lock before you use these... there is probably
// a better name for them...

/*
   For a channel locked with :proc:`channel.lock`, return the offset
   of that channel.
 */
proc _channel._offset():int(64) {
  var ret:int(64);
  on this._home {
    ret = qio_channel_offset_unlocked(_channel_internal);
  }
  return ret;
}

/*
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
 */
proc _channel._mark() throws {
  const offset = this.offset();
  const err = qio_channel_mark(false, _channel_internal);

  if err then
    throw createSystemError(err);

  return offset;
}

/*
   Abort an *I/O transaction*. See :proc:`channel._mark`.  This
   function will pop the last element from the *mark stack* and then
   leave the previous channel offset unchanged.  This function should
   only be called on a channel that has already been locked and
   marked.
*/
inline proc _channel._revert() {
  qio_channel_revert_unlocked(_channel_internal);
}

/*
   Commit an *I/O transaction*. See :proc:`channel._mark`.  This
   function will pop the last element from the *mark stack* and then
   set the channel offset to the popped offset.  This function should
   only be called on a channel that has already been locked and
   marked.
*/
inline proc _channel._commit() {
  qio_channel_commit_unlocked(_channel_internal);
}

// TODO -- come up with better names for these
/*

   Return the current style used by a channel. This function should only be
   called on a locked channel.

 */
@unstable "channel._style is unstable because it returns a type that is unstable"
proc _channel._style():iostyle {
  var ret:iostyle;
  on this._home {
    var local_style:iostyle;
    qio_channel_get_style(_channel_internal, local_style);
    ret = local_style;
  }
  return ret;
}

// Replacement for channel._style(), but it really shouldn't be used by users,
// it will get replaced at some point.
pragma "no doc"
proc _channel._styleInternal(): iostyleInternal {
  var ret:iostyleInternal;
  on this._home {
    var local_style:iostyleInternal;
    qio_channel_get_style(_channel_internal, local_style);
    ret = local_style;
  }
  return ret;
}

/*

   Set the style associated with a channel. This function should only
   be called on a locked channel.

 */
@unstable "channel._set_style is unstable because its purpose involves an unstable type"
proc _channel._set_style(style:iostyle) {
  on this._home {
    var local_style:iostyle = style;
    qio_channel_set_style(_channel_internal, local_style);
  }
}

// Replacement for channel._set_style(), but it really shouldn't be used by
// users, it will get replaced at some point.
pragma "no doc"
proc _channel._set_styleInternal(style: iostyleInternal) {
  on this._home {
    var local_style:iostyleInternal = style;
    qio_channel_set_style(_channel_internal, local_style);
  }
}

/*

   Return the locale on which an ongoing I/O was started with a channel.
   This method will return ``nilLocale`` unless it is called on a channel that is
   the formal argument to a `readThis` or `writeThis` method.

 */
inline
proc _channel.readWriteThisFromLocale() {
  return _readWriteThisFromLocale;
}

// Returns the original locale that the I/O started on
// Uses readWriteThisFromLocale in order to propagate that
// information across readThis/writeThis calls.
// If readWriteThisFromLocale returns nil, that means the channel
// was not created to call readThis/writeThis and
// so the original locale of the I/O is `here`.
pragma "no doc"
inline
proc _channel.getLocaleOfIoRequest() {
  var ret = this.readWriteThisFromLocale();
  if ret == nilLocale then
    ret = here;
  return ret;
}

// QIO plugins don't have stable interface yet, hence no-doc
// only works when called on locale owning channel.
pragma "no doc"
proc _channel.channelPlugin() : borrowed QioPluginChannel? {
  var vptr = qio_channel_get_plugin(this._channel_internal);
  return vptr:borrowed QioPluginChannel?;
}
pragma "no doc"
proc _channel.filePlugin() : borrowed QioPluginFile? {
  var vptr = qio_file_get_plugin(qio_channel_get_file(this._channel_internal));
  return vptr:borrowed QioPluginFile?;
}

@unstable "openreader with a style argument is unstable"
proc openreader(path:string,
                param kind=iokind.dynamic, param locking=true,
                start:int(64) = 0, end:int(64) = max(int(64)),
                hints=ioHintSet.empty,
                style:iostyle)
    : fileReader(kind, locking) throws {
  return openreaderHelper(path, kind, locking, start..end, hints,
                          style: iostyleInternal);
}

/* Used to control the behavior of the region argument for :proc:`openreader`.
   When set to ``true``, the region argument will fully specify the bounds of
   the :type:`fileReader`.  When set to ``false``, the region argument will
   exclude the high bound.  Defaults to ``false``, the original behavior.
 */
config param useNewOpenReaderRegionBounds = false;

// We can simply call channel.close() on these, since the underlying file will
// be closed once we no longer have any references to it (which in this case,
// since we only will have one reference, will be right after we close this
// channel presumably).

/*

Open a file at a particular path and return a reading channel for it.
This function is equivalent to calling :proc:`open` and then
:proc:`file.reader` on the resulting file.

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
:arg region: zero-based byte offset indicating where in the file the
            channel should start and stop reading. Defaults to
            ``0..``, meaning from the start of the file to no specified end
            point.
:arg hints: optional argument to specify any hints to the I/O system about
            this file. See :record:`ioHintSet`.
:returns: an open reading channel to the requested resource.

.. warning::

   The region argument will ignore any specified stride other than 1.

:throws FileNotFoundError: Thrown if part of the provided path did not exist
:throws PermissionError: Thrown if part of the provided path had inappropriate
                         permissions
:throws NotADirectoryError: Thrown if part of the provided path was expected to
                            be a directory but was not
:throws SystemError: Thrown if a reading channel could not be returned.
:throws IllegalArgumentError: Thrown if trying to read explicitly prior to byte
                              0.
 */
proc openreader(path:string,
                param kind=iokind.dynamic, param locking=true,
                region: range(?) = 0.., hints=ioHintSet.empty)
    : fileReader(kind, locking) throws where (!region.hasHighBound() ||
                                              useNewOpenReaderRegionBounds) {
  return openreaderHelper(path, kind, locking, region, hints);
}

deprecated "Currently the region argument's high bound specifies the first location in the file that is not included.  This behavior is deprecated, please compile your program with `-suseNewOpenReaderRegionBounds=true` to have the region argument specify the entire segment of the file covered, inclusive."
proc openreader(path:string,
                param kind=iokind.dynamic, param locking=true,
                region: range(?) = 0.., hints=ioHintSet.empty)
    : fileReader(kind, locking) throws where (region.hasHighBound() &&
                                              !useNewOpenReaderRegionBounds) {
  return openreaderHelper(path, kind, locking, region, hints);
}

private proc openreaderHelper(path:string,
                              param kind=iokind.dynamic, param locking=true,
                              region: range(?) = 0..,
                              hints=ioHintSet.empty,
                              style:iostyleInternal = defaultIOStyleInternal())
  : fileReader(kind, locking) throws {

  var fl:file = try open(path, iomode.r);
  return try fl.readerHelper(kind, locking, region, hints, style,
                             fromOpenReader=true);
}

@unstable "openwriter with a style argument is unstable"
proc openwriter(path:string,
                param kind=iokind.dynamic, param locking=true,
                start:int(64) = 0, end:int(64) = max(int(64)),
                hints=ioHintSet.empty,
                style:iostyle)
    : fileWriter(kind, locking) throws {
  return openwriterHelper(path, kind, locking, start, end, hints,
                    style: iostyleInternal);
}

/*

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
:arg hints: optional argument to specify any hints to the I/O system about
            this file. See :record:`ioHintSet`.
:returns: an open writing channel to the requested resource.

:throws FileNotFoundError: Thrown if part of the provided path did not exist
:throws PermissionError: Thrown if part of the provided path had inappropriate
                         permissions
:throws NotADirectoryError: Thrown if part of the provided path was expected to
                            be a directory but was not
:throws SystemError: Thrown if a writing channel could not be returned.
*/
proc openwriter(path:string,
                param kind=iokind.dynamic, param locking=true,
                hints = ioHintSet.empty)
    : fileWriter(kind, locking) throws {
  return openwriterHelper(path, kind, locking, hints=hints);
}

private proc openwriterHelper(path:string,
                              param kind=iokind.dynamic, param locking=true,
                              start:int(64) = 0, end:int(64) = max(int(64)),
                              hints = ioHintSet.empty,
                              style:iostyleInternal = defaultIOStyleInternal())
  : fileWriter(kind, locking) throws {

  var fl:file = try open(path, iomode.cw);
  return try fl.writerHelper(kind, locking, start..end, hints, style);
}

@unstable "reader with a style argument is unstable"
proc file.reader(param kind=iokind.dynamic, param locking=true,
                 start:int(64) = 0, end:int(64) = max(int(64)),
                 hints=ioHintSet.empty,
                 style:iostyle): fileReader(kind, locking)
                 throws {
  return this.readerHelper(kind, locking, start..end, hints, style: iostyleInternal);
}

/* Used to control the behavior of the region argument for :proc:`file.reader`.
   When set to ``true``, the region argument will fully specify the bounds of
   the :type:`fileReader`.  When set to ``false``, the region argument will
   exclude the high bound.  Defaults to ``false``, the original behavior.
 */
config param useNewFileReaderRegionBounds = false;

/*
   Create a :record:`fileReader` that supports reading from a file. See
   :ref:`about-io-overview`.

   The ``region=`` argument defines the portion of the file that the fileReader
   will read from.  This is a byte offset; the beginning of the file is at the
   offset 0.  The default for this argument enables the fileReader to access the
   entire file.

   A fileReader will never read beyond its maximum end position. In addition,
   reading from a fileReader beyond the end of the underlying file will not
   extend that file.  Reading beyond the end of the file or beyond the end
   offset of the fileReader will produce the error ``EEOF`` (and return `false`
   in many cases such as :proc:`channel.read`) to indicate that the end was
   reached.

   :arg kind: :type:`iokind` compile-time argument to determine the
              corresponding parameter of the :record:`fileReader` type. Defaults
              to ``iokind.dynamic``, meaning that the associated
              :record:`iostyle` controls the formatting choices.
   :arg locking: compile-time argument to determine whether or not the
                 channel should use locking; sets the
                 corresponding parameter of the :record:`fileReader` type.
                 Defaults to true, but when safe, setting it to false
                 can improve performance.
   :arg region: zero-based byte offset indicating where in the file the
               channel should start and stop reading. Defaults to
               ``0..`` - meaning from the start of the file to no end point.
   :arg hints: provide hints about the I/O that this channel will perform. See
               :record:`ioHintSet`. The default value of `ioHintSet.empty`
               will cause the channel to use the hints provided when the
               file was opened.

   .. warning::

      The region argument will ignore any specified stride other than 1.

   :throws SystemError: Thrown if a file reader channel could not be returned.
   :throws IllegalArgumentError: Thrown if trying to read explicitly prior to
                                 byte 0.
 */
proc file.reader(param kind=iokind.dynamic, param locking=true,
                 region: range(?) = 0.., hints = ioHintSet.empty)
  : fileReader(kind, locking) throws where (!region.hasHighBound() ||
                                            useNewFileReaderRegionBounds) {
  return this.readerHelper(kind, locking, region, hints);
}

deprecated "Currently the region argument's high bound specifies the first location in the file that is not included.  This behavior is deprecated, please compile your program with `-suseNewFileReaderRegionBounds=true` to have the region argument specify the entire segment of the file covered, inclusive."
proc file.reader(param kind=iokind.dynamic, param locking=true,
                 region: range(?) = 0.., hints = ioHintSet.empty)
  : fileReader(kind, locking) throws where (region.hasHighBound() &&
                                            !useNewFileReaderRegionBounds) {
  return this.readerHelper(kind, locking, region, hints);
}

// TODO: remove fromOpenReader and fromOpenUrlReader when deprecated versions
// are removed
pragma "no doc"
proc file.readerHelper(param kind=iokind.dynamic, param locking=true,
                       region: range(?) = 0.., hints = ioHintSet.empty,
                       style:iostyleInternal = this._style,
                       fromOpenReader=false, fromOpenUrlReader=false)
  : fileReader(kind, locking) throws {
  if (region.hasLowBound() && region.low < 0) {
    throw new IllegalArgumentError("region", "file region's lowest accepted bound is 0");
  }

  // It is the responsibility of the caller to release the returned channel
  // if the error code is nonzero.
  // The return error code should be checked to avoid double-deletion errors.
  var ret:fileReader(kind, locking);
  var err:errorCode = 0;
  on this._home {
    try this.checkAssumingLocal();
    if (region.hasLowBound() && region.hasHighBound()) {
      // This is to ensure the user sees consistent behavior and can control it.
      // - All calls from `openUrlReader` should use the new behavior.
      // - Only calls from `openreader` that are compiled with the flag that
      //   controls its region argument should affect its behavior.
      // - Only calls from `file.reader` that are compiled with the flag that
      //   controls its region argument should affect its behavior.
      // Calls from `openreader` should not be impacted by `file.reader`'s flag
      // and vice versa.
      if ((fromOpenReader && useNewOpenReaderRegionBounds) ||
          fromOpenUrlReader ||
          (!fromOpenReader && useNewFileReaderRegionBounds)) {
        ret = new fileReader(kind, locking, this, err, hints, region.low,
                             region.high + 1, style);
      } else {
        ret = new fileReader(kind, locking, this, err, hints, region.low,
                             region.high, style);
      }

    } else if (region.hasLowBound()) {
      ret = new fileReader(kind, locking, this, err, hints, region.low,
                           max(int(64)), style);

    } else if (region.hasHighBound()) {
      // This is to ensure the user sees consistent behavior and can control it.
      // - All calls from `openUrlReader` should use the new behavior.
      // - Only calls from `openreader` that are compiled with the flag that
      //   controls its region argument should affect its behavior.
      // - Only calls from `file.reader` that are compiled with the flag that
      //   controls its region argument should affect its behavior.
      // Calls from `openreader` should not be impacted by `file.reader`'s flag
      // and vice versa.
      if ((fromOpenReader && useNewOpenReaderRegionBounds) ||
          fromOpenUrlReader ||
          (!fromOpenReader && useNewFileReaderRegionBounds)) {
        ret = new fileReader(kind, locking, this, err, hints, 0,
                             region.high + 1, style);
      } else {
        ret = new fileReader(kind, locking, this, err, hints, 0, region.high,
                             style);
      }

    } else {
      ret = new fileReader(kind, locking, this, err, hints, 0, max(int(64)),
                           style);
    }
  }
  if err then try ioerror(err, "in file.reader", this._tryGetPath());

  return ret;
}

@unstable "lines with a local_style argument is unstable"
proc file.lines(param locking:bool = true, start:int(64) = 0,
                end:int(64) = max(int(64)), hints=ioHintSet.empty,
                in local_style:iostyle) throws {
  return this.linesHelper(locking, start..end, hints,
                          local_style: iostyleInternal);
}

/* Used to control the behavior of the region argument for :proc:`file.lines`.
   When set to ``true``, the region argument will fully specify the bounds that
   this function would cover.  When set to ``false``, the region argument will
   exclude the high bound.  Defaults to ``false``, the original behavior.
 */
config param useNewLinesRegionBounds = false;

/* Iterate over all of the lines in a file.

   :returns: an object which yields strings read from the file

   :throws SystemError: Thrown if an object could not be returned.
 */
deprecated "`file.lines` is deprecated; please use `fileReader.lines` instead"
proc file.lines(param locking:bool = true, region: range(?) = 0..,
                hints = ioHintSet.empty) throws where (!region.hasHighBound() ||
                                                       useNewLinesRegionBounds) {
  return this.linesHelper(locking, region, hints);
}

deprecated "Currently the region argument's high bound specifies the first location in the file that is not included.  This behavior is deprecated, please compile your program with `-suseNewLinesRegionBounds=true` to have the region argument specify the entire segment of the file covered, inclusive."
proc file.lines(param locking:bool = true, region: range(?) = 0..,
                hints = ioHintSet.empty) throws where (region.hasHighBound() &&
                                                       !useNewLinesRegionBounds) {
  return this.linesHelper(locking, region, hints);
}


pragma "no doc"
proc file.linesHelper(param locking:bool = true, region: range(?) = 0..,
                      hints = ioHintSet.empty,
                      in local_style:iostyleInternal = this._style) throws {
  local_style.string_format = QIO_STRING_FORMAT_TOEND;
  local_style.string_end = 0x0a; // '\n'
  param kind = iokind.dynamic;

  var ret:itemReaderInternal(string, kind, locking);
  var err:errorCode = 0;
  on this._home {
    try this.checkAssumingLocal();
    var ch: fileReader;
    if (region.hasLowBound() && region.hasHighBound()) {
      if (useNewLinesRegionBounds) {
        ch = new fileReader(kind, locking, this, err, hints, region.low,
                            region.high + 1, local_style);

      } else {
        ch = new fileReader(kind, locking, this, err, hints, region.low,
                            region.high, local_style);
      }

    } else if (region.hasLowBound()) {
      ch = new fileReader(kind, locking, this, err, hints, region.low,
                          max(int(64)), local_style);

    } else if (region.hasHighBound()) {
      if (useNewLinesRegionBounds) {
        ch = new fileReader(kind, locking, this, err, hints, 0, region.high + 1,
                            local_style);

      } else {
        ch = new fileReader(kind, locking, this, err, hints, 0, region.high,
                            local_style);
      }

    } else {
      ch = new fileReader(kind, locking, this, err, hints, 0, max(int(64)),
                          local_style);
    }
    ret = new itemReaderInternal(string, kind, locking, ch);
  }
  if err then try ioerror(err, "in file.lines", this._tryGetPath());

  return ret;
}

@unstable "writer with a style argument is unstable"
proc file.writer(param kind=iokind.dynamic, param locking=true,
                 start:int(64) = 0, end:int(64) = max(int(64)),
                 hints=ioHintSet.empty, style:iostyle):
                 fileWriter(kind,locking) throws {
  return this.writerHelper(kind, locking, start..end, hints, style: iostyleInternal);
}

/* Used to control the behavior of the region argument for :proc:`file.writer`.
   When set to ``true``, the region argument will fully specify the bounds of
   the :record:`fileWriter`.  When set to ``false``, the region argument will
   exclude the high bound.  Defaults to ``false``, the original behavior.

 */
config param useNewFileWriterRegionBounds = false;

/*
   Create a :record:`fileWriter` that supports writing to a file. See
   :ref:`about-io-overview`.

   The ``region=`` argument defines the portion of the file that the fileWriter
   will write to.  This is a byte offset; the beginning of the file is at the
   offset 0.  The default for this argument enables the fileWriter to access the
   entire file.

   When a fileWriter writes to a file, it will replace file data that was
   previously stored at the relevant offset. If the offset is beyond the
   end of the file, the file will be extended.

   A fileWriter will never write beyond its maximum end position.  It will
   extend the file only as necessary to store data written to the fileWriter. In
   other words, specifying the high bound of the region argument here does not
   impact the file size directly; it impacts only the section of the file that
   this fileWriter can write to. After all fileWriters to a file are closed,
   that file will have a size equal to the last position written to by any
   fileWriter.

   :arg kind: :type:`iokind` compile-time argument to determine the
              corresponding parameter of the :record:`fileWriter` type. Defaults
              to ``iokind.dynamic``, meaning that the associated
              :record:`iostyle` controls the formatting choices.
   :arg locking: compile-time argument to determine whether or not the
                 channel should use locking; sets the
                 corresponding parameter of the :record:`fileWriter` type.
                 Defaults to true, but when safe, setting it to false
                 can improve performance.
   :arg region: zero-based byte offset indicating where in the file the
               fileWriter should start and stop writing. Defaults to
               ``0..`` - meaning from the start of the file to no specified end
               point.
   :arg hints: provide hints about the I/O that this channel will perform. See
               :record:`ioHintSet`. The default value of `ioHintSet.empty`
               will cause the fileWriter to use the hints provided when the
               file was opened.

   .. warning::

      The region argument will ignore any specified stride other than 1.

   :throws SystemError: Thrown if a file writer channel could not be returned.
   :throws IllegalArgumentError: Thrown if trying to write explicitly prior to
                                 byte 0.
 */
proc file.writer(param kind=iokind.dynamic, param locking=true,
                 region: range(?) = 0.., hints = ioHintSet.empty):
                 fileWriter(kind,locking) throws where (!region.hasHighBound() ||
                                                        useNewFileWriterRegionBounds) {
  return this.writerHelper(kind, locking, region, hints);
}

deprecated "Currently the region argument's high bound specifies the first location in the file that is not included.  This behavior is deprecated, please compile your program with `-suseNewFileWriterRegionBounds=true` to have the region argument specify the entire segment of the file covered, inclusive."
proc file.writer(param kind=iokind.dynamic, param locking=true,
                 region: range(?) = 0.., hints = ioHintSet.empty):
                 fileWriter(kind,locking) throws where (region.hasHighBound() &&
                                                        !useNewFileWriterRegionBounds) {
  return this.writerHelper(kind, locking, region, hints);
}

pragma "no doc"
proc file.writerHelper(param kind=iokind.dynamic, param locking=true,
                       region: range(?) = 0.., hints = ioHintSet.empty,
                       style:iostyleInternal = this._style,
                       fromOpenUrlWriter = false):
  fileWriter(kind,locking) throws {

  if (region.hasLowBound() && region.low < 0) {
    throw new IllegalArgumentError("region", "file region's lowest accepted bound is 0");
  }

  // It is the responsibility of the caller to retain and release the returned
  // channel.
  // If the return error code is nonzero, the ref count will be 0 not 1.
  // The error code should be checked to avoid double-deletion errors.
  var ret:fileWriter(kind, locking);
  var err:errorCode = 0;
  on this._home {
    try this.checkAssumingLocal();
    if (region.hasLowBound() && region.hasHighBound()) {
      // TODO: remove the fromOpenUrlWriter arg when the deprecated version is
      // removed
      if (useNewFileWriterRegionBounds || fromOpenUrlWriter) {
        ret = new fileWriter(kind, locking, this, err, hints, region.low,
                             region.high + 1, style);
      } else {
        ret = new fileWriter(kind, locking, this, err, hints, region.low,
                             region.high, style);
      }

    } else if (region.hasLowBound()) {
      ret = new fileWriter(kind, locking, this, err, hints, region.low,
                           max(int(64)), style);

    } else if (region.hasHighBound()) {
      if (useNewFileWriterRegionBounds || fromOpenUrlWriter) {
        ret = new fileWriter(kind, locking, this, err, hints, 0,
                             region.high + 1, style);
      } else {
        ret = new fileWriter(kind, locking, this, err, hints, 0, region.high,
                             style);
      }

    } else {
      ret = new fileWriter(kind, locking, this, err, hints, 0, max(int(64)),
                           style);
    }
  }
  if err then try ioerror(err, "in file.writer", this._tryGetPath());

  return ret;
}

pragma "no doc"
proc _isSimpleIoType(type t) param return
  isBoolType(t) || isNumericType(t) || isEnumType(t);

pragma "no doc"
proc _isIoPrimitiveType(type t) param return
  _isSimpleIoType(t) || (t == string) || (t == bytes);

pragma "no doc"
 proc _isIoPrimitiveTypeOrNewline(type t) param return
  _isIoPrimitiveType(t) || t == ioNewline || t == ioLiteral || t == ioChar || t == ioBits;

// Read routines for all primitive types.
private proc _read_text_internal(_channel_internal:qio_channel_ptr_t,
    ref x:?t):errorCode where _isIoPrimitiveType(t) {
  if isBoolType(t) {
    var err:errorCode = 0;
    var got:bool = false;

    err = qio_channel_scan_literal(false, _channel_internal, c"true", "true".numBytes:c_ssize_t, 1);
    if !err {
      got = true;
    } else if err == EFORMAT {
      // try reading false instead.
      err = qio_channel_scan_literal(false, _channel_internal, c"false", "false".numBytes:c_ssize_t, 1);
      // got is already false, so we don't need to set it.
    }
    if !err then x = got;
    return err;
  } else if isIntegralType(t) {
    // handles int types
    return qio_channel_scan_int(false, _channel_internal, x, numBytes(t), isIntType(t));
  } else if isRealType(t) {
    // handles real
    return qio_channel_scan_float(false, _channel_internal, x, numBytes(t));
  } else if isImagType(t) {
    return qio_channel_scan_imag(false, _channel_internal, x, numBytes(t));
  } else if isComplexType(t)  {
    // handle complex types
    var re:x.re.type;
    var im:x.im.type;
    var err:errorCode = 0;
    err = qio_channel_scan_complex(false, _channel_internal, re, im, numBytes(x.re.type));
    x = (re, im):t; // cast tuple to complex to get complex num.
    return err;
  } else if t == string {
    // handle c_string and string
    var len:int(64);
    var tx: c_string;
    var ret = qio_channel_scan_string(false, _channel_internal, tx, len, -1);
    x = try! createStringWithOwnedBuffer(tx, length=len);
    return ret;
  } else if t == bytes {
    // handle _bytes
    var len:int(64);
    var tx: c_string;
    var ret = qio_channel_scan_bytes(false, _channel_internal, tx, len, -1);
    x = createBytesWithOwnedBuffer(tx, length=len);
    return ret;
  } else if isEnumType(t) {
    var err:errorCode = 0;
    var st = qio_channel_style_element(_channel_internal, QIO_STYLE_ELEMENT_AGGREGATE);
    for i in t {
      { // try to read e.g. red for colorenum.red
        var str = i:string;
        if st == QIO_AGGREGATE_FORMAT_JSON then str = '"'+str+'"';
        var slen:c_ssize_t = str.numBytes.safeCast(c_ssize_t);
        err = qio_channel_scan_literal(false, _channel_internal, str.c_str(), slen, 1);
        if !err {
          x = i;
          break;
        } else if err != EFORMAT then break;
      }

      { // try to read e.g. colorenum.red for colorenum.red
        var str = t:string + "." + i:string;
        if st == QIO_AGGREGATE_FORMAT_JSON then str = '"'+str+'"';
        var slen:c_ssize_t = str.numBytes.safeCast(c_ssize_t);
        err = qio_channel_scan_literal(false, _channel_internal, str.c_str(), slen, 1);
        if !err {
          x = i;
          break;
        } else if err != EFORMAT then break;
      }
    }
    return err;
  } else {
    compilerError("Unknown primitive type in _read_text_internal ", t:string);
  }
  return EINVAL;
}

private proc _write_text_internal(_channel_internal:qio_channel_ptr_t, x:?t):errorCode where _isIoPrimitiveType(t) {
  if isBoolType(t) {
    if x {
      return qio_channel_print_literal(false, _channel_internal, c"true", "true".numBytes:c_ssize_t);
    } else {
      return qio_channel_print_literal(false, _channel_internal, c"false", "false".numBytes:c_ssize_t);
    }
  } else if isIntegralType(t) {
    // handles int types
    return qio_channel_print_int(false, _channel_internal, x, numBytes(t), isIntType(t));

  } else if isRealType(t) {
    // handles real
    return qio_channel_print_float(false, _channel_internal, x, numBytes(t));
  } else if isImagType(t) {
    return qio_channel_print_imag(false, _channel_internal, x, numBytes(t));
  } else if isComplexType(t)  {
    // handle complex types
    var re = x.re;
    var im = x.im;
    return qio_channel_print_complex(false, _channel_internal, re, im, numBytes(re.type));
  } else if t == string {
    // handle string
    const local_x = x.localize();
    // check if the string has escapes
    if local_x.hasEscapes {
      return EILSEQ;
    }
    return qio_channel_print_string(false, _channel_internal, local_x.c_str(), local_x.numBytes:c_ssize_t);
  } else if t == bytes {
    // handle bytes
    const local_x = x.localize();
    return qio_channel_print_bytes(false, _channel_internal, local_x.c_str(), local_x.numBytes:c_ssize_t);
  } else if isEnumType(t) {
    var st = qio_channel_style_element(_channel_internal, QIO_STYLE_ELEMENT_AGGREGATE);
    var s = x:string;
    if st == QIO_AGGREGATE_FORMAT_JSON then s = '"'+s+'"';
    return qio_channel_print_literal(false, _channel_internal, s.c_str(), s.numBytes:c_ssize_t);
  } else {
    compilerError("Unknown primitive type in _write_text_internal ", t:string);
  }
  return EINVAL;
}

pragma "no doc"
config param chpl_testReadBinaryInternalEIO = false;

private proc _read_binary_internal(_channel_internal:qio_channel_ptr_t, param byteorder:iokind, ref x:?t):errorCode where _isIoPrimitiveType(t) {
  if chpl_testReadBinaryInternalEIO {
    return EIO;
  }
  if isBoolType(t) {
    var got:int(32);
    got = qio_channel_read_byte(false, _channel_internal);
    if got >= 0 {
      x = (got != 0);
      return 0;
    } else {
      return (-got):errorCode;
    }
  } else if isIntegralType(t) {
    if numBytes(t) == 1 {
      // this is an optimization, since read_byte returns in a register
      var got:int(32);
      got = qio_channel_read_byte(false, _channel_internal);
      if got >= 0 {
        x = (got:uint(8)):t;
        return 0;
      } else {
        return (-got):errorCode;
      }
    } else if t == int(16) {
      return qio_channel_read_int16(false, byteorder:c_int, _channel_internal, x);
    } else if t == uint(16) {
      return qio_channel_read_uint16(false, byteorder:c_int, _channel_internal, x);
    } else if t == int(32) {
      return qio_channel_read_int32(false, byteorder:c_int, _channel_internal, x);
    } else if t == uint(32) {
      return qio_channel_read_uint32(false, byteorder:c_int, _channel_internal, x);
    } else if t == int(64) {
      return qio_channel_read_int64(false, byteorder:c_int, _channel_internal, x);
    } else if t == uint(64) {
      return qio_channel_read_uint64(false, byteorder:c_int, _channel_internal, x);
    } else {
      compilerError("Unknown int type in _read_binary_internal ", t:string);
    }
  } else if isRealType(t) || isImagType(t) {
    // handles real, imag
    if t == real(32) || t == imag(32) {
      return qio_channel_read_float32(false, byteorder:c_int, _channel_internal, x);
    } else if t == real(64) || t == imag(64) {
      return qio_channel_read_float64(false, byteorder:c_int, _channel_internal, x);
    } else {
      compilerError("Unknown float type in _read_binary_internal ", t:string);
    }
  } else if isComplexType(t)  {
    // handle complex types
    var re:x.re.type;
    var im:x.im.type;
    var err:errorCode = 0;
    if re.type == real(32) {
      err = qio_channel_read_float32(false, byteorder:c_int, _channel_internal, re);
      if ! err {
        err = qio_channel_read_float32(false, byteorder:c_int, _channel_internal, im);
      }
    } else if re.type == real(64) {
      err = qio_channel_read_float64(false, byteorder:c_int, _channel_internal, re);
      if ! err {
        err = qio_channel_read_float64(false, byteorder:c_int, _channel_internal, im);
      }
    } else {
      compilerError("Unknown complex type in _read_binary_internal ", t:string);
    }
    x = (re, im):t; // cast tuple to complex to get complex num.
    return err;
  } else if t == string {
    // handle string
    var len:int(64);
    var tx: c_string;
    var ret = qio_channel_read_string(false, byteorder:c_int,
                                      qio_channel_str_style(_channel_internal),
                                      _channel_internal, tx, len, -1);
    x = try! createStringWithOwnedBuffer(tx, length=len);
    return ret;
  } else if t == bytes {
    // handle _bytes (nothing special for bytes vs string in this case)
    var len:int(64);
    var tx: c_string;
    var ret = qio_channel_read_string(false, byteorder:c_int,
                                      qio_channel_str_style(_channel_internal),
                                      _channel_internal, tx, len, -1);
    x = createBytesWithOwnedBuffer(tx, length=len);
    return ret;
  } else if isEnumType(t) {
    var i:chpl_enum_mintype(t);
    var err:errorCode = 0;
    // call the integer version
    err = _read_binary_internal(_channel_internal, byteorder, i);
    x = chpl__orderToEnum(i, t);
    return err;
  } else {
    compilerError("Unknown primitive type in _read_binary_internal ", t:string);
  }
  return EINVAL;
}

pragma "no doc"
config param chpl_testWriteBinaryInternalEIO = false;

private proc _write_binary_internal(_channel_internal:qio_channel_ptr_t, param byteorder:iokind, x:?t):errorCode where _isIoPrimitiveType(t) {
  if chpl_testWriteBinaryInternalEIO {
    return EIO;
  }
  if isBoolType(t) {
    var zero_one:uint(8) = if x then 1:uint(8) else 0:uint(8);
    return qio_channel_write_byte(false, _channel_internal, zero_one);
  } else if isIntegralType(t) {
    if numBytes(t) == 1 {
      return qio_channel_write_byte(false, _channel_internal, x:uint(8));
    } else if t == int(16) {
      return qio_channel_write_int16(false, byteorder:c_int, _channel_internal, x);
    } else if t == uint(16) {
      return qio_channel_write_uint16(false, byteorder:c_int, _channel_internal, x);
    } else if t == int(32) {
      return qio_channel_write_int32(false, byteorder:c_int, _channel_internal, x);
    } else if t == uint(32) {
      return qio_channel_write_uint32(false, byteorder:c_int, _channel_internal, x);
    } else if t == int(64) {
      return qio_channel_write_int64(false, byteorder:c_int, _channel_internal, x);
    } else if t == uint(64) {
      return qio_channel_write_uint64(false, byteorder:c_int, _channel_internal, x);
    } else {
      compilerError("Unknown int type in _write_binary_internal ", t:string);
    }
  } else if isRealType(t) || isImagType(t) {
    if t == real(32) || t == imag(32) {
      return qio_channel_write_float32(false, byteorder:c_int, _channel_internal, x);
    } else if t == real(64) || t == imag(64) {
      return qio_channel_write_float64(false, byteorder:c_int, _channel_internal, x);
    } else {
      compilerError("Unknown float type in _write_binary_internal ", t:string);
    }
  } else if isComplexType(t)  {
    // handle complex types
    var re = x.re;
    var im = x.im;
    var err:errorCode = 0;
    if re.type == real(32) {
      err = qio_channel_write_float32(false, byteorder:c_int, _channel_internal, re);
      if ! err {
        err = qio_channel_write_float32(false, byteorder:c_int, _channel_internal, im);
      }
    } else if re.type == real(64) {
      err = qio_channel_write_float64(false, byteorder:c_int, _channel_internal, re);
      if ! err {
        err = qio_channel_write_float64(false, byteorder:c_int, _channel_internal, im);
      }
    } else {
      compilerError("Unknown complex type in _write_binary_internal ", t:string);
    }
    return err;
  } else if t == string {
    var local_x = x.localize();
    // check if the string has escapes
    if local_x.hasEscapes {
      return EILSEQ;
    }
    return qio_channel_write_string(false, byteorder:c_int, qio_channel_str_style(_channel_internal), _channel_internal, local_x.c_str(), local_x.numBytes: c_ssize_t);
  } else if t == bytes {
    var local_x = x.localize();
    return qio_channel_write_string(false, byteorder:c_int, qio_channel_str_style(_channel_internal), _channel_internal, local_x.c_str(), local_x.numBytes: c_ssize_t);
  } else if isEnumType(t) {
    var i = chpl__enumToOrder(x):chpl_enum_mintype(t);
    // call the integer version
    return _write_binary_internal(_channel_internal, byteorder, i);
  } else {
    compilerError("Unknown primitive type in write_binary_internal ", t:string);
  }
  return EINVAL;
}

pragma "no doc"
proc _channel._constructIoErrorMsg(param kind: iokind, const x:?t): string {
  var result: string = "while ";

  result += if this.writing then "writing " else "reading ";
  result += t:string;

  select t {
    when ioNewline do result += " " + "newline";
    when ioLiteral do result += " " + "\"" + x:string + "\"";
  }

  return result;
}

//
// The channel must be locked and running on this._home.
// The intent of x is ref (vs out) because it might contain a string literal.
//
pragma "no doc"
proc _channel._readOne(param kind: iokind, ref x:?t,
                             loc:locale) throws {
  // TODO: Make _read_one_internal(s) a method instead.
  var err = try _read_one_internal(_channel_internal, kind, x, loc);

  if err != 0 {
    const msg = _constructIoErrorMsg(kind, x);
    try _ch_ioerror(err, msg);
  }
}

private proc escapedNonUTF8ErrorMessage() : string {
  const ret = "Strings with escaped non-UTF8 bytes cannot be used with I/O. " +
        "Try using string.encode(encodePolicy.unescape) first.\n";
  return ret;
}

//
// The channel must be locked and running on this._home.
//
pragma "no doc"
proc _channel._writeOne(param kind: iokind, const x:?t, loc:locale) throws {
  // TODO: Make _write_one_internal(s) a method instead.
  var err = _write_one_internal(_channel_internal, kind, x, loc);

  if err != 0 {
    var msg = _constructIoErrorMsg(kind, x);
    if err == EILSEQ {
      // TODO: Is this error tested?
      msg = escapedNonUTF8ErrorMessage() + msg;
    }
    try _ch_ioerror(err, msg);
  }
}
private proc _read_io_type_internal(_channel_internal:qio_channel_ptr_t,
                                    param kind:iokind,
                                    ref x:?t,
                                    loc:locale): errorCode throws {
  var e:errorCode = 0;
  if t == ioNewline {
    return qio_channel_skip_past_newline(false, _channel_internal, x.skipWhitespaceOnly);
  } else if t == ioChar {
    return qio_channel_read_char(false, _channel_internal, x.ch);
  } else if t == ioLiteral {
    return qio_channel_scan_literal(false, _channel_internal,
                                    x.val.localize().c_str(),
                                    x.val.numBytes: c_ssize_t, x.ignoreWhiteSpace);
  } else if t == ioBits {
    return qio_channel_read_bits(false, _channel_internal, x.v, x.nbits);
  } else if kind == iokind.dynamic {
    var binary:uint(8) = qio_channel_binary(_channel_internal);
    var byteorder:uint(8) = qio_channel_byteorder(_channel_internal);
    if binary {
      select byteorder:iokind {
        when iokind.big    do e = _read_binary_internal(_channel_internal, iokind.big, x);
        when iokind.little do e = _read_binary_internal(_channel_internal, iokind.little, x);
        otherwise             e = _read_binary_internal(_channel_internal, iokind.native, x);
      }
    } else {
      e = _read_text_internal(_channel_internal, x);
    }
  } else {
    e = _read_binary_internal(_channel_internal, kind, x);
  }

  return e;

}

pragma 'fn exempt instantiation limit'
private proc _read_one_internal(_channel_internal:qio_channel_ptr_t,
                                param kind:iokind,
                                ref x:?t,
                                loc:locale): errorCode throws where _isIoPrimitiveTypeOrNewline(t) {

  var e:errorCode = 0;

  if x.locale == here {
    e = _read_io_type_internal(_channel_internal, kind, x, loc);
  } else {
    // use a temporary to avoid passing a remote wide pointer to a C function
    var tmp: t;
    e = _read_io_type_internal(_channel_internal, kind, tmp, loc);
    x = tmp;
  }

  return e;
}

pragma 'fn exempt instantiation limit'
private proc _write_one_internal(_channel_internal:qio_channel_ptr_t,
                                        param kind:iokind,
                                        const x:?t,
                                        loc:locale): errorCode throws where _isIoPrimitiveTypeOrNewline(t) {
  var e:errorCode = 0;
  if t == ioNewline {
    return qio_channel_write_newline(false, _channel_internal);
  } else if t == ioChar {
    return qio_channel_write_char(false, _channel_internal, x.ch);
  } else if t == ioLiteral {
    return qio_channel_print_literal(false, _channel_internal, x.val.localize().c_str(), x.val.numBytes:c_ssize_t);
  } else if t == ioBits {
    return qio_channel_write_bits(false, _channel_internal, x.v, x.nbits);
  } else if kind == iokind.dynamic {
    var binary:uint(8) = qio_channel_binary(_channel_internal);
    var byteorder:uint(8) = qio_channel_byteorder(_channel_internal);
    if binary {
      select byteorder:iokind {
        when iokind.big    do e = try _write_binary_internal(_channel_internal, iokind.big, x);
        when iokind.little do e = try _write_binary_internal(_channel_internal, iokind.little, x);
        otherwise             e = try _write_binary_internal(_channel_internal, iokind.native, x);
      }
    } else {
      e = _write_text_internal(_channel_internal, x);
    }
  } else {
    e = _write_binary_internal(_channel_internal, kind, x);
  }

  return e;
}

pragma 'fn exempt instantiation limit'
private proc _read_one_internal(_channel_internal:qio_channel_ptr_t,
                                       param kind:iokind,
                                       ref x:?t,
                                       loc:locale): errorCode throws {
  // Create a new channel that borrows the pointer in the
  // existing channel so we can avoid locking (because we
  // already have the lock)
  var reader = new fileReader(iokind.dynamic, locking=false,
                              home=here,
                              _channel_internal=_channel_internal,
                              _readWriteThisFromLocale=loc);

  // Set the channel pointer to NULL to make the
  // destruction of the local reader record safe
  // (it shouldn't release anything since it's a local copy).
  defer { reader._channel_internal = QIO_CHANNEL_PTR_NULL; }

  if isNilableClassType(t) {
    // future - write class IDs, have serialization format, handle binary
    var st = reader.styleElement(QIO_STYLE_ELEMENT_AGGREGATE);
    var iolit:ioLiteral;
    if st == QIO_AGGREGATE_FORMAT_JSON {
      iolit = new ioLiteral("null");
    } else {
      iolit = new ioLiteral("nil");
    }
    var e:errorCode = 0;
    e = try _read_one_internal(_channel_internal, iokind.dynamic, iolit, loc);
    if !e {
      x = nil;
      return 0;
    }
  }

  if isClassType(t) {
    if x == nil then
      throw new IllegalArgumentError("cannot read into a nil class");

    var tmp = x!;
    try tmp.readThis(reader);
    // the read cannot change the class pointer
    if tmp != x! then halt ("internal error - class pointer changed");
  } else {
    try x.readThis(reader);
  }

  return 0;
}

pragma "suppress lvalue error"
pragma 'fn exempt instantiation limit'
private proc _write_one_internal(_channel_internal:qio_channel_ptr_t,
                                        param kind:iokind,
                                        const x:?t,
                                        loc:locale): errorCode throws {
  // Create a new channel that borrows the pointer in the
  // existing channel so we can avoid locking (because we
  // already have the lock)
  var writer = new fileWriter(iokind.dynamic, locking=false,
                              home=here,
                              _channel_internal=_channel_internal,
                              _readWriteThisFromLocale=loc);

  // Set the channel pointer to NULL to make the
  // destruction of the local writer record safe
  // (it shouldn't release anything since it's a local copy).
  defer { writer._channel_internal = QIO_CHANNEL_PTR_NULL; }

  var err: errorCode = 0;

  if isClassType(t) || chpl_isDdata(t) || isAnyCPtr(t) {
    if x == nil {
      // future - write class IDs, have serialization format, handle binary
      var st = writer.styleElement(QIO_STYLE_ELEMENT_AGGREGATE);
      var iolit:ioLiteral;
      if st == QIO_AGGREGATE_FORMAT_JSON {
        iolit = new ioLiteral("null");
      } else {
        iolit = new ioLiteral("nil");
      }
      err = try _write_one_internal(_channel_internal, iokind.dynamic,
                                    iolit, loc);
    } else if isClassType(t) {
      var notNilX = x!;
      try notNilX.writeThis(writer);
    } else {
      // ddata / cptr
      try x.writeThis(writer);
    }
  } else {
    try x.writeThis(writer);
  }

  return err;
}

pragma "no doc"
proc _channel.readIt(ref x) throws {
  if writing then compilerError("read on write-only channel");
  const origLocale = this.getLocaleOfIoRequest();

  on this._home {
    try! this.lock(); defer { this.unlock(); }
    try _readOne(kind, x, origLocale);
  }
}

pragma "no doc"
proc _channel.writeIt(const x) throws {
  if !writing then compilerError("write on read-only channel");
  const origLocale = this.getLocaleOfIoRequest();

  on this._home {
    try! this.lock(); defer { this.unlock(); }
    try _writeOne(kind, x, origLocale);
  }
}

  /* Explicit call for reading or writing a literal as an
     alternative to using :type:`IO.ioLiteral`.
   */
  inline
  proc _channel.readWriteLiteral(lit:string, ignoreWhiteSpace=true) throws
  {
    var iolit = new ioLiteral(lit:string, ignoreWhiteSpace);
    if this.writing then
      this.writeIt(iolit);
    else
      this.readIt(iolit);
  }

  pragma "no doc"
  inline proc _channel._checkLiteralError(x:?t, err:errorCode,
                                          action:string,
                                          isLiteral:bool) : void throws {
    // Error message construction is handled here so that messages are
    // consistent across the cross product of:
    //   {read,write,match} x {literal,newline} x {string, bytes}
    //
    // Note that newlines do not involve strings or bytes

    if err != 0 {
      var msg: string = "while " + action + " ";

      if isLiteral {
        if t == string then
          msg += "string literal \"" + x + "\"";
        else
          msg += "bytes literal";
      } else {
        msg += "newline";
      }

      if err == EILSEQ {
        // TODO: Is this error tested?
        msg = escapedNonUTF8ErrorMessage() + "Error: " + msg;
      }

      try _ch_ioerror(err, msg);
    }
  }

  pragma "no doc"
  inline proc _channel._readLiteralCommon(x:?t, ignore:bool,
                                          param isMatch:bool) throws {
    if t != string && t != bytes then
      compilerError("expecting string or bytes");

    if writing {
      param name = if isMatch then "matchLiteral" else "readLiteral";
      param depth = if isMatch then 3 else 2;
      compilerError(name + " on write-only channel", depth);
    }

    on this._home {
      try! this.lock(); defer { this.unlock(); }
      const cstr = x.localize().c_str();
      const err = qio_channel_scan_literal(false, _channel_internal,
                                           cstr, x.numBytes:c_ssize_t,
                                           ignore);

      const action = if isMatch then "matching" else "reading";
      try _checkLiteralError(x, err, action, isLiteral=true);
    }
  }

  // non-unstable version we can use internally
  pragma "no doc"
  inline
  proc _channel._readLiteral(literal:string,
                            ignoreWhitespace=true) : void throws {
    var iolit = new ioLiteral(literal, ignoreWhitespace);
    this.readIt(iolit);
  }

  /*
    Advances the position of a channel by reading the exact text of the given
    string ``literal`` from the channel.

    If the string is not matched exactly, then the channel's position is
    unchanged. In such cases a :class:`OS.BadFormatError` will be thrown, unless the end of
    the channel is encountered in which case an :class:`OS.EofError` will be thrown. By
    default this method will ignore leading whitespace when attempting to read
    a literal.

    :arg literal: the string to be matched.
    :arg ignoreWhitespace: determines whether leading whitespace is ignored.

    :throws BadFormatError: Thrown if literal could not be matched.
    :throws EofError: Thrown if end of channel is encountered.

  */
  @unstable "channel.readLiteral is unstable and subject to change."
  inline
  proc _channel.readLiteral(literal:string,
                           ignoreWhitespace=true) : void throws {
    _readLiteralCommon(literal, ignoreWhitespace, isMatch=false);
  }

  /*
    Advances the position of a channel by reading the exact bytes of the given
    ``literal`` from the channel.

    If the bytes are not matched exactly, then the channel's position is
    unchanged. In such cases a :class:`OS.BadFormatError` will be thrown, unless the end of
    the channel is encountered in which case an :class:`OS.EofError` will be thrown. By
    default this method will ignore leading whitespace when attempting to read
    a literal.

    :arg literal: the bytes to be matched.
    :arg ignoreWhitespace: determines whether leading whitespace is ignored.

    :throws BadFormatError: Thrown if literal could not be matched.
    :throws EofError: Thrown if end of channel is encountered.

  */
  @unstable "channel.readLiteral is unstable and subject to change."
  inline
  proc _channel.readLiteral(literal:bytes,
                            ignoreWhitespace=true) : void throws {
    _readLiteralCommon(literal, ignoreWhitespace, isMatch=false);
  }

  // TODO: Don't we need an option to ignore whitespace or not?
  // Note: We don't want to allow skipping over non-whitespace.
  //
  // Note: We can add an 'ignoreWhitespace' optional argument that defaults
  // to 'true' without changing behavior in existing programs.
  pragma "no doc"
  inline
  proc _channel._readNewlineCommon(param isMatch:bool) throws {
    if writing {
      param name = if isMatch then "matchNewline" else "readNewline";
      compilerError(name + " on write-only channel", 2);
    }

    on this._home {
      try! this.lock(); defer { this.unlock(); }
      const err = qio_channel_skip_past_newline(false, _channel_internal,
                                                /*skipWhitespaceOnly=*/ true);

      const action = if isMatch then "matching" else "reading";
      try _checkLiteralError("", err, action, isLiteral=false);
    }
  }

  // non-unstable version we can use internally
  pragma "no doc"
  inline proc _channel._readNewline() : void throws {
    var ionl = new ioNewline(true);
    this.readIt(ionl);
  }

  // TODO: How does this differ from readln() ?
  /*
    Advances the position of the channel by reading a newline.

    If a newline is not matched exactly, then the channel's position is
    unchanged. In such cases a :class:`OS.BadFormatError` will be thrown, unless the end of
    the channel is encountered in which case an :class:`OS.EofError` will be thrown. By
    default this method will ignore leading whitespace when attempting to read
    a newline.

    :throws BadFormatError: Thrown if a newline could not be matched.
    :throws EofError: Thrown if end of channel is encountered.

  */
  @unstable "channel.readNewline is unstable and subject to change."
  inline
  proc _channel.readNewline() : void throws {
    _readNewlineCommon(isMatch=false);
  }

  pragma "no doc"
  inline
  proc _channel._matchLiteralCommon(literal, ignore : bool) : bool throws {
    try {
      _readLiteralCommon(literal, ignore, isMatch=true);
    } catch e : BadFormatError {
      return false;
    } catch e : EofError {
      return false;
    }

    return true;
  }

  /*
    Advances the position of a channel by reading the exact text of the given
    string ``literal`` from the channel.

    If the string is not matched exactly, then the channel's position is
    unchanged and this method will return ``false``. In other words, this
    channel will return ``false`` in the cases where :proc:`channel.readLiteral`
    would throw a :class:`OS.BadFormatError` or an :class:`OS.EofError`.

    By default this method will ignore leading whitespace when attempting to
    read a literal.

    :arg literal: the string to be matched.
    :arg ignoreWhitespace: determines whether leading whitespace is ignored.

    :returns: ``true`` if the read succeeded, and ``false`` on end of file or if
      the literal could not be matched.
  */
  @unstable "channel.matchLiteral is unstable and subject to change."
  inline
  proc _channel.matchLiteral(literal:string,
                            ignoreWhitespace=true) : bool throws {
    return _matchLiteralCommon(literal, ignoreWhitespace);
  }

  /*
    Advances the position of a channel by reading the exact bytes of the given
    ``literal`` from the channel.

    If the bytes are not matched exactly, then the channel's position is
    unchanged and this method will return ``false``. In other words, this
    channel will return ``false`` in the cases where :proc:`channel.readLiteral`
    would throw a :class:`OS.BadFormatError` or an :class:`OS.EofError`.

    By default this method will ignore leading whitespace when attempting to
    read a literal.

    :arg literal: the bytes to be matched.
    :arg ignoreWhitespace: determines whether leading whitespace is ignored.

    :returns: ``true`` if the read succeeded, and ``false`` on end of file or if
      the literal could not be matched.
  */
  @unstable "channel.matchLiteral is unstable and subject to change."
  inline
  proc _channel.matchLiteral(literal:bytes,
                            ignoreWhitespace=true) : bool throws {
    return _matchLiteralCommon(literal, ignoreWhitespace);
  }

  /*
    Advances the position of the channel by reading a newline.

    If a newline is not matched exactly, then the channel's position is
    unchanged and this method will return ``false``. In other words, this
    channel will return ``false`` in the cases where :proc:`channel.readNewline`
    would throw a :class:`OS.BadFormatError` or an :class:`OS.EofError`.

    By default this method will ignore leading whitespace when attempting to
    read a newline.

    :returns: ``true`` if the read succeeded, and ``false`` on end of file or if
      the newline could not be matched.
  */
  @unstable "channel.matchNewline is unstable and subject to change."
  inline
  proc _channel.matchNewline() : bool throws {
    try {
      _readNewlineCommon(isMatch=true);
    } catch e : BadFormatError {
      return false;
    } catch e : EofError {
      return false;
    }

    return true;
  }

  pragma "no doc"
  inline
  proc _channel._writeLiteralCommon(x:?t) : void throws {
    if t != string && t != bytes then
      compilerError("expecting string or bytes");

    if !writing then compilerError("writeLiteral on read-only channel", 2);

    on this._home {
      try! this.lock(); defer { this.unlock(); }
      const cstr = x.localize().c_str();
      const err = qio_channel_print_literal(false, _channel_internal, cstr,
                                            x.numBytes:c_ssize_t);
      try _checkLiteralError(x, err, "writing", isLiteral=true);
    }
  }

  pragma "no doc"
  inline
  proc _channel._writeLiteral(literal:string) : void throws {
    var iolit = new ioLiteral(literal);
    this.writeIt(iolit);
  }

  /*
    Writes a string to the channel, ignoring any formatting configured for
    this channel.
  */
  @unstable "channel.writeLiteral is unstable and subject to change."
  inline
  proc _channel.writeLiteral(literal:string) : void throws {
    _writeLiteralCommon(literal);
  }

  /*
    Writes bytes to the channel, ignoring any formatting configured for this
    channel.
  */
  @unstable "channel.writeLiteral is unstable and subject to change."
  inline
  proc _channel.writeLiteral(literal:bytes) : void throws {
    _writeLiteralCommon(literal);
  }

  // TODO: How does this differ from writeln() ?
  /*
    Writes a newline to the channel, ignoring any formatting configured for
    this channel.
  */
  @unstable "channel.writeNewline is unstable and subject to change."
  inline
  proc _channel.writeNewline() : void throws {
    if !writing then compilerError("writeNewline on read-only channel");

    on this._home {
      try! this.lock(); defer { this.unlock(); }
      const err = qio_channel_write_newline(false, _channel_internal);
      try _checkLiteralError("", err, "writing", isLiteral=false);
    }
  }

  /* Explicit call for reading or writing a newline as an
     alternative to using :type:`IO.ioNewline`.
   */
  inline proc _channel.readWriteNewline() throws
  {
    var ionl = new ioNewline();
    if this.writing then
      this.writeIt(ionl);
    else
      this.readIt(ionl);
  }

  /* Returns `true` if this channel is configured for binary I/O.
   */
  proc _channel.binary():bool {
    var ret:uint(8);
    on this._home {
      ret = qio_channel_binary(_channel_internal);
    }
    return ret != 0;
  }

  /* return other style elements. */
  pragma "no doc"
  proc _channel.styleElement(element:int):int {
    var ret:int = 0;
    on this._home {
      ret = qio_channel_style_element(_channel_internal, element);
    }
    return ret;
  }

  /*
     Write a sequence of bytes.

     :throws UnexpectedEofError: Thrown if EOF encountered before all bytes could be written.
     :throws SystemError: Thrown if the byte sequence could not be written for another reason.
  */
  pragma "last resort"
  deprecated "'writeBytes' with a generic pointer argument is deprecated; please use the variant that takes a 'bytes' object"
  proc _channel.writeBytes(x, len:c_ssize_t) throws {
    try this._writeBytes(x, len);
  }

  pragma "no doc"
  proc _channel._writeBytes(x, len:c_ssize_t) throws {
    var err:errorCode = 0;
    on this._home {
      try this.lock(); defer { this.unlock(); }
      err = qio_channel_write_amt(false, _channel_internal, x, len);
    }
    if err then try this._ch_ioerror(err, "in channel.writeBytes()");
  }

/*
  Iterate over all of the lines ending in ``\n`` in a channel - the channel
  lock will be held while iterating over the lines.

  Only serial iteration is supported.

  .. warning::

    This iterator executes on the current locale. This may impact multilocale
    performance if the current locale is not the same locale on which the
    channel was created.

  :yields: lines ending in ``\n`` in channel
 */
iter _channel.lines() {

  try! this.lock();

  // Save iostyleInternal
  const saved_style: iostyleInternal = this._styleInternal();
  // Update iostyleInternal
  var newline_style: iostyleInternal = this._styleInternal();

  newline_style.string_format = QIO_STRING_FORMAT_TOEND;
  newline_style.string_end = 0x0a; // '\n'
  this._set_styleInternal(newline_style);

  // Iterate over lines
  var itemReader = new itemReaderInternal(string, kind, locking, this);
  for line in itemReader {
    yield line;
  }

  // Set the iostyle back to original state
  this._set_styleInternal(saved_style);

  this.unlock();
}

public use ChapelIOStringifyHelper;

// Note that stringify is called with primitive/range/tuple arguments
// in modules that are loaded early. To avoid module ordering issues,
// it supports such types directly via stringify_simple.
/*
    Creates a string representing the result of writing the arguments.

    Writes each argument, possibly using a `writeThis` method,
    to a string and returns the result.
  */
proc stringify(const args ...?k):string {
  if _can_stringify_direct(args) {
    return stringify_simple((...args));
  } else {
    // otherwise, write it using the I/O system.
    try! {
      // Open a memory buffer to store the result
      var f = openmem();
      defer try! f.close();

      var w = f.writer(locking=false);
      defer try! w.close();

      w.write((...args));

      var offset = w.offset();

      var buf = c_malloc(uint(8), offset+1);

      var r = f.reader(locking=false);
      defer try! r.close();

      r._readBytes(buf, offset:c_ssize_t);
      // Add the terminating NULL byte to make C string conversion easy.
      buf[offset] = 0;

      const ret = createStringWithNewBuffer(buf, offset, offset+1,
                                            decodePolicy.replace);
      c_free(buf);
      return ret;
    }
  }
}

private var _arg_to_proto_names = ("a", "b", "c", "d", "e", "f");

private proc _args_to_proto(const args ...?k, preArg:string) {
  // FIX ME: lot of potential leaking going on here with string concat
  // But this is used for error handling so maybe we don't care.
  var err_args: string;
  for param i in 0..k-1 {
    var name: string;
    if i < _arg_to_proto_names.size then name = _arg_to_proto_names[i];
    else name = "x" + i:string;
    err_args += preArg + name + ":" + args(i).type:string;
    if i != k-1 then err_args += ", ";
  }
  return err_args;
}

pragma "no doc"
inline proc _channel._readInner(ref args ...?k):void throws {
  if writing then compilerError("read on write-only channel");
  const origLocale = this.getLocaleOfIoRequest();

  on this._home {
    try this.lock(); defer { this.unlock(); }
    for param i in 0..k-1 {
      _readOne(kind, args[i], origLocale);
    }
  }
}

/*

   Read values from a channel. The input will be consumed atomically - the
   channel lock will be held while reading all of the passed values.

   :arg args: a list of arguments to read. Basic types are handled
              internally, but for other types this function will call
              value.readThis() with a ``Reader`` argument as described
              in :ref:`readThis-writeThis`.
   :returns: `true` if the read succeeded, and `false` on end of file.

   :throws UnexpectedEofError: Thrown if unexpected EOF encountered while reading.
   :throws SystemError: Thrown if the channel could not be read.
 */
inline proc _channel.read(ref args ...?k):bool throws {
  try {
    this._readInner((...args));
  } catch err: EofError {
    return false;
  }

  return true;
}

@unstable "read with a style argument is unstable"
proc _channel.read(ref args ...?k, style:iostyle):bool throws {
  return this.readHelper((...args), style: iostyleInternal);
}

pragma "no doc"
proc _channel.readHelper(ref args ...?k, style:iostyleInternal):bool throws {
  if writing then compilerError("read on write-only channel");
  const origLocale = this.getLocaleOfIoRequest();

  try {
    on this._home {
      try this.lock(); defer { this.unlock(); }

      var saveStyle: iostyleInternal = this._styleInternal();
      defer {
        this._set_styleInternal(saveStyle);
      }
      this._set_styleInternal(style);

      for param i in 0..k-1 {
        _readOne(kind, args[i], origLocale);
      }
    }
  } catch err: EofError {
    return false;
  }

  return true;
}

/*
  Read a line into a Chapel array of bytes. Reads until a ``\n`` is reached.
  The ``\n`` is returned in the array.

  Note that this routine currently requires a 1D rectangular non-strided array.

  Throws a SystemError if a line could not be read from the channel.

  :arg arg: A 1D DefaultRectangular array which must have at least 1 element.
  :arg numRead: The number of bytes read.
  :arg start: Index to begin reading into.
  :arg amount: The maximum amount of bytes to read.
  :returns: true if the bytes were read without error.
*/
deprecated "channel.readline is deprecated. Use :proc:`channel.readLine` instead"
proc _channel.readline(arg: [] uint(8), out numRead : int, start = arg.domain.lowBound,
                      amount = arg.domain.highBound - start + 1) : bool throws
                      where arg.rank == 1 && arg.isRectangular() {
  if arg.size == 0 || !arg.domain.contains(start) ||
     amount <= 0 || (start + amount - 1 > arg.domain.highBound) then return false;

  var err:errorCode = 0;
  on this._home {
    try this.lock(); defer { this.unlock(); }
    param newLineChar = 0x0A;
    var got: int;
    var i = start;
    const maxIdx = start + amount - 1;
    while i <= maxIdx {
      got = qio_channel_read_byte(false, this._channel_internal);
      if got < 0 then break;
      arg[i] = got:uint(8);
      i += 1;
      if got == newLineChar then break;
    }
    numRead = i - start;
    if i == start && got < 0 then err = (-got):errorCode;
  }

  if !err {
    return true;
  } else if err == EEOF {
    return false;
  } else {
    try this._ch_ioerror(err, "in channel.readline(arg : [] uint(8))");
  }
  return false;
}

/*
  Read a line into a Chapel array of bytes. Reads until a ``\n`` is reached.
  This function always does a binary read (i.e. it is not aware of UTF-8 etc)
  and is similar in some ways to `readLine(bytes)` but works with an array directly.
  However, it does not resize the array but rather stores up to first
  'size' bytes in to it. The exact number of bytes in the array set will depend
  on the line length (and on stripNewline since the newline will be counted if it is
  stored in the array).

  :arg a: A 1D DefaultRectangular non-strided array storing int(8) or uint(8) which must have at least 1 element.
  :arg maxSize: The maximum number of bytes to store into the ``a`` array. Defaults to the size of the array.
  :arg stripNewline: Whether to strip the trailing ``\n`` from the line.
  :returns: Returns `0` if EOF is reached and no data is read. Otherwise, returns the number of array elements that were set by this call.

  :throws UnexpectedEofError: Thrown if unexpected EOF encountered while reading line.
  :throws SystemError: Thrown if data could not be read from the channel for another reason.
  :throws BadFormatError: Thrown if the line is longer than `maxSize`. It leaves the input marker at the beginning of the offending line.
 */
proc _channel.readLine(ref a: [] ?t, maxSize=a.size, stripNewline=false): int throws
      where (t == uint(8) || t == int(8)) && a.rank == 1 && a.isRectangular() && !a.stridable {
  if a.size == 0 || maxSize == 0 ||
  ( a.domain.lowBound + maxSize - 1 > a.domain.highBound) then return 0;

  var err:errorCode = 0;
  var numRead:int;
  on this._home {
    try this.lock(); defer { this.unlock(); }
    this._mark();
    param newLineChar = 0x0A;
    var got: int;
    var i = a.domain.lowBound;
    const maxIdx = a.domain.lowBound + maxSize - 1;
    var foundNewline = false;
    while !foundNewline {
      got = qio_channel_read_byte(false, this._channel_internal);
      if got == -EEOF {
        // encountered EOF
        break;
      } else if got < 0 {
        // encountered an error so throw
        this._revert();
        var err:errorCode = -got;
        try this._ch_ioerror(err, "in channel.readLine(a : [] uint(8))");
      }
      if got == newLineChar {
        foundNewline = true;
      }
      if i > maxIdx {
        if foundNewline && stripNewline {
          // don't worry about it since we wouldn't return the newline
        } else {
          // The line is longer than was specified so we throw an error
          this._revert();
          try this._ch_ioerror(EFORMAT:errorCode, "line longer than maxSize in channel.readLine(a : [] uint(8))");
        }
      }
      if !(foundNewline && stripNewline) {
        a[i] = got:uint(8);
        i += 1;
      }
    }
    numRead = i - a.domain.lowBound;
    if i == a.domain.lowBound && got < 0 then err = (-got):errorCode;
    this._commit();
  }

  if !err {
    return numRead;
  } else if err == EEOF {
    return 0;
  } else {
    try this._ch_ioerror(err, "in channel.readLine(a : [] uint(8))");
  }
  return 0;

}

pragma "no doc"
pragma "last resort"
inline proc _channel.readLine(ref a: [] ?t, maxSize=a.size, stripNewline=false): int throws {
  compilerError("'readLine()' is currently only supported for non-strided 1D rectangular arrays");
}

/*
  Read a line into a Chapel string or bytes. Reads until a ``\n`` is reached.
  The ``\n`` is included in the resulting value.

  :arg arg: a string or bytes to receive the line
  :returns: `true` if a line was read without error, `false` upon EOF

  :throws UnexpectedEofError: Thrown if unexpected EOF encountered while reading.
  :throws SystemError: Thrown if data could not be read from the channel.
*/
deprecated "channel.readline is deprecated. Use :proc:`channel.readLine` instead"
proc _channel.readline(ref arg: ?t): bool throws where t==string || t==bytes {
  if writing then compilerError("read on write-only channel");
  const origLocale = this.getLocaleOfIoRequest();

  try {
    on this._home {
      try this.lock(); defer { this.unlock(); }
      var saveStyle: iostyleInternal = this._styleInternal();
      defer {
        this._set_styleInternal(saveStyle);
      }
      var myStyle = saveStyle.text();
      myStyle.string_format = QIO_STRING_FORMAT_TOEND;
      myStyle.string_end = 0x0a; // ascii newline.
      this._set_styleInternal(myStyle);
      try _readOne(iokind.dynamic, arg, origLocale);
    }
  } catch err: EofError {
    return false;
  }

  return true;
}

// Helper function to replace the contents of a string or bytes
// by reading up to a fixed number of bytes / codepoints.
// Returns an error code, and ESHORT if less than that number of
// bytes was read.
// Does not validate that the string has valid encoding -- the call
// site should do that.
// Assumes we are already on the locale with the channel and that
// it is already locked.
private proc readStringBytesData(ref s /*: string or bytes*/,
                                 _channel_internal:qio_channel_ptr_t,
                                 nBytes: int,
                                 nCodepoints: int): errorCode {
  import BytesStringCommon;

  BytesStringCommon.resizeBuffer(s, nBytes);

  // TODO: if the channel is working with non-UTF-8 data
  // (which is a feature not yet implemented at all)
  // this would need to call a read than can do character set conversion
  // in the event that s.type == string.

  var len:c_ssize_t = nBytes.safeCast(c_ssize_t);
  var err = qio_channel_read_amt(false, _channel_internal, s.buff, len);
  if !err {
    s.buffLen = nBytes;
    if nBytes != 0 then s.buff[nBytes] = 0; // include null-byte
    if s.type == string {
      s.cachedNumCodepoints = nCodepoints;
      s.hasEscapes = false;
    }
  } else {
    s.buffLen = 0;
    if s.type == string {
      s.cachedNumCodepoints = 0;
      s.hasEscapes = false;
    }
  }
  return err;
}

/*
  Read a line into a Chapel string. Reads until a ``\n`` is reached.

  :arg s: a string to receive the line
  :arg maxSize: The maximum number of codepoints to store into ``s``. The default of -1 means to read an unlimited number of codepoints.
  :arg stripNewline: Whether to strip the trailing ``\n`` from the line.
  :returns: `true` if a line was read without error, `false` upon EOF

  :throws UnexpectedEofError: Thrown if unexpected EOF encountered while reading.
  :throws SystemError: Thrown if data could not be read from the channel.
  :throws BadFormatError: Thrown if the line is longer than `maxSize`. It leaves the input marker at the beginning of the offending line.
*/
proc _channel.readLine(ref s: string,
                      maxSize=-1,
                      stripNewline=false): bool throws {
  if writing then compilerError("read on write-only channel");
  const origLocale = this.getLocaleOfIoRequest();
  var ret: bool = false;

  on this._home {
    try this.lock(); defer { this.unlock(); }
    param newLineChar = 0x0A; // ascii newline.
    var maxCodepoints = if maxSize < 0 then max(int) else maxSize;
    var nCodepoints: int = 0; // num codepoints, including newline
    var chr : int(32);
    var err: errorCode = 0;
    var foundNewline = false;
    // use the channel's buffering to compute how many bytes/codepoints
    // we are reading
    this._mark();
    while !foundNewline {
      // read a single codepoint
      err = qio_channel_read_char(false, this._channel_internal, chr);
      if err == EEOF {
        // encountered EOF
        break;
      } else if err {
        // encountered an error so throw
        this._revert();
        try this._ch_ioerror(err, "in channel.readLine(ref s: string)");
      }
      nCodepoints += 1;
      if chr == newLineChar {
        foundNewline = true;
      }
      if nCodepoints > maxCodepoints {
        if stripNewline && foundNewline {
          // don't worry about it since we wouldn't return the newline
        } else {
          // The line is longer than was specified so we throw an error
          this._revert();
          try this._ch_ioerror(EFORMAT:errorCode,
               "line longer than maxSize in channel.readLine(ref s: string)");
        }
      }
    }
    var endOffset = this._offset();
    this._revert();
    var nBytes:int = endOffset - this._offset();
    // now, nCodepoints and nBytes include the newline
    if foundNewline && stripNewline {
      // but we don't want to read the newline if stripNewline=true.
      nBytes -= 1;
      nCodepoints -= 1;
    }

    // now read the data into the string
    // readStringBytesData will advance the channel by exactly `nBytes`.
    // This may consume or leave the newline based on the logic above.
    err = readStringBytesData(s, this._channel_internal, nBytes, nCodepoints);
    if foundNewline && stripNewline && !err {
      // pass the newline in the input
      err = qio_channel_read_char(false, this._channel_internal, chr);
    }

    if err != 0 && err != EEOF {
      try this._ch_ioerror(err, "in channel.readLine(ref s: string)");
    }

    // return 'true' if we read anything
    ret = foundNewline || nBytes > 0;
  }

  return ret;
}

/*
  Read a line into Chapel bytes. Reads until a ``\n`` is reached.

  :arg b: bytes to receive the line
  :arg maxSize: The maximum number of bytes to store into ``b``. The default of -1 means to read an unlimited number of bytes.
  :arg stripNewline: Whether to strip the trailing ``\n`` from the line.
  :returns: `true` if a line was read without error, `false` upon EOF

  :throws UnexpectedEofError: Thrown if unexpected EOF encountered while reading.
  :throws SystemError: Thrown if data could not be read from the channel.
  :throws BadFormatError: Thrown if the line is longer than `maxSize`. It leaves the input marker at the beginning of the offending line.
*/
proc _channel.readLine(ref b: bytes,
                      maxSize=-1,
                      stripNewline=false): bool throws {
  if writing then compilerError("read on write-only channel");
  const origLocale = this.getLocaleOfIoRequest();
  var ret: bool = false;

  on this._home {
    try this.lock(); defer { this.unlock(); }
    param newLineChar = 0x0A; // ascii newline.
    var maxBytes = if maxSize < 0 then max(int) else maxSize;
    var nBytes: int = 0;
    // use the channel's buffering to compute how many bytes we are reading
    this._mark();
    var got : int;
    var err: errorCode = 0;
    var foundNewline = false;
    while !foundNewline {
      // read a single byte
      got = qio_channel_read_byte(false, this._channel_internal);
      if got == -EEOF {
        // encountered EOF
        break;
      } else if got < 0 {
        // encountered an error so throw
        this._revert();
        err = -got;
        try this._ch_ioerror(err, "in channel.readLine(ref b: bytes)");
        break;
      }
      nBytes += 1;
      if got == newLineChar {
        foundNewline = true;
      }
      if nBytes > maxBytes {
        if foundNewline && stripNewline {
          // don't worry about it since we wouldn't return the newline
        } else {
          // The line is longer than was specified so we throw an error
          this._revert();
          try this._ch_ioerror(EFORMAT:errorCode,
                   "line longer than maxSize in channel.readLine(ref b: bytes)");
        }
      }
    }
    this._revert();
    // now, nBytes includes the newline
    if foundNewline && stripNewline {
      // but we don't want to read the newline if stripNewline=true.
      nBytes -= 1;
    }

    // now read the data into the bytes
    // readStringBytesData will advance the channel by exactly `nBytes`.
    // This may consume or leave the newline based on the logic above.
    err = readStringBytesData(b, this._channel_internal, nBytes,
                              nCodepoints=-1);
    if foundNewline && stripNewline && !err {
      // pass the newline in the input
      got = qio_channel_read_byte(false, this._channel_internal);
      if got < 0 {
        err = -got;
      }
    }

    if err != 0 && err != EEOF {
      try this._ch_ioerror(err, "in channel.readLine(ref s: string)");
    }

    // return 'true' if we read anything
    ret = foundNewline || nBytes > 0;
  }

  return ret;
}

/*
  Read a line. Reads until a ``\n`` is reached.

  :arg t: the type of data to read, which must be ``string`` or ``bytes``. Defaults to ``string`` if not specified.
  :arg maxSize: The maximum number of codepoints to read. The default of -1 means to read an unlimited number of codepoints.
  :arg stripNewline: Whether to strip the trailing ``\n`` from the line.
  :returns: The data that was read.

  :throws UnexpectedEofError: Thrown if unexpected EOF encountered while reading.
  :throws SystemError: Thrown if data could not be read from the channel.
  :throws IoError: Thrown if the line is longer than `maxSize`. It leaves the input marker at the beginning of the offending line.
*/
proc _channel.readLine(type t=string, maxSize=-1, stripNewline=false): t throws where t==string || t==bytes {
  var retval: t;
  this.readLine(retval, maxSize, stripNewline);
  return retval;
}

/*
  Read the remaining contents of the channel into an instance of the specified type

  :arg t: the type to read into; must be ``string`` or ``bytes``. Defaults to ``bytes`` if not specified.
  :returns: the contents of the channel as a ``t``

  :throws UnexpectedEofError: Thrown if unexpected EOF encountered while reading.
  :throws SystemError: Thrown if data could not be read from the channel for another reason.
*/
proc _channel.readAll(type t=bytes): t throws
  where t==string || t==bytes
{
  if this.writing then compilerError("attempt to read on write-only channel");

  var out_var : t;

  if t == bytes {
    out_var = b"";
    this.readAll(out_var);
  } else {
    out_var = "";
    this.readAll(out_var);
  }

  return out_var;
}

/*
  Read the remaining contents of the channel into a ``string``.

  Note that any existing contents of the ``string`` are overwritten.

  :arg s: the ``string`` to read into
  :returns: the number of codepoints that were stored in ``s``
  :rtype: int

  :throws UnexpectedEofError: Thrown if unexpected EOF encountered while reading.
  :throws SystemError: Thrown if data could not be read from the channel for another reason.
*/
proc _channel.readAll(ref s: string): int throws {
  if this.writing then compilerError("attempt to read on write-only channel");

  const (err, lenread) = readBytesOrString(this, s, -1);

  if err != 0 && err != EEOF {
    try this._ch_ioerror(err, "in channel.readAll(ref s: string)");
  }

  return lenread;
}

/*
  Read the remaining contents of the channel into a ``bytes``.

  Note that any existing contents of the ``bytes`` are overwritten.

  :arg b: the ``bytes`` to read into
  :returns: the number of bytes that were stored in ``b``
  :rtype: int

  :throws UnexpectedEofError: Thrown if unexpected EOF encountered while reading.
  :throws SystemError: Thrown if data could not be read from the channel for another reason.
*/
proc _channel.readAll(ref b: bytes): int throws {
  if this.writing then compilerError("attempt to read on write-only channel");

  const (err, lenread) = readBytesOrString(this, b, -1);

  if err != 0 && err != EEOF {
    try this._ch_ioerror(err, "in channel.readAll(ref b: bytes)");
  }

  return lenread;
}

/*
  Read the remaining contents of the channel into a an array of bytes.

  Note that this routine currently requires a 1D rectangular non-strided array.
  Additionally, If the remaining contents of the channel exceed the size of
  ``a``, the first ``a.size`` bytes will be read into ``a``, and then an
  ``InsufficientCapacityError`` will be thrown.

  :arg a: the array of bytes to read into
  :returns: the number of bytes that were stored in ``a``
  :rtype: int

  :throws InsufficientCapacityError: Thrown if the channel's contents do not fit into ``a``.
  :throws UnexpectedEofError: Thrown if unexpected EOF encountered while reading.
  :throws SystemError: Thrown if data could not be read from the channel for another reason.
*/
proc _channel.readAll(ref a: [?d] ?t): int throws
  where (t == uint(8) || t == int(8)) && d.rank == 1 && d.stridable == false
{
  if this.writing then compilerError("attempt to read on write-only channel");
  var i = d.low;

  on this._home {
    try this.lock(); defer { this.unlock(); }
    var got : int;

    while d.contains(i) {
      // read a byte
      got = qio_channel_read_byte(false, this._channel_internal);

      if got == -EEOF {
        // reached EOF, stop reading
        break;
      } else if got < 0 {
        // hit an IO error, throw
        try this._ch_ioerror((-got):errorCode, "in channel.readAll(ref a: [])");
      } else {
        // got a byte, store it
        a[i] = got:t;
        i += 1;
      }
    }

    // if a is full, but we haven't reached EOF, throw
    if i-1 == d.high {
      var has_more = false;

      this._mark();
      got = qio_channel_read_byte(false, this._channel_internal);
      has_more = (got >= 0);
      this._revert();

      if has_more {
        const sz = qio_channel_get_size(this._channel_internal);
        const err_msg = "Channel's contents" + (if sz == -1 then " " else " (" + sz:string + " bytes) ") +
          "exceeded capacity of array argument (" + a.size:string + " bytes) in 'readAll'";

        throw new owned InsufficientCapacityError(err_msg);
      }
    }
  }

  return (i - d.low);
}

/* read a given number of bytes from a channel

   :arg str_out: The string to be read into
   :arg len: Read up to len bytes from the channel, up until EOF
             (or some kind of I/O error). If the default value of -1
             is provided, read until EOF starting from the channel's
             current offset.
   :returns: `true` if we read something, `false` upon EOF

   :throws UnexpectedEofError: Thrown if unexpected EOF encountered while reading.
   :throws SystemError: Thrown if the bytes could not be read from the channel.
 */
deprecated "'readstring' is deprecated; please use 'readString' instead"
proc _channel.readstring(ref str_out:string, len:int(64) = -1):bool throws {
  var (err, _) = readBytesOrString(this, str_out, len);

  if !err {
    return true;
  } else if err == EEOF {
    return false;
  } else {
    try this._ch_ioerror(err, "in channel.readstring(ref str_out:string, len:int(64))");
  }
  return false;
}

/*
  Read a given number of codepoints from a ``fileReader``, returning a new
  :type:`~String.string`.

  The ``string``'s length may be less than ``maxSize`` if EOF is reached while
  reading. If nothing is read, the empty string (``""``) will be returned.

  :arg maxSize: the maximum number of codepoints to read from the ``fileReader``
  :returns: a new ``string`` containing up to the next ``maxSize`` codepoints
              from the ``fileReader``

  :throws SystemError: Thrown if a ``string`` could not be read from the ``fileReader``.
*/
proc fileReader.readString(maxSize: int): string throws {
  var ret: string = "";
  var (e, _) = readBytesOrString(this, ret, maxSize);

  if e != 0 && e != EEOF then throw createSystemError(e);

  return ret;
}

/*
  Read a given number of codepoints from a ``fileReader`` into a
  :type:`~String.string`.

  The updated ``string``'s length may be less than ``maxSize`` if EOF is
  reached while reading. If nothing is read, it will be set to the empty
  string (``""``).

  :arg s: the ``string`` to read into — contents will be overwritten
  :arg maxSize: the maximum number of codepoints to read from the ``fileReader``
  :returns: ``false`` if nothing could be read, or ``true`` if something was read.

  :throws SystemError: Thrown if a ``string`` could not be read from the ``fileReader``.
*/
proc fileReader.readString(ref s: string, maxSize: int): bool throws {
  var (e, lenRead) = readBytesOrString(this, s, maxSize);

  if e != 0 && e != EEOF then throw createSystemError(e);

  return lenRead > 0;
}

/* read a given number of bytes from a channel

   :arg bytes_out: The bytes to be read into
   :arg len: Read up to len bytes from the channel, up until EOF
             (or some kind of I/O error). If the default value of -1
             is provided, read until EOF starting from the channel's
             current offset.
   :returns: `true` if we read something, `false` upon EOF

  :throws UnexpectedEofError: Thrown if unexpected EOF encountered while reading.
  :throws SystemError: Thrown if data could not be read from the channel for another reason.
 */
deprecated "'readbytes' is deprecated; please use 'readBytes' instead"
proc _channel.readbytes(ref bytes_out:bytes, len:int(64) = -1):bool throws {
  var (err, _) = readBytesOrString(this, bytes_out, len);

  if !err {
    return true;
  } else if err == EEOF {
    return false;
  } else {
    try this._ch_ioerror(err, "in channel.readbytes(ref str_out:bytes, len:int(64))");
  }
  return false;
}

/*
  Read a given number of bytes from a ``fileReader``, returning a new
  :type:`~Bytes.bytes`.

  The ``bytes``'s length may be less than ``maxSize`` if EOF is reached while
  reading. If nothing is read, the empty bytes (``b""``) will be returned.

  :arg maxSize: the maximum number of bytes to read from the ``fileReader``
  :returns: a new ``bytes`` containing up to the next ``maxSize`` bytes
              from the ``fileReader``

  :throws SystemError: Thrown if a ``bytes`` could not be read from the ``fileReader``
                       for another reason.
*/
proc fileReader.readBytes(maxSize: int): bytes throws {
  var ret: bytes = b"";
  var (e, _) = readBytesOrString(this, ret, maxSize);

  if e != 0 && e != EEOF then throw createSystemError(e);

  return ret;
}

/*
  Read a given number of bytes from a ``fileReader`` into a
  :type:`~Bytes.bytes`.

  The updated ``bytes``'s length may be less than ``maxSize`` if EOF is
  reached while reading. If nothing is read, it will be set to the empty
  bytes (``b""``).

  :arg b: the ``bytes`` to read into — contents will be overwritten
  :arg maxSize: the maximum number of bytes to read from the ``fileReader``
  :returns: ``false`` if nothing could be read, or ``true`` if something was read.

  :throws SystemError: Thrown if a ``bytes`` could not be read from the ``fileReader``
                       for another reason.
*/
proc fileReader.readBytes(ref b: bytes, maxSize: int): bool throws {
  var (e, lenRead) = readBytesOrString(this, b, maxSize);

  if e != 0 && e != EEOF then throw createSystemError(e);

  return lenRead > 0;
}

private proc readBytesOrString(ch: fileReader, ref out_var: ?t, len: int(64)) : (errorCode, int(64))
  throws
{

  var err:errorCode = 0;
  var lenread:int(64);

  on ch._home {
    var tx:c_string;
    var lentmp:int(64);
    var actlen:int(64);
    var uselen:c_ssize_t;

    if len == -1 then uselen = max(c_ssize_t);
    else {
      uselen = len:c_ssize_t;
      if c_ssize_t != int(64) then assert( len == uselen );
    }

    try ch.lock(); defer { ch.unlock(); }

    var binary:uint(8) = qio_channel_binary(ch._channel_internal);
    var byteorder:uint(8) = qio_channel_byteorder(ch._channel_internal);

    if binary {
      err = qio_channel_read_string(false, byteorder,
                                    iostringstyleInternal.data_toeof:int(64),
                                    ch._channel_internal, tx,
                                    lenread, uselen);
    } else {
      var save_style: iostyleInternal = ch._styleInternal();
      var style: iostyleInternal = ch._styleInternal();
      style.string_format = QIO_STRING_FORMAT_TOEOF;
      ch._set_styleInternal(style);

      if t == string {
        err = qio_channel_scan_string(false,
                                      ch._channel_internal, tx,
                                      lenread, uselen);
      }
      else {
        err = qio_channel_scan_bytes(false,
                                     ch._channel_internal, tx,
                                     lenread, uselen);
      }
      ch._set_styleInternal(save_style);
    }

    if t == string {
      var tmp = createStringWithOwnedBuffer(tx, length=lenread);
      out_var <=> tmp;
    }
    else {
      var tmp = createBytesWithOwnedBuffer(tx, length=lenread);
      out_var <=> tmp;
    }
  }

  return (err, lenread);

}

/*
   Read bits with binary I/O

   :arg v: where to store the read bits. This value will have its *nbits*
           least-significant bits set.
   :arg nbits: how many bits to read
   :returns: `true` if the bits were read without error, `false` upon EOF

   :throws UnexpectedEofError: Thrown if unexpected EOF encountered while reading.
   :throws SystemError: Thrown if the bits could not be read from the channel.
 */
proc _channel.readbits(ref v:integral, nbits:integral):bool throws {
  if castChecking {
    // Error if reading more bits than fit into v
    if numBits(v.type) < nbits then
      throw new owned IllegalArgumentError("v, nbits", "readbits nbits=" + nbits:string +
                                                 " > bits in v:" + v.type:string);
    // Error if reading negative number of bits
    if isIntType(nbits.type) && nbits < 0 then
      throw new owned IllegalArgumentError("nbits", "readbits nbits=" + nbits:string + " < 0");
  }

  var tmp:ioBits;
  tmp.nbits = nbits:int(8);
  var ret = try this.read(tmp);
  v = tmp.v:v.type;
  return ret;
}

/*
   Write bits with binary I/O

   :arg v: a value containing *nbits* bits to write the least-significant bits
   :arg nbits: how many bits to write

   :throws UnexpectedEofError
   :throws IllegalArgumentError: Thrown if writing more bits than fit into `v`.
   :throws SystemError: Thrown if the bits could not be written to the channel.
 */
proc _channel.writebits(v:integral, nbits:integral) throws {
  if castChecking {
    // Error if writing more bits than fit into v
    if numBits(v.type) < nbits then
      throw new owned IllegalArgumentError("v, nbits", "writebits nbits=" + nbits:string +
                                                 " > bits in v:" + v.type:string);
    // Error if writing negative number of bits
    if isIntType(nbits.type) && nbits < 0 then
      throw new owned IllegalArgumentError("nbits", "writebits nbits=" + nbits:string + " < 0");
  }

  try this.write(new ioBits(v:uint(64), nbits:int(8)));
}

/*
  Write ``size`` coidpoints from a :type:`~String.string` to a ``filewriter``

  :arg s: the ``string`` to write
  :arg size: the number of codepoints to write from the ``string``

  :throws UnexpectedEofError
  :throws SystemError: Thrown if the string could not be written to the fileWriter.
  :throws IllegalArgumentError: Thrown if ``size`` is larger than ``s.size``
*/
proc fileWriter.writeString(s: string, size = s.size) throws {
  // TODO: use a separate implementation when `fileWriter`s start supporting
  //        non UTF-8 character encodings
  try this.writeBinary(s, size);
}

/*
  Write ``size`` bytes from a :type:`~Bytes.bytes` to a ``fileWriter``

  :arg b: the ``bytes`` to write
  :arg size: the number of bytes to write from the ``bytes``

  :throws UnexpectedEofError
  :throws SystemError: Thrown if the bytes could not be written to the fileWriter.
  :throws IllegalArgumentError: Thrown if ``size`` is larger than ``b.size``
*/
proc fileWriter.writeBytes(b: bytes, size = b.size) throws {
  try this.writeBinary(b, size);
}

/*
   Write ``numBytes`` of data from a :class:`~CTypes.c_ptr` to a ``fileWriter``

   Note that native endianness is always used.

   If ``numBytes`` is not evenly divisible by the size of ``t``, the remaining
   bytes will be ignored. For example, if the ``c_ptr``'s internal type is 4
   bytes in length, and ``numBytes=17``, only 16 bytes will be written.

   .. warning::
      This method provides no protection against attempting to access invalid memory

   :arg ptr: a :class:`~CTypes.c_ptr` to some valid memory
   :arg numBytes: the number of bytes to write

   :throws UnexpectedEofError
   :throws SystemError: Thrown if an error occurred while writing to the ``fileWriter``
*/
proc fileWriter.writeBinary(ptr: c_ptr(?t), numBytes: int) throws
{
  var e:errorCode = 0,
      numWritten:c_ssize_t;
  const t_size = c_sizeof(t),
        numBytesToWrite = (numBytes / t_size) * t_size;

  e = try qio_channel_write(false, this._channel_internal, ptr[0], numBytesToWrite:c_ssize_t, numWritten);

  if (e != 0) {
    throw createSystemOrChplError(e);
  }
}

/*
   Write ``numBytes`` of data from a :type:`~CTypes.c_void_ptr` to a ``fileWriter``

   The data are written to the file one byte at a time.

   .. warning::
      This method provides no protection against attempting to access invalid memory

   :arg ptr: a typeless :type:`~CTypes.c_void_ptr` to some valid memory
   :arg numBytes: the number of bytes to write

   :throws UnexpectedEofError
   :throws SystemError: Thrown if an error occurred while writing to the ``fileWriter``
*/
proc fileWriter.writeBinary(ptr: c_void_ptr, numBytes: int) throws {
  var e:errorCode = 0,
      numWritten:c_ssize_t;

  var byte_ptr = ptr : c_ptr(uint(8));
  e = try qio_channel_write(false, this._channel_internal, byte_ptr[0], numBytes:c_ssize_t, numWritten);

  if (e != 0) {
    throw createSystemOrChplError(e);
  }
}

/*
   Write a binary number to the channel

   :arg arg: number to be written
   :arg endian: :type:`ioendian` compile-time argument that specifies the byte order in which
              to write the number. Defaults to ``ioendian.native``.

   :throws UnexpectedEofError
   :throws SystemError: Thrown if the number could not be written to the channel.
 */
proc _channel.writeBinary(arg:numeric, param endian:ioendian = ioendian.native) throws {
  var e:errorCode = 0;

  select (endian) {
    when ioendian.native {
      e = try _write_binary_internal(_channel_internal, iokind.native, arg);
    }
    when ioendian.big {
      e = try _write_binary_internal(_channel_internal, iokind.big, arg);
    }
    when ioendian.little {
      e = try _write_binary_internal(_channel_internal, iokind.little, arg);
    }
  }
  if (e != 0) {
    throw createSystemOrChplError(e);
  }
}

/*
   Write a binary number to the channel

   :arg arg: number to be written
   :arg endian: :type:`ioendian` specifies the byte order in which
              to write the number.

   :throws UnexpectedEofError
   :throws SystemError: Thrown if the number could not be written to the channel.
 */
proc _channel.writeBinary(arg:numeric, endian:ioendian) throws {
  select (endian) {
    when ioendian.native {
      this.writeBinary(arg, ioendian.native);
    }
    when ioendian.big {
      this.writeBinary(arg, ioendian.big);
    }
    when ioendian.little {
      this.writeBinary(arg, ioendian.little);
    }
  }
}

/*
   Write a :type:`~String.string` to a fileWriter in binary format

   :arg s: the ``string`` to write
   :arg size: the number of codepoints to write from the ``string``

   :throws UnexpectedEofError
   :throws SystemError: Thrown if the string could not be written to the fileWriter.
   :throws IllegalArgumentError: Thrown if ``size`` is larger than ``s.size``
*/
proc fileWriter.writeBinary(s: string, size: int = s.size) throws {
  // handle bad arguments
  if size > s.size then
    throw new owned IllegalArgumentError("size", "cannot exceed length of provided string");
  if s.hasEscapes then
    throw createSystemOrChplError(EILSEQ, "illegal use of escaped string characters in 'writeBinary'");

  on this._home {
    // count the number of bytes to write
    var sLocal = s.localize();
    var bytesLen = 0;
    if size == sLocal.size {
      bytesLen = s.numBytes;
    } else {
      for ((_, cp_byte_len), i) in zip(sLocal._indexLen(), 0..) {
        if i == size then break;
        bytesLen += cp_byte_len;
      }
    }

    // write the first bytesLen bytes of the string to the fileWriter
    var e: errorCode = qio_channel_write_string(
      false,
      iokind.native: c_int,
      qio_channel_str_style(this._channel_internal),
      this._channel_internal,
      sLocal.c_str(),
      bytesLen: c_ssize_t
    );

    if e != 0 then
      throw createSystemOrChplError(e);
  }
}

/*
   Write a :type:`~Bytes.bytes` to a fileWriter in binary format

   :arg b: the ``bytes`` to write
   :arg size: the number of bytes to write from the ``bytes``

   :throws UnexpectedEofError
   :throws SystemError: Thrown if the bytes could not be written to the fileWriter.
   :throws IllegalArgumentError: Thrown if ``size`` is larger than ``b.size``
*/
proc fileWriter.writeBinary(b: bytes, size: int = b.size) throws {
  // handle bad arguments
  if size > b.size then
    throw new owned IllegalArgumentError("size", "cannot exceed length of provided bytes");

  on this._home {
    // write the first size bytes to the fileWriter
    var bLocal = b.localize();
    var e: errorCode = qio_channel_write_string(
      false,
      iokind.native: c_int,
      qio_channel_str_style(this._channel_internal),
      this._channel_internal,
      bLocal.c_str(),
      size: c_ssize_t
    );

    if e != 0 then
      throw createSystemOrChplError(e);
  }
}

/*
   Write an array of binary numbers to a fileWriter

   Note that this routine currently requires a 1D rectangular non-strided array.

   :arg data: an array of numbers to write to the fileWriter
   :arg endian: :type:`ioendian` compile-time argument that specifies the byte order in which
              to read the numbers. Defaults to ``ioendian.native``.

   :throws SystemError: Thrown if the values could not be written to the fileWriter.
   :throws UnexpectedEofError: Thrown if EOF is reached before all the values could be written.
*/
proc fileWriter.writeBinary(const ref data: [?d] ?t, param endian:ioendian = ioendian.native) throws
  where (d.rank == 1 && d.stridable == false) && (
          isIntegralType(t) || isRealType(t) || isImagType(t) || isComplexType(t))
{
  var e : errorCode = 0;

  on this._home {
    try this.lock(); defer { this.unlock(); }

    for b in data {
      select (endian) {
        when ioendian.native {
          e = try _write_binary_internal(this._channel_internal, iokind.native, b);
        }
        when ioendian.big {
          e = try _write_binary_internal(this._channel_internal, iokind.big, b);
        }
        when ioendian.little {
          e = try _write_binary_internal(this._channel_internal, iokind.little, b);
        }
      }

      if e == EEOF {
        throw new owned UnexpectedEofError("Unable to write entire array of values in 'writeBinary'");
      } else if e != 0 {
        throw createSystemOrChplError(e);
      }
    }
  }
}

/*
   Write an array of binary numbers to a fileWriter

   Note that this routine currently requires a 1D rectangular non-strided array.

   :arg data: an array of numbers to write to the fileWriter
   :arg endian: :type:`ioendian` specifies the byte order in which
              to write the number.

   :throws SystemError: Thrown if the values could not be written to the fileWriter.
   :throws UnexpectedEofError: Thrown if EOF is reached before all the values could be written.
*/
proc fileWriter.writeBinary(const ref data: [?d] ?t, endian:ioendian) throws
  where (d.rank == 1 && d.stridable == false) && (
          isIntegralType(t) || isRealType(t) || isImagType(t) || isComplexType(t))
{
  select (endian) {
    when ioendian.native {
      this.writeBinary(data, ioendian.native);
    }
    when ioendian.big {
      this.writeBinary(data, ioendian.big);
    }
    when ioendian.little {
      this.writeBinary(data, ioendian.little);
    }
  }
}

/*
   Read a binary number from the channel

   :arg arg: number to be read
   :arg endian: :type:`ioendian` compile-time argument that specifies the byte order in which
              to read the number. Defaults to ``ioendian.native``.
   :returns: `true` if the number was read, `false` otherwise

   :throws UnexpectedEofError
   :throws SystemError: Thrown if an error occurred reading the number.
 */
proc _channel.readBinary(ref arg:numeric, param endian:ioendian = ioendian.native):bool throws {
  var e:errorCode = 0;

  select (endian) {
    when ioendian.native {
      e = try _read_binary_internal(_channel_internal, iokind.native, arg);
    }
    when ioendian.big {
      e = try _read_binary_internal(_channel_internal, iokind.big, arg);
    }
    when ioendian.little {
      e = try _read_binary_internal(_channel_internal, iokind.little, arg);
    }
  }
  if (e == EEOF) {
    return false;
  } else if (e != 0) {
    throw createSystemOrChplError(e);
  }
  return true;
}

/*
   Read a binary number from the channel

   :arg arg: number to be read
   :arg endian: :type:`ioendian` specifies the byte order in which
              to read the number.
   :returns: `true` if the number was read, `false` otherwise

   :throws UnexpectedEofError
   :throws SystemError: Thrown if an error occurred reading the number.
 */
proc _channel.readBinary(ref arg:numeric, endian: ioendian):bool throws {
  var rv: bool = false;

  select (endian) {
    when ioendian.native {
      rv = this.readBinary(arg, ioendian.native);
    }
    when ioendian.big {
      rv = this.readBinary(arg, ioendian.big);
    }
    when ioendian.little {
      rv = this.readBinary(arg, ioendian.little);
    }
  }
  return rv;
}

/*
   Read a specified number of codepoints into a :type:`~String.string`

   The string ``s`` may be smaller than ``maxSize`` if EOF is reached before
   reading the specified number of codepoints. Additionally, if nothing
   is read from the fileReader, ``s`` will be set to ``""`` (the empty string)
   and the method will return ``false``.

   .. note::

      This method always uses UTF-8 encoding regardless of the fileReader's
      configuration

   :arg s: the string to read into — this value is overwritten
   :arg maxSize: the number of codepoints to read from the fileReader
   :returns: `false` if EOF is reached before reading anything, `true` otherwise

   :throws UnexpectedEofError
   :throws SystemError: Thrown if an error occurred while reading from the fileReader.
*/
proc fileReader.readBinary(ref s: string, maxSize: int): bool throws {
  var e:errorCode = 0,
      didRead = false;

  on this._home {
    var len: int(64),
        tx: c_string;

    e = qio_channel_read_string(false, ioendian.native: c_int,
                                qio_channel_str_style(this._channel_internal),
                                this._channel_internal, tx, len, maxSize:c_ssize_t);

    if len > 0 then didRead = true;
    s = try! createStringWithOwnedBuffer(tx, length=len);
  }

  if e == EEOF {
    return didRead;
  } else if e != 0 {
    throw createSystemOrChplError(e);
  }
  return true;
}

/*
   Read a specified number of bytes into a :type:`~Bytes.bytes`

   The bytes ``b`` may be smaller than ``maxSize`` if EOF is reached before
   reading the specified number of bytes. Additionally, if nothing is read
   from the fileReader, ``b`` will be set to ``b""`` (the empty bytes) and
   the method will return ``false``.

   :arg b: the bytes to read into — this value is overwritten
   :arg maxSize: the number of bytes to read from the fileReader
   :returns: `false` if EOF is reached before reading anything, `true` otherwise

   :throws UnexpectedEofError
   :throws SystemError: Thrown if an error occurred while reading from the fileReader.
*/
proc fileReader.readBinary(ref b: bytes, maxSize: int): bool throws {
  var e:errorCode = 0,
      didRead = false;

  on this._home {
    var len: int(64),
        tx: c_string;

    e = qio_channel_read_string(false, ioendian.native: c_int,
                                qio_channel_str_style(this._channel_internal),
                                this._channel_internal, tx, len, maxSize:c_ssize_t);

    if len > 0 then didRead = true;
    b = try! createBytesWithOwnedBuffer(tx, length=len);
  }

  if e == EEOF {
    return didRead;
  } else if e != 0 {
    throw createSystemOrChplError(e);
  }
  return true;
}

/*
   Read an array of binary numbers from a fileReader

   Binary values of the type ``data.eltType`` are consumed from the fileReader
   until ``data`` is full or EOF is reached. An :class:`~OS.UnexpectedEofError`
   is thrown if EOF is reached before the array is filled.

   Note that this routine currently requires a 1D rectangular non-strided array.

   :arg data: an array to read into – existing values are overwritten.
   :arg endian: :type:`ioendian` compile-time argument that specifies the byte order in which
              to read the numbers. Defaults to ``ioendian.native``.
   :returns: `false` if EOF is encountered before reading anything,
              `true` otherwise

   :throws SystemError: Thrown if an error occurred while reading from the fileReader
   :throws UnexpectedEofError: Thrown if EOF is encountered before ``data.size`` values are read
*/
proc fileReader.readBinary(ref data: [?d] ?t, param endian = ioendian.native): bool throws
  where (d.rank == 1 && d.stridable == false) && (
          isIntegralType(t) || isRealType(t) || isImagType(t) || isComplexType(t))
{
  var e : errorCode = 0,
      readSomething = false;

  on this._home {
    try this.lock(); defer { this.unlock(); }

    for (i, b) in zip(data.domain, data) {
      select (endian) {
        when ioendian.native {
          e = try _read_binary_internal(this._channel_internal, iokind.native, b);
        }
        when ioendian.big {
          e = try _read_binary_internal(this._channel_internal, iokind.big,    b);
        }
        when ioendian.little {
          e = try _read_binary_internal(this._channel_internal, iokind.little, b);
        }
      }

      if e == EEOF {
        if i == data.domain.first {
          break;
        } else {
          throw new owned UnexpectedEofError("Unable to read entire array of values in 'readBinary'");
        }
      } else if e != 0 {
        throw createSystemOrChplError(e);
      } else {
        readSomething = true;
      }
    }
  }

  return readSomething;
}

/*
   Read an array of binary numbers from a fileReader

   Binary values of the type ``data.eltType`` are consumed from the fileReader
   until ``data`` is full or EOF is reached. An :class:`~OS.UnexpectedEofError`
   is thrown if EOF is reached before the array is filled.

   Note that this routine currently requires a 1D rectangular non-strided array.

   :arg data: an array to read into – existing values are overwritten.
   :arg endian: :type:`ioendian` specifies the byte order in which
              to read the number.
   :returns: `false` if EOF is encountered before reading anything,
              `true` otherwise

   :throws SystemError: Thrown if an error occurred while reading the from fileReader
   :throws UnexpectedEofError: Thrown if EOF is encountered before ``data.size`` values are read
*/
proc fileReader.readBinary(ref data: [?d] ?t, endian: ioendian):bool throws
  where (d.rank == 1 && d.stridable == false) && (
          isIntegralType(t) || isRealType(t) || isImagType(t) || isComplexType(t))
{
  var rv: bool = false;

  select (endian) {
    when ioendian.native {
      rv = this.readBinary(data, ioendian.native);
    }
    when ioendian.big {
      rv = this.readBinary(data, ioendian.big);
    }
    when ioendian.little {
      rv = this.readBinary(data, ioendian.little);
    }
  }

  return rv;
}

/*
   Read up to ``maxBytes`` bytes from a ``fileReader`` into a :class:`~CTypes.c_ptr`

   Note that native endianness is always used.

   If ``maxBytes`` is not evenly divisible by the size of ``t``, then the
   remaining bytes are ignored.

   :arg ptr: a :class:`~CTypes.c_ptr` to some memory — existing values will be
              overwritten
   :arg maxBytes: the maximum number of bytes to read from the ``fileReader``
   :returns: the number of bytes that were read. this can be less than
              ``maxBytes`` if EOF was reached before reading the specified
              number of bytes, or if ``maxBytes`` is not evenly divisible by
              the size of ``t``

   :throws UnexpectedEofError
   :throws SystemError: Thrown if an error occurred while reading from the ``fileReader``
*/
proc fileReader.readBinary(ptr: c_ptr(?t), maxBytes: int): int throws {
  var e: errorCode = 0,
      numRead: c_ssize_t = 0;
  const t_size = c_sizeof(t),
        numBytesToRead = (maxBytes / t_size) * t_size;

  e = qio_channel_read(false, this._channel_internal, ptr[0], numBytesToRead: c_ssize_t, numRead);

  if e != 0 && e != EEOF then throw createSystemOrChplError(e);
  return numRead;
}

/*
   Read up to ``maxBytes`` bytes from a ``fileReader`` into a :type:`~CTypes.c_void_ptr`

   Note that data are read from the file one byte at a time.

   :arg ptr: a typeless :type:`~CTypes.c_void_ptr` to some memory — existing values
              will be overwritten
   :arg maxBytes: the maximum number of bytes to read from the ``fileReader``
   :returns: the number of bytes that were read. this can be less than ``maxBytes``
              if EOF was reached before reading the specified number of bytes

   :throws UnexpectedEofError
   :throws SystemError: Thrown if an error occurred while reading from the ``fileReader``
*/
proc fileReader.readBinary(ptr: c_void_ptr, maxBytes: int): int throws {
  var e: errorCode = 0,
      numRead: c_ssize_t = 0;
  var bytes_ptr = ptr: c_ptr(uint(8));

  e = qio_channel_read(false, this._channel_internal, bytes_ptr[0], maxBytes: c_ssize_t, numRead);

  if e != 0 && e != EEOF then throw createSystemOrChplError(e);
  return numRead;
}


// Documented in the varargs version
pragma "no doc"
proc _channel.readln():bool throws {
  var nl = new ioNewline();
  return try this.read(nl);
}

/*

   Read values from a channel and then consume any bytes until
   newline is reached. The input will be consumed atomically - the
   channel lock will be held while reading all of the passed values.

   :arg args: a list of arguments to read. This routine can be called
              with zero or more such arguments. Basic types are handled
              internally, but for other types this function will call
              value.readThis() with a ``Reader`` argument as described
              in :ref:`readThis-writeThis`.
   :returns: `true` if the read succeeded, and `false` upon end of file.

   :throws UnexpectedEofError
   :throws SystemError: Thrown if a line could not be read from the channel.
 */
proc _channel.readln(ref args ...?k):bool throws {
  var nl = new ioNewline();
  return try this.read((...args), nl);
}

@unstable "readln with a style argument is unstable"
proc _channel.readln(ref args ...?k,
                     style:iostyle):bool throws {
  return this.readlnHelper((...args), style: iostyleInternal);
}

pragma "no doc"
proc _channel.readlnHelper(ref args ...?k,
                          style:iostyleInternal):bool throws {
  var nl = new ioNewline();
  return try this.readHelper((...args), nl, style=style);
}

/*
   Read a value of passed type.

   For example, the following line of code reads a value of type `int`
   from :var:`stdin` and uses it to initialize a variable ``x``:

   .. code-block:: chapel

     var x = stdin.read(int)


   :arg t: the type to read
   :returns: the value read

   :throws UnexpectedEofError
   :throws SystemError: Thrown if the type could not be read from the channel.
 */
proc _channel.read(type t) throws {
  var tmp:t;
  this._readInner(tmp);
  return tmp;
}

/*
   Read a value of passed type followed by a newline.

   :arg t: the type to read
   :returns: the value read

   :throws UnexpectedEofError
   :throws SystemError: Thrown if the type could not be read from the channel.
 */
proc _channel.readln(type t) throws {
  var tmp:t;
  var nl = new ioNewline();
  this._readInner(tmp, nl);
  return tmp;
}

/*
   Read values of passed types followed by a newline
   and return a tuple containing the read values.

   :arg t: more than one type to read
   :returns: a tuple of the read values

   :throws UnexpectedEofError
   :throws SystemError: Thrown if the types could not be read from the channel.
 */
proc _channel.readln(type t ...?numTypes) throws where numTypes > 1 {
  var tupleVal: t;
  var nl = new ioNewline();
  this._readInner((...tupleVal), nl);
  return tupleVal;
}

/*
   Read values of passed types and return a tuple containing the read values.

   :arg t: more than one type to read
   :returns: a tuple of the read values

   :throws UnexpectedEofError
   :throws SystemError: Thrown if the types could not be read from the channel.
 */
proc _channel.read(type t ...?numTypes) throws where numTypes > 1 {
  var tupleVal: t;
  this._readInner((...tupleVal));
  return tupleVal;
}

/*
   Write values to a channel. The output will be produced atomically -
   the channel lock will be held while writing all of the passed
   values.

   :arg args: a list of arguments to write. Basic types are handled
              internally, but for other types this function will call
              value.writeThis() with the channel as an argument.

   :throws UnexpectedEofError
   :throws SystemError: Thrown if the values could not be written to the channel.
   :throws EofError: Thrown if EOF is reached before all the arguments
                      could be written.
 */
pragma "fn exempt instantiation limit"
inline proc _channel.write(const args ...?k) throws {
  if !writing then compilerError("write on read-only channel");

  const origLocale = this.getLocaleOfIoRequest();
  on this._home {
    try this.lock(); defer { this.unlock(); }
    for param i in 0..k-1 {
      try _writeOne(kind, args(i), origLocale);
    }
  }
}

@unstable "write with a style argument is unstable"
proc _channel.write(const args ...?k, style:iostyle) throws {
  this.writeHelper((...args), style: iostyleInternal);
}

// helper function for iostyle deprecation
pragma "no doc"
proc _channel.writeHelper(const args ...?k, style:iostyleInternal) throws {
  if !writing then compilerError("write on read-only channel");
  const origLocale = this.getLocaleOfIoRequest();

  on this._home {
    try this.lock(); defer { this.unlock(); }

    var saveStyle: iostyleInternal = this._styleInternal();
    this._set_styleInternal(style);
    defer {
      this._set_styleInternal(saveStyle);
    }

    for param i in 0..k-1 {
      try _writeOne(iokind.dynamic, args(i), origLocale);
    }
  }
}

// documented in varargs version
pragma "no doc"
proc _channel.writeln() throws {
  try this.write(new ioNewline());
}

/*

   Write values to a channel followed by a newline.  The output will be
   produced atomically - the channel lock will be held while writing all of the
   passed values.

   :arg args: a variable number of arguments to write. This method can be
              called with zero or more arguments. Basic types are handled
              internally, but for other types this function will call
              value.writeThis() with the channel as an argument.

   :throws UnexpectedEofError
   :throws SystemError: Thrown if the values could not be written to the channel.
   :throws UnexpectedEofError: Thrown if EOF is reached before all the arguments
                      could be written.
 */
proc _channel.writeln(const args ...?k) throws {
  try this.write((...args), new ioNewline());
}

@unstable "writeln with a style argument is unstable"
proc _channel.writeln(const args ...?k, style:iostyle) throws {
  try this.writeHelper((...args), new ioNewline(), style=style);
}

/*

  Makes all writes to the channel, if any, available to concurrent viewers
  of its associated file, such as other channels or other applications
  accessing this file concurrently.
  Unlike :proc:`file.fsync`, this does not commit the written data
  to the file's device.

  :throws SystemError: Thrown if the flush fails.
*/
proc _channel.flush() throws {
  var err:errorCode = 0;
  on this._home {
    err = qio_channel_flush(locking, _channel_internal);
  }
  if err then try this._ch_ioerror(err, "in channel.flush");
}

// documented in throws version
pragma "no doc"
proc _channel.flush(out error:errorCode) {
  error = 0;
  try {
    this.flush();
  } catch e: SystemError {
    error = e.err;
  } catch {
    error = EINVAL;
  }
}

/* Assert that a channel has reached end-of-file and that there was no error
   doing the read.
 */
proc _channel.assertEOF(errStr: string = "- Not at EOF") {
  var isEOF = try! this.atEOF();
  if !isEOF then
    try! this._ch_ioerror("assert failed", errStr);
}

/* Returns true if a channel has reached end-of-file, false if not.
   Throws an error if this is a writing channel, or if there was
   an error doing the read.

   Inherently racy for channels, hence no doc.
 */
pragma "no doc"
proc _channel.atEOF(): bool throws {
  if writing {
    try this._ch_ioerror(EINVAL, "assertEOF on writing channel");
  } else {
    var tmp:uint(8);
    return !(try this.read(tmp));
  }
}

/*
  Close a channel. Implicitly performs the :proc:`channel.flush` operation
  (see :ref:`about-io-channel-synchronization`).

  :throws SystemError: Thrown if the channel is not successfully closed.
*/
proc _channel.close() throws {
  var err:errorCode = 0;

  if is_c_nil(_channel_internal) then
    throw createSystemOrChplError(EINVAL, "cannot close invalid channel");

  on this._home {
    err = qio_channel_close(locking, _channel_internal);
  }
  if err then try this._ch_ioerror(err, "in channel.close");
}

/*
   Return `true` if a channel is currently closed.
 */
proc _channel.isClosed() : bool {
  var ret:bool;
  on this._home {
    ret = qio_channel_isclosed(locking, _channel_internal);
  }
  return ret;
}


deprecated "channel.isclosed is deprecated. Please use channel.isClosed instead"
proc _channel.isclosed() : bool {
  return this.isClosed();
}


// TODO -- we should probably have separate c_ptr ddata and ref versions
// in this function for it to become user-facing. Right now, errors
// in the type of the argument will only be caught by a type mismatch
// in the call to qio_channel_read_amt.
pragma "no doc"
proc _channel._readBytes(x, len:c_ssize_t) throws {
  if here != this._home then
    throw new owned IllegalArgumentError("bad remote channel._readBytes");
  var err = qio_channel_read_amt(false, _channel_internal, x, len);
  if err then try this._ch_ioerror(err, "in channel._readBytes");
}

pragma "no doc"
record itemReaderInternal {
  /* What type do we read and yield? */
  type ItemType;
  /* the kind field for our channel */
  param kind:iokind;
  /* the locking field for our channel */
  param locking:bool;
  /* our channel */
  var ch:fileReader(kind,locking);
  /* read a single item, throwing on error */
  proc read(out arg:ItemType):bool throws {
    return ch.read(arg);
  }

  /* iterate through all items of that type read from the channel */
  iter these() { // TODO: this should be throws
    while true {
      var x:ItemType;
      var gotany:bool;
      try! { // TODO: this should by try
        gotany = ch.read(x);
      }
      if ! gotany then break;
      yield x;
    }
  }
}

// And now, the toplevel items.

/* standard input, otherwise known as file descriptor 0 */
const stdin:fileReader(iokind.dynamic, true);
stdin = try! openfd(0).reader();

pragma "no doc"
extern proc chpl_cstdout():c_FILE;
/* standard output, otherwise known as file descriptor 1 */
const stdout:fileWriter(iokind.dynamic, true);
stdout = try! openfp(chpl_cstdout()).writer();

pragma "no doc"
extern proc chpl_cstderr():c_FILE;
/* standard error, otherwise known as file descriptor 2 */
const stderr:fileWriter(iokind.dynamic, true);
stderr = try! openfp(chpl_cstderr()).writer();
/* Equivalent to ``stdin.read``. See :proc:`channel.read` */
proc read(ref args ...?n):bool throws {
  return stdin.read((...args));
}
/* Equivalent to ``stdin.read``. See :proc:`channel.read` for types */
proc read(type t ...?numTypes) throws {
  return stdin.read((...t));
}

/* Equivalent to ``stdin.readLine``.  See :proc:`channel.readLine` */
proc readLine(ref a: [] ?t, maxSize=a.size, stripNewline=false): int throws
      where (t == uint(8) || t == int(8)) && a.rank == 1 && a.isRectangular() && ! a.stridable {
  return stdin.readLine(a, maxSize, stripNewline);
}

pragma "last resort"
pragma "no doc"
proc readLine(ref a: [] ?t, maxSize=a.size, stripNewline=false): int throws
      where (t == uint(8) || t == int(8)) {
  compilerError("'readLine()' is currently only supported for non-strided 1D rectangular arrays");
}

/* Equivalent to ``stdin.readline``.  See :proc:`channel.readline` */
deprecated "readline is deprecated. Use :proc:`readLine` instead"
proc readline(arg: [] uint(8), out numRead : int, start = arg.domain.lowBound,
              amount = arg.domain.highBound - start + 1) : bool throws
                where arg.rank == 1 && arg.isRectangular() {
  return stdin.readline(arg, numRead, start, amount);
}

/* Equivalent to ``stdin.readline``.  See :proc:`channel.readline` */
deprecated "readline is deprecated. Use :proc:`readLine` instead"
proc readline(ref arg: ?t): bool throws where t==string || t==bytes {
  return stdin.readline(arg);
}

/* Equivalent to ``stdin.readLine``.  See :proc:`channel.readLine` */
proc readLine(ref s: string, maxSize=-1, stripNewline=false): bool throws{
  return stdin.readLine(s, maxSize, stripNewline);
}

/* Equivalent to ``stdin.readLine``.  See :proc:`channel.readLine` */
proc readLine(ref b: bytes, maxSize=-1, stripNewline=false): bool throws{
  return stdin.readLine(b, maxSize, stripNewline);
}

/* Equivalent to ``stdin.readLine``.  See :proc:`channel.readLine` */
proc readLine(type t=string, maxSize=-1, stripNewline=false): t throws where t==string || t==bytes {
  return stdin.readLine(t, maxSize, stripNewline);
}

/* Equivalent to ``stdin.readln``. See :proc:`channel.readln` */
proc readln(ref args ...?n):bool throws {
  return stdin.readln((...args));
}
// documented in the arguments version.
pragma "no doc"
proc readln():bool throws {
  return stdin.readln();
}

/* Equivalent to ``stdin.readln``. See :proc:`channel.readln` for types */
proc readln(type t ...?numTypes) throws {
  return stdin.readln((...t));
}

/*
   :returns: `true` if this version of the Chapel runtime supports UTF-8 output.
 */
deprecated "unicodeSupported is deprecated due to always returning true"
proc unicodeSupported():bool {
  return true;
}


/************** Distributed File Systems ***************/

private extern const FTYPE_NONE   : c_int;
private extern const FTYPE_LUSTRE : c_int;

pragma "no doc"
proc file.fstype():int throws {
  var t:c_int;
  var err:errorCode = 0;
  on this._home {
    err = qio_get_fs_type(this._file_internal, t);
  }
  if err then try ioerror(err, "in file.fstype()");
  return t:int;
}

/*
   Returns the 'best' locale to run something working with the region
   of the file in start..end-1.

   This *must* return the same result when called from different locales.
   Returns a domain of locales that are "best" for the given region. If no
   locales are "best" we return a domain containing all locales.

   :arg start: the file offset (starting from 0) where the region begins
   :arg end: the file offset just after the region
   :returns: a set of locales that are best for working with this region
   :rtype: domain(locale)
 */
 deprecated "file.localesForRegion is deprecated"
proc file.localesForRegion(start:int(64), end:int(64)) {

  proc findloc(loc:string, locs:c_ptr(c_string), end:int) {
    for i in 0..end-1 {
      if (loc == locs[i]) then
        return true;
    }
    return false;
  }

  var ret: domain(locale);
  on this._home {
    var err:errorCode;
    var locs: c_ptr(c_string);
    var num_hosts:c_int;
    err = qio_locales_for_region(this._file_internal, start, end, c_ptrTo(locs), num_hosts);
    // looping over Locales enforces the ordering constraint on the locales.
    for loc in Locales {
      if (findloc(loc.name, locs, num_hosts:int)) then
        ret += loc;
    }

    // We allocated memory in the runtime for this, so free it now
    if num_hosts != 0 {
      for i in 0..num_hosts-1 do
        chpl_free_c_string(locs[i]);
      c_free(locs);
    }

    // We found no "good" locales. So any locale is just as good as the next
    if ret.size == 0 then
      for loc in Locales do
        ret += loc;
  }
  return ret;
}


/*


Support for formatted input and output.


.. _about-io-formatted-io:

Formatted I/O
-------------

See below for a :ref:`sample-based overview <about-io-formatted-io-overview>`
of the format strings. Further below, we describes the format string syntax
:ref:`in detail <about-io-formatted-io-in-detail>`. Finally, we demonstrate the
functionality with :ref:`example function calls
<about-io-formatted-io-examples>`.

.. _about-io-formatted-io-overview:

Overview of Format Strings
++++++++++++++++++++++++++

In a manner similar to C's 'printf' and 'scanf', the IO package includes
:proc:`channel.writef` and :proc:`channel.readf` functions. These functions take
in a format string and some arguments. The :proc:`string.format` method is also
available and is loosely equivalent to C's 'sprintf'. For example, one might do:

.. code-block:: chapel

  writef("My favorite %s is %i\n", "number", 7);

  var s:string = "My favorite %s is %i".format("number", 7);
  writeln(s);

  // prints:
  // My favorite number is 7
  // My favorite number is 7

The following sections offer a tour through the conversions to illustrate the
common cases. A more precise definition follows in the "Format String
Syntax in Detail" section below.

In this file, we use "integral" to refer to the Chapel types int or uint and
"floating-point" to refer to real, imaginary, or complex, of any bit width.

.. warning::

   Binary conversions are now deprecated. Binary numeric conversions have been
   replaced by :proc:`IO.channel.readBinary` and :proc:`IO.channel.writeBinary`.
   Replacements for binary string conversions are under development.

Formatted I/O for C Programmers
+++++++++++++++++++++++++++++++

This table is designed to help C programmers understand the equivalent
Chapel format specifiers.

========  ===========  ==========================================
C         Chapel       Meaning
========  ===========  ==========================================
%i        %i           an integer in decimal
%d        %i           an integer in decimal
%u        %u           an unsigned integer in decimal
%x        %xu          an unsigned integer in hexadecimal
%g        %r           real number in exponential or decimal (if compact)
%7.2g     %7.2r        real, 2 significant digits, padded to 7 columns
%f        %dr          real number always in decimal
%7.3f     %7.3dr       real, 3 digits after ``.``, padded to 7 columns
%e        %er          real number always in exponential
%7.3e     %7.3er       real, 3 digits after ``.``, padded to 7 columns
%s        %s           a string without any quoting
========  ===========  ==========================================

Unlike in C, a value of the wrong type will be cast appropriately - so for
example printing 2 (an ``int``)  with ``%.2dr`` will result in ``2.00``.  Note
that ``%n`` and ``%t`` are equivalent to ``%r`` for real conversions and ``%i``
for numeric conversions; so these are also equivalent to ``%i`` ``%d`` or
``%g`` in C. Also note that Chapel format strings includes many capabilities
not available with C formatted I/O routines - including quoted strings,
binary numbers, and complex numbers.


Generic Numeric Conversions
+++++++++++++++++++++++++++

``%{##.###}``
  decimal number padded on the left with spaces to 2 digits before
  the point, rounded to 3 after. Works with integral, real, imaginary,
  or complex arguments.

  In all cases, the output is padded on the left to the total length
  of the conversion specifier (6 in this example).  The output
  can be longer, when needed to accommodate the number.

``%{##}``
  integral value padded out to 2 digits. Also works with real, imaginary
  or complex numbers by rounding them to integers. Numbers with more
  digits will take up more space instead of being truncated.

In both conversions above, an imaginary argument gets an 'i' afterwards
and the entire expression is padded out to the width of ##### digits.
For example:

.. code-block:: chapel

  writef("|${#####}|\n", 2.0i);
       // outputs:
       //   |   2i|

  writef("|%{#####.#}|\n", 2.0i);
       // outputs:
       //   |   2.0i|

Complex arguments are printed in the format a + bi, where each of a and b is
rounded individually as if printed under that conversion on its own. Then, the
formatted complex number is padded to the requested size. For example:

.. code-block:: chapel

  writef("|%{#########}|\n", 1.0+2.0i);
       // outputs:
       //   |   1 + 2i|

  writef("|%{#########.#}|\n", 1.0+2.0i);
       // outputs:
       //   | 1.0 + 2.0i|

See :ref:`about-io-formatted-pound-details` for more details
on this conversion type.

``%n``
  a "number" - equivalent to one of %i, %u, %r, %m, or %z below,
  depending on the type

``%17n``
  a number padded out to 17 columns

``%.4n``
  a number with 4 significant digits or a precision of 4

Integral Conversions
++++++++++++++++++++

``%i`` or ``%di``
  a signed integer in decimal, possibly negative
  (note - when reading an ``%i``, ``-`` is allowed)
``%u`` or ``%du``
  an unsigned decimal integer
  (note - when reading a ``%u``, ``-`` is not allowed)
``%bi``
 a binary signed integer
``%bu``
 a binary unsigned integer
``%@bu``
 a binary unsigned integer prefixed with ``0b``
``%oi``
 an octal signed integer
``%ou``
 an octal unsigned integer
``%@ou``
 an octal unsigned integer prefixed with ``0o``
``%xu``
 a hexadecimal unsigned integer
``%xi``
 a hexadecimal signed integer
``%@xu``
 a hexadecimal unsigned integer prefixed with ``0x``
``%Xu``
 a hexadecimal unsigned integer in uppercase
``%@Xu``
 a hexadecimal unsigned integer prefixed with ``0X`` and uppercase
``%17i``
 a decimal integer padded on the left with spaces to 17 columns
 (That is, it is right-justified in a 17-column field.
 Padding width is ignored when reading integers)
``%*i``
 as with ``%17i`` but read the minimum width from the preceding argument
``%017i``
 a decimal integer padded on the left with zeros to 17 columns
``%-17i``
 a decimal integer left-justified (padded on the right) to 17 columns
``%+i``
 a decimal integer showing ``+`` for positive numbers
``% i``
 a decimal integer with a space for positive numbers
``%|4i``
 output 4 raw, binary bytes of the passed integer in native endianness *(deprecated)*
``%<4i``
 output 4 raw, binary bytes of the passed integer little endian *(deprecated)*
``%>4i``
 output 4 raw, binary bytes of the passed integer big endian *(deprecated)*
``%<8i``
 output 8 raw, binary bytes of the passed integer little endian
 (byte widths of 1, 2, 4, and 8 are supported for integral conversions) *(deprecated)*

Real Conversions
++++++++++++++++

``%r``
 a real number with decimal or exponential notation, where
 exponential is chosen if the decimal version would be too long

``%6r``
 as with ``%r`` but padded on the left to 6 columns (ie right-justified)
``%-6r``
 as with ``%r`` but padded on the right to 6 columns (ie left-justified)
``%.4r``
 as with ``%r`` but with 4 significant digits
``%.*r``
 as with ``%.4r`` but with significant digits read from preceding argument
``%6.4r``
 as with ``%r`` but padded on the left to 6 columns
 and with 4 significant digits
``%*.*r``
 as with ``%6.4r`` but read minimum width and significant digits from
 preceding arguments

``%dr``
 a real number in decimal notation, e.g. ``12.34``
``%6dr``
 a decimal number padded on the left to 6 columns (right-justified)
``%.4dr``
 a decimal number with 4 digits after the radix point
``%6.4dr``
 a decimal number padded on the left to 6 columns and with 4 digits
 after the radix point
 (width and precision are ignored when reading numbers in readf)

``%er``
 a real number in exponential notation, e.g. ``8.2e-23``
``%Er``
 like %er but with the 'e' in uppercase, e.g. ``8.2E-23``
``%.4er``
 exponential notation with 4 digits after the period, e.g. ``8.2000e-23``

``%xer``
 hexadecimal number using p to mark exponent e.g. ``6c.3f7p-2a``

``%|4r``
 emit 4 raw, binary bytes of the passed number in native endianness *(deprecated)*
``%<8r``
 emit 8 raw, binary bytes of the passed number in little endian *(deprecated)*
``%<4r``
 emit 4 raw, binary bytes of the passed number in little endian
 (``<`` ``|`` and ``>`` are supported for widths 4 or 8) *(deprecated)*

Complex and Imaginary Conversions
+++++++++++++++++++++++++++++++++

``%m``
 an imaginary number, like a real with ``%r`` but ends with an ``i``

``%z``
 print complex number with ``%r`` for each part in the format ``a + bi``
``%@z``
 print complex number with ``%r`` for each part in the format ``(a,b)``
``%6z``
 as with ``%z`` but pad the entire complex number out to 6 columns
``%6.4z``
 print a and b 4 significant digits and pad the entire complex
 number out to 6 columns
``%dz``
 print a and b with ``%dr``
``%ez``
 print a and b with ``%er``

``%|4m``
 same as ``%|4r`` *(deprecated)*
``%|8z``
 emit 8 raw, binary bytes of native-endian complex (a,b are each 4 bytes) *(deprecated)*
``%<16z``
 emit 16 raw, binary bytes of little-endian complex (a,b each 8 bytes) *(deprecated)*

String and Bytes Conversions
++++++++++++++++++++++++++++

``%s``
 a string. When reading, read until whitespace.
 Note that if you want to be able to read your string back in,
 you should use one of the quoted or encoded binary versions (see below),
 since generally with %s it's not clear where the string ends.
``%c``
 a single Unicode character (argument should be a string or an integral
 storing the character code)
``%17s``
  * when writing - a string left padded (right justified) to 17 columns
  * when reading - read up to 17 bytes or a whitespace, whichever comes
    first, rounding down to whole characters
``%-17s``
 * when writing - a string right padded (left justified) to 17 columns
``%.17s``
 * when writing - a string truncated to 17 columns. When combined
   with quoting strings, for example ``%.17"S``, the conversion
   will print ... after a string if it was truncated. The
   truncation includes leaving room for the quotes and -
   if needed - the periods, so the shortest truncated
   string is ``""...``  Generally, you won't be able to read
   these back in.
 * when reading - read exactly 17 Unicode codepoints
``%|17s``
 * when writing - emit string but cause runtime error if length
   does not match
 * when reading - read exactly 17 bytes (error if we read < 17 bytes)
``%|*s``
  as with %17s but the length is specified in the argument before the string.
``%"S``
 use double-quotes to delimit string
``%'S``
 use single-quotes to delimit string
``%cS``
 use any character (c) to delimit string
``%{(S)}``
 quoted string, starting with ``(``, ending with ``)``, where the
 parens could be replaced by arbitrary characters
``%*S``
 quoted string, the arg before the string to specifies quote character
``%|0S``
 write a string null-terminated or read bytes until a null-terminator
``%|*S``
 means read bytes until a terminator byte. The terminator byte is read
 from the argument before the string.
``%|1S`` ``%|2S`` ``%|4S`` and ``%|8S``
  work with encoded strings storing a length
  and then the string data. The digit before ``S`` is
  the number of bytes of length which is by default
  stored native endian. ``<``, ``|``, ``>`` can be used
  to specify the endianness of the length field,
  for example ``%<8S`` is 8 bytes of little-endian length
  and then string data. *(deprecated)*
``%|vS``
 as with ``%|1S``-``%|8S`` but the string length is encoded using a
 variable-length byte scheme (which is always the same no matter what
 endianness). In this scheme, the high bit of each encoded length byte
 records whether or not there are more length bytes (and the remaining
 bits encode the length in a big-endian manner). *(deprecated)*

``%|*vS`` or ``%|*0S``
 read an encoded string but limit it to a number of bytes
 read from the argument before the string; when writing
 cause a runtime error if the string is longer than the
 maximum. *(deprecated)*

``%/a+/``
 where any regular expression can be used instead of ``a+``
 consume one or more 'a's when reading, gives an error when printing,
 and does not assign to any arguments
 (note - regular expression support is dependent on RE2 build;
 see :mod:`Regex`)

``%/(a+)/``
 consume one or more 'a's and then set the corresponding string
 argument to the read value

``%17/a+/``
 match a regular expression up to 17 bytes
 (note that ``%.17/a+/``, which would mean to match 17 characters,
 is not supported).

``%/*/``
 next argument contains the regular expression to match

.. (comment) the above started a nested comment, so here we end it */

General Conversions
+++++++++++++++++++

``%t``
 read or write the object according to its readThis/writeThis routine
``%jt``
 read or write an object in JSON format using readThis/writeThis
``%ht``
 read or write an object in Chapel syntax using readThis/writeThis
``%|t``
 read or write an object in binary native-endian with readThis/writeThis *(deprecated)*
``%<t``
 read or write an object little-endian in binary with readThis/writeThis *(deprecated)*
``%>t``
 read or write an object big-endian in binary with readThis/writeThis *(deprecated)*

Note About Whitespace
+++++++++++++++++++++

When reading, ``\n`` in a format string matches any zero or more space
characters other than newline and then exactly one newline character. In
contrast, ``" "`` matches at least one space character of any kind.

When writing, whitespace is printed from the format string just like any
other literal would be.

Finally, space characters after a binary conversion will be ignored, so
that a binary format string can appear more readable.

.. _about-io-formatted-io-in-detail:

Format String Syntax in Detail
++++++++++++++++++++++++++++++

Chapel's format strings are simpler than those in C in one way: it is no longer
necessary to specify the types of the arguments in the format string. For
example, in C the l in %ld is specifying the type of the argument for integer
(decimal) conversion. That is not necessary in Chapel since the compiler is
able to use type information from the call.

Format strings in Chapel consist of:

 * conversion specifiers e.g. ``"%xi"`` (described below)
 * newline e.g. ``"\n"``

   * when writing - prints a newline
   * when reading - reads any amount of non-newline whitespace and then
     exactly one newline. Causes the format string not to
     match if it did not read a newline.

 * other whitespace e.g. ``" "``

    * when writing - prints as the specified whitespace
    * when reading - matches at least one character of whitespace, possibly
      including newlines.

 * other text e.g. "test"

    * when writing - prints the specified text
    * when reading - matches the specified text

.. _about-io-formatted-pound-details:

# Specifiers
++++++++++++

All # specifiers must be enclosed in ``%{}`` syntax, for example ``%{#}`` is the
shortest one, and ``%{#.#}`` is a more typical one. The integer portion of the
number will be padded out to match the number of ``#`` s before the decimal
point, and the number of ``#`` s after the decimal point indicate how many
digits to print after the decimal point. In other words, display how many
digits to use when printing a floating-point number by using the # symbol to
stand for digits. The fractional portion of the number will be rounded
appropriately and extra space will be made if the integer portion is too small:

.. code-block:: chapel

  writef("n:%{###.###}\n", 1.2349);
       // outputs:
       // n:  1.235

This syntax also works for numbers without a decimal point by rounding them
appropriately.

A # specifier may start with a ``.``.

.. code-block:: chapel

  writef("%{.##}\n", 0.777);
       // outputs:
       //  0.78

% Specifiers
++++++++++++

Specifiers starting with % offer quite a few options. First, some basic
rules.

``%%``
 means a literal ``%``
``\n``
 means a literal newline
``\\``
 means a single backslash
``%{}``
 curly braces can wrap a ``%`` or ``#`` conversion specifier. That way, even
 odd specifiers can be interpreted unambiguously. Some of the more complex
 features require the use of the ``%{}`` syntax, but it's always
 acceptable to use curly braces to make the format string clearer.
 Curly braces are required for # conversion specifiers.

In general, a ``%`` specifier consists of either text or binary conversions:

::

 %
 [optional endian flag (binary conversions only) *(deprecated)*]
 [optional flags]
 [optional field width or size in bytes]
 [optional . then precision]
 [optional base flag]
 [optional exponential type]
 [conversion type]

Going through each section for text conversions:


[optional flags]
  ``@``
   means "alternate form". It means to print out a base when not using
   decimal (e.g. ``0xFFF`` or ``0b101011``); and it will format a complex
   number with parens instead of as e.g. ``1.0+2.0i``
  ``+``
   means to show a plus sign when printing positive numbers
  ``0``
   means to pad numeric conversions with 0 instead of space
  ``" "``
   (a space) leaves a blank before a positive number
   (in order to help line up with negative numbers)
  ``-``
   left-justify the converted value instead of right-justifying.
   Note, if both ``0`` and ``-`` are given, the effect is as if only ``-``
   were given.
  ``~``
   when reading a record or class instance, skip over fields in the input not
   present in the Chapel type. This flag currently only works in combination
   with the JSON format.  This flag allows a Chapel program to describe only the
   relevant fields in a record when the input might contain many more fields.


[optional field width]
   When printing numeric or string values, the field width specifies the number
   of *columns* that the conversion should use to display the value. It can be
   ``*``, which means to read the field width from an integral argument before
   the converted value.

   For string conversions in readf (``%s`` ``%"`` ``%'`` ``%//``), the field
   width specifies the maximum number of bytes to read.

   For numeric conversions in readf, the field width is ignored.

[optional . then precision]
   When printing floating point values, the precision is used to control
   the number of decimal digits to print.  For ``%r`` conversions, it
   specifies the number of significant digits to print; for ``%dr`` or ``%er``
   conversions, it specifies the number of digits following the decimal point.
   It can also be ``*``, which means to read the precision from an integral
   argument before the converted value.

   For textual string conversions in writef, (``%s`` ``%"`` ``%'``), the
   precision indicates the maximum number of columns to print - and the result
   will be truncated if it does not fit. In readf for these textual string
   conversions, the precision indicates the maximum number of characters
   (e.g. Unicode codepoints) to input.

   The precision is silently ignored for integral conversions
   (``%i``, ``%u``, etc) and for ``%//`` conversions.

[optional base flag]
   ``d``
    means decimal (and not exponential for floating-point)
   ``x``
    means lower-case hexadecimal
   ``X``
    means upper-case hexadecimal
   ``o``
    means octal
   ``b``
    means binary
   ``j``
    means JSON-style strings, numbers, and structures
   ``h``
    means Chapel-style strings, numbers, and structures
   ``'``
    means single-quoted string (with \\ and \')
   ``"``
    means double-quoted string (with \\ and \")

[optional exponential type]
   ``e``
    means floating-point conversion printing exponential ``-12.34e+56``
   ``E``
    means floating-point conversion printing uppercase
    exponential ``-12.34E+56``

[conversion type]
   ``t``
    means *type-based* or *thing* - uses writeThis/readThis but ignores
    width. Precision will impact any floating point values output
    in this conversion.
   ``n``
    means type-based number, allowing width and precision
   ``i``
    means integral conversion
   ``u``
    means unsigned integral conversion
   ``r``
    means real conversion (e.g. ``12.23``)
   ``m``
    means imaginary conversion with an ``i`` after it (e.g. ``12.23i``)
   ``z``
    means complex conversion
   ``s``
    means string conversion
   ``S``
    means a quoted string conversion
   ``{cS}``
    means string conversion with quote char *c*
   ``{*S}``
    means string conversion with quote char in argument before the string
   ``{xSy}``
    means string conversion with left and right quote chars *x* and *y*
   ``/.../``
    means a regular expression (for reading only)
   ``{/.../xyz}``
    means regular expression with flags *xyz*
   ``c``
    means a Unicode character - either the first character in a string
    or an integral character code

For binary conversions *(deprecated)*:

[optional endian flag]
   ``<``
    means little-endian
   ``>``
    means big-endian
   ``|``
    means native-endian

[optional size in bytes]
   This is the number of bytes the format should read or write in this
   conversion. For integral conversions (e.g. ``%|i``) it specifies the number
   of bytes in the integer, and 1, 2, 4, and 8 are supported. For real and
   imaginary conversions, 4 and 8 are supported. For complex conversions,
   8 and 16 are supported. The size in bytes is *required* for binary
   integral and floating-point conversions.

   The size can be ``*``, which means that the number of bytes is read
   from the argument before the conversion.

   For strings, if a terminator or length field is specified, exactly this
   number is the maximum size in bytes; if the terminator or length is not
   specified, the string must be exactly that size (and if the argument is not
   exactly that number of bytes it will cause an error even when writing).

[conversion type]
   ``t``
    means *type-based* or *thing* - to read or write with readThis/writeThis
   ``n``
    means type-based number (size is not mandatory)
   ``i``
    means integral. Note that the size is mandatory for binary integral
    conversions
   ``u``
    means unsigned integral. Note that the size is mandatory for binary
    integral conversions
   ``r``
    means real. Note that the size is mandatory for binary real conversions
   ``m``
    works the same as ``r`` for binary conversions
   ``z``
    means complex. Note that the size is mandatory for binary complex
    conversions
   ``s``
    * means string binary I/O *(deprecated)*
    * ``%|17s`` means exactly 17 byte string
   ``0S``/``1S``/``2S``/``4S``/``8S``
    * mean encoded string binary I/O *(deprecated)*:
    * ``%|0S`` means null-terminated string
    * ``%{|S*}`` means  next-argument specifies string terminator byte
    * ``%|1S`` means a one-byte length and then the string
    * ``%|2S`` means a two-byte length and then the string
    * ``%|4S`` means a four-byte length and then the string
    * ``%|8S`` means an eight-byte length and then the string
    * ``%|vS`` means a variable-byte-encoded length and then the string
   ``c``
    means a Unicode character - either the first character in a string
    or an integral character code


.. _about-io-formatted-io-examples:

Formatted I/O Examples
++++++++++++++++++++++

.. code-block:: chapel

  writef("%5i %5s %5r\n", 1, "test", 6.34);
       // outputs:
       //    1  test  6.34

  writef("%2.4z\n", 43.291 + 279.112i);
       // outputs:
       // 43.29 + 279.1i

  writef("%<4u", 0x11223344); // (deprecated)
       // outputs:
       // (hexdump of the output)
       // 4433 2211
  writef("%>4u", 0x11223344); // (deprecated)
       // outputs:
       // (hexdump of the output)
       // 1122 3344
  writef("%<4i %<4i", 2, 32); // (deprecated)
       // outputs:
       // (hexdump of the output -- note that spaces after
       //  a binary format specifier are ignored)
       // 0200 0000 2000 0000


  writef("%|0S\n", "test"); // (deprecated)
       // outputs:
       // (hexdump of the output)
       // 7465 7374 000a
  writef("%|1S\n", "test"); // (deprecated)
       // outputs:
       // (hexdump of the output)
       // 0474 6573 740a
  writef("%>2S\n", "test"); // (deprecated)
       // outputs:
       // (hexdump of the output)
       // 0004 7465 7374 0a
  writef("%>4S\n", "test"); // (deprecated)
       // outputs:
       // (hexdump of the output)
       // 0000 0004 7465 7374 0a
  writef("%>8S\n", "test"); // (deprecated)
       // outputs:
       // (hexdump of the output)
       // 0000 0000 0000 0004 7465 7374 0a
  writef("%|vS\n", "test"); // (deprecated)
       // outputs:
       // (hexdump of the output)
       // 04 7465 7374 0a

  writef('%"S\n', "test \"\" \'\' !");
       // outputs:
       // "test \"\" '' !"
  writef("%'S\n", "test \"\" \'\' !");
       // outputs:
       // 'test "" \'\' !'
  writef("%{(S)}\n", "test ()", "(", ")");
       // outputs:
       // (test (\))


  writef("%40s|\n", "test");
  writef("%-40s|\n", "test");
       // outputs:
       //                                     test|
       // test                                    |

  writef("123456\n");
  writef("%6.6'S\n", "a");
  writef("%6.6'S\n", "abcdefg");
  writef("%.3'S\n", "a");
  writef("%.3'S\n", "abcd");
       // outputs:
       // 123456
       //    'a'
       // 'a'...
       // 'a'
       // ''...


  var s:string;
  var got = readf(" %c", s);
  // if the input is " a", "\na", "  a", etc, s will contain "a"
  // if the input is "b", got will be false and s will contain ""

  var s:string;
  var got = readf("\n%c", s);
  // if the input is "\na", or " \na", s will contain "a"
  // if the input is "b", got will be false and s will be ""

  var got = readf("%/a+/");
  // if the input is "a" or "aa" (and so on), got will return true
  // if the input is "c" got will be false

  var s:string;
  var got = readf("%/a(b+)/", s);
  // if the input is "c" got will be false and s will be ""
  // if the input is "ab", got will be true and s will be "b"
  // if the input is "abb", got will be true and s will be "bb"

FormattedIO Functions and Types
-------------------------------

 */
module FormattedIO {
  use IO;
  use CTypes;
  use OS.POSIX;
  use OS;
//use IO;

// ---------------------------------------------------------------
// ---------------------------------------------------------------
// Starting support for Formatted I/O
// ---------------------------------------------------------------
// ---------------------------------------------------------------

private inline
proc _toIntegral(x:?t) where isIntegralType(t)
{
  return (x, true);
}
private inline
proc _toIntegral(x:?t) throws where _isIoPrimitiveType(t) && !isIntegralType(t)
{
  var ret: (int, bool);
  if isAbstractEnumType(t) then
    ret = (0, false);
  else
    ret = (x:int, true);
  return ret;
}
private inline
proc _toIntegral(x:?t) where !_isIoPrimitiveType(t)
{
  return (0, false);
}

private inline
proc _toSigned(x:int(?w))
{
  return (x, true);
}
private inline
proc _toSigned(x:uint(?w))
{
  return (x:int(w), true);
}

private inline
proc _toSigned(x:?t) throws where _isIoPrimitiveType(t) && !isIntegralType(t)
{
  var ret: (int, bool);
  if isAbstractEnumType(t) then
    ret = (0, false);
  else
    ret = (x:int, true);
  return ret;
}
private inline
proc _toSigned(x:?t) where !_isIoPrimitiveType(t)
{
  return (0:int, false);
}

private inline
proc _toUnsigned(x:uint(?w))
{
  return (x, true);
}
private inline
proc _toUnsigned(x:int(?w))
{
  return (x:uint(w), true);
}

private inline
proc _toUnsigned(x:?t) throws where _isIoPrimitiveType(t) && !isIntegralType(t)
{
  var ret: (uint, bool);
  if isAbstractEnumType(t) then
    ret = (0:uint, false);
  else
    ret = (x:uint, true);
  return ret;
}
private inline
proc _toUnsigned(x:?t) where !_isIoPrimitiveType(t)
{
  return (0:uint, false);
}


private inline
proc _toReal(x:?t) where isRealType(t)
{
  return (x, true);
}
private inline
proc _toReal(x:?t) throws where _isIoPrimitiveType(t) && !isRealType(t)
{
  var ret: (real, bool);
  if isAbstractEnumType(t) then
    ret = (0.0, false);
  else
    ret = (x:real, true);
  return ret;
}
private inline
proc _toReal(x:?t) where !_isIoPrimitiveType(t)
{
  return (0.0, false);
}

private inline
proc _toImag(x:?t) where isImagType(t)
{
  return (x, true);
}
private inline
proc _toImag(x:?t) throws where _isIoPrimitiveType(t) && !isImagType(t)
{
  var ret: (imag, bool);
  if isAbstractEnumType(t) then
    ret = (0.0i, false);
  else
    ret = (x:imag, true);
  return ret;
}
private inline
proc _toImag(x:?t) where !_isIoPrimitiveType(t)
{
  return (0.0i, false);
}


private inline
proc _toComplex(x:?t) where isComplexType(t)
{
  return (x, true);
}
private inline
proc _toComplex(x:?t) throws where _isIoPrimitiveType(t) && !isComplexType(t)
{
  var ret: (complex, bool);
  if isAbstractEnumType(t) then
    ret = (0.0+0.0i, false);
  else
    ret = (x:complex, true);
  return ret;
}
private inline
proc _toComplex(x:?t) where !_isIoPrimitiveType(t)
{
  return (0.0+0.0i, false);
}

private inline
proc _toRealOrComplex(x:?t) where isComplexType(t)
{
  return (x, true);
}
private inline
proc _toRealOrComplex(x:?t) where isRealType(t) || isImagType(t)
{
  return (x, true);
}
private inline
proc _toRealOrComplex(x:?t) where _isIoPrimitiveType(t) && !isComplexType(t) && !
isRealType(t) && !isImagType(t)
{
  return (x:real, true);
}
private inline
proc _toRealOrComplex(x:?t) where !_isIoPrimitiveType(t)
{
  return (0.0, false);
}

private inline
proc _toNumeric(x:?t) where isNumericType(t)
{
  return (x, true);
}
private inline
proc _toNumeric(x:?t) throws where _isIoPrimitiveType(t) && !isNumericType(t)
{
  // enums, bools get cast to int.
  var ret: (int, bool);
  if isAbstractEnumType(t) then
    ret = (0, false);
  else
    ret = (x:int, true);
  return ret;

}
private inline
proc _toNumeric(x:?t) where !_isIoPrimitiveType(t)
{
  return (0, false);
}

private inline
proc _toBytes(x:bytes)
{
  return (x, true);
}

private inline
proc _toBytes(x:string)
{
  return (x:bytes, true);
}

// don't allow anything else to be cast to bytes for now
private inline
proc _toBytes(x:?t)
{
  return ("":bytes, false);
}

private inline
proc _toString(x:string)
{
  return (x, true);
}
private inline
proc _toString(x:bytes)
{
  return ("", false);
}
private inline
proc _toString(x:?t) where (_isIoPrimitiveType(t) && t!=bytes && t!=string)
{
  return (x:string, true);
}
private inline
proc _toString(x:?t) where !_isIoPrimitiveType(t)
{
  return ("", false);
}

private inline
proc _toChar(x:?t) where isIntegralType(t)
{
  return (x:int(32), true);
}
private
proc _toChar(x:?t) where t == string
{
  var chr:int(32);
  var nbytes:c_int;
  var local_x = x.localize();
  qio_decode_char_buf(chr, nbytes, local_x.c_str(), local_x.numBytes:c_ssize_t);
  return (chr, true);
}
private inline
proc _toChar(x:?t) where !(t==string || isIntegralType(t))
{
  return (0:int(32), false);
}


// If we wanted to give ERANGE if (for example
// var x:int(8); readf("%i", x);
// was given the input 1000, this would be the place to do it.
private
proc _setIfPrimitive(ref lhs:?t, rhs:?t2, argi:int):errorCode where t==bool&&_isIoPrimitiveType(t2)
{
  var empty:t2;
  if rhs == empty {
    lhs = false;
  } else {
    lhs = true;
  }
  return 0;
}
private
proc _setIfPrimitive(ref lhs:?t, rhs:?t2, argi:int):errorCode where t!=bool&&_isIoPrimitiveType(t)
{
  try {
    if isAbstractEnumType(t) {
      if isStringType(t2) {
        lhs = rhs:t;
      } else {
        return ERANGE;
      }
    } else if isEnumType(t) {
      if (isIntType(t2) || isStringType(t2)) {
        lhs = rhs:t;
      } else {
        lhs = rhs:int:t;
      }
    } else {
      if isBytesType(t2) && isStringType(t) {
        lhs = rhs.decode(decodePolicy.strict);
      }
      else {
        lhs = rhs:t;
      }
    }
  } catch {
    return ERANGE;
  }
  return 0;
}
private inline
proc _setIfPrimitive(ref lhs:?t, rhs, argi:int):errorCode where !_isIoPrimitiveType(t)
{
  return qio_format_error_arg_mismatch(argi);
}

private inline
proc _setIfChar(ref lhs:?t, rhs:int(32)) where t == string
{
  lhs = new ioChar(rhs):string;
}
private inline
proc _setIfChar(ref lhs:?t, rhs:int(32)) where isIntegralType(t)
{
  lhs = rhs:t;
}
private inline
proc _setIfChar(ref lhs:?t, rhs:int(32)) where !(t==string||isIntegralType(t))
{
  // do nothing
}



private inline
proc _toRegex(x:?t) where isSubtype(t, regex(?))
{
  return (x, true);
}
private inline
proc _toRegex(x:?t)
{
  var r:regex(string);
  return (r, false);
}

pragma "no doc"
class _channel_regex_info {
  var hasRegex = false;
  var matchedRegex = false;
  var releaseRegex = false;
  var theRegex = qio_regex_null();
  var matches: _ddata(qio_regex_string_piece_t) = nil; // size = ncaptures+1
  var capArr: _ddata(bytes) = nil; // size = ncaptures
  var capturei: int;
  var ncaptures: int;

  proc clear() {
    if releaseRegex {
      qio_regex_release(theRegex);
    }
    theRegex = qio_regex_null();
    hasRegex = false;
    matchedRegex = false;
    releaseRegex = false;
    if matches then _ddata_free(matches, ncaptures+1);
    for i in 0..#ncaptures do capArr[i] = b"";
    if capArr then _ddata_free(capArr, ncaptures);
  }
  proc allocate_captures() {
    ncaptures = qio_regex_get_ncaptures(theRegex);
    matches = _ddata_allocate(qio_regex_string_piece_t, ncaptures+1);
    capArr = _ddata_allocate(bytes, ncaptures);
    capturei = 0;
  }
  proc deinit() {
    clear();
  }
  override proc writeThis(f) throws {
    f.write("{hasRegex = " + hasRegex: string);
    f.write(", matchedRegex = " + matchedRegex: string);
    f.write(", releaseRegex = " + releaseRegex: string);
    f.write(", ... capturei = " + capturei: string);
    f.write(", ncaptures = " + ncaptures: string + "}");
  }
}

pragma "no doc"
proc _channel._match_regex_if_needed(cur:c_size_t, len:c_size_t, ref error:errorCode,
    ref style:iostyleInternal, r:unmanaged _channel_regex_info)
{
  if qio_regex_ok(r.theRegex) {
    if r.matchedRegex then return;
    r.matchedRegex = true;
    r.allocate_captures(); // also allocates matches and capArr
    var ncaps = r.ncaptures;
    var nm = ncaps + 1;
    var maxlen:int(64) = style.max_width_characters;
    // If we are working on the last part of the format string,
    // and there is a match, we can immediately discard
    // data before any captures in the match (or if there
    // are no captures - the entire match).
    var can_discard = (cur == len);
    if maxlen == max(uint(32)) then maxlen = max(int(64));
    var before_match = qio_channel_offset_unlocked(_channel_internal);
    // Do the actual regex search.
    // Now read, matching the regex.
    error = qio_regex_channel_match(r.theRegex, false, _channel_internal,
                                     maxlen, QIO_REGEX_ANCHOR_START,
                                     can_discard,
                                     /* keep_unmatched */ true,
                                     /* keep_whole_pattern */ false,
                                     r.matches, nm);
    var after_match = qio_channel_offset_unlocked(_channel_internal);

    // Now, if there was no match, error=EFORMAT
    // if there was a match, error = no error
    // Either way, we have to handle the next several
    // arguments as capture groups.
    if ! error {
      for j in 0..#ncaps {
        // matches[0] is the whole pattern, and
        // we only want to extract capture groups.
        var m = _to_regexMatch(r.matches[1+j]);
        _extractMatch(m, r.capArr[j], error);
        if error then break;
      }
      // And, advance the channel to the end of the match.
      var cur = qio_channel_offset_unlocked(_channel_internal);
      var target = (r.matches[0].offset + r.matches[0].len):int;
      error = qio_channel_advance(false, _channel_internal, target - cur);
    } else {
      // otherwise, clear out caps...
      for j in 0..#ncaps {
        r.capArr[j] = b"";
      }
      // ... and put the channel before the match.
      var cur = qio_channel_offset_unlocked(_channel_internal);
      qio_channel_advance(false, _channel_internal, before_match - cur);
      // EFORMAT means the pattern did not match.
    }
  } else {
    error = qio_format_error_bad_regex();
  }
}

// Reads the next format string that will require argument handling.
// Handles literals and regexes itself; everything else will
// be returned in conv and with gotConv = true.
// Assumes, for a reading channel, that we are within a mark/revert/commit
//  in readf. (used in the regex handling here).
pragma "no doc"
proc _channel._format_reader(
    fmtStr:?fmtType, ref cur:c_size_t, len:c_size_t, ref error:errorCode,
    ref conv:qio_conv_t, ref gotConv:bool, ref style:iostyleInternal,
    ref r:unmanaged _channel_regex_info?,
    isReadf:bool)
{
  if r != nil then r!.hasRegex = false;
  if !error {
    var fmt = fmtStr.localize().c_str();
    while cur < len {
      gotConv = false;
      if error then break;
      var end:uint(64);
      error = qio_conv_parse(fmt, cur, end, isReadf, conv, style);
      if error {
      }
      cur = end:c_size_t;
      if error then break;
      if conv.argType == QIO_CONV_ARG_TYPE_NONE_LITERAL {
        // Print whitespace or I/O literal.
        // literal string in conv
        if isReadf {
          // Scan whitespace or I/O literal.
          // literal string in conv
          if conv.literal_is_whitespace == 2 {
            // Handle a \n newline in the format string.
            // Other space.
            var offsetA = qio_channel_offset_unlocked(_channel_internal);
            error = qio_channel_skip_past_newline(false, _channel_internal, true);
            var offsetB = qio_channel_offset_unlocked(_channel_internal);
            if (!error) && offsetA == offsetB {
              // didn't really read newline.
              error = EFORMAT;
            }
          } else if conv.literal_is_whitespace == 1 {
            // Other space.
            var offsetA = qio_channel_offset_unlocked(_channel_internal);
            error = qio_channel_scan_literal_2(false, _channel_internal, conv.literal, 0, 1);
            var offsetB = qio_channel_offset_unlocked(_channel_internal);
            if (!error) && offsetA == offsetB {
              // didn't really read whitespace.
              error = EFORMAT;
            }
          } else {
            error = qio_channel_scan_literal_2(false, _channel_internal, conv.literal, conv.literal_length:c_ssize_t, 0);
          }
        } else {
          // when printing we don't care if it's just whitespace.
          error = qio_channel_print_literal_2(false, _channel_internal, conv.literal, conv.literal_length:c_ssize_t);
        }
      } else if conv.argType == QIO_CONV_ARG_TYPE_NONE_REGEX_LITERAL {
        if ! isReadf {
          // It's not so clear what to do when printing
          // a regex. So we just don't handle it.
          error = qio_format_error_write_regex();
        } else {
          // allocate regex info if needed
          if r == nil then r = new unmanaged _channel_regex_info();
          const rnn = r!;  // indicate that it is non-nil
          // clear out old data, if there is any.
          rnn.clear();
          // Compile a regex from the format string
          var errstr:string;
          // build a regex out of regex and regex_flags
          qio_regex_create_compile_flags_2(conv.regex, conv.regex_length,
                                            conv.regex_flags,
                                            conv.regex_flags_length,
                                            /* utf8? */ fmtType==string,
                                            rnn.theRegex);
          rnn.releaseRegex = true;
          if qio_regex_ok(rnn.theRegex) {
            rnn.hasRegex = true;
            rnn.ncaptures = qio_regex_get_ncaptures(rnn.theRegex);
            // If there are no captures, and we don't have arguments
            // to consume, go ahead and match the regex.
            if rnn.ncaptures > 0 ||
               conv.preArg1 != QIO_CONV_UNK ||
               conv.preArg2 != QIO_CONV_UNK ||
               conv.preArg3 != QIO_CONV_UNK
            {
              // We need to consume args as part of matching this regex.
              gotConv = true;
              break;
            } else {
              // No args will be consumed.
              _match_regex_if_needed(cur, len, error, style, rnn);
            }
          } else {
            error = qio_format_error_bad_regex();
            //if dieOnError then assert(!error, errstr);
          }
        }
      } else {
        // Some other kind of format specifier... we
        // will return to handle.
        gotConv = true;
        break;
      }
    }
  }
}

pragma "no doc"
proc _channel._conv_helper(
    ref error:errorCode,
    ref conv:qio_conv_t, ref gotConv:bool,
    ref j:int,
    argType: c_ptr(c_int),
    argTypeLen: int)
{
  proc boundsCheckHelp() {
    if boundsChecking {
      if j >= argTypeLen {
        halt("Index ", j, " is accessed on argType of length ", argTypeLen);
      }
    }
  }

  boundsCheckHelp();

  if error then return;
  if gotConv {
    // Perhaps we need to handle pre/post args
    // that adjust the style
    if conv.preArg1 != QIO_CONV_UNK {
      argType(j) = conv.preArg1;
      j += 1;
    }
    boundsCheckHelp();
    if conv.preArg2 != QIO_CONV_UNK {
      argType(j) = conv.preArg2;
      j += 1;
    }
    boundsCheckHelp();
    if conv.preArg3 != QIO_CONV_UNK {
      argType(j) = conv.preArg3;
      j += 1;
    }
    boundsCheckHelp();
    if conv.argType != QIO_CONV_UNK {
      if argType(j) == QIO_CONV_UNK {
        // Some regex paths set it earlier..
        argType(j) = conv.argType;
      }
      j += 1;
    }
  }
}

pragma "no doc"
proc _channel._conv_sethandler(
    ref error:errorCode,
    argtypei:c_int,
    ref style:iostyleInternal,
    i:int, argi,
    isReadf:bool):bool throws
{
  if error then return false;
  // Now, set style elements based on action
  // at i
  select argtypei {
    when QIO_CONV_SET_MIN_WIDTH_COLS {
      var (t,ok) = _toIntegral(argi);
      if ! ok {
        error = qio_format_error_arg_mismatch(i);
      } else style.min_width_columns = t:uint(32);
    }
    when QIO_CONV_SET_MAX_WIDTH_COLS {
      var (t,ok) = _toIntegral(argi);
      if ! ok {
        error = qio_format_error_arg_mismatch(i);
      } else style.max_width_columns = t:uint(32);
    }
    when QIO_CONV_SET_MAX_WIDTH_CHARS {
      var (t,ok) = _toIntegral(argi);
      if ! ok {
        error = qio_format_error_arg_mismatch(i);
      } else style.max_width_characters = t:uint(32);
    }
    when QIO_CONV_SET_MAX_WIDTH_BYTES {
      var (t,ok) = _toIntegral(argi);
      if ! ok {
        error = qio_format_error_arg_mismatch(i);
      } else style.max_width_bytes = t:uint(32);
    }
    when QIO_CONV_SET_PRECISION {
      var (t,ok) = _toIntegral(argi);
      if ! ok {
        error = qio_format_error_arg_mismatch(i);
      } else style.precision = t:int(32);
    }
    when QIO_CONV_SET_STRINGSTART {
      var (t,ok) = _toChar(argi);
      if ! ok {
        error = qio_format_error_arg_mismatch(i);
      } else style.string_start = t:style_char_t;
    }
    when QIO_CONV_SET_STRINGEND {
      var (t,ok) = _toChar(argi);
      if ! ok {
        error = qio_format_error_arg_mismatch(i);
      } else style.string_end = t:style_char_t;
    }
    when QIO_CONV_SET_STRINGSTARTEND {
      var (t,ok) = _toChar(argi);
      if ! ok {
        error = qio_format_error_arg_mismatch(i);
      } else {
        style.string_start = t:style_char_t;
        style.string_end = t:style_char_t;
      }
    }
    when QIO_CONV_SET_STRINGLEN {
      var (t,ok) = _toIntegral(argi);
      if ! ok {
        error = qio_format_error_arg_mismatch(i);
      } else {
        style.str_style = t:int(64);
      }
    }
    when QIO_CONV_SET_TERMINATOR {
      var (t,ok) = _toChar(argi);
      if ! ok {
        error = qio_format_error_arg_mismatch(i);
      } else {
        style.str_style = -(t:uint(8) - iostringstyleInternal.data_null:int(64));
      }
    }
    when QIO_CONV_SET_DONE {
      // Do nothing. Already handled.
    }
    when QIO_CONV_UNK {
      // Too many arguments.
      error = qio_format_error_too_many_args();
    } otherwise {
      return true;
    }
  }
  return false;
}

pragma "no doc"
proc _channel._write_signed(width:uint(32), t:int, i:int):errorCode
{
  var err:errorCode;
  var byteorder = qio_channel_byteorder(_channel_internal);
  select width {
    when 1 {
      var x = t:int(8);
      err = qio_channel_write_int8(false, _channel_internal, x);
    } when 2 {
      var x = t:int(16);
      err = qio_channel_write_int16(false, byteorder, _channel_internal, x);
    } when 4 {
      var x = t:int(32);
      err = qio_channel_write_int32(false, byteorder, _channel_internal, x);
    } when 8 {
      var x = t:int(64);
      err = qio_channel_write_int64(false, byteorder, _channel_internal, x);
    } otherwise err = qio_format_error_arg_mismatch(i);
  }
  return err;
}

pragma "no doc"
proc _channel._read_signed(width:uint(32), out t:int, i:int):errorCode
{
  var err:errorCode;
  var byteorder = qio_channel_byteorder(_channel_internal);
  select width {
    when 1 {
      var x:int(8);
      err = qio_channel_read_int8(false, _channel_internal, x);
      t = x;
    } when 2 {
      var x:int(16);
      err = qio_channel_read_int16(false, byteorder, _channel_internal, x);
      t = x;
    } when 4 {
      var x:int(32);
      err = qio_channel_read_int32(false, byteorder, _channel_internal, x);
      t = x;
    } when 8 {
      var x:int(64);
      err = qio_channel_read_int64(false, byteorder, _channel_internal, x);
      t = x;
    } otherwise err = qio_format_error_arg_mismatch(i);
  }
  return err;
}

pragma "no doc"
proc _channel._write_unsigned(width:uint(32), t:uint, i:int)
{
  var err:errorCode;
  var byteorder = qio_channel_byteorder(_channel_internal);
  select width {
    when 1 {
      var x = t:uint(8);
      err = qio_channel_write_uint8(false, _channel_internal, x);
    } when 2 {
      var x = t:uint(16);
      err = qio_channel_write_uint16(false, byteorder, _channel_internal, x);
    } when 4 {
      var x = t:uint(32);
      err = qio_channel_write_uint32(false, byteorder, _channel_internal, x);
    } when 8 {
      var x = t:uint(64);
      err = qio_channel_write_uint64(false, byteorder, _channel_internal, x);
    } otherwise err = qio_format_error_arg_mismatch(i);
  }
  return err;
}
pragma "no doc"
proc _channel._read_unsigned(width:uint(32), out t:uint, i:int)
{
  var err:errorCode;
  var byteorder = qio_channel_byteorder(_channel_internal);
  select width {
    when 1 {
      var x:uint(8);
      err = qio_channel_read_uint8(false, _channel_internal, x);
      t = x;
    } when 2 {
      var x:uint(16);
      err = qio_channel_read_uint16(false, byteorder, _channel_internal, x);
      t = x;
    } when 4 {
      var x:uint(32);
      err = qio_channel_read_uint32(false, byteorder, _channel_internal, x);
      t = x;
    } when 8 {
      var x:uint(64);
      err = qio_channel_read_uint64(false, byteorder, _channel_internal, x);
      t = x;
    } otherwise err = qio_format_error_arg_mismatch(i);
  }
  return err;
}


pragma "no doc"
proc _channel._write_real(width:uint(32), t:real, i:int)
{
  var err:errorCode;
  var byteorder = qio_channel_byteorder(_channel_internal);
  select width {
    when 4 {
      var x = t:real(32);
      err = qio_channel_write_float32(false, byteorder, _channel_internal, x);
    } when 8 {
      var x = t:real(64);
      err = qio_channel_write_float64(false, byteorder, _channel_internal, x);
    } otherwise err = qio_format_error_arg_mismatch(i);
  }
  return err;
}
pragma "no doc"
proc _channel._read_real(width:uint(32), out t:real, i:int)
{
  var err:errorCode;
  var byteorder = qio_channel_byteorder(_channel_internal);
  select width {
    when 4 {
      var x:real(32);
      err = qio_channel_read_float32(false, byteorder, _channel_internal, x);
      t = x;
    } when 8 {
      var x:real(64);
      err = qio_channel_read_float64(false, byteorder, _channel_internal, x);
      t = x;
    } otherwise err = qio_format_error_arg_mismatch(i);
  }
  return err;
}


pragma "no doc"
proc _channel._write_complex(width:uint(32), t:complex, i:int)
{
  var err:errorCode = 0;
  var byteorder = qio_channel_byteorder(_channel_internal);
  select width {
    when 8 {
      var x = t:complex(64);
      var re = x.re;
      var im = x.im;
      err = qio_channel_write_float32(false, byteorder, _channel_internal, re);
      if ! err {
        err = qio_channel_write_float32(false, byteorder, _channel_internal, im);
      }
    } when 16 {
      var x = t:complex(128);
      var re = x.re;
      var im = x.im;
      err = qio_channel_write_float64(false, byteorder, _channel_internal, re);
      if ! err {
        err = qio_channel_write_float64(false, byteorder, _channel_internal, im);
      }
    } otherwise err = qio_format_error_arg_mismatch(i);
  }
  return err;
}

pragma "no doc"
proc _channel._read_complex(width:uint(32), out t:complex, i:int)
{
  var err:errorCode = 0;
  var byteorder = qio_channel_byteorder(_channel_internal);
  select width {
    when 8 {
      var x:complex(64);
      var re:x.re.type;
      var im:x.im.type;
      err = qio_channel_read_float32(false, byteorder, _channel_internal, re);
      if ! err {
        err = qio_channel_read_float32(false, byteorder, _channel_internal, im);
      }
      x = (re, im):complex(64); // tuple to complex
      t = x;
    } when 16 {
      var x:complex(128);
      var re:x.re.type;
      var im:x.im.type;
      err = qio_channel_read_float64(false, byteorder, _channel_internal, re);
      if ! err {
        err = qio_channel_read_float64(false, byteorder, _channel_internal, im);
      }
      x = (re, im):complex(128); // tuple to complex
      t = x;
    } otherwise err = qio_format_error_arg_mismatch(i);
  }
  return err;
}


// This is called from within a param for loop in writef. It is important to
// note here that the main benefit of this helper is that we don't pass all the
// arguments from writef. This way, we can use the same code for an `arg` type
// for which we have already created and instantiation of this.
pragma "no doc"
proc _channel._writefOne(fmtStr, ref arg, i: int,
                        ref cur: c_size_t, ref j: int,
                        ref r: unmanaged _channel_regex_info?,
                        argType: c_ptr(c_int), argTypeLen: int,
                        ref conv: qio_conv_t, ref gotConv: bool,
                        ref style: iostyleInternal, ref err: errorCode,
                        origLocale: locale, len: c_size_t) throws {
  if boundsChecking {
    if i >= argTypeLen {
      halt("Index ", i, " is accessed on argType of length ", argTypeLen);
    }
  }
  gotConv = false;

  if j <= i {
    _format_reader(fmtStr, cur, len, err,
                   conv, gotConv, style, r,
                   false);
  }

  _conv_helper(err, conv, gotConv, j, argType, argTypeLen);

  var domore = _conv_sethandler(err, argType(i), style, i,arg,false);

  if domore {
    this._set_styleInternal(style);
    // otherwise we will consume at least one argument.
    select argType(i) {
      when QIO_CONV_ARG_TYPE_SIGNED, QIO_CONV_ARG_TYPE_BINARY_SIGNED {
        var (t,ok) = _toSigned(arg);
        if ! ok {
          err = qio_format_error_arg_mismatch(i);
        } else {
          if argType(i) == QIO_CONV_ARG_TYPE_BINARY_SIGNED then
            err = _write_signed(style.max_width_bytes, t, i);
          else
            try _writeOne(iokind.dynamic, t, origLocale);
        }
      } when QIO_CONV_ARG_TYPE_UNSIGNED, QIO_CONV_ARG_TYPE_BINARY_UNSIGNED {
        var (t,ok) = _toUnsigned(arg);
        if ! ok {
          err = qio_format_error_arg_mismatch(i);
        } else {
          if argType(i) == QIO_CONV_ARG_TYPE_BINARY_UNSIGNED then
            err = _write_unsigned(style.max_width_bytes, t, i);
          else
            try _writeOne(iokind.dynamic, t, origLocale);
        }
      } when QIO_CONV_ARG_TYPE_REAL, QIO_CONV_ARG_TYPE_BINARY_REAL {
        var (t,ok) = _toReal(arg);
        if ! ok {
          err = qio_format_error_arg_mismatch(i);
        } else {
          if argType(i) == QIO_CONV_ARG_TYPE_BINARY_REAL then
            err = _write_real(style.max_width_bytes, t, i);
          else
            try _writeOne(iokind.dynamic, t, origLocale);
        }
      } when QIO_CONV_ARG_TYPE_IMAG, QIO_CONV_ARG_TYPE_BINARY_IMAG {
        var (t,ok) = _toImag(arg);
        if ! ok {
          err = qio_format_error_arg_mismatch(i);
        } else {
          if argType(i) == QIO_CONV_ARG_TYPE_BINARY_IMAG then
            err = _write_real(style.max_width_bytes, t:real, i);
          else
            try _writeOne(iokind.dynamic, t, origLocale);
        }
      } when QIO_CONV_ARG_TYPE_COMPLEX, QIO_CONV_ARG_TYPE_BINARY_COMPLEX {
        var (t,ok) = _toComplex(arg);
        if ! ok {
          err = qio_format_error_arg_mismatch(i);
        } else {
          if argType(i) == QIO_CONV_ARG_TYPE_BINARY_COMPLEX then
            err = _write_complex(style.max_width_bytes, t, i);
          else try _writeOne(iokind.dynamic, t, origLocale);
        }
      } when QIO_CONV_ARG_TYPE_NUMERIC {
        var (t,ok) = _toNumeric(arg);
        if ! ok {
          err = qio_format_error_arg_mismatch(i);
        } else try _writeOne(iokind.dynamic, t, origLocale);
      } when QIO_CONV_ARG_TYPE_CHAR {
        var (t,ok) = _toChar(arg);
        if ! ok {
          err = qio_format_error_arg_mismatch(i);
        } else try _writeOne(iokind.dynamic, new ioChar(t), origLocale);
      } when QIO_CONV_ARG_TYPE_BINARY_STRING {
        var (t,ok) = _toBytes(arg);
        if ! ok {
          err = qio_format_error_arg_mismatch(i);
        } else try _writeOne(iokind.dynamic, t, origLocale);
      } when QIO_CONV_ARG_TYPE_STRING { // can only happen with string
        var (t,ok) = _toString(arg);
        if ! ok {
          err = qio_format_error_arg_mismatch(i);
        } else try _writeOne(iokind.dynamic, t, origLocale);
      } when QIO_CONV_ARG_TYPE_REGEX { // It's not so clear what to do when printing
        // a regex. So we just don't handle it.
        err = qio_format_error_write_regex();
      } when QIO_CONV_ARG_TYPE_REPR {
        try _writeOne(iokind.dynamic, arg, origLocale);
      } otherwise {
        // Unhandled argument type!
        throw new owned IllegalArgumentError("args(" + i:string + ")",
                                       "writef internal error " + argType(i):string);
      }
    }
  }
}

/*

   Write arguments according to a format. See
   :ref:`about-io-formatted-io`.

   :arg fmt: the format as string or bytes

   :arg args: 0 or more arguments to write

   :throws UnexpectedEofError
   :throws IllegalArgumentError: if an unsupported argument type is encountered.
   :throws SystemError: if the arguments could not be written.
 */
proc _channel.writef(fmtStr: ?t, const args ...?k) throws
  where isStringType(t) || isBytesType(t)
{
  if !writing then compilerError("writef on read-only channel");
  const origLocale = this.getLocaleOfIoRequest();
  var err: errorCode = 0;
  on this._home {
    try this.lock(); defer { this.unlock(); }

    var save_style: iostyleInternal = this._styleInternal();
    defer {
      this._set_styleInternal(save_style);
    }
    var cur:c_size_t = 0;
    var len:c_size_t = fmtStr.size:c_size_t;
    var conv:qio_conv_t;
    var gotConv:bool;
    var style:iostyleInternal;

    param argTypeLen = k+5;
    // we don't use a tuple here so that we can pass this to writefOne as a
    // c_ptr. This should reduce number of instantiations of writefOne
    var argType: c_array(c_int, argTypeLen);

    var r:unmanaged _channel_regex_info?;
    defer {
      if r then delete r;
    }

    for i in 0..argType.size-1 {
      argType(i) = QIO_CONV_UNK;
    }

    var j = 0;

    for param i in 0..k-1 {
      _writefOne(fmtStr, args(i), i, cur, j, r, c_ptrTo(argType[0]), argTypeLen,
                 conv, gotConv, style, err, origLocale, len);
    }

    if ! err {
      if cur < len {
        var dummy:c_int;
        _format_reader(fmtStr, cur, len, err,
                       conv, gotConv, style, r,
                       false);
      }

      if cur < len {
        // Mismatched number of arguments!
        err = qio_format_error_too_few_args();
      }
    }
  }

  if err then try this._ch_ioerror(err, "in channel.writef(fmt:string)");
}

// documented in varargs version
proc _channel.writef(fmtStr:?t) throws
  where isStringType(t) || isBytesType(t)
{
  if !writing then compilerError("writef on read-only channel");
  var err:errorCode = 0;
  on this._home {
    try this.lock(); defer { this.unlock(); }
    var save_style: iostyleInternal = this._styleInternal();
    defer {
      this._set_styleInternal(save_style);
    }
    var cur:c_size_t = 0;
    var len:c_size_t = fmtStr.size:c_size_t;
    var conv:qio_conv_t;
    var gotConv:bool;
    var style:iostyleInternal;
    var end:c_size_t;
    var dummy:c_int;

    var r:unmanaged _channel_regex_info?;
    defer {
      if r then delete r;
    }

    _format_reader(fmtStr, cur, len, err,
                   conv, gotConv, style, r,
                   false);

    if ! err {
      if gotConv {
        err = qio_format_error_too_few_args();
      }
    }

    if ! err {
      if cur < len {
        // Mismatched number of arguments!
        err = qio_format_error_too_few_args();
      }
    }

    this._set_styleInternal(save_style);
  }

  if err then try this._ch_ioerror(err, "in channel.writef(fmt:string, ...)");
  return true;
}

/*

   Read arguments according to a format. See
   :ref:`about-io-formatted-io`.

   .. note::

      Intents for all arguments except the format string are `ref`. If `readf`
      is used with formats that require an additional argument such as `%*i` and
      `%*S`, then those arguments cannot be constants. Instead, store the value
      into a variable and pass that.

   :arg fmt: the format as string or bytes
   :arg args: the arguments to read
   :returns: true if all arguments were read according to the format string,
             false on EOF.

   :throws UnexpectedEofError
   :throws SystemError: Thrown if the arguments could not be read.
 */
proc _channel.readf(fmtStr:?t, ref args ...?k): bool throws
    where isStringType(t) || isBytesType(t) {

  if writing then compilerError("readf on write-only channel");
  const origLocale = this.getLocaleOfIoRequest();

  var err:errorCode = 0;

  on this._home {
    try this.lock(); defer { this.unlock(); }
    var save_style: iostyleInternal = this._styleInternal();
    defer {
      this._set_styleInternal(save_style);
    }
    var cur:c_size_t = 0;
    var len:c_size_t = fmtStr.size:c_size_t;
    var conv:qio_conv_t;
    var gotConv:bool;
    var style:iostyleInternal;
    var end:c_size_t;

    param argTypeLen = k+5;
    // we don't use a tuple here for being able to use conv_helper. This will be
    // more meaningful when we refactor the param loop's body out into a separate
    // helper. See writef
    var argType: c_array(c_int, argTypeLen);

    var r:unmanaged _channel_regex_info?;
    defer {
      if r then delete r;
    }

    for i in 0..argType.size-1 {
      argType(i) = QIO_CONV_UNK;
    }

    err = qio_channel_mark(false, _channel_internal);

    if !err then try {
      var j = 0;

      for param i in 0..k-1 {
        // The inside of this loop is a bit crazy because
        // we're writing it all in a param for in order to
        // get generic argument handling.
        if j <= i {
          _format_reader(fmtStr, cur, len, err,
                         conv, gotConv, style, r,
                         true);

          if r != nil {
           const rnn = r!;  // indicate that it is non-nil
           if (rnn.hasRegex) {
            // We need to handle the next ncaptures arguments.
            if i + rnn.ncaptures - 1 > k {
              err= qio_format_error_too_few_args();
            }
            for z in 0..#rnn.ncaptures {
              if i+z <= argType.size {
                argType(i+z) = QIO_CONV_SET_CAPTURE;
              }
            }
           }
          }
        }

        _conv_helper(err, conv, gotConv, j, argType, argTypeLen=k+5);

        var domore = _conv_sethandler(err, argType(i),style,i,args(i),false);

        if domore {
          this._set_styleInternal(style);
          // otherwise we will consume at least one argument.
          select argType(i) {
            when QIO_CONV_ARG_TYPE_SIGNED, QIO_CONV_ARG_TYPE_BINARY_SIGNED {
              var (t,ok) = _toSigned(args(i));
              if ! ok {
                err = qio_format_error_arg_mismatch(i);
              } else {
                var ti:int;
                if argType(i) == QIO_CONV_ARG_TYPE_BINARY_SIGNED then
                  err = _read_signed(style.max_width_bytes, ti, i);
                else
                  try _readOne(iokind.dynamic, ti, origLocale);
                if ! err then err = _setIfPrimitive(args(i),ti,i);
              }
            }
            when QIO_CONV_ARG_TYPE_UNSIGNED, QIO_CONV_ARG_TYPE_BINARY_UNSIGNED {
              var (t,ok) = _toUnsigned(args(i));
              if ! ok {
                err = qio_format_error_arg_mismatch(i);
              } else {
                var ti:uint;
                if argType(i) == QIO_CONV_ARG_TYPE_BINARY_UNSIGNED then
                  err = _read_unsigned(style.max_width_bytes, ti, i);
                else
                  try _readOne(iokind.dynamic, ti, origLocale);
                if ! err then err = _setIfPrimitive(args(i),ti,i);
              }
            } when QIO_CONV_ARG_TYPE_REAL, QIO_CONV_ARG_TYPE_BINARY_REAL {
              var (t,ok) = _toReal(args(i));
              if ! ok {
                err = qio_format_error_arg_mismatch(i);
              } else {
                var ti:real;
                if argType(i) == QIO_CONV_ARG_TYPE_BINARY_REAL then
                  err = _read_real(style.max_width_bytes, ti, i);
                else
                  try _readOne(iokind.dynamic, ti, origLocale);
                if ! err then err = _setIfPrimitive(args(i),ti,i);
              }
            } when QIO_CONV_ARG_TYPE_IMAG, QIO_CONV_ARG_TYPE_BINARY_IMAG {
              var (t,ok) = _toImag(args(i));
              if ! ok {
                err = qio_format_error_arg_mismatch(i);
              } else {
                var ti:imag;
                if argType(i) == QIO_CONV_ARG_TYPE_BINARY_IMAG {
                  var tr:real;
                  err = _read_real(style.max_width_bytes, tr, i);
                  ti = tr:imag;
                } else
                  try _readOne(iokind.dynamic, ti, origLocale);
                if ! err then err = _setIfPrimitive(args(i),ti,i);
              }
            } when QIO_CONV_ARG_TYPE_COMPLEX, QIO_CONV_ARG_TYPE_BINARY_COMPLEX {
              var (t,ok) = _toComplex(args(i));
              if ! ok {
                err = qio_format_error_arg_mismatch(i);
              } else {
                var ti:complex;
                if argType(i) == QIO_CONV_ARG_TYPE_BINARY_COMPLEX then
                  err = _read_complex(style.max_width_bytes, ti, i);
                else
                  try _readOne(iokind.dynamic, ti, origLocale);
                if ! err then err = _setIfPrimitive(args(i),ti,i);
              }
            } when QIO_CONV_ARG_TYPE_NUMERIC {
              var (t,ok) = _toNumeric(args(i));
              if !ok {
                err = qio_format_error_arg_mismatch(i);
              } else {
                var ti = t;
                try _readOne(iokind.dynamic, ti, origLocale);
                if !err then err = _setIfPrimitive(args(i),ti,i);
              }
            } when QIO_CONV_ARG_TYPE_CHAR {
              var (t,ok) = _toChar(args(i));
              var chr = new ioChar(t);
              if ! ok {
                err = qio_format_error_arg_mismatch(i);
              } else try _readOne(iokind.dynamic, chr, origLocale);
              if ! err then _setIfChar(args(i),chr.ch);
            } when QIO_CONV_ARG_TYPE_BINARY_STRING {
              var (t,ok) = _toBytes(args(i));
              if ! ok {
                err = qio_format_error_arg_mismatch(i);
              } else try _readOne(iokind.dynamic, t, origLocale);
              if ! err then err = _setIfPrimitive(args(i),t,i);
            } when QIO_CONV_ARG_TYPE_STRING {
              var (t,ok) = _toString(args(i));
              if ! ok {
                err = qio_format_error_arg_mismatch(i);
              } else try _readOne(iokind.dynamic, t, origLocale);
              if ! err then err = _setIfPrimitive(args(i),t,i);
            } when QIO_CONV_ARG_TYPE_REGEX {
              var (t,ok) = _toRegex(args(i));
              if ! ok {
                err = qio_format_error_arg_mismatch(i);
              } else {
                // match it here.
                if r == nil then r = new unmanaged _channel_regex_info();
                const rnn = r!;  // indicate that it is non-nil
                rnn.clear();
                rnn.theRegex = t._regex;
                rnn.hasRegex = true;
                rnn.releaseRegex = false;
                _match_regex_if_needed(cur, len, err, style, rnn);

                // Set the capture groups.
                // We need to handle the next ncaptures arguments.
                if i + rnn.ncaptures - 1 > k {
                  err = qio_format_error_too_few_args();
                }
                for z in 0..#rnn.ncaptures {
                  if i+z <= argType.size {
                    argType(i+z+1) = QIO_CONV_SET_CAPTURE;
                  }
                }
              }
            } when QIO_CONV_ARG_TYPE_REPR {
              try _readOne(iokind.dynamic, args(i), origLocale);
            } when QIO_CONV_SET_CAPTURE {
              if r == nil {
                err = qio_format_error_bad_regex();
              } else {
                const rnn = r!;  // indicate that it is non-nil
                _match_regex_if_needed(cur, len, err, style, rnn);
                // Set args(i) to the capture at capturei.
                if rnn.capturei >= rnn.ncaptures {
                  err = qio_format_error_bad_regex();
                } else {
                  // We have a string in captures[capturei] and
                  // we need to set args(i) to that.
                  if _isIoPrimitiveType(args(i).type) {
                    // but only if it's a primitive type
                    // (so that we can avoid problems with string-to-record).
                    try {
                      if args(i).type == string then
                        args(i) = rnn.capArr[rnn.capturei].decode();
                      // the following is a workaround for bytes->enum casts
                      // being unimplemented See #14553
                      else if isEnumType(args(i).type) then
                        args(i) = rnn.capArr[rnn.capturei].decode():args(i).type;
                      else
                        args(i) = rnn.capArr[rnn.capturei]:args(i).type;
                    } catch e: DecodeError {
                      // someone's trying to capture a non UTF8 sequence in a
                      // string -- argument type mismatch
                      err = qio_format_error_arg_mismatch(i);

                      // Engin: maybe in the future we can just propagate
                      // DecodeError here?
                    } catch {
                      // maybe a cast error for the enum cast
                      err = qio_format_error_bad_regex();
                    }
                  }
                  rnn.capturei += 1;
                }
              }
            } otherwise {
              throw new owned IllegalArgumentError("args(" + i:string + ")",
                                             "readf internal error " + argType(i):string);
            }
          }
        }
      }

      if ! err {
        if cur < len {
          var dummy:c_int;
          _format_reader(fmtStr, cur, len, err,
                         conv, gotConv, style, r,
                         true);
        }
      }

      if ! err {
        if cur < len {
          // Mismatched number of arguments!
          err = qio_format_error_too_few_args();
        }
      }

      if ! err {
        // commit.
        qio_channel_commit_unlocked(_channel_internal);
      } else {
        // revert
        qio_channel_revert_unlocked(_channel_internal);
      }
    } catch thrownError: EofError {
      err = EEOF;
    }
  }

  if !err {
    return true;
  } else if err == EEOF {
    return false;
  } else {
    try this._ch_ioerror(err, "in channel.readf(fmt:string, ...)");
  }

  return false;
}

// documented in varargs version
pragma "no doc"
proc _channel.readf(fmtStr:?t) throws
    where isStringType(t) || isBytesType(t) {

  if writing then compilerError("readf on write-only channel");
  var err:errorCode = 0;
  on this._home {
    try this.lock(); defer { this.unlock(); }
    var save_style: iostyleInternal = this._styleInternal();
    defer {
      this._set_styleInternal(save_style);
    }
    var cur:c_size_t = 0;
    var len:c_size_t = fmtStr.size:c_size_t;
    var conv:qio_conv_t;
    var gotConv:bool;
    var style:iostyleInternal;
    var end:c_size_t;
    var dummy:c_int;

    var r:unmanaged _channel_regex_info?;
    defer {
      if r then delete r;
    }

    err = qio_channel_mark(false, _channel_internal);
    if !err {
      _format_reader(fmtStr, cur, len, err,
                     conv, gotConv, style, r,
                     true);
      if gotConv {
        err = qio_format_error_too_few_args();
      }
    }
    if !err {
      if cur < len {
        err = qio_format_error_too_few_args();
      }
    }
    if ! err {
      // commit.
      qio_channel_commit_unlocked(_channel_internal);
    } else {
      // revert
      qio_channel_revert_unlocked(_channel_internal);
    }
  }

  if !err {
    return true;
  } else if err == EEOF {
    return false;
  } else {
    try this._ch_ioerror(err, "in channel.readf(fmt:string)");
    return false;
  }
}

/* Call ``stdin.readf``; see :proc:`channel.readf`. */
proc readf(fmt:string, ref args ...?k):bool throws {
  return try stdin.readf(fmt, (...args));
}
// documented in string version
pragma "no doc"
proc readf(fmt:string):bool throws {
  return try stdin.readf(fmt);
}


/*
   Skip a field in the current aggregate format. This method is currently only
   supported for JSON format and returns ENOTSUP for other formats. In other
   formats, it may not be possible in general to know when a field ends.

   The field skipped includes a field name and value but not a following
   separator. For example, for a JSON format channel, given the input:

   ::

      "fieldName":"fieldValue", "otherField":3

   this function will skip to (but leave unread) the comma after
   the first field value.

   :throws UnexpectedEofError: Thrown if EOF encountered skipping field.
   :throws SystemError: Thrown if the field could not be skipped.
 */
proc _channel.skipField() throws {
  var err:errorCode = 0;
  on this._home {
    try this.lock(); defer { this.unlock(); }
    var st = this.styleElement(QIO_STYLE_ELEMENT_AGGREGATE);
    if st == QIO_AGGREGATE_FORMAT_JSON {
      err = qio_channel_skip_json_field(false, _channel_internal);
    } else {
      err = ENOTSUP;
    }
  }
  if err then try this._ch_ioerror(err, "in skipField");
}

/*

  Return a new string consisting of values formatted according to a
  format string.  See :ref:`about-io-formatted-io`.

  :arg this: the format string
  :arg args: the arguments to format
  :returns: the resulting string

  :throws EofError
  :throws UnexpectedEofError
  :throws BadFormatError
  :throws SystemError: Thrown if the string could not be formatted for a less
                       specific reason.
 */
proc string.format(args ...?k): string throws {
  try {
    return chpl_do_format(this, (...args));
  } catch e: IllegalArgumentError {
    throw e;
  } catch e: EofError {
    throw e;
  } catch e: UnexpectedEofError {
    throw e;
  } catch e: BadFormatError {
    throw e;
  } catch e: SystemError {
    try ioerror(e.err, "in string.format");
  } catch e: DecodeError {
    try ioerror(EILSEQ:errorCode, "in string.format");
  } catch {
    try ioerror(EINVAL:errorCode, "in string.format");
  }
  return "";
}

/*

  Return a new bytes consisting of values formatted according to a
  format bytes.  See :ref:`about-io-formatted-io`.

  :arg this: the format bytes
  :arg args: the arguments to format
  :returns: the resulting bytes

  :throws EofError
  :throws UnexpectedEofError
  :throws BadFormatError
  :throws SystemError: Thrown if the bytes could not be formatted for a less
                       specific reason.
 */
proc bytes.format(args ...?k): bytes throws {
  try {
    return chpl_do_format(this, (...args));
  } catch e: EofError {
    throw e;
  } catch e: UnexpectedEofError {
    throw e;
  } catch e: BadFormatError {
    throw e;
  } catch e: SystemError {
    try ioerror(e.err, "in bytes.format");
  } catch {
    try ioerror(EINVAL:errorCode, "in bytes.format");
  }
  return b"";
}

private proc chpl_do_format(fmt:?t, args ...?k): t throws
    where isStringType(t) || isBytesType(t) {

  // Open a memory buffer to store the result
  var f = try openmem();
  defer {
    try {
      f.close();
    } catch { /* ignore deferred close error */ }
  }

  var offset:int = 0;
  {
    var w = try f.writer(locking=false);
    defer {
      try {
        w.close();
      } catch { /* ignore deferred close error */ }
    }
    try w.writef(fmt, (...args));
    offset = w.offset();

    // close error is thrown instead of ignored
    try w.close();
  }

  var buf = c_malloc(uint(8), offset+1);
  var r = try f.reader(locking=false);
  defer {
    try {
      r.close();
    } catch { /* ignore deferred close error */ }
  }

  try r._readBytes(buf, offset:c_ssize_t);

  // close errors are thrown instead of ignored
  try r.close();
  try f.close();

  // Add the terminating NULL byte to make C string conversion easy.
  buf[offset] = 0;

  if isStringType(t) then
    return createStringWithOwnedBuffer(buf, offset, offset+1);
  else
    return createBytesWithOwnedBuffer(buf, offset, offset+1);
}


// ---------------------------------------------------------------
// ---------------------------------------------------------------
// Starting support for regular expression search on channels
// ---------------------------------------------------------------
// ---------------------------------------------------------------



use Regex;

private extern proc qio_regex_channel_match(const ref re:qio_regex_t, threadsafe:c_int, ch:qio_channel_ptr_t, maxlen:int(64), anchor:c_int, can_discard:bool, keep_unmatched:bool, keep_whole_pattern:bool, submatch:_ddata(qio_regex_string_piece_t), nsubmatch:int(64)):errorCode;

pragma "no doc"
proc _channel._extractMatch(m:regexMatch, ref arg:regexMatch, ref error:errorCode) {
  // If the argument is a match record, just return it.
  arg = m;
}

pragma "no doc"
proc _channel._extractMatch(m:regexMatch, ref arg:bytes, ref error:errorCode) {
  var cur:int(64);
  var target = m.byteOffset:int;
  var len = m.numBytes;
  const oldPosition = qio_channel_offset_unlocked(_channel_internal);

  // If there was no match, return the default value of the type
  if !m.matched {
    arg = b"";
  }

  // Read into a string the appropriate region of the file.
  if !error {
    qio_channel_revert_unlocked(_channel_internal);
    error = qio_channel_mark(false, _channel_internal);
    cur = qio_channel_offset_unlocked(_channel_internal);
  }

  if ! error {
    // There was a match, so we have to read the
    // strings for the capture groups.
    error = qio_channel_advance(false, _channel_internal, target - cur);
  }

  var s:bytes;
  if ! error {
    var gotlen:int(64);
    var ts: c_string;
    error =
        qio_channel_read_string(false, iokind.native:c_int, len: int(64),
                                _channel_internal, ts, gotlen, len: c_ssize_t);
    s = createBytesWithOwnedBuffer(ts, length=gotlen);
  }

  if ! error {
    arg = s;
  } else {
    arg = b"";
  }

  // Put back 'oldPosition' at the top of the mark stack.
  cur = qio_channel_offset_unlocked(_channel_internal);
  if oldPosition > cur then
    qio_channel_advance(false, _channel_internal, oldPosition - cur);
}

pragma "no doc"
proc _channel._extractMatch(m:regexMatch, ref arg:?t, ref error:errorCode)
      where t != regexMatch && t != bytes {
  // If there was no match, return the default value of the type
  if !m.matched {
    var empty:arg.type;
    arg = empty;
  }

  // Read into a bytes the appropriate region of the file.
  var s:bytes;
  _extractMatch(m, s, error);


  if ! error {
    if arg.type == string {
      try {
        // throws if trying to extract a non UTF8 match into a string
        // at this point this is unlikely to happen. So, just catch the error
        // and set error to EFORMAT
        arg = s.decode();
      }
      catch {
        error = EFORMAT;
      }
    }
    else {
      arg = s:arg.type;
    }
  } else {
    var empty:arg.type;
    arg = empty;
  }
}


/*  Sets arg to the string of a match.

    Assumes that the channel has been marked before where
    the captures are being returned. Will change the channel
    position to just after the match. Will not do anything
    if error is set.

    :arg m: a :record:`Regex.regexMatch` storing a location that matched
    :arg arg: an argument to retrieve the match into. If it is not a string,
              the string match will be cast to arg.type.

    :throws SystemError: Thrown if a match could not be extracted.
 */
proc _channel.extractMatch(m:regexMatch, ref arg) throws {
  var err:errorCode = 0;
  on this._home {
    try this.lock(); defer { this.unlock(); }
    _extractMatch(m, arg, err);
  }
  if err {
    try this._ch_ioerror(err, "in channel.extractMatch(m:regexMatch, ref " +
                              arg.type:string + ")");
  }
}

// Assumes that the channel has been marked where the search began
// (or at least before the capture groups if discarding)
pragma "no doc"
proc _channel._ch_handle_captures(matches:_ddata(qio_regex_string_piece_t),
                                 nmatches:int,
                                 ref captures, ref error:errorCode) {
  assert(nmatches >= captures.size);
  for param i in 0..captures.size-1 {
    var m = _to_regexMatch(matches[i]);
    _extractMatch(m, captures[i], error);
  }
}

// Private implementation helper for _channel.search(re:regex(?))
// Todo: inline it into its single callsite.
pragma "no doc"
proc _channel._searchHelp(re:regex(?), ref error:errorCode):regexMatch
{
  var m:regexMatch;
  on this._home {
    try! this.lock();
    var nm = 1;
    var matches = _ddata_allocate(qio_regex_string_piece_t, nm);
    error = qio_channel_mark(false, _channel_internal);
    if !error {
      error = qio_regex_channel_match(re._regex,
                                       false, _channel_internal, max(int(64)),
                                       QIO_REGEX_ANCHOR_UNANCHORED,
                                       /* can_discard */ true,
                                       /* keep_unmatched */ false,
                                       /* keep_whole_pattern */ true,
                                       matches, nm);
    }
    // Don't report "didn't match" errors
    if error == EFORMAT || error == EEOF then error = 0;
    if !error {
      m = _to_regexMatch(matches[0]);
      if m.matched {
        // Advance to the match.
        qio_channel_revert_unlocked(_channel_internal);
        var cur = qio_channel_offset_unlocked(_channel_internal);
        var target = m.byteOffset:int;
        error = qio_channel_advance(false, _channel_internal, target - cur);
      } else {
        // If we didn't match... leave the channel position at EOF
        qio_channel_commit_unlocked(_channel_internal);
      }
    }
    _ddata_free(matches, nm);
    this.unlock();
  }
  return m;
}

// documented in the version with captures
pragma "no doc"
proc _channel.search(re:regex(?)):regexMatch throws
{
  var e:errorCode = 0;
  var ret = this._searchHelp(re, error=e);
  if e then try this._ch_ioerror(e, "in channel.search");
  return ret;
}

/*  Search for an offset in the channel matching the
    passed regular expression, possibly pulling out capture groups.
    If there is a match, leaves the channel position at the
    match. If there is no match, the channel position will be
    advanced to the end of the channel (or end of the file).

    Throws a SystemError if an error occurs.

    :arg re: a :record:`Regex.regex` record representing a compiled
             regular expression.
    :arg captures: an optional variable number of arguments in which to
                   store the regions of the file matching the capture groups
                   in the regular expression.
    :returns: the region of the channel that matched
 */
proc _channel.search(re:regex(?), ref captures ...?k): regexMatch throws
{
  var m:regexMatch;
  var err:errorCode = 0;
  on this._home {
    try this.lock(); defer { this.unlock(); }
    var nm = captures.size + 1;
    var matches = _ddata_allocate(qio_regex_string_piece_t, nm);
    err = qio_channel_mark(false, _channel_internal);
    if ! err {
      err = qio_regex_channel_match(re._regex,
                                     false, _channel_internal, max(int(64)),
                                     QIO_REGEX_ANCHOR_UNANCHORED,
                                     /* can_discard */ true,
                                     /* keep_unmatched */ false,
                                     /* keep_whole_pattern */ true,
                                     matches, nm);
    }
    if err == EFORMAT || err == EEOF then err = 0;
    if !err {
      m = _to_regexMatch(matches[0]);
      if m.matched {
        // Extract the capture groups.
        _ch_handle_captures(matches, nm, captures, err);

        // Advance to the match.
        qio_channel_revert_unlocked(_channel_internal);
        var cur = qio_channel_offset_unlocked(_channel_internal);
        var target = m.byteOffset:int;
        err = qio_channel_advance(false, _channel_internal, target - cur);
      } else {
        // If we didn't match... leave the channel position at EOF
        qio_channel_commit_unlocked(_channel_internal);
      }
    }
    _ddata_free(matches, nm);
  }

  if err then try this._ch_ioerror(err, "in channel.search");
  return m;
}

/* Enumerates matches in the string as well as capture groups.

   Yields tuples of :record:`Regex.regexMatch` objects, the 1st is always
   the match for the whole pattern.

   At the time each match is returned, the channel position is
   at the start of that match. Note though that you would have
   to use :proc:`IO.channel.advance` to get to the position of a capture group.

   After returning each match, advances to just after that
   match and looks for another match. Thus, it will not return
   overlapping matches.

   In the end, leaves the channel position at the end of the
   last reported match (if we ran out of maxmatches)
   or at the end of the channel (if we no longer matched)

   Holds the channel lock for the duration of the search.

   :arg re: a :record:`Regex.regex` record representing a compiled
            regular expression.
   :arg captures: an optional compile-time constant representing the number
                  of captures to be yielded in tuple elements.
   :arg maxmatches: the maximum number of matches to report.
   :yields: tuples of :record:`Regex.regexMatch` objects, where the first element
            is the whole pattern.  The tuples will have 1+captures elements.
 */
iter _channel.matches(re:regex(?), param captures=0, maxmatches:int = max(int))
// TODO: should be throws
{
  var go = true;
  var i = 0;
  var error:errorCode = 0;
  param nret = captures+1;
  var ret:nret*regexMatch;

  // TODO should be try not try!  ditto try! _mark() below
  try! lock();
  on this._home do try! _mark();

  while go && i < maxmatches {
    on this._home {
      // todo: create local clones of 'error', 'ret', 'go'
      var m:regexMatch;
      // todo: hoist the alloc+free outside the loop, allocating on this._home
      const matches = _ddata_allocate(qio_regex_string_piece_t, nret);

      if ! error {
        error = qio_regex_channel_match(re._regex,
                                 false, _channel_internal, max(int(64)),
                                 QIO_REGEX_ANCHOR_UNANCHORED,
                                 /* can_discard */ true,
                                 /* keep_unmatched */ false,
                                 /* keep_whole_pattern */ true,
                                 matches, nret);
      }
      if !error {
        m = _to_regexMatch(matches[0]);
        if m.matched {
          for param i in 0..nret-1 {
            m = _to_regexMatch(matches[i]);
            _extractMatch(m, ret[i], error);
          }
          // Advance to the start of the match.
          // TODO: avoid duplication with qio_regex_channel_match()
          qio_channel_revert_unlocked(_channel_internal);
          error = qio_channel_mark(false, _channel_internal);
          if !error {
            var cur = qio_channel_offset_unlocked(_channel_internal);
            var target = m.byteOffset:int;
            error = qio_channel_advance(false, _channel_internal, target - cur);
            // Advance by 1 if the regex matched an empty string.
            if m.numBytes == 0 {
              error = qio_channel_advance(false, _channel_internal, 1);
              if error == EEOF {
                error = 0;
                go = false;
              }
            }
          }
        } else {
          // Stay at the end of the searched region.
        }
      }
      _ddata_free(matches, nret);
      if error then go = false;
    }
    if ! error then yield ret;
    i += 1;
  }
  _commit();
  unlock();
  // Don't report didn't find or end-of-file errors.
  if error == EFORMAT || error == EEOF then error = 0;
  // TODO should be try not try!
  if error then try! this._ch_ioerror(error, "in channel.matches");
}

} /* end of FormattedIO module */

public use FormattedIO;

} /* end of IO module */
