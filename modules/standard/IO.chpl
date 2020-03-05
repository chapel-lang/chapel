/*
 * Copyright 2004-2020 Hewlett Packard Enterprise Development LP
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
    - Create leader/follower iterators for ItemReader/ItemWriter so that these
      are as efficient as possible when working with fixed-size data types
      (ie, they can open up channels that are not shared).
*/

private use SysBasic, SysCTypes;
use SysError;

/*

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
enum iostringstyle {
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
enum iostringformat {
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
            to store in :var:`iostyle.str_style`.
 */
proc stringStyleTerminated(terminator:uint(8)) {
  return -(terminator - iostringstyle.data_null:int(64));
}

/*
  This method returns the appropriate :record:`iostyle` ``str_style`` value
  to indicate a string format where strings are terminated by a
  zero byte.
 */
proc stringStyleNullTerminated() {
  return iostringstyle.data_null;
}

/*
  This method returns the appropriate :record:`iostyle` ``str_style`` value
  to indicate a string format where strings have an exact length.
 */
proc stringStyleExactLen(len:int(64)) {
  return len;
}

/*
  This method returns the appropriate :record:`iostyle` ``str_style`` value
  to indicate a string format where string data is preceded by a variable-byte
  length as described in :type:`iostringstyle`.
 */
proc stringStyleWithVariableLength() {
  return iostringstyle.lenVb_data: int(64);
}

/*
  Return the appropriate :record:`iostyle` ``str_style`` value
  to indicate a string format where string data is preceded by a
  `lengthBytes` of length. Only lengths of 1, 2, 4, or 8 are supported.
  When `lengthBytes` is 0, the returned value indicates variable-byte length.

  :throws SystemError: Thrown for an unsupported value of `lengthBytes`.
 */
proc stringStyleWithLength(lengthBytes:int) throws {
  var x = iostringstyle.lenVb_data;
  select lengthBytes {
    when 0 do x = iostringstyle.lenVb_data;
    when 1 do x = iostringstyle.len1b_data;
    when 2 do x = iostringstyle.len2b_data;
    when 4 do x = iostringstyle.len4b_data;
    when 8 do x = iostringstyle.len8b_data;
    otherwise
      throw SystemError.fromSyserr(EINVAL,
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

/*  IOHINT_NONE means normal operation, nothing special
    to hint. Expect to use NONE most of the time.
    The other hints can be bitwise-ORed in.
 */
const IOHINT_NONE = 0:c_int;

/* IOHINT_RANDOM means we expect random access to a file */
const IOHINT_RANDOM = QIO_HINT_RANDOM;

/*  IOHINT_SEQUENTIAL means expect sequential access. On
    Linux, this should double the readahead.
 */
const IOHINT_SEQUENTIAL = QIO_HINT_SEQUENTIAL;

/*  IOHINT_CACHED means we expect the entire file
    to be cached and/or we pull it in all at
    once. May request readahead on the entire file.
 */
const IOHINT_CACHED = QIO_HINT_CACHED;

/*  IOHINT_PARALLEL means that we expect to have many
    channels working with this file in parallel.
    It might change the reading/writing implementation
    to something more efficient in that scenario.
 */
const IOHINT_PARALLEL = QIO_HINT_PARALLEL;

pragma "no doc"
extern type qio_file_ptr_t;
private extern const QIO_FILE_PTR_NULL:qio_file_ptr_t;


pragma "no doc"
extern record qiovec_t {
  var iov_base: c_void_ptr;
  var iov_len: size_t;
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

The :record:`iostyle` type represents I/O styles
defining how Chapel's basic types should be read or written.

See :ref:`about-io-style`.

*/
extern record iostyle { // aka qio_style_t
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
  var str_style:int(64) = iostringstyle.data_toeof: int(64);

  // text style choices
  /* When performing text I/O, pad out to this many columns */
  var min_width_columns:uint(32) = 0;
  /* When performing text I/O, do not use more than this many columns */
  var max_width_columns:uint(32) = max(uint(32));
  /* When performing text I/O, do not use more than this many characters */
  var max_width_characters:uint(32) = max(uint(32));
  /* When performing text I/O, do not use more than this many bytes */
  var max_width_bytes:uint(32) = max(uint(32));

  /* What character do we start strings with, when appropriate? Default is " */
  var string_start:style_char_t = 0x22; // "
  /* What character do we end strings with, when appropriate? Default is " */
  var string_end:style_char_t = 0x22; // "

  /* How should we format strings when performing text I/O?
     See :type:`iostringstyle` for more information
     on what the values of ``str_style`` mean.
   */
  var string_format:uint(8) = iostringformat.word:uint(8);

  /* What character do we start bytes with, when appropriate? Default is " */
  var bytes_prefix:style_char_t = 0x62; // b

  // numeric scanning/printing choices
  /* When reading or writing a numeric value in a text mode channel,
     what base should be used for the number? Default of 0 means decimal.
     Bases 2, 8, 10, 16 are supported for integers. Bases 10 and 16
     are supported for real values.*/
  var base:uint(8) = 0;
  /* When reading or writing a numeric value in a text mode channel,
     how is the integer portion separated from the fractional portion?
     Default is '.' */
  var point_char:style_char_t = 0x2e; // .
  /* When reading or writing a numeric value in a text mode channel,
     how is the exponent written? Default is 'e' */
  var exponent_char:style_char_t = 0x65; // e
  /* When reading or writing a numeric value in a text mode channel,
     when base is > 10, how is the exponent written? Default is 'e' */
  var other_exponent_char:style_char_t = 0x70; // p
  /* What character denotes a positive number? Default is '+' */
  var positive_char:style_char_t = 0x2b; // +;
  /* What character denotes a negative number? Default is '-' */
  var negative_char:style_char_t = 0x2d; // -;
  /* What character follows an the imaginary number? Default is 'i' */
  var i_char:style_char_t = 0x69; // i
  /* When writing in a base other than 10, should the prefix be used?
     (e.g. hexadecimal numbers are prefixed with 0x) */
  var prefix_base:uint(8) = 1;
  // numeric printing choices
  /* When padding with spaces, which pad character to use? Default is ' ' */
  var pad_char:style_char_t = 0x20; // ' '
  /* When printing a positive numeric value, should the + be shown? */
  var showplus:uint(8) = 0;
  /* When printing a numeric value in hexadecimal, should it be
     uppercase? */
  var uppercase:uint(8) = 0;
  /* When printing a numeric value in a field of specified width, should
     the number be on the left (that is padded on the right?). The default
     is to right-justify the number. */
  var leftjustify:uint(8) = 0;
  /* When printing an integral value using a real format, should a trailing
     decimal point be included? If so, the value 0 will be written as '0.' */
  var showpoint:uint(8) = 0;
  /* When printing an integral value using a real format, should a trailing
     decimal point and zero be included? If so, the value 0 will be written
     as '0.0' */
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
                    qioChannelPtr:qio_channel_ptr_t):syserr {
    return ENOSYS;
  }

  /* Returns the length of an open file. */
  proc filelength(out length:int(64)):syserr {
    return ENOSYS;
  }

  /* Returns the path to an open file.
     The caller has the responsibility to free the returned c_string.
   */
  // TODO: use Chapel strings for this, one day
  proc getpath(out path:c_string, out len:int(64)):syserr {
    return ENOSYS;
  }

  /* Write the file data to persistent storage. */
  proc fsync():syserr {
    return ENOSYS;
  }
  /* Get the optimal I/O chunk size for the file. */
  proc getChunk(out length:int(64)):syserr {
    return ENOSYS;
  }
  /* Returns a locale best at working with each chunk within a region in a
     file */
  // TODO: This is not currently implemented or used anywhere.
  proc getLocalesForRegion(start:int(64), end:int(64), out
      localeNames:c_ptr(c_string), ref nLocales:int(64)):syserr {
    return ENOSYS;
  }

  /* Close the file. */
  proc close():syserr {
    return ENOSYS;
  }
}

// This class helps with C runtime I/O plugins. It represents additional
// information for a channel.
pragma "no doc"
class QioPluginChannel {
  /* Read at least ``amt`` bytes and store these in the related channel. */
  proc readAtLeast(amt:int(64)):syserr {
    return ENOSYS;
  }
  /* Write up to ``amt`` bytes from the channel. */
  proc write(amt:int(64)):syserr {
    return ENOSYS;
  }
  /* Close the channel. */
  proc close():syserr {
    return ENOSYS;
  }
}

// These functions let the C QIO code call the plugins
// TODO: Move more of the QIO code to be pure Chapel
pragma "no doc"
export proc chpl_qio_setup_plugin_channel(file:c_void_ptr, ref plugin_ch:c_void_ptr, start:int(64), end:int(64), qio_ch:qio_channel_ptr_t):syserr {
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
export proc chpl_qio_channel_close(ch:c_void_ptr):syserr {
  var c=(ch:unmanaged QioPluginChannel?)!;
  var err = c.close();
  delete c;
  return err;
}

pragma "no doc"
export proc chpl_qio_filelength(file:c_void_ptr, ref length:int(64)):syserr {
  var f=(file:unmanaged QioPluginFile?)!;
  return f.filelength(length);
}
pragma "no doc"
export proc chpl_qio_getpath(file:c_void_ptr, ref str:c_string, ref len:int(64)):syserr {
  var f=(file:unmanaged QioPluginFile?)!;
  return f.getpath(str, len);
}
pragma "no doc"
export proc chpl_qio_fsync(file:c_void_ptr):syserr {
  var f=(file:unmanaged QioPluginFile?)!;
  return f.fsync();
}
pragma "no doc"
export proc chpl_qio_get_chunk(file:c_void_ptr, ref length:int(64)):syserr {
  var f=(file:unmanaged QioPluginFile?)!;
  return f.getChunk(length);
}
pragma "no doc"
export proc chpl_qio_get_locales_for_region(file:c_void_ptr, start:int(64),
    end:int(64), ref localeNames:c_void_ptr, ref nLocales:int(64)):syserr {
  var strPtr:c_ptr(c_string);
  var f=(file:unmanaged QioPluginFile?)!;
  var ret = f.getLocalesForRegion(start, end, strPtr, nLocales);
  localeNames = strPtr:c_void_ptr;
  return ret;
}
pragma "no doc"
export proc chpl_qio_file_close(file:c_void_ptr):syserr {
  var f = (file:unmanaged QioPluginFile?)!;
  var err = f.close();
  delete f;
  return err;
}

// Extern functions
// TODO -- move these declarations to where they are used or into
// a helper module to reduce namespace noise.

private extern proc qio_style_init_default(ref s: iostyle);

private extern proc qio_file_retain(f:qio_file_ptr_t);
private extern proc qio_file_release(f:qio_file_ptr_t);

private extern proc qio_file_init(ref file_out:qio_file_ptr_t, fp:_file, fd:fd_t, iohints:c_int, const ref style:iostyle, usefilestar:c_int):syserr;
private extern proc qio_file_open_access(ref file_out:qio_file_ptr_t, path:c_string, access:c_string, iohints:c_int, const ref style:iostyle):syserr;
private extern proc qio_file_open_tmp(ref file_out:qio_file_ptr_t, iohints:c_int, const ref style:iostyle):syserr;
private extern proc qio_file_open_mem(ref file_out:qio_file_ptr_t, buf:qbuffer_ptr_t, const ref style:iostyle):syserr;

pragma "no doc"
extern proc qio_file_close(f:qio_file_ptr_t):syserr;


private extern proc qio_file_lock(f:qio_file_ptr_t):syserr;
private extern proc qio_file_unlock(f:qio_file_ptr_t);
private extern proc qio_file_isopen(f:qio_file_ptr_t):bool;

/* The general way to make sure data is written without error */
private extern proc qio_file_sync(f:qio_file_ptr_t):syserr;

private extern proc qio_channel_end_offset_unlocked(ch:qio_channel_ptr_t):int(64);
private extern proc qio_file_get_style(f:qio_file_ptr_t, ref style:iostyle);
private extern proc qio_file_get_plugin(f:qio_file_ptr_t):c_void_ptr;
private extern proc qio_channel_get_plugin(ch:qio_channel_ptr_t):c_void_ptr;
private extern proc qio_file_length(f:qio_file_ptr_t, ref len:int(64)):syserr;

private extern proc qio_channel_create(ref ch:qio_channel_ptr_t, file:qio_file_ptr_t, hints:c_int, readable:c_int, writeable:c_int, start:int(64), end:int(64), const ref style:iostyle):syserr;

private extern proc qio_channel_path_offset(threadsafe:c_int, ch:qio_channel_ptr_t, ref path:c_string, ref offset:int(64)):syserr;

private extern proc qio_channel_retain(ch:qio_channel_ptr_t);
private extern proc qio_channel_release(ch:qio_channel_ptr_t);

private extern proc qio_channel_clear_error(ch:qio_channel_ptr_t);
private extern proc _qio_channel_set_error_unlocked(ch:qio_channel_ptr_t, err:syserr);
private extern proc qio_channel_error(ch:qio_channel_ptr_t):syserr;


private extern proc qio_channel_lock(ch:qio_channel_ptr_t):syserr;
private extern proc qio_channel_unlock(ch:qio_channel_ptr_t);

private extern proc qio_channel_get_style(ch:qio_channel_ptr_t, ref style:iostyle);
private extern proc qio_channel_set_style(ch:qio_channel_ptr_t, const ref style:iostyle);

private extern proc qio_channel_binary(ch:qio_channel_ptr_t):uint(8);
private extern proc qio_channel_byteorder(ch:qio_channel_ptr_t):uint(8);
private extern proc qio_channel_str_style(ch:qio_channel_ptr_t):int(64);
private extern proc qio_channel_style_element(ch:qio_channel_ptr_t, element:int(64)):int(64);

private extern proc qio_channel_flush(threadsafe:c_int, ch:qio_channel_ptr_t):syserr;
private extern proc qio_channel_close(threadsafe:c_int, ch:qio_channel_ptr_t):syserr;
pragma "no doc"
extern proc qio_channel_isclosed(threadsafe:c_int, ch:qio_channel_ptr_t):bool;

private extern proc qio_channel_read(threadsafe:c_int, ch:qio_channel_ptr_t, ref ptr, len:ssize_t, ref amt_read:ssize_t):syserr;
private extern proc qio_channel_read_amt(threadsafe:c_int, ch:qio_channel_ptr_t, ref ptr, len:ssize_t):syserr;
pragma "no doc"
// A specialization is needed for _ddata as the value is the pointer its memory
private extern proc qio_channel_read_amt(threadsafe:c_int, ch:qio_channel_ptr_t, ptr:_ddata, len:ssize_t):syserr;
// and for c_ptr
private extern proc qio_channel_read_amt(threadsafe:c_int, ch:qio_channel_ptr_t, ptr:c_ptr, len:ssize_t):syserr;
private extern proc qio_channel_read_byte(threadsafe:c_int, ch:qio_channel_ptr_t):int(32);

private extern proc qio_channel_write(threadsafe:c_int, ch:qio_channel_ptr_t, const ref ptr, len:ssize_t, ref amt_written:ssize_t):syserr;
private extern proc qio_channel_write_amt(threadsafe:c_int, ch:qio_channel_ptr_t, const ref ptr, len:ssize_t):syserr;
pragma "no doc"
// A specialization is needed for _ddata as the value is the pointer its memory
private extern proc qio_channel_write_amt(threadsafe:c_int, ch:qio_channel_ptr_t, const ptr:_ddata, len:ssize_t):syserr;
private extern proc qio_channel_write_byte(threadsafe:c_int, ch:qio_channel_ptr_t, byte:uint(8)):syserr;

private extern proc qio_channel_offset_unlocked(ch:qio_channel_ptr_t):int(64);
private extern proc qio_channel_advance(threadsafe:c_int, ch:qio_channel_ptr_t, nbytes:int(64)):syserr;
private extern proc qio_channel_advance_past_byte(threadsafe:c_int, ch:qio_channel_ptr_t, byte:c_int):syserr;

private extern proc qio_channel_mark(threadsafe:c_int, ch:qio_channel_ptr_t):syserr;
private extern proc qio_channel_revert_unlocked(ch:qio_channel_ptr_t);
private extern proc qio_channel_commit_unlocked(ch:qio_channel_ptr_t);

private extern proc qio_channel_seek(ch:qio_channel_ptr_t, start:int(64), end:int(64)):syserr;

private extern proc qio_channel_write_bits(threadsafe:c_int, ch:qio_channel_ptr_t, v:uint(64), nbits:int(8)):syserr;
private extern proc qio_channel_flush_bits(threadsafe:c_int, ch:qio_channel_ptr_t):syserr;
private extern proc qio_channel_read_bits(threadsafe:c_int, ch:qio_channel_ptr_t, ref v:uint(64), nbits:int(8)):syserr;

private extern proc qio_locales_for_region(fl:qio_file_ptr_t,
                                   start:int(64), end:int(64),
                                   loc_names_out:c_void_ptr,
                                   ref num_locs_out:c_int):syserr;
private extern proc qio_get_chunk(fl:qio_file_ptr_t, ref len:int(64)):syserr;
private extern proc qio_get_fs_type(fl:qio_file_ptr_t, ref tp:c_int):syserr;

private extern proc qio_file_path_for_fd(fd:fd_t, ref path:c_string):syserr;
private extern proc qio_file_path_for_fp(fp:_file, ref path:c_string):syserr;
private extern proc qio_file_path(f:qio_file_ptr_t, ref path:c_string):syserr;
private extern proc qio_shortest_path(fl: qio_file_ptr_t, ref path_out:c_string, path_in:c_string):syserr;

// we don't use qio_channel_read_int/write_int since the code there is pretty
// much a dispatch based on type and that fits better in Chapel. Doing it
// this way also happens to avoid an Intel compiler bug.

private extern proc qio_channel_read_int8(threadsafe:c_int, ch:qio_channel_ptr_t, ref ptr:int(8)):syserr;
private extern proc qio_channel_write_int8(threadsafe:c_int, ch:qio_channel_ptr_t, x:int(8)):syserr;

private extern proc qio_channel_read_uint8(threadsafe:c_int, ch:qio_channel_ptr_t, ref ptr:uint(8)):syserr;
private extern proc qio_channel_write_uint8(threadsafe:c_int, ch:qio_channel_ptr_t, x:uint(8)):syserr;


private extern proc qio_channel_read_int16(threadsafe:c_int, byteorder:c_int, ch:qio_channel_ptr_t, ref ptr:int(16)):syserr;
private extern proc qio_channel_write_int16(threadsafe:c_int, byteorder:c_int, ch:qio_channel_ptr_t, x:int(16)):syserr;

private extern proc qio_channel_read_uint16(threadsafe:c_int, byteorder:c_int, ch:qio_channel_ptr_t, ref ptr:uint(16)):syserr;
private extern proc qio_channel_write_uint16(threadsafe:c_int, byteorder:c_int, ch:qio_channel_ptr_t, x:uint(16)):syserr;

private extern proc qio_channel_read_int32(threadsafe:c_int, byteorder:c_int, ch:qio_channel_ptr_t, ref ptr:int(32)):syserr;
private extern proc qio_channel_write_int32(threadsafe:c_int, byteorder:c_int, ch:qio_channel_ptr_t, x:int(32)):syserr;

private extern proc qio_channel_read_uint32(threadsafe:c_int, byteorder:c_int, ch:qio_channel_ptr_t, ref ptr:uint(32)):syserr;
private extern proc qio_channel_write_uint32(threadsafe:c_int, byteorder:c_int, ch:qio_channel_ptr_t, x:uint(32)):syserr;

private extern proc qio_channel_read_int64(threadsafe:c_int, byteorder:c_int, ch:qio_channel_ptr_t, ref ptr:int(64)):syserr;
private extern proc qio_channel_write_int64(threadsafe:c_int, byteorder:c_int, ch:qio_channel_ptr_t, x:int(64)):syserr;

private extern proc qio_channel_read_uint64(threadsafe:c_int, byteorder:c_int, ch:qio_channel_ptr_t, ref ptr:uint(64)):syserr;
private extern proc qio_channel_write_uint64(threadsafe:c_int, byteorder:c_int, ch:qio_channel_ptr_t, x:uint(64)):syserr;

private extern proc qio_channel_read_float32(threadsafe:c_int, byteorder:c_int, ch:qio_channel_ptr_t, ref ptr:real(32)):syserr;
private extern proc qio_channel_write_float32(threadsafe:c_int, byteorder:c_int, ch:qio_channel_ptr_t, x:real(32)):syserr;

private extern proc qio_channel_read_float32(threadsafe:c_int, byteorder:c_int, ch:qio_channel_ptr_t, ref ptr:imag(32)):syserr;
private extern proc qio_channel_write_float32(threadsafe:c_int, byteorder:c_int, ch:qio_channel_ptr_t, x:imag(32)):syserr;

private extern proc qio_channel_read_float64(threadsafe:c_int, byteorder:c_int, ch:qio_channel_ptr_t, ref ptr:real(64)):syserr;
private extern proc qio_channel_write_float64(threadsafe:c_int, byteorder:c_int, ch:qio_channel_ptr_t, x:real(64)):syserr;

private extern proc qio_channel_read_float64(threadsafe:c_int, byteorder:c_int, ch:qio_channel_ptr_t, ref ptr:imag(64)):syserr;
private extern proc qio_channel_write_float64(threadsafe:c_int, byteorder:c_int, ch:qio_channel_ptr_t, x:imag(64)):syserr;

private extern proc qio_channel_read_string(threadsafe:c_int, byteorder:c_int, str_style:int(64), ch:qio_channel_ptr_t, ref s:c_string, ref len:int(64), maxlen:ssize_t):syserr;
private extern proc qio_channel_write_string(threadsafe:c_int, byteorder:c_int, str_style:int(64), ch:qio_channel_ptr_t, const s:c_string, len:ssize_t):syserr;

private extern proc qio_channel_scan_int(threadsafe:c_int, ch:qio_channel_ptr_t, ref ptr, len:size_t, issigned:c_int):syserr;
private extern proc qio_channel_print_int(threadsafe:c_int, ch:qio_channel_ptr_t, const ref ptr, len:size_t, issigned:c_int):syserr;

private extern proc qio_channel_scan_float(threadsafe:c_int, ch:qio_channel_ptr_t, ref ptr, len:size_t):syserr;
private extern proc qio_channel_print_float(threadsafe:c_int, ch:qio_channel_ptr_t, const ref ptr, len:size_t):syserr;

// These are the same as scan/print float but they assume an 'i' afterwards.
private extern proc qio_channel_scan_imag(threadsafe:c_int, ch:qio_channel_ptr_t, ref ptr, len:size_t):syserr;
private extern proc qio_channel_print_imag(threadsafe:c_int, ch:qio_channel_ptr_t, const ref ptr, len:size_t):syserr;


private extern proc qio_channel_scan_complex(threadsafe:c_int, ch:qio_channel_ptr_t, ref re_ptr, ref im_ptr, len:size_t):syserr;
private extern proc qio_channel_print_complex(threadsafe:c_int, ch:qio_channel_ptr_t, const ref re_ptr, const ref im_ptr, len:size_t):syserr;


private extern proc qio_channel_read_char(threadsafe:c_int, ch:qio_channel_ptr_t, ref char:int(32)):syserr;

private extern proc qio_nbytes_char(chr:int(32)):c_int;
private extern proc qio_encode_to_string(chr:int(32)):c_string;
private extern proc qio_decode_char_buf(ref chr:int(32), ref nbytes:c_int, buf:c_string, buflen:ssize_t):syserr;

private extern proc qio_channel_write_char(threadsafe:c_int, ch:qio_channel_ptr_t, char:int(32)):syserr;
private extern proc qio_channel_skip_past_newline(threadsafe:c_int, ch:qio_channel_ptr_t, skipOnlyWs:c_int):syserr;
private extern proc qio_channel_write_newline(threadsafe:c_int, ch:qio_channel_ptr_t):syserr;

private extern proc qio_channel_scan_string(threadsafe:c_int, ch:qio_channel_ptr_t, ref ptr:c_string, ref len:int(64), maxlen:ssize_t):syserr;
private extern proc qio_channel_scan_bytes(threadsafe:c_int, ch:qio_channel_ptr_t, ref ptr:c_string, ref len:int(64), maxlen:ssize_t):syserr;
private extern proc qio_channel_print_bytes(threadsafe:c_int, ch:qio_channel_ptr_t, const ptr:c_string, len:ssize_t):syserr;
private extern proc qio_channel_print_string(threadsafe:c_int, ch:qio_channel_ptr_t, const ptr:c_string, len:ssize_t):syserr;

private extern proc qio_channel_scan_literal(threadsafe:c_int, ch:qio_channel_ptr_t, const match:c_string, len:ssize_t, skipwsbefore:c_int):syserr;
private extern proc qio_channel_scan_literal_2(threadsafe:c_int, ch:qio_channel_ptr_t, match:c_void_ptr, len:ssize_t, skipwsbefore:c_int):syserr;
private extern proc qio_channel_print_literal(threadsafe:c_int, ch:qio_channel_ptr_t, const match:c_string, len:ssize_t):syserr;
private extern proc qio_channel_print_literal_2(threadsafe:c_int, ch:qio_channel_ptr_t, match:c_void_ptr, len:ssize_t):syserr;

private extern proc qio_channel_skip_json_field(threadsafe:c_int, ch:qio_channel_ptr_t):syserr;

pragma "no doc"
extern record qio_conv_t {
  var preArg1:uint(8);
  var preArg2:uint(8);
  var preArg3:uint(8);
  var argType:uint(8);
  var literal_is_whitespace:uint(8);
  var literal_length:uint(32);
  var literal:c_void_ptr;
  var regexp_length:uint(32);
  var regexp:c_void_ptr;
  var regexp_flags_length:uint(32);
  var regexp_flags:c_void_ptr;
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
private extern const QIO_CONV_ARG_TYPE_REGEXP:c_int;
private extern const QIO_CONV_ARG_TYPE_NONE_REGEXP_LITERAL:c_int;
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

private extern proc qio_conv_parse(const fmt:c_string, start:size_t, ref end:uint(64), scanning:c_int, ref spec:qio_conv_t, ref style:iostyle):syserr;

private extern proc qio_format_error_too_many_args():syserr;
private extern proc qio_format_error_too_few_args():syserr;
private extern proc qio_format_error_arg_mismatch(arg:int):syserr;
pragma "no doc"
extern proc qio_format_error_bad_regexp():syserr;
private extern proc qio_format_error_write_regexp():syserr;

/*
   :returns: the default I/O style. See :record:`iostyle`
             and :ref:`about-io-styles`

 */
proc defaultIOStyle():iostyle {
  var ret:iostyle;
  qio_style_init_default(ret);
  return ret;
}

/* Get an I/O style indicating binary I/O in native byte order.

   :arg str_style: see :type:`iostringstyle` - which format to use when reading
                   or writing strings. Defaults to variable-byte length.
   :returns: the requested :record:`iostyle`
 */
proc iostyle.native(str_style:int(64)=stringStyleWithVariableLength()):iostyle {
  var ret = this;
  ret.binary = 1;
  ret.byteorder = iokind.native:uint(8);
  ret.str_style = str_style;
  return ret;
}

/* Get an I/O style indicating binary I/O in big-endian byte order.

   :arg str_style: see :type:`iostringstyle` - which format to use when reading
                   or writing strings. Defaults to variable-byte length.
   :returns: the requested :record:`iostyle`
 */
proc iostyle.big(str_style:int(64)=stringStyleWithVariableLength()):iostyle {
  var ret = this;
  ret.binary = 1;
  ret.byteorder = iokind.big:uint(8);
  ret.str_style = str_style;
  return ret;
}

/* Get an I/O style indicating binary I/O in little-endian byte order.

   :arg str_style: see :type:`iostringstyle` - which format to use when reading
                   or writing strings. Defaults to variable-byte length.
   :returns: the requested :record:`iostyle`
 */
proc iostyle.little(str_style:int(64)=stringStyleWithVariableLength()):iostyle  {
  var ret = this;
  ret.binary = 1;
  ret.byteorder = iokind.little:uint(8);
  ret.str_style = str_style;
  return ret;
}

/* Get an I/O style indicating text I/O.

   :returns: the requested :record:`iostyle`
 */
// TODO -- add arguments to this function
proc iostyle.text(/* args coming later */):iostyle  {
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

/*

A value of the :type:`iohints` type defines a set of hints about the I/O that
the file or channel will perform.  These hints may be used by the
implementation to select optimized versions of the I/O operations.

The :type:`iohints` type is implementation-defined.
The following :type:`iohints` constants are provided:

  * :const:`IOHINT_NONE` defines an empty set, which provides no hints.
  * :const:`IOHINT_RANDOM` suggests to expect random access.
  * :const:`IOHINT_SEQUENTIAL` suggests to expect sequential access.
  * :const:`IOHINT_CACHED` suggests that the file data is or should be
    cached in memory, possibly all at once.
  * :const:`IOHINT_PARALLEL` suggests to expect many channels
    working with this file in parallel.


Other hints might be added in the future.

The following binary operators are defined on :type:`iohints`:

* ``|`` for set union
* ``&`` for set intersection
* ``==`` for set equality
* ``1=`` for set inequality

When an :type:`iohints` formal has default intent, the
actual is copied to the formal upon a function call and
the formal cannot be assigned within the function.

The default value of the :type:`iohints` type is undefined.

*/

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

extern type iohints = c_int;

/*

The :record:`file` type is implementation-defined.  A value of the
:record:`file` type refers to the state that is used by the implementation to
identify and interact with the OS file.

When a :record:`file` formal argument has default intent, the
actual is copied to the formal upon a function call and
the formal cannot be assigned within the function.

The default value of the :record:`file` type does not represent any OS file.
It is illegal to perform any I/O operations on the default value.

*/
pragma "ignore noinit"
record file {
  pragma "no doc"
  var home: locale = here;
  pragma "no doc"
  var _file_internal:qio_file_ptr_t = QIO_FILE_PTR_NULL;

  // INIT TODO: This would be a useful case for requesting a default initializer
  // be built even when handwritten initializers (copy init) exist.
  proc init() {
  }
}

// TODO -- shouldn't have to write this this way!
pragma "no doc"
proc file.init(x: file) {
  this.home = x.home;
  this._file_internal = x._file_internal;
  this.complete();
  on home {
    qio_file_retain(_file_internal);
  }
}

proc file.init=(x: file) {
  this.init(x);
}

pragma "no doc"
proc =(ref ret:file, x:file) {
  // retain -- release
  on x.home {
    qio_file_retain(x._file_internal);
  }

  on ret.home {
    qio_file_release(ret._file_internal);
  }

  // compiler will do this copy.
  ret.home = x.home;
  ret._file_internal = x._file_internal;
}

pragma "no doc"
proc file.checkAssumingLocal() throws {
  if is_c_nil(_file_internal) then
    throw SystemError.fromSyserr(EBADF, "Operation attempted on an invalid file");
  if !qio_file_isopen(_file_internal) then
    throw SystemError.fromSyserr(EBADF, "Operation attempted on closed file");
}

/* Throw an error if `this` is not a valid representation of an OS file.

   :throws SystemError: Indicates that `this` does not represent an OS file.
*/
proc file.check() throws {
  on this.home {
    this.checkAssumingLocal();
  }
}

pragma "no doc"
proc ref file.deinit() {
  on this.home {
    qio_file_release(_file_internal);
    this._file_internal = QIO_FILE_PTR_NULL;
  }
}

/*
   We could support file locking and unlocking, but
   at the moment I don't see any use case in which
   it would make sense.
proc file.lock() {
  on this.home {
    seterr(nil, qio_file_lock(_file_internal));
  }
}
proc file.unlock() {
  on this.home {
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
proc file._style:iostyle throws {
  var ret:iostyle;
  on this.home {
    try this.checkAssumingLocal();
    var local_style:iostyle;
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
    throw SystemError.fromSyserr(EBADF, "Operation attempted on an invalid file");

  var err:syserr = ENOERR;
  on this.home {
    err = qio_file_close(_file_internal);
  }
  if err then try ioerror(err, "in file.close", this.tryGetPath());
}

/*

Sync a file to disk.

Commits file data to the device associated with this file.
Data written to the file by a channel will be committed
only if the channel has been closed or flushed.

This function will typically call the ``fsync`` system call.

:throws SystemError: Thrown if the file could not be synced.
 */
proc file.fsync() throws {
  var err:syserr = ENOERR;
  on this.home {
    try this.checkAssumingLocal();
    err = qio_file_sync(_file_internal);
  }
  if err then try ioerror(err, "in file.fsync", this.tryGetPath());
}


/*

Get the path to an open file.

Note that not all files have a path (e.g. files opened with :proc:`openmem`),
and that this function may not work on all operating systems.

The function :proc:`Path.file.realPath` is an alternative way
to get the path to a file.

:throws SystemError: Thrown if the path could not be retrieved.
 */
proc file.path : string throws {
  var ret: string;
  var err:syserr = ENOERR;
  on this.home {
    try this.checkAssumingLocal();
    var tmp:c_string;
    var tmp2:c_string;
    err = qio_file_path(_file_internal, tmp);
    if !err {
      err = qio_shortest_path(_file_internal, tmp2, tmp);
    }
    chpl_free_c_string(tmp);
    if !err {
      ret = createStringWithNewBuffer(tmp2,
                                      errors=decodePolicy.escape);
    }
    chpl_free_c_string(tmp2);
  }
  if err then try ioerror(err, "in file.path");
  return ret;
}

/*

Get the path to an open file, or return "unknown" if there was
a problem getting the path to the open file.

*/
proc file.tryGetPath() : string {
  try {
    return this.path;
  } catch {
    return "unknown";
  }
}

/*

Get the current size of an open file. Note that the size can always
change if other channels, tasks or programs are writing to the file.

:returns: the current file size

:throws SystemError: Thrown if the size could not be retrieved.
*/
proc file.size:int(64) throws {
  var err:syserr = ENOERR;
  var len:int(64) = 0;
  on this.home {
    err = qio_file_length(this._file_internal, len);
  }
  if err then try ioerror(err, "in file.size");
  return len;
}

/*
  Deprecated - please use :proc:`file.size`.
*/
proc file.length():int(64) throws {
  compilerWarning("'file.length()' is deprecated - " +
                  "please use 'file.size' instead");
  return this.size;
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

/*

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
*/
proc open(path:string, mode:iomode, hints:iohints=IOHINT_NONE,
          style:iostyle = defaultIOStyle()): file throws {

  var local_style = style;
  var error: syserr = ENOERR;
  var ret: file;
  ret.home = here;

  if (path == "") then
    try ioerror(ENOENT:syserr, "in open: path is the empty string");

  error = qio_file_open_access(ret._file_internal,
                               path.encode(errors=encodePolicy.unescape).c_str(),
                               _modestring(mode).c_str(), hints, local_style);
  if error then
    try ioerror(error, "in open", path);

  return ret;
}

proc openplugin(pluginFile: QioPluginFile, mode:iomode,
                seekable:bool, style:iostyle) throws {
  import HaltWrappers;

  extern proc qio_file_init_plugin(ref file_out:qio_file_ptr_t,
      file_info:c_void_ptr, flags:c_int, const ref style:iostyle):syserr;

  var local_style = style;
  var ret:file;
  ret.home = here;

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
                                         errors=decodePolicy.replace);
      }
    }

    try ioerror(err, "in openplugin", path);
  }

  return ret;
}

// documented in open() throws version
pragma "no doc"
proc open(out error:syserr, path:string="",
          mode:iomode, hints:iohints=IOHINT_NONE,
          style:iostyle = defaultIOStyle()):file {
  compilerWarning("This version of open() is deprecated; " +
                  "please switch to a throwing version");
  error = ENOERR;
  var ret: file;
  try {
    ret = open(path, mode, hints, style);
  } catch e: SystemError {
    error = e.err;
  } catch {
    error = EINVAL;
  }
  return ret;
}

/*

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

 */
proc openfd(fd: fd_t, hints:iohints=IOHINT_NONE, style:iostyle = defaultIOStyle()):file throws {
  var local_style = style;
  var ret:file;
  ret.home = here;
  var err = qio_file_init(ret._file_internal, chpl_cnullfile(), fd, hints, local_style, 0);

  // On return, either ret._file_internal.ref_cnt == 1, or ret._file_internal is NULL.
  // err should be nonzero in the latter case.
  if err {
    var path_cs:c_string;
    var path_err = qio_file_path_for_fd(fd, path_cs);
    var path = if path_err then "unknown"
                           else createStringWithNewBuffer(path_cs,
                                                          errors=decodePolicy.replace);
    try ioerror(err, "in openfd", path);
  }
  return ret;
}

/*

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
 */
proc openfp(fp: _file, hints:iohints=IOHINT_NONE, style:iostyle = defaultIOStyle()):file throws {
  var local_style = style;
  var ret:file;
  ret.home = here;
  var err = qio_file_init(ret._file_internal, fp, -1, hints, local_style, 1);

  // On return either ret._file_internal.ref_cnt == 1, or ret._file_internal is NULL.
  // error should be nonzero in the latter case.
  if err {
    var path_cs:c_string;
    var path_err = qio_file_path_for_fp(fp, path_cs);
    var path = if path_err then "unknown"
                           else createStringWithNewBuffer(path_cs,
                                                          errors=decodePolicy.replace);
    chpl_free_c_string(path_cs);
    try ioerror(err, "in openfp", path);
  }
  return ret;
}

/*

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
 */
proc opentmp(hints:iohints=IOHINT_NONE, style:iostyle = defaultIOStyle()):file throws {
  var local_style = style;
  var ret:file;
  ret.home = here;

  // On return ret._file_internal.ref_cnt == 1.
  var err = qio_file_open_tmp(ret._file_internal, hints, local_style);
  if err then try ioerror(err, "in opentmp");
  return ret;
}

/*

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
 */
proc openmem(style:iostyle = defaultIOStyle()):file throws {
  var local_style = style;
  var ret:file;
  ret.home = here;

  // On return ret._file_internal.ref_cnt == 1.
  var err = qio_file_open_mem(ret._file_internal, QBUFFER_PTR_NULL, local_style);
  if err then try ioerror(err, "in openmem");
  return ret;
}

/*

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

 */
pragma "ignore noinit"
record channel {
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
  var home:locale = here;
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
  var _readWriteThisFromLocale:locale?;
}

pragma "no doc"
proc =(ref ret:channel, x:channel) {
  // retain -- release
  on x.home {
    qio_channel_retain(x._channel_internal);
  }

  on ret.home {
    qio_channel_release(ret._channel_internal);
  }

  ret.home = x.home;
  ret._channel_internal = x._channel_internal;
}

pragma "no doc"
proc channel.init(param writing:bool, param kind:iokind, param locking:bool) {
  this.writing = writing;
  this.kind = kind;
  this.locking = locking;
}

pragma "no doc"
proc channel.init(x: channel) {
  this.writing = x.writing;
  this.kind = x.kind;
  this.locking = x.locking;
  this.home = x.home;
  this._channel_internal = x._channel_internal;
  _readWriteThisFromLocale = x._readWriteThisFromLocale;
  this.complete();
  on x.home {
    qio_channel_retain(x._channel_internal);
  }
}

proc channel.init=(x: this.type) {
  this.init(x);
}

//
// Note that this is effectively the initializer that the compiler
// would typically provide and that, by providing the next initializer
// below, we have to write it out manually...  A good case for having
// a means to "opt-in" to including the compiler-provided initializer?
//
pragma "no doc"
proc channel.init(param writing:bool, param kind:iokind, param locking:bool,
                  home: locale, _channel_internal:qio_channel_ptr_t,
                  _readWriteThisFromLocale: locale?) {
  this.writing = writing;
  this.kind = kind;
  this.locking = locking;
  this.home = home;
  this._channel_internal = _channel_internal;
  this._readWriteThisFromLocale = _readWriteThisFromLocale;
}

pragma "no doc"
proc channel.init(param writing:bool, param kind:iokind, param locking:bool, f:file, out error:syserr, hints:c_int, start:int(64), end:int(64), in local_style:iostyle) {
  this.init(writing, kind, locking);
  on f.home {
    this.home = f.home;
    if kind != iokind.dynamic {
      local_style.binary = true;
      local_style.byteorder = kind:uint(8);
    }
    error = qio_channel_create(this._channel_internal, f._file_internal, hints, !writing, writing, start, end, local_style);
    // On return this._channel_internal.ref_cnt == 1.
    // Failure to check the error return code may result in a double-deletion error.
  }
}

pragma "no doc"
proc ref channel.deinit() {
  on this.home {
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
inline proc _cast(type t:string, x: ioChar) {
  var csc: c_string =  qio_encode_to_string(x.ch);
  // The caller has responsibility for freeing the returned string.
  try! {
    return createStringWithOwnedBuffer(csc);
  }
}


/*

Represents a newline character or character sequence (ie ``\n``). I/O routines
(such as :proc:`channel.read` and :proc:`channel.write`) can use arguments of
this type in order to read or write a newline. This is different from '\n'
because an ioNewline always produces an actual newline, but in some cases
writing ``\n`` will produce an escaped string (such as ``"\n"``).

When reading an ioNewline, read routines will skip any character sequence
(including e.g. letters and numbers) to get to the newline character unless
``skipWhitespaceOnly`` is set to true.

 */
record ioNewline {
  /*
    Normally, we will skip anything at all to get to a \n,
    but if skipWhitespaceOnly is set, it will be an error
    if we run into non-space characters other than \n.
   */
  var skipWhitespaceOnly: bool = false;
  pragma "no doc"
  proc writeThis(f) throws {
    // Normally this is handled explicitly in read/write.
    f <~> "\n";
  }
}

pragma "no doc"
inline proc _cast(type t:string, x: ioNewline) {
  return "\n";
}


/*

Used to represent a constant string we want to read or write.

When writing, the ioLiteral is output without any quoting or escaping.

When reading, the ioLiteral must be matched exactly - or else the read call
will return an error with code :data:`SysBasic.EFORMAT`.

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
    f <~> val;
  }
}

pragma "no doc"
inline proc _cast(type t:string, x: ioLiteral) {
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
    f <~> v;
  }
}

pragma "no doc"
inline proc _cast(type t:string, x: ioBits) {
  const ret = "ioBits(v=" + x.v:string + ", nbits=" + x.nbits:string + ")";
  return ret;
}


pragma "no doc"
proc channel._ch_ioerror(error:syserr, msg:string) throws {
  var path:string = "unknown";
  var offset:int(64) = -1;
  on this.home {
    var tmp_path:c_string;
    var tmp_offset:int(64);
    var err:syserr = ENOERR;
    err = qio_channel_path_offset(locking, _channel_internal, tmp_path, tmp_offset);
    if !err {
      // shouldn't throw
      path = createStringWithNewBuffer(tmp_path,
                                       errors=decodePolicy.replace);
      chpl_free_c_string(tmp_path);
      offset = tmp_offset;
    }
  }
  try ioerror(error, msg, path, offset);
}

pragma "no doc"
proc channel._ch_ioerror(errstr:string, msg:string) throws {
  var path:string = "unknown";
  var offset:int(64) = -1;
  on this.home {
    var tmp_path:c_string;
    var tmp_offset:int(64);
    var err:syserr = ENOERR;
    err = qio_channel_path_offset(locking, _channel_internal, tmp_path, tmp_offset);
    if !err {
      // shouldn't throw
      path = createStringWithNewBuffer(tmp_path,
                                       errors=decodePolicy.replace);
      chpl_free_c_string(tmp_path);
      offset = tmp_offset;
    }
  }
  try ioerror(errstr, msg, path, offset);
}

/*
   Acquire a channel's lock.

   :throws SystemError: Thrown if the lock could not be acquired.
 */
inline proc channel.lock() throws {
  var err:syserr = ENOERR;

  if is_c_nil(_channel_internal) then
    throw SystemError.fromSyserr(EINVAL, "Operation attempted on an invalid channel");

  if locking {
    on this.home {
      err = qio_channel_lock(_channel_internal);
    }
  }
  if err then try this._ch_ioerror(err, "in lock");
}

/*
   Release a channel's lock.
 */
inline proc channel.unlock() {
  if locking {
    on this.home {
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
      :proc:`channel.lock`, do the operations, then unlock it
      with :proc:`channel.unlock`.
      While holding the lock, use :proc:`channel._offset` instead.

   :returns: the current offset of the channel
 */
proc channel.offset():int(64) {
  var ret:int(64);
  on this.home {
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

   :throws SystemError: Throws if the channel offset was not moved.
 */
proc channel.advance(amount:int(64)) throws {
  var err:syserr = ENOERR;
  on this.home {
    try this.lock(); defer { this.unlock(); }
    err = qio_channel_advance(false, _channel_internal, amount);
  }
  if err then try this._ch_ioerror(err, "in advance");
}

/*
   Reads until ``byte`` is found and then leave the channel offset
   just after it.

   :throws EOFError: if the requested `byte` could not be found.
   :throws SystemError: if another error occurred.
 */
proc channel.advancePastByte(byte:uint(8)) throws {
  var err:syserr = ENOERR;
  on this.home {
    try this.lock(); defer { this.unlock(); }
    err = qio_channel_advance_past_byte(false, _channel_internal, byte:c_int);
  }
  if err then try this._ch_ioerror(err, "in advanceToByte");
}

/*
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
 */
inline proc channel.mark() throws where this.locking == false {
  const offset = this.offset();
  const err = qio_channel_mark(false, _channel_internal);

  if err then
    throw SystemError.fromSyserr(err);

  return offset;
}

/*
   Abort an *I/O transaction*. See :proc:`channel.mark`. This function
   will pop the last element from the *mark stack* and then leave the
   previous channel offset unchanged.  This function can only be
   called on a channel with ``locking==false``.
*/
inline proc channel.revert() where this.locking == false {
  qio_channel_revert_unlocked(_channel_internal);
}

/*
   Commit an *I/O transaction*. See :proc:`channel.mark`.  This
   function will pop the last element from the *mark stack* and then
   set the channel offset to the popped offset.  This function can
   only be called on a channel with ``locking==false``.

*/
inline proc channel.commit() where this.locking == false {
  qio_channel_commit_unlocked(_channel_internal);
}

/*
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
 */
proc channel.seek(start:int, end:int = max(int)) throws {

  if this.locking then
    compilerError("Cannot seek on a locking channel");

  const err = qio_channel_seek(_channel_internal, start, end);

  if err then
    throw SystemError.fromSyserr(err);
}


// These begin with an _ to indicated that
// you should have a lock before you use these... there is probably
// a better name for them...

/*
   For a channel locked with :proc:`channel.lock`, return the offset
   of that channel.
 */
inline proc channel._offset():int(64) {
  var ret:int(64);
  on this.home {
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
inline proc channel._mark() throws {
  const offset = this.offset();
  const err = qio_channel_mark(false, _channel_internal);

  if err then
    throw SystemError.fromSyserr(err);

  return offset;
}

/*
   Abort an *I/O transaction*. See :proc:`channel._mark`.  This
   function will pop the last element from the *mark stack* and then
   leave the previous channel offset unchanged.  This function should
   only be called on a channel that has already been locked and
   marked.
*/
inline proc channel._revert() {
  qio_channel_revert_unlocked(_channel_internal);
}

/*
   Commit an *I/O transaction*. See :proc:`channel._mark`.  This
   function will pop the last element from the *mark stack* and then
   set the channel offset to the popped offset.  This function should
   only be called on a channel that has already been locked and
   marked.
*/
inline proc channel._commit() {
  qio_channel_commit_unlocked(_channel_internal);
}

/*

   Return the current style used by a channel. This function should only be
   called on a locked channel.

 */
// TODO -- come up with better names for these
proc channel._style():iostyle {
  var ret:iostyle;
  on this.home {
    var local_style:iostyle;
    qio_channel_get_style(_channel_internal, local_style);
    ret = local_style;
  }
  return ret;
}

/*

   Set the style associated with a channel. This function should only
   be called on a locked channel.

 */
proc channel._set_style(style:iostyle) {
  on this.home {
    var local_style:iostyle = style;
    qio_channel_set_style(_channel_internal, local_style);
  }
}

/*

   Return the locale on which an ongoing I/O was started with a channel.
   This method will return nil unless it is called on a channel that is
   the formal argument to a `readThis`, `writeThis`, or `readWriteThis` method.

 */
inline
proc channel.readWriteThisFromLocale() {
  return _readWriteThisFromLocale;
}

// Returns the original locale that the I/O started on
// Uses readWriteThisFromLocale in order to propagate that
// information across readThis/writeThis/readWriteThis calls.
// If readWriteThisFromLocale returns nil, that means the channel
// was not created to call readThis/writeThis/readWriteThis and
// so the original locale of the I/O is `here`.
pragma "no doc"
inline
proc channel.getLocaleOfIoRequest() {
  var ret = this.readWriteThisFromLocale();
  if ret == nil then
    ret = here;
  return ret;
}

// QIO plugins don't have stable interface yet, hence no-doc
// only works when called on locale owning channel.
pragma "no doc"
proc channel.channelPlugin() : borrowed QioPluginChannel? {
  var vptr = qio_channel_get_plugin(this._channel_internal);
  return vptr:borrowed QioPluginChannel?;
}
pragma "no doc"
proc channel.filePlugin() : borrowed QioPluginFile? {
  var vptr = qio_file_get_plugin(qio_channel_get_file(this._channel_internal));
  return vptr:borrowed QioPluginFile?;
}


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
:arg start: zero-based byte offset indicating where in the file the
            channel should start reading. Defaults to 0.
:arg end: zero-based byte offset indicating where in the file the
          channel should no longer be allowed to read. Defaults
          to a ``max(int)`` - meaning no end point.
:arg hints: optional argument to specify any hints to the I/O system about
            this file. See :type:`iohints`.
:returns: an open reading channel to the requested resource.

:throws SystemError: Thrown if a reading channel could not be returned.
 */
// We can simply call channel.close() on these, since the underlying file will
// be closed once we no longer have any references to it (which in this case,
// since we only will have one reference, will be right after we close this
// channel presumably).
// TODO: include optional iostyle argument for consistency
proc openreader(path:string,
                param kind=iokind.dynamic, param locking=true,
                start:int(64) = 0, end:int(64) = max(int(64)),
                hints:iohints = IOHINT_NONE,
                style:iostyle = defaultIOStyle())
    : channel(false, kind, locking) throws {

  var fl:file = try open(path, iomode.r);
  return try fl.reader(kind, locking, start, end, hints, style);
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
:arg start: zero-based byte offset indicating where in the file the
            channel should start writing. Defaults to 0.
:arg end: zero-based byte offset indicating where in the file the
          channel should no longer be allowed to write. Defaults
          to a ``max(int)`` - meaning no end point.
:arg hints: optional argument to specify any hints to the I/O system about
            this file. See :type:`iohints`.
:returns: an open writing channel to the requested resource.

:throws SystemError: Thrown if a writing channel could not be returned.
*/
proc openwriter(path:string,
                param kind=iokind.dynamic, param locking=true,
                start:int(64) = 0, end:int(64) = max(int(64)),
                hints:iohints = IOHINT_NONE,
                style:iostyle = defaultIOStyle())
    : channel(true, kind, locking) throws {

  var fl:file = try open(path, iomode.cw);
  return try fl.writer(kind, locking, start, end, hints, style);
}

/*
   Create a :record:`channel` that supports reading from a file. See
   :ref:`about-io-overview`.

   The ``start=`` and ``end=`` arguments define the region of the file that the
   channel will read from.  These are byte offsets; the beginning of the file is
   at the offset 0.  The defaults for these arguments enable the channel to
   access the entire file.

   A channel will never read beyond its maximum end position. In addition,
   reading from a channel beyond the end of the underlying file will not extend
   that file.  Reading beyond the end of the file or beyond the end offset of
   the channel will produce the error ``EEOF`` (and return `false` in many
   cases such as :proc:`channel.read`) to indicate that the end was reached.

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
               channel should start reading. Defaults to 0.
   :arg end: zero-based byte offset indicating where in the file the
             channel should no longer be allowed to read. Defaults
             to a ``max(int)`` - meaning no end point.
   :arg hints: provide hints about the I/O that this channel will perform. See
               :type:`iohints`. The default value of :const:`IOHINT_NONE`
               will cause the channel to use the hints provided when opening
               the file.
   :arg style: provide a :record:`iostyle` to use with this channel. The
               default value will be the :record:`iostyle` associated with
               this file.

   :throws SystemError: Thrown if a file reader channel could not be returned.
 */
// It is the responsibility of the caller to release the returned channel
// if the error code is nonzero.
// The return error code should be checked to avoid double-deletion errors.
proc file.reader(param kind=iokind.dynamic, param locking=true, start:int(64) = 0,
                 end:int(64) = max(int(64)), hints:iohints = IOHINT_NONE,
                 style:iostyle = this._style): channel(false, kind, locking) throws {
  var ret:channel(false, kind, locking);
  var err:syserr = ENOERR;
  on this.home {
    try this.checkAssumingLocal();
    ret = new channel(false, kind, locking, this, err, hints, start, end, style);
  }
  if err then try ioerror(err, "in file.reader", this.tryGetPath());

  return ret;
}

/* Iterate over all of the lines in a file.

   :returns: an object which yields strings read from the file

   :throws SystemError: Thrown if an ItemReader could not be returned.
 */
proc file.lines(param locking:bool = true, start:int(64) = 0, end:int(64) = max(int(64)),
                hints:iohints = IOHINT_NONE, in local_style:iostyle = this._style) throws {
  local_style.string_format = QIO_STRING_FORMAT_TOEND;
  local_style.string_end = 0x0a; // '\n'
  param kind = iokind.dynamic;

  var ret:ItemReader(string, kind, locking);
  var err:syserr = ENOERR;
  on this.home {
    try this.checkAssumingLocal();
    var ch = new channel(false, kind, locking, this, err, hints, start, end, local_style);
    ret = new ItemReader(string, kind, locking, ch);
  }
  if err then try ioerror(err, "in file.lines", this.tryGetPath());

  return ret;
}

/*
   Create a :record:`channel` that supports writing to a file. See
   :ref:`about-io-overview`.

   The ``start=`` and ``end=`` arguments define the region of the file that the
   channel will write to.  These are byte offsets; the beginning of the file is
   at the offset 0.  The defaults for these arguments enable the channel to
   access the entire file.

   When a channel writes to a file, it will replace file data that was
   previously stored at the relevant offset. If the offset is beyond the
   end of the file, the file will be extended.

   A channel will never write beyond its maximum end position.  It will extend
   the file only as necessary to store data written to the channel. In other
   words, specifying end here does not impact the file size directly; it
   impacts only the section of the file that this channel can write to. After
   all channels to a file are closed, that file will have a size equal to the
   last position written to by any channel.

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
   :arg hints: provide hints about the I/O that this channel will perform. See
               :type:`iohints`. The default value of :const:`IOHINT_NONE`
               will cause the channel to use the hints provided when opening
               the file.
   :arg style: provide a :record:`iostyle` to use with this channel. The
               default value will be the :record:`iostyle` associated with
               this file.

   :throws SystemError: Thrown if a file writer channel could not be returned.
 */
// It is the responsibility of the caller to retain and release the returned
// channel.
// If the return error code is nonzero, the ref count will be 0 not 1.
// The error code should be checked to avoid double-deletion errors.
proc file.writer(param kind=iokind.dynamic, param locking=true, start:int(64) = 0,
                 end:int(64) = max(int(64)), hints:c_int = 0, style:iostyle = this._style):
                 channel(true,kind,locking) throws {
  var ret:channel(true, kind, locking);
  var err:syserr = ENOERR;
  on this.home {
    try this.checkAssumingLocal();
    ret = new channel(true, kind, locking, this, err, hints, start, end, style);
  }
  if err then try ioerror(err, "in file.writer", this.tryGetPath());

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
    out x:?t):syserr where _isIoPrimitiveType(t) {
  if isBoolType(t) {
    var err:syserr = ENOERR;
    var got:bool = false;

    err = qio_channel_scan_literal(false, _channel_internal, c"true", "true".numBytes:ssize_t, 1);
    if !err {
      got = true;
    } else if err == EFORMAT {
      // try reading false instead.
      err = qio_channel_scan_literal(false, _channel_internal, c"false", "false".numBytes:ssize_t, 1);
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
    var err:syserr = ENOERR;
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
    var err:syserr = ENOERR;
    var st = qio_channel_style_element(_channel_internal, QIO_STYLE_ELEMENT_AGGREGATE);
    for i in chpl_enumerate(t) {
      var str = i:string;
      if st == QIO_AGGREGATE_FORMAT_JSON then str = '"'+str+'"';
      var slen:ssize_t = str.numBytes.safeCast(ssize_t);
      err = qio_channel_scan_literal(false, _channel_internal, str.c_str(), slen, 1);
      // Do not free str, because enum literals are C string literals
      if !err {
        x = i;
        break;
      } else if err != EFORMAT then break;
    }
    return err;
  } else {
    compilerError("Unknown primitive type in _read_text_internal ", t:string);
  }
  return EINVAL;
}

private proc _write_text_internal(_channel_internal:qio_channel_ptr_t,
    x:?t):syserr where _isIoPrimitiveType(t) {
  if isBoolType(t) {
    if x {
      return qio_channel_print_literal(false, _channel_internal, c"true", "true".numBytes:ssize_t);
    } else {
      return qio_channel_print_literal(false, _channel_internal, c"false", "false".numBytes:ssize_t);
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
    return qio_channel_print_string(false, _channel_internal, local_x.c_str(), local_x.numBytes:ssize_t);
  } else if t == bytes {
    // handle bytes
    const local_x = x.localize();
    return qio_channel_print_bytes(false, _channel_internal, local_x.c_str(), local_x.numBytes:ssize_t);
  } else if isEnumType(t) {
    var st = qio_channel_style_element(_channel_internal, QIO_STYLE_ELEMENT_AGGREGATE);
    var s = x:string;
    if st == QIO_AGGREGATE_FORMAT_JSON then s = '"'+s+'"';
    return qio_channel_print_literal(false, _channel_internal, s.c_str(), s.numBytes:ssize_t);
  } else {
    compilerError("Unknown primitive type in _write_text_internal ", t:string);
  }
  return EINVAL;
}

private inline proc _read_binary_internal(_channel_internal:qio_channel_ptr_t, param byteorder:iokind, out x:?t):syserr where _isIoPrimitiveType(t) {
  if isBoolType(t) {
    var got:int(32);
    got = qio_channel_read_byte(false, _channel_internal);
    if got >= 0 {
      x = (got != 0);
      return ENOERR;
    } else {
      return (-got):syserr;
    }
  } else if isIntegralType(t) {
    if numBytes(t) == 1 {
      // this is an optimization, since read_byte returns in a register
      var got:int(32);
      got = qio_channel_read_byte(false, _channel_internal);
      if got >= 0 {
        x = (got:uint(8)):t;
        return ENOERR;
      } else {
        return (-got):syserr;
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
  } else if isFloatType(t) {
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
    var err:syserr = ENOERR;
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
    var err:syserr = ENOERR;
    // call the integer version
    err = _read_binary_internal(_channel_internal, byteorder, i);
    x = chpl__orderToEnum(i, t);
    return err;
  } else {
    compilerError("Unknown primitive type in _read_binary_internal ", t:string);
  }
  return EINVAL;
}

private inline proc _write_binary_internal(_channel_internal:qio_channel_ptr_t, param byteorder:iokind, x:?t):syserr where _isIoPrimitiveType(t) {
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
  } else if isFloatType(t) {
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
    var err:syserr = ENOERR;
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
    return qio_channel_write_string(false, byteorder:c_int, qio_channel_str_style(_channel_internal), _channel_internal, local_x.c_str(), local_x.numBytes: ssize_t);
  } else if t == bytes {
    var local_x = x.localize();
    return qio_channel_write_string(false, byteorder:c_int, qio_channel_str_style(_channel_internal), _channel_internal, local_x.c_str(), local_x.numBytes: ssize_t);
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
proc channel._constructIoErrorMsg(param kind: iokind, const x:?t): string {
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
// The channel must be locked and running on this.home.
// The intent of x is ref (vs out) because it might contain a string literal.
//
pragma "no doc"
proc channel._readOne(param kind: iokind, ref x:?t,
                             loc:locale?) throws {
  // TODO: Make _read_one_internal(s) a method instead.
  var err = try _read_one_internal(_channel_internal, kind, x, loc); 

  if err != ENOERR {
    const msg = _constructIoErrorMsg(kind, x);
    try _ch_ioerror(err, msg);
  }
}

//
// The channel must be locked and running on this.home.
//
pragma "no doc"
proc channel._writeOne(param kind: iokind, const x:?t, loc:locale?) throws {
  // TODO: Make _write_one_internal(s) a method instead.
  var err = _write_one_internal(_channel_internal, kind, x, loc);

  if err != ENOERR {
    const msg = _constructIoErrorMsg(kind, x);
    try _ch_ioerror(err, msg);
  }
}

private inline proc _read_one_internal(_channel_internal:qio_channel_ptr_t,
                                       param kind:iokind,
                                       ref x:?t,
                                       loc:locale?): syserr throws where _isIoPrimitiveTypeOrNewline(t) {
  var e:syserr = ENOERR;
  if t == ioNewline {
    return qio_channel_skip_past_newline(false, _channel_internal, x.skipWhitespaceOnly);
  } else if t == ioChar {
    return qio_channel_read_char(false, _channel_internal, x.ch);
  } else if t == ioLiteral {
    return qio_channel_scan_literal(false, _channel_internal,
                                    x.val.localize().c_str(),
                                    x.val.numBytes: ssize_t, x.ignoreWhiteSpace);
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

private inline proc _write_one_internal(_channel_internal:qio_channel_ptr_t,
                                        param kind:iokind,
                                        const x:?t,
                                        loc:locale?): syserr throws where _isIoPrimitiveTypeOrNewline(t) {
  var e:syserr = ENOERR;
  if t == ioNewline {
    return qio_channel_write_newline(false, _channel_internal);
  } else if t == ioChar {
    return qio_channel_write_char(false, _channel_internal, x.ch);
  } else if t == ioLiteral {
    return qio_channel_print_literal(false, _channel_internal, x.val.localize().c_str(), x.val.numBytes:ssize_t);
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

private inline proc _read_one_internal(_channel_internal:qio_channel_ptr_t,
                                       param kind:iokind,
                                       ref x:?t,
                                       loc:locale?): syserr throws {
  // Create a new channel that borrows the pointer in the
  // existing channel so we can avoid locking (because we
  // already have the lock)
  var reader = new channel(writing=false, iokind.dynamic, locking=false,
                           home=here,
                           _channel_internal=_channel_internal,
                           _readWriteThisFromLocale=loc);

  // Set the channel pointer to NULL to make the
  // destruction of the local reader record safe
  // (it shouldn't release anything since it's a local copy).
  defer { reader._channel_internal = QIO_CHANNEL_PTR_NULL; }

  try x.readThis(reader);

  return ENOERR;
}

pragma "suppress lvalue error"
private inline proc _write_one_internal(_channel_internal:qio_channel_ptr_t,
                                        param kind:iokind,
                                        const x:?t,
                                        loc:locale?): syserr throws {
  // Create a new channel that borrows the pointer in the
  // existing channel so we can avoid locking (because we
  // already have the lock)
  var writer = new channel(writing=true, iokind.dynamic, locking=false,
                           home=here,
                           _channel_internal=_channel_internal,
                           _readWriteThisFromLocale=loc);

  // Set the channel pointer to NULL to make the
  // destruction of the local writer record safe
  // (it shouldn't release anything since it's a local copy).
  defer { writer._channel_internal = QIO_CHANNEL_PTR_NULL; }

  var err: syserr = ENOERR;

  if isClassType(t) || chpl_isDdata(t) || isAnyCPtr(t) {
    if x == nil {
      // future - write class IDs, have serialization format
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
proc channel.readIt(ref x) throws {
  if writing then compilerError("read on write-only channel");
  const origLocale = this.getLocaleOfIoRequest();

  on this.home {
    try! this.lock(); defer { this.unlock(); }
    try _readOne(kind, x, origLocale);
  }
}

pragma "no doc"
proc channel.writeIt(const x) throws {
  if !writing then compilerError("write on read-only channel");
  const origLocale = this.getLocaleOfIoRequest();

  on this.home {
    try! this.lock(); defer { this.unlock(); }
    try _writeOne(kind, x, origLocale);
  }
}

/*
   For a writing channel, writes as with :proc:`channel.write`.
   For a reading channel, reads as with :proc:`channel.read`.
   Stores any error encountered in the channel. Does not return anything.

   :throws SystemError: When an IO error has occurred.
 */
inline proc channel.readwrite(const x) throws where this.writing {
  try this.writeIt(x);
}
// documented in the writing version.
pragma "no doc"
inline proc channel.readwrite(ref x) throws where !this.writing {
  try this.readIt(x);
}

  /*

     The _`<~> operator`

     This `<~>` operator is the same as calling :proc:`channel.readwrite`,
     except that it returns the channel so that multiple operator
     calls can be chained together.

     :returns: ch
     :throws SystemError: When an IO error has occurred.
   */
  inline proc <~>(const ref ch: channel, x) const ref throws
  where ch.writing {
    try ch.readwrite(x);
    return ch;
  }

  // documented in the writing version.
  pragma "no doc"
  inline proc <~>(const ref ch: channel, ref x) const ref throws
  where !ch.writing {
    try ch.readwrite(x);
    return ch;
  }

  // these are overridden to not be inout
  // since they don't change when read anyway
  // and it's much more convenient to be able to do e.g.
  //   reader <~> new ioLiteral("=")

  /* Overload to support reading an :type:`IO.ioLiteral` without
     passing ioLiterals by reference, so that

     .. code-block:: chapel

       reader <~> new ioLiteral("=")

     works without requiring an explicit temporary value to store
     the ioLiteral.
   */
  inline proc <~>(const ref r: channel, lit:ioLiteral) const ref throws
  where !r.writing {
    var litCopy = lit;
    try r.readIt(litCopy);
    return r;
  }

  /* Overload to support reading an :type:`IO.ioNewline` without
     passing ioNewline by reference, so that

     .. code-block:: chapel

       reader <~> new ioNewline()

     works without requiring an explicit temporary value to store
     the ioNewline.
   */
  inline proc <~>(const ref r: channel, nl:ioNewline) const ref throws
  where !r.writing {
    var nlCopy = nl;
    try r.readIt(nlCopy);
    return r;
  }

  /* Explicit call for reading or writing a literal as an
     alternative to using :type:`IO.ioLiteral`.
   */
  inline
  proc channel.readWriteLiteral(lit:string, ignoreWhiteSpace=true) throws
  {
    var iolit = new ioLiteral(lit:string, ignoreWhiteSpace);
    this.readwrite(iolit);
  }

  /* Explicit call for reading or writing a newline as an
     alternative to using :type:`IO.ioNewline`.
   */
  inline proc channel.readWriteNewline() throws
  {
    var ionl = new ioNewline();
    this.readwrite(ionl);
  }

  /* Returns `true` if this channel is configured for binary I/O.
   */
  proc channel.binary():bool {
    var ret:uint(8);
    on this.home {
      ret = qio_channel_binary(_channel_internal);
    }
    return ret != 0;
  }

  /* return other style elements. */
  pragma "no doc"
  proc channel.styleElement(element:int):int {
    var ret:int = 0;
    on this.home {
      ret = qio_channel_style_element(_channel_internal, element);
    }
    return ret;
  }

  /*
     Return any saved error code.
   */
  proc channel.error():syserr {
    var ret:syserr;
    on this.home {
      var local_error:syserr;
      try! this.lock();
      local_error = qio_channel_error(_channel_internal);
      this.unlock();
      ret = local_error;
    }
    return ret;
  }

  /*
     Save an error code.
   */
  proc channel.setError(e:syserr) {
    on this.home {
      var error = e;
      try! this.lock();
      _qio_channel_set_error_unlocked(_channel_internal, error);
      this.unlock();
    }
  }

  /*
     Clear any saved error code.
   */
  proc channel.clearError() {
    on this.home {
      try! this.lock();
      qio_channel_clear_error(_channel_internal);
      this.unlock();
    }
  }

  /*
     Write a sequence of bytes.

     :throws SystemError: Thrown if the byte sequence could not be written.
   */
  proc channel.writeBytes(x, len:ssize_t):bool throws {
    var err:syserr = ENOERR;
    on this.home {
      try this.lock(); defer { this.unlock(); }
      err = qio_channel_write_amt(false, _channel_internal, x, len);
    }
    if err then try this._ch_ioerror(err, "in channel.writeBytes()");
    return true;
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
iter channel.lines() {

  try! this.lock();

  // Save iostyle
  const saved_style: iostyle = this._style();

  // Update iostyle
  var newline_style: iostyle = this._style();
  newline_style.string_format = QIO_STRING_FORMAT_TOEND;
  newline_style.string_end = 0x0a; // '\n'
  this._set_style(newline_style);

  // Iterate over lines
  for line in this.itemReader(string, this.kind) {
    yield line;
  }

  // Set the iostyle back to original state
  this._set_style(saved_style);

  this.unlock();
}


pragma "no doc"
proc _can_stringify_direct(t) param : bool {
  if (t.type == string ||
      t.type == bytes ||
      t.type == c_string ||
      isRangeType(t.type) ||
      isPrimitiveType(t.type)) {
    return true;
  } else if (isTupleType(t.type)) {
    for param i in 1..t.size {
      if !_can_stringify_direct(t[i]) then
        return false;
    }
    return true;
  } else {
    return false;
  }
}

// This routine is called in DefaultRectangular in order
// to report an out of bounds access for a halt. A normal
// call to halt might not be possible because of module
// order issues.
pragma "no doc"
proc _stringify_tuple(tup:?t) where isTuple(t)
{
  var str = "(";

  for param i in 1..tup.size {
    if i != 1 then str += ", ";
    if tup[i].type == c_string {
      try! {
        str += createStringWithNewBuffer(tup[i]);
      }
    }
    else {
      str += tup[i]:string;
    }
  }

 str += ")";

  return str;
}

// Note that stringify is called with primitive/range/tuple arguments
// in modules that are loaded early. To avoid module ordering issues,
// it supports such types directly.
/*
    Creates a string representing the result of writing the arguments.

    Writes each argument, possibly using a `writeThis` method,
    to a string and returns the result.
  */
proc stringify(const args ...?k):string {
  if _can_stringify_direct(args) {
    // As an optimization, use string concatenation for
    // all primitive type stringify...
    // This helps to work around some resolution errors
    // when internal modules use halt, which calls stringify.

    var str = "";

    for param i in 1..k {
      if args[i].type == string {
        str += args[i];
      } else if args[i].type == c_string {
        //decodePolicy.replace never throws
        try! {
          str += createStringWithNewBuffer(args[i],
                                           errors=decodePolicy.replace);
        }
      } else if args[i].type == bytes {
        //decodePolicy.replace never throws
        try! {
          str += args[i].decode(decodePolicy.replace);
        }
      } else if isRangeType(args[i].type) ||
                isPrimitiveType(args[i].type) {
        str += args[i]:string;
      } else if isTupleType(args[i].type) {
        str += _stringify_tuple(args[i]);
      }
    }

    return str;
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

      r.readBytes(buf, offset:ssize_t);
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
  for param i in 1..k {
    var name: string;
    if i <= _arg_to_proto_names.size then name = _arg_to_proto_names[i];
    else name = "x" + i:string;
    err_args += preArg + name + ":" + args(i).type:string;
    if i != k then err_args += ", ";
  }
  return err_args;
}

/* returns true if read successfully, false if we encountered EOF */
// better documented in the style= version
inline proc channel.read(ref args ...?k):bool throws {
  if writing then compilerError("read on write-only channel");
  const origLocale = this.getLocaleOfIoRequest();

  try {
    on this.home {
      try this.lock(); defer { this.unlock(); }
      for param i in 1..k {
        if args[i].locale == here {
          _readOne(kind, args[i], origLocale);
        } else {
          var tmp = args[i];
          _readOne(kind, tmp, origLocale);
          args[i] = tmp;
        }
      }
    }
  } catch err: SystemError {
    if err.err != EEOF then throw err;
    return false;
  }

  return true;
}

/*

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
 */
proc channel.read(ref args ...?k, style:iostyle):bool throws {
  if writing then compilerError("read on write-only channel");
  const origLocale = this.getLocaleOfIoRequest();

  try {    
    on this.home {
      try this.lock(); defer { this.unlock(); }

      var saveStyle = this._style(); defer { this._set_style(saveStyle); }
      this._set_style(style);

      for param i in 1..k {
        _readOne(kind, args[i], origLocale);
      }
    }
  } catch err: SystemError {
    if err.err != EEOF then throw err;
    return false;
  }

  return true;
}

/*
  Read a line into a Chapel array of bytes. Reads until a ``\n`` is reached.
  The ``\n`` is returned in the array.

  Throws a SystemError if a line could not be read from the channel.

  :arg arg: A 1D DefaultRectangular array which must have at least 1 element.
  :arg numRead: The number of bytes read.
  :arg start: Index to begin reading into.
  :arg amount: The maximum amount of bytes to read.
  :returns: true if the bytes were read without error.
*/
proc channel.readline(arg: [] uint(8), out numRead : int, start = arg.domain.low,
                      amount = arg.domain.high - start + 1) : bool throws
                      where arg.rank == 1 && isRectangularArr(arg) {

  if arg.size == 0 || !arg.domain.contains(start) ||
     amount <= 0 || (start + amount - 1 > arg.domain.high) then return false;

  var err:syserr = ENOERR;
  on this.home {
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
    if i == start && got < 0 then err = (-got):syserr;
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
  Read a line into a Chapel string or bytes. Reads until a ``\n`` is reached.
  The ``\n`` is included in the resulting value.

  :arg arg: a string or bytes to receive the line
  :returns: `true` if a line was read without error, `false` upon EOF

  :throws SystemError: Thrown if data could not be read from the channel.
*/
proc channel.readline(ref arg: ?t): bool throws where t==string || t==bytes {
  if writing then compilerError("read on write-only channel");
  const origLocale = this.getLocaleOfIoRequest();

  try {
    on this.home {
      try this.lock(); defer { this.unlock(); }
      var saveStyle = this._style(); defer { this._set_style(saveStyle); }
      var myStyle = saveStyle.text();
      myStyle.string_format = QIO_STRING_FORMAT_TOEND;
      myStyle.string_end = 0x0a; // ascii newline.
      this._set_style(myStyle);
      try _readOne(iokind.dynamic, arg, origLocale);
    }
  } catch err: SystemError {
    if err.err != EEOF then throw err;
    return false;
  }

  return true;
}

/* read a given number of bytes from a channel

   :arg str_out: The string to be read into
   :arg len: Read up to len bytes from the channel, up until EOF
             (or some kind of I/O error). If the default value of -1
             is provided, read until EOF starting from the channel's
             current offset.
   :returns: `true` if we read something, `false` upon EOF

   :throws SystemError: Thrown if the bytes could not be read from the channel.
 */
proc channel.readstring(ref str_out:string, len:int(64) = -1):bool throws {
  var err = readBytesOrString(this, str_out, len);

  if !err {
    return true;
  } else if err == EEOF {
    return false;
  } else {
    try this._ch_ioerror(err, "in channel.readstring(ref str_out:string, len:int(64))");
  }
  return false;
}

/* read a given number of bytes from a channel

   :arg bytes_out: The bytes to be read into
   :arg len: Read up to len bytes from the channel, up until EOF
             (or some kind of I/O error). If the default value of -1
             is provided, read until EOF starting from the channel's
             current offset.
   :returns: `true` if we read something, `false` upon EOF

   :throws SystemError: Thrown if the bytes could not be read from the channel.
 */
proc channel.readbytes(ref bytes_out:bytes, len:int(64) = -1):bool throws {
  var err = readBytesOrString(this, bytes_out, len);

  if !err {
    return true;
  } else if err == EEOF {
    return false;
  } else {
    try this._ch_ioerror(err, "in channel.readbytes(ref str_out:bytes, len:int(64))");
  }
  return false;
}

private proc readBytesOrString(ch: channel, ref out_var: ?t,  len: int(64)) 
    throws {

  var err:syserr = ENOERR;
  on ch.home {
    var lenread:int(64);
    var tx:c_string;
    var lentmp:int(64);
    var actlen:int(64);
    var uselen:ssize_t;

    if len == -1 then uselen = max(ssize_t);
    else {
      uselen = len:ssize_t;
      if ssize_t != int(64) then assert( len == uselen );
    }

    try ch.lock(); defer { ch.unlock(); }

    var binary:uint(8) = qio_channel_binary(ch._channel_internal);
    var byteorder:uint(8) = qio_channel_byteorder(ch._channel_internal);

    if binary {
      err = qio_channel_read_string(false, byteorder,
                                    iostringstyle.data_toeof:int(64),
                                    ch._channel_internal, tx,
                                    lenread, uselen);
    } else {
      var save_style = ch._style();
      var style = ch._style();
      style.string_format = QIO_STRING_FORMAT_TOEOF;
      ch._set_style(style);

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
      ch._set_style(save_style);
    }

    if t == string {
      out_var = createStringWithOwnedBuffer(tx, length=lenread);
    }
    else {
      out_var = createBytesWithOwnedBuffer(tx, length=lenread);
    }
  }

  return err;

}

/*
   Read bits with binary I/O

   :arg v: where to store the read bits. This value will have its *nbits*
           least-significant bits set.
   :arg nbits: how many bits to read
   :returns: `true` if the bits were read without error, `false` upon EOF

   :throws SystemError: Thrown if the bits could not be read from the channel.
 */
inline proc channel.readbits(out v:integral, nbits:integral):bool throws {
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
   :returns: `true` if the bits were written without error, `false` on error

   :throws IllegalArgumentError: Thrown if writing more bits than fit into `v`.
   :throws SystemError: Thrown if the bits could not be written to the channel.
 */
proc channel.writebits(v:integral, nbits:integral):bool throws {
  if castChecking {
    // Error if writing more bits than fit into v
    if numBits(v.type) < nbits then
      throw new owned IllegalArgumentError("v, nbits", "writebits nbits=" + nbits:string +
                                                 " > bits in v:" + v.type:string);
    // Error if writing negative number of bits
    if isIntType(nbits.type) && nbits < 0 then
      throw new owned IllegalArgumentError("nbits", "writebits nbits=" + nbits:string + " < 0");
  }

  return try this.write(new ioBits(v:uint(64), nbits:int(8)));
}

// documented in the style= error= version
pragma "no doc"
proc channel.readln():bool throws {
  var nl = new ioNewline();
  return try this.read(nl);
}

// documented in the style= error= version
pragma "no doc"
proc channel.readln(ref args ...?k):bool throws {
  var nl = new ioNewline();
  return try this.read((...args), nl);
}

/*

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
 */
proc channel.readln(ref args ...?k,
                    style:iostyle):bool throws {
  var nl = new ioNewline();
  return try this.read((...args), nl, style=style);
}

/*
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
 */
proc channel.read(type t) throws {
  var tmp:t;
  try this.read(tmp);
  return tmp;
}

/*
   Read a value of passed type followed by a newline.

   .. note::

     It is difficult to handle errors or to handle reaching the end of
     the file with this function. If such cases are important please use
     :proc:`channel.readln` instead.


   :arg t: the type to read
   :returns: the value read

   :throws SystemError: Thrown if the type could not be read from the channel.
 */
proc channel.readln(type t) throws {
  var tmp:t;
  try this.readln(tmp);
  return tmp;
}

/*
   Read values of passed types followed by a newline
   and return a tuple containing the read values.

   :arg t: more than one type to read
   :returns: a tuple of the read values

   :throws SystemError: Thrown if the types could not be read from the channel.
 */
proc channel.readln(type t ...?numTypes) throws where numTypes > 1 {
  var tupleVal: t;
  for param i in 1..(numTypes-1) do
    tupleVal(i) = this.read(t(i));
  tupleVal(numTypes) = this.readln(t(numTypes));
  return tupleVal;
}

/*
   Read values of passed types and return a tuple containing the read values.

   :arg t: more than one type to read
   :returns: a tuple of the read values

   :throws SystemError: Thrown if the types could not be read from the channel.
 */
proc channel.read(type t ...?numTypes) throws where numTypes > 1 {
  var tupleVal: t;
  for param i in 1..numTypes do
    tupleVal(i) = this.read(t(i));
  return tupleVal;
}

// documented in style= error= version
pragma "no doc"
inline proc channel.write(const args ...?k):bool throws {
  if !writing then compilerError("write on read-only channel");

  const origLocale = this.getLocaleOfIoRequest();
  on this.home {
    try this.lock(); defer { this.unlock(); }
    for param i in 1..k {
      try _writeOne(kind, args(i), origLocale);
    }
  }

  return true;
}

/*
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
 */
proc channel.write(const args ...?k, style:iostyle):bool throws {
  if !writing then compilerError("write on read-only channel");
  const origLocale = this.getLocaleOfIoRequest();

  on this.home {
    try this.lock(); defer { this.unlock(); }

    var saveStyle = this._style();
    this._set_style(style); defer { this._set_style(saveStyle); }

    for param i in 1..k {
      try _writeOne(iokind.dynamic, args(i), origLocale);
    }
  }

  return true;
}

// documented in style= error= version
pragma "no doc"
proc channel.writeln():bool throws {
  return try this.write(new ioNewline());
}

// documented in style= error= version
pragma "no doc"
proc channel.writeln(const args ...?k):bool throws {
  return try this.write((...args), new ioNewline());
}

/*

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
 */
proc channel.writeln(const args ...?k, style:iostyle):bool throws {
  return try this.write((...args), new ioNewline(), style=style);
}

/*

  Makes all writes to the channel, if any, available to concurrent viewers
  of its associated file, such as other channels or other applications
  accessing this file concurrently.
  Unlike :proc:`file.fsync`, this does not commit the written data
  to the file's device.

  :throws SystemError: Thrown if the flush fails.
*/
proc channel.flush() throws {
  var err:syserr = ENOERR;
  on this.home {
    err = qio_channel_flush(locking, _channel_internal);
  }
  if err then try this._ch_ioerror(err, "in channel.flush");
}
// documented in error= version
pragma "no doc"
proc channel.flush(out error:syserr) {
  error = ENOERR;
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
proc channel.assertEOF(errStr: string = "- Not at EOF") {
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
proc channel.atEOF(): bool throws {
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
proc channel.close() throws {
  var err:syserr = ENOERR;

  if is_c_nil(_channel_internal) then
    throw SystemError.fromSyserr(EINVAL, "cannot close invalid channel");

  on this.home {
    err = qio_channel_close(locking, _channel_internal);
  }
  if err then try this._ch_ioerror(err, "in channel.close");
}

/*
   Return `true` if a channel is currently closed.
 */
proc channel.isclosed() {
  var ret:bool;
  on this.home {
    ret = qio_channel_isclosed(locking, _channel_internal);
  }
  return ret;
}

// TODO -- we should probably have separate c_ptr ddata and ref versions
// in this function for it to become user-facing. Right now, errors
// in the type of the argument will only be caught by a type mismatch
// in the call to qio_channel_read_amt.
pragma "no doc"
proc channel.readBytes(x, len:ssize_t) throws {
  if here != this.home then
    throw new owned IllegalArgumentError("bad remote channel.readBytes");
  var err = qio_channel_read_amt(false, _channel_internal, x, len);
  if err then try this._ch_ioerror(err, "in channel.readBytes");
}

/*
proc channel.modifyStyle(f:func(iostyle, iostyle))
{
  on this.home {
    this.lock();
    var style = this._style();
    style = f(style);
    this._set_style(style);
    this.unlock();
  }
}
*/

/* Wrapper class on a channel to make it only read values
   of a single type. Also supports an iterator yielding
   the read values.
 */
record ItemReader {
  /* What type do we read and yield? */
  type ItemType;
  /* the kind field for our channel */
  param kind:iokind;
  /* the locking field for our channel */
  param locking:bool;
  /* our channel */
  var ch:channel(false,kind,locking);
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

/* Create and return an :record:`ItemReader` that can yield read values of
   a single type.
 */
proc channel.itemReader(type ItemType, param kind:iokind=iokind.dynamic) {
  if writing then compilerError(".itemReader on write-only channel");
  return new ItemReader(ItemType, kind, locking, this);
}

record ItemWriter {
  /* What type do we write? */
  type ItemType;
  /* the kind field for our channel */
  param kind:iokind;
  /* the locking field for our channel */
  param locking:bool;
  /* our channel */
  var ch:channel(true,kind,locking);
  /* write a single item, throwing on error */
  proc write(arg:ItemType):bool throws {
    return ch.write(arg);
  }
}

/* Create and return an :record:`ItemWriter` that can write values of
   a single type.
 */
proc channel.itemWriter(type ItemType, param kind:iokind=iokind.dynamic) {
  if !writing then compilerError(".itemWriter on read-only channel");
  return new ItemWriter(ItemType, kind, locking, this);
}

// And now, the toplevel items.

/* standard input, otherwise known as file descriptor 0 */
const stdin:channel(false, iokind.dynamic, true) = stdinInit();
/* standard output, otherwise known as file descriptor 1 */
const stdout:channel(true, iokind.dynamic, true) = stdoutInit();
/* standard error, otherwise known as file descriptor 2 */
const stderr:channel(true, iokind.dynamic, true) = stderrInit();

proc stdinInit() {
  try! {
    return openfd(0).reader();
  }
}

proc stdoutInit() {
  try! {
    return openfp(chpl_cstdout()).writer();
  }
}

proc stderrInit() {
  try! {
    return openfp(chpl_cstderr()).writer();
  }
}

/* Equivalent to ``stdin.read``. See :proc:`channel.read` */
proc read(ref args ...?n):bool throws {
  return stdin.read((...args));
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
/* Equivalent to ``stdin.read``. See :proc:`channel.read` for types */
proc read(type t ...?numTypes) throws {
  return stdin.read((...t));
}


/* Delete a file. This function is likely to be replaced
   by :proc:`FileSystem.remove`.

   :arg path: the path to the file to remove

   :throws SystemError: Thrown if the file is not successfully deleted.
 */
// TODO -- change to FileSystem.remove
proc unlink(path:string) throws {
  extern proc sys_unlink(path:c_string):err_t;
  var err = sys_unlink(path.localize().c_str());
  if err then try ioerror(err:syserr, "in unlink", path);
}

/*
   :returns: `true` if this version of the Chapel runtime supports UTF-8 output.
 */
proc unicodeSupported():bool {
  extern proc qio_unicode_supported():c_int;
  return qio_unicode_supported() > 0;
}


/************** Distributed File Systems ***************/

private extern const FTYPE_NONE   : c_int;
private extern const FTYPE_LUSTRE : c_int;

pragma "no doc"
proc file.fstype():int throws {
  var t:c_int;
  var err:syserr = ENOERR;
  on this.home {
    err = qio_get_fs_type(this._file_internal, t);
  }
  if err then try ioerror(err, "in file.fstype()");
  return t:int;
}

/*
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
 */
proc file.getchunk(start:int(64) = 0, end:int(64) = max(int(64))):(int(64),int(64)) throws {
  var err:syserr = ENOERR;
  var s = 0;
  var e = 0;

  on this.home {
    var real_end = min(end, this.size);
    var len:int(64);

    err = qio_get_chunk(this._file_internal, len);
    if err then try ioerror(err, "in file.getchunk(start:int(64), end:int(64))");

    if (len != 0 && (real_end > start)) {
      // TAKZ - Note that we are only wanting to return an inclusive range -- i.e., we
      // will only return a non-zero start and end [n,m], iff n and m are in [start, end].
      for i in start..real_end by len {
        // Our stripes are too large, so we can't give back a range within the given
        // bounds
        if i > end then
          break;

        if i >= start {
          var new_start = i;
          var new_end:int(64);
          if (i / len + 1) * len >= real_end then
            new_end = real_end;
          // rounding
          else new_end = (i / len + 1) * len;
          if new_start == new_end {
            break;
          } else {
            s = new_start;
            e = new_end;
            break;
          }
        }
      }
    }
  }
  return (s, e);
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
proc file.localesForRegion(start:int(64), end:int(64)) {

  proc findloc(loc:string, locs:c_ptr(c_string), end:int) {
    for i in 0..end-1 {
      if (loc == locs[i]) then
        return true;
    }
    return false;
  }

  var ret: domain(locale);
  on this.home {
    var err:syserr;
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
    if ret.numIndices == 0 then
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
binary numbers, complex numbers, and raw binary I/O.

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
 output 4 raw, binary bytes of the passed integer in native endianness
``%<4i``
 output 4 raw, binary bytes of the passed integer little endian
``%>4i``
 output 4 raw, binary bytes of the passed integer big endian
``%<8i``
 output 8 raw, binary bytes of the passed integer little endian
 (byte widths of 1, 2, 4, and 8 are supported for integral conversions)

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
 emit 4 raw, binary bytes of the passed number in native endianness
``%<8r``
 emit 8 raw, binary bytes of the passed number in little endian
``%<4r``
 emit 4 raw, binary bytes of the passed number in little endian
 (``<`` ``|`` and ``>`` are supported for widths 4 or 8)

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
 same as ``%|4r``
``%|8z``
 emit 8 raw, binary bytes of native-endian complex (a,b are each 4 bytes)
``%<16z``
 emit 16 raw, binary bytes of little-endian complex (a,b each 8 bytes)

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
  and then string data.
``%|vS``
 as with ``%|1S``-``%|8S`` but the string length is encoded using a
 variable-length byte scheme (which is always the same no matter what
 endianness). In this scheme, the high bit of each encoded length byte
 records whether or not there are more length bytes (and the remaining
 bits encode the length in a big-endian manner).

``%|*vS`` or ``%|*0S``
 read an encoded string but limit it to a number of bytes
 read from the argument before the string; when writing
 cause a runtime error if the string is longer than the
 maximum.

``%/a+/``
 where any regular expression can be used instead of ``a+``
 consume one or more 'a's when reading, gives an error when printing,
 and does not assign to any arguments
 (note - regular expression support is dependent on RE2 build;
 see :mod:`Regexp`)

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
 read or write an object in binary native-endian with readThis/writeThis
``%<t``
 read or write an object little-endian in binary with readThis/writeThis
``%>t``
 read or write an object big-endian in binary with readThis/writeThis

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
 [optional endian flag (binary conversions only)]
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

For binary conversions:

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
    * means string binary I/O
    * ``%|17s`` means exactly 17 byte string
   ``0S``/``1S``/``2S``/``4S``/``8S``
    * mean encoded string binary I/O:
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

  writef("%<4u", 0x11223344);
       // outputs:
       // (hexdump of the output)
       // 4433 2211
  writef("%>4u", 0x11223344);
       // outputs:
       // (hexdump of the output)
       // 1122 3344
  writef("%<4i %<4i", 2, 32);
       // outputs:
       // (hexdump of the output -- note that spaces after
       //  a binary format specifier are ignored)
       // 0200 0000 2000 0000


  writef("%|0S\n", "test");
       // outputs:
       // (hexdump of the output)
       // 7465 7374 000a
  writef("%|1S\n", "test");
       // outputs:
       // (hexdump of the output)
       // 0474 6573 740a
  writef("%>2S\n", "test");
       // outputs:
       // (hexdump of the output)
       // 0004 7465 7374 0a
  writef("%>4S\n", "test");
       // outputs:
       // (hexdump of the output)
       // 0000 0004 7465 7374 0a
  writef("%>8S\n", "test");
       // outputs:
       // (hexdump of the output)
       // 0000 0000 0000 0004 7465 7374 0a
  writef("%|vS\n", "test");
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

//use SysBasic;
//use SysError;
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
proc _toIntegral(x:?t) where _isIoPrimitiveType(t) && !isIntegralType(t)
{
  var ret: (int, bool);
  try {
    if isAbstractEnumType(t) then
      ret = (0, false);
    else
      ret = (x:int, true);
  } catch {
    ret = (0, false);
  }
  return ret;
}
private inline
proc _toIntegral(x:?t) where !_isIoPrimitiveType(t)
{
  return (0, false);
}

private inline
proc _toSigned(x:?t) where isIntType(t)
{
  return (x, true);
}
private inline
proc _toSigned(x:uint(8))
{
  return (x:int(8), true);
}
private inline
proc _toSigned(x:uint(16))
{
  return (x:int(16), true);
}
private inline
proc _toSigned(x:uint(32))
{
  return (x:int(32), true);
}
private inline
proc _toSigned(x:uint(64))
{
  return (x:int(64), true);
}

private inline
proc _toSigned(x:?t) where _isIoPrimitiveType(t) && !isIntegralType(t)
{
  var ret: (int, bool);
  try {
    if isAbstractEnumType(t) then
      ret = (0, false);
    else
      ret = (x:int, true);
  } catch {
    ret = (0, false);
  }
  return ret;
}
private inline
proc _toSigned(x:?t) where !_isIoPrimitiveType(t)
{
  return (0:int, false);
}

private inline
proc _toUnsigned(x:?t) where isUintType(t)
{
  return (x, true);
}
private inline
proc _toUnsigned(x:int(8))
{
  return (x:uint(8), true);
}
private inline
proc _toUnsigned(x:int(16))
{
  return (x:uint(16), true);
}
private inline
proc _toUnsigned(x:int(32))
{
  return (x:uint(32), true);
}
private inline
proc _toUnsigned(x:int(64))
{
  return (x:uint(64), true);
}


private inline
proc _toUnsigned(x:?t) where _isIoPrimitiveType(t) && !isIntegralType(t)
{
  var ret: (uint, bool);
  try {
    if isAbstractEnumType(t) then
      ret = (0:uint, false);
    else
      ret = (x:uint, true);
  } catch {
    ret = (0:uint, false);
  }
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
proc _toReal(x:?t) where _isIoPrimitiveType(t) && !isRealType(t)
{
  var ret: (real, bool);
  try {
    if isAbstractEnumType(t) then
      ret = (0.0, false);
    else
      ret = (x:real, true);
  } catch {
    ret = (0.0, false);
  }
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
proc _toImag(x:?t) where _isIoPrimitiveType(t) && !isImagType(t)
{
  var ret: (imag, bool);
  try {
    if isAbstractEnumType(t) then
      ret = (0.0i, false);
    else
      ret = (x:imag, true);
  } catch {
    ret = (0.0i, false);
  }
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
proc _toComplex(x:?t) where _isIoPrimitiveType(t) && !isComplexType(t)
{
  var ret: (complex, bool);
  try {
    if isAbstractEnumType(t) then
      ret = (0.0+0.0i, false);
    else
      ret = (x:complex, true);
  } catch {
    ret = (0.0+0.0i, false);
  }
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
proc _toRealOrComplex(x:?t) where isFloatType(t)
{
  return (x, true);
}
private inline
proc _toRealOrComplex(x:?t) where _isIoPrimitiveType(t) && !isComplexType(t) && !isFloatType(t)
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
proc _toNumeric(x:?t) where _isIoPrimitiveType(t) && !isNumericType(t)
{
  // enums, bools get cast to int.
  var ret: (int, bool);
  try {
    if isAbstractEnumType(t) then
      ret = (0, false);
    else
      ret = (x:int, true);
  } catch {
    ret = (0, false);
  }
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
private inline
proc _toChar(x:?t) where t == string
{
  var chr:int(32);
  var nbytes:c_int;
  var local_x = x.localize();
  qio_decode_char_buf(chr, nbytes, local_x.c_str(), local_x.numBytes:ssize_t);
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
private inline
proc _setIfPrimitive(ref lhs:?t, rhs:?t2, argi:int):syserr where t==bool&&_isIoPrimitiveType(t2)
{
  var empty:t2;
  if rhs == empty {
    lhs = false;
  } else {
    lhs = true;
  }
  return ENOERR;
}
private inline
proc _setIfPrimitive(ref lhs:?t, rhs:?t2, argi:int):syserr where t!=bool&&_isIoPrimitiveType(t)
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
  return ENOERR;
}
private inline
proc _setIfPrimitive(ref lhs:?t, rhs, argi:int):syserr where !_isIoPrimitiveType(t)
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
proc _toRegexp(x:?t) where isSubtype(t, regexp(?))
{
  return (x, true);
}
private inline
proc _toRegexp(x:?t)
{
  var r:regexp(string);
  return (r, false);
}

pragma "no doc"
class _channel_regexp_info {
  var hasRegexp = false;
  var matchedRegexp = false;
  var releaseRegexp = false;
  var theRegexp = qio_regexp_null();
  var matches: _ddata(qio_regexp_string_piece_t) = nil; // size = ncaptures+1
  var capArr: _ddata(bytes) = nil; // size = ncaptures
  var capturei: int;
  var ncaptures: int;

  proc clear() {
    if releaseRegexp {
      qio_regexp_release(theRegexp);
    }
    theRegexp = qio_regexp_null();
    hasRegexp = false;
    matchedRegexp = false;
    releaseRegexp = false;
    if matches then _ddata_free(matches, ncaptures+1);
    for i in 0..#ncaptures do capArr[i] = b"";
    if capArr then _ddata_free(capArr, ncaptures);
  }
  proc allocate_captures() {
    ncaptures = qio_regexp_get_ncaptures(theRegexp);
    matches = _ddata_allocate(qio_regexp_string_piece_t, ncaptures+1);
    capArr = _ddata_allocate(bytes, ncaptures);
    capturei = 0;
  }
  proc deinit() {
    clear();
  }
  override proc writeThis(f) throws {
    f <~> "{hasRegexp = " + hasRegexp: string;
    f <~> ", matchedRegexp = " + matchedRegexp: string;
    f <~> ", releaseRegexp = " + releaseRegexp: string;
    f <~> ", ... capturei = " + capturei: string;
    f <~> ", ncaptures = " + ncaptures: string + "}";
  }
}

pragma "no doc"
proc channel._match_regexp_if_needed(cur:size_t, len:size_t, ref error:syserr,
    ref style:iostyle, r:unmanaged _channel_regexp_info)
{
  if qio_regexp_ok(r.theRegexp) {
    if r.matchedRegexp then return;
    r.matchedRegexp = true;
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
    // Do the actual regexp search.
    // Now read, matching the regexp.
    error = qio_regexp_channel_match(r.theRegexp, false, _channel_internal,
                                     maxlen, QIO_REGEXP_ANCHOR_START,
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
        var m = _to_reMatch(r.matches[1+j]);
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
    error = qio_format_error_bad_regexp();;
  }
}

// Reads the next format string that will require argument handling.
// Handles literals and regexps itself; everything else will
// be returned in conv and with gotConv = true.
// Assumes, for a reading channel, that we are within a mark/revert/commit
//  in readf. (used in the regexp handling here).
pragma "no doc"
proc channel._format_reader(
    fmtStr:?fmtType, ref cur:size_t, len:size_t, ref error:syserr,
    ref conv:qio_conv_t, ref gotConv:bool, ref style:iostyle,
    ref r:unmanaged _channel_regexp_info?,
    isReadf:bool)
{
  if r != nil then r!.hasRegexp = false;
  if !error {
    var fmt = fmtStr.localize().c_str();
    while cur < len {
      gotConv = false;
      if error then break;
      var end:uint(64);
      error = qio_conv_parse(fmt, cur, end, isReadf, conv, style);
      if error {
      }
      cur = end:size_t;
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
            error = qio_channel_scan_literal_2(false, _channel_internal, conv.literal, conv.literal_length:ssize_t, 0);
          }
        } else {
          // when printing we don't care if it's just whitespace.
          error = qio_channel_print_literal_2(false, _channel_internal, conv.literal, conv.literal_length:ssize_t);
        }
      } else if conv.argType == QIO_CONV_ARG_TYPE_NONE_REGEXP_LITERAL {
        if ! isReadf {
          // It's not so clear what to do when printing
          // a regexp. So we just don't handle it.
          error = qio_format_error_write_regexp();
        } else {
          // allocate regexp info if needed
          if r == nil then r = new unmanaged _channel_regexp_info();
          const rnn = r!;  // indicate that it is non-nil
          // clear out old data, if there is any.
          rnn.clear();
          // Compile a regexp from the format string
          var errstr:string;
          // build a regexp out of regexp and regexp_flags
          qio_regexp_create_compile_flags_2(conv.regexp, conv.regexp_length,
                                            conv.regexp_flags,
                                            conv.regexp_flags_length,
                                            /* utf8? */ fmtType==string,
                                            rnn.theRegexp);
          rnn.releaseRegexp = true;
          if qio_regexp_ok(rnn.theRegexp) {
            rnn.hasRegexp = true;
            rnn.ncaptures = qio_regexp_get_ncaptures(rnn.theRegexp);
            // If there are no captures, and we don't have arguments
            // to consume, go ahead and match the regexp.
            if rnn.ncaptures > 0 ||
               conv.preArg1 != QIO_CONV_UNK ||
               conv.preArg2 != QIO_CONV_UNK ||
               conv.preArg3 != QIO_CONV_UNK
            {
              // We need to consume args as part of matching this regexp.
              gotConv = true;
              break;
            } else {
              // No args will be consumed.
              _match_regexp_if_needed(cur, len, error, style, rnn);
            }
          } else {
            error = qio_format_error_bad_regexp();
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
proc channel._conv_helper(
    ref error:syserr,
    ref conv:qio_conv_t, ref gotConv:bool,
    ref j:int,
    ref argType)
{
  if error then return;
  if gotConv {
    // Perhaps we need to handle pre/post args
    // that adjust the style
    if conv.preArg1 != QIO_CONV_UNK {
      argType(j) = conv.preArg1;
      j += 1;
    }
    if conv.preArg2 != QIO_CONV_UNK {
      argType(j) = conv.preArg2;
      j += 1;
    }
    if conv.preArg3 != QIO_CONV_UNK {
      argType(j) = conv.preArg3;
      j += 1;
    }
    if conv.argType != QIO_CONV_UNK {
      if argType(j) == QIO_CONV_UNK {
        // Some regexp paths set it earlier..
        argType(j) = conv.argType;
      }
      j += 1;
    }
  }
}

pragma "no doc"
proc channel._conv_sethandler(
    ref error:syserr,
    argtypei:c_int,
    ref style:iostyle,
    i:int, argi,
    isReadf:bool):bool
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
        style.str_style = stringStyleTerminated(t:uint(8));
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
proc channel._write_signed(width:uint(32), t:int, i:int):syserr
{
  var err:syserr;
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
proc channel._read_signed(width:uint(32), out t:int, i:int):syserr
{
  var err:syserr;
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
proc channel._write_unsigned(width:uint(32), t:uint, i:int)
{
  var err:syserr;
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
proc channel._read_unsigned(width:uint(32), out t:uint, i:int)
{
  var err:syserr;
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
proc channel._write_real(width:uint(32), t:real, i:int)
{
  var err:syserr;
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
proc channel._read_real(width:uint(32), out t:real, i:int)
{
  var err:syserr;
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
proc channel._write_complex(width:uint(32), t:complex, i:int)
{
  var err:syserr = ENOERR;
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
proc channel._read_complex(width:uint(32), out t:complex, i:int)
{
  var err:syserr = ENOERR;
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



/*

   Write arguments according to a format. See
   :ref:`about-io-formatted-io`.

   :arg fmt: the format as string or bytes

   :arg args: 0 or more arguments to write
   :returns: true

   :throws IllegalArgumentError: if an unsupported argument type is encountered.
   :throws SystemError: if the arguments could not be written.
 */
proc channel.writef(fmtStr: ?t, const args ...?k): bool throws 
    where isStringType(t) || isBytesType(t) {

  if !writing then compilerError("writef on read-only channel");
  const origLocale = this.getLocaleOfIoRequest();
  var err: syserr = ENOERR;
  on this.home {
    try this.lock(); defer { this.unlock(); }
    var save_style = this._style();
    var cur:size_t = 0;
    var len:size_t = fmtStr.size:size_t;
    var conv:qio_conv_t;
    var gotConv:bool;
    var style:iostyle;
    var end:size_t;
    var argType:(k+5)*c_int;

    var r:unmanaged _channel_regexp_info?;
    defer {
      if r then delete r;
    }

    for i in 1..argType.size {
      argType(i) = QIO_CONV_UNK;
    }

    var j = 1;

    for param i in 1..k {
      // The inside of this loop is a bit crazy because
      // we're writing it all in a param for in order to
      // get generic argument handling.

      gotConv = false;

      if j <= i {
        _format_reader(fmtStr, cur, len, err,
                       conv, gotConv, style, r,
                       false);
      }

      _conv_helper(err, conv, gotConv, j, argType);

      var domore = _conv_sethandler(err, argType(i), style, i,args(i),false);

      if domore {
        this._set_style(style);
        // otherwise we will consume at least one argument.
        select argType(i) {
          when QIO_CONV_ARG_TYPE_SIGNED, QIO_CONV_ARG_TYPE_BINARY_SIGNED {
            var (t,ok) = _toSigned(args(i));
            if ! ok {
              err = qio_format_error_arg_mismatch(i);
            } else {
              if argType(i) == QIO_CONV_ARG_TYPE_BINARY_SIGNED then
                err = _write_signed(style.max_width_bytes, t, i);
              else
                try _writeOne(iokind.dynamic, t, origLocale);
            }
          } when QIO_CONV_ARG_TYPE_UNSIGNED, QIO_CONV_ARG_TYPE_BINARY_UNSIGNED {
            var (t,ok) = _toUnsigned(args(i));
            if ! ok {
              err = qio_format_error_arg_mismatch(i);
            } else {
              if argType(i) == QIO_CONV_ARG_TYPE_BINARY_UNSIGNED then
                err = _write_unsigned(style.max_width_bytes, t, i);
              else
                try _writeOne(iokind.dynamic, t, origLocale);
            }
          } when QIO_CONV_ARG_TYPE_REAL, QIO_CONV_ARG_TYPE_BINARY_REAL {
            var (t,ok) = _toReal(args(i));
            if ! ok {
              err = qio_format_error_arg_mismatch(i);
            } else {
              if argType(i) == QIO_CONV_ARG_TYPE_BINARY_REAL then
                err = _write_real(style.max_width_bytes, t, i);
              else
                try _writeOne(iokind.dynamic, t, origLocale);
            }
          } when QIO_CONV_ARG_TYPE_IMAG, QIO_CONV_ARG_TYPE_BINARY_IMAG {
            var (t,ok) = _toImag(args(i));
            if ! ok {
              err = qio_format_error_arg_mismatch(i);
            } else {
              if argType(i) == QIO_CONV_ARG_TYPE_BINARY_IMAG then
                err = _write_real(style.max_width_bytes, t:real, i);
              else
                try _writeOne(iokind.dynamic, t, origLocale);
            }
          } when QIO_CONV_ARG_TYPE_COMPLEX, QIO_CONV_ARG_TYPE_BINARY_COMPLEX {
            var (t,ok) = _toComplex(args(i));
            if ! ok {
              err = qio_format_error_arg_mismatch(i);
            } else {
              if argType(i) == QIO_CONV_ARG_TYPE_BINARY_COMPLEX then
                err = _write_complex(style.max_width_bytes, t, i);
              else try _writeOne(iokind.dynamic, t, origLocale);
            }
          } when QIO_CONV_ARG_TYPE_NUMERIC {
            var (t,ok) = _toNumeric(args(i));
            if ! ok {
              err = qio_format_error_arg_mismatch(i);
            } else try _writeOne(iokind.dynamic, t, origLocale);
          } when QIO_CONV_ARG_TYPE_CHAR {
            var (t,ok) = _toChar(args(i));
            if ! ok {
              err = qio_format_error_arg_mismatch(i);
            } else try _writeOne(iokind.dynamic, new ioChar(t), origLocale);
          } when QIO_CONV_ARG_TYPE_BINARY_STRING {
            var (t,ok) = _toBytes(args(i));
            if ! ok {
              err = qio_format_error_arg_mismatch(i);
            } else try _writeOne(iokind.dynamic, t, origLocale);
          } when QIO_CONV_ARG_TYPE_STRING { // can only happen with string
            var (t,ok) = _toString(args(i));
            if ! ok {
              err = qio_format_error_arg_mismatch(i);
            } else try _writeOne(iokind.dynamic, t, origLocale);
          } when QIO_CONV_ARG_TYPE_REGEXP { // It's not so clear what to do when printing
            // a regexp. So we just don't handle it.
            err = qio_format_error_write_regexp();
          } when QIO_CONV_ARG_TYPE_REPR {
            try _writeOne(iokind.dynamic, args(i), origLocale);
          } otherwise {
            // Unhandled argument type!
            throw new owned IllegalArgumentError("args(" + i:string + ")",
                                           "writef internal error " + argType(i):string);
          }
        }
      }
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

    this._set_style(save_style);
  }

  if err then try this._ch_ioerror(err, "in channel.writef(fmt:string)");
  return true;
}

// documented in varargs version
proc channel.writef(fmtStr:?t): bool throws 
    where isStringType(t) || isBytesType(t) {

  if !writing then compilerError("writef on read-only channel");
  var err:syserr = ENOERR;
  on this.home {
    try this.lock(); defer { this.unlock(); }
    var save_style = this._style();
    var cur:size_t = 0;
    var len:size_t = fmtStr.size:size_t;
    var conv:qio_conv_t;
    var gotConv:bool;
    var style:iostyle;
    var end:size_t;
    var dummy:c_int;

    var r:unmanaged _channel_regexp_info?;
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

    this._set_style(save_style);
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

   :throws SystemError: Thrown if the arguments could not be read.
 */
proc channel.readf(fmtStr:?t, ref args ...?k): bool throws 
    where isStringType(t) || isBytesType(t) {

  if writing then compilerError("readf on write-only channel");
  const origLocale = this.getLocaleOfIoRequest();

  var err:syserr = ENOERR;

  on this.home {
    try this.lock(); defer { this.unlock(); }
    var save_style = this._style(); defer { this._set_style(save_style); }
    var cur:size_t = 0;
    var len:size_t = fmtStr.size:size_t;
    var conv:qio_conv_t;
    var gotConv:bool;
    var style:iostyle;
    var end:size_t;
    var argType:(k+5)*c_int;

    var r:unmanaged _channel_regexp_info?;
    defer {
      if r then delete r;
    }

    for i in 1..argType.size {
      argType(i) = QIO_CONV_UNK;
    }

    err = qio_channel_mark(false, _channel_internal);

    if !err then try {
      var j = 1;

      for param i in 1..k {
        // The inside of this loop is a bit crazy because
        // we're writing it all in a param for in order to
        // get generic argument handling.
        if j <= i {
          _format_reader(fmtStr, cur, len, err,
                         conv, gotConv, style, r,
                         true);

          if r != nil {
           const rnn = r!;  // indicate that it is non-nil
           if (rnn.hasRegexp) {
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

        _conv_helper(err, conv, gotConv, j, argType);

        var domore = _conv_sethandler(err, argType(i),style,i,args(i),false);

        if domore {
          this._set_style(style);
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
            } when QIO_CONV_ARG_TYPE_REGEXP {
              var (t,ok) = _toRegexp(args(i));
              if ! ok {
                err = qio_format_error_arg_mismatch(i);
              } else {
                // match it here.
                if r == nil then r = new unmanaged _channel_regexp_info();
                const rnn = r!;  // indicate that it is non-nil
                rnn.clear();
                rnn.theRegexp = t._regexp;
                rnn.hasRegexp = true;
                rnn.releaseRegexp = false;
                _match_regexp_if_needed(cur, len, err, style, rnn);

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
                err = qio_format_error_bad_regexp();
              } else {
                const rnn = r!;  // indicate that it is non-nil
                _match_regexp_if_needed(cur, len, err, style, rnn);
                // Set args(i) to the capture at capturei.
                if rnn.capturei >= rnn.ncaptures {
                  err = qio_format_error_bad_regexp();
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
                      err = qio_format_error_bad_regexp();
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
    } catch thrownError: SystemError {
      if thrownError.err != EEOF then throw thrownError;
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
proc channel.readf(fmtStr:?t) throws 
    where isStringType(t) || isBytesType(t) {

  if writing then compilerError("readf on write-only channel");
  var err:syserr = ENOERR;
  on this.home {
    try this.lock(); defer { this.unlock(); }
    var save_style = this._style(); defer { this._set_style(save_style); }
    var cur:size_t = 0;
    var len:size_t = fmtStr.size:size_t;
    var conv:qio_conv_t;
    var gotConv:bool;
    var style:iostyle;
    var end:size_t;
    var dummy:c_int;

    var r:unmanaged _channel_regexp_info?;
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

   :throws SystemError: Thrown if the field could not be skipped.
 */
proc channel.skipField() throws {
  var err:syserr = ENOERR;
  on this.home {
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

  :throws SystemError: Thrown if the string could not be formatted.
 */
proc string.format(args ...?k): string throws {
  try {
    return chpl_do_format(this, (...args));
  } catch e: SystemError {
    try ioerror(e.err, "in string.format");
  } catch e: DecodeError {
    try ioerror(EILSEQ:syserr, "in string.format");
  } catch {
    try ioerror(EINVAL:syserr, "in string.format");
  }
  return "";
}

/*

  Return a new bytes consisting of values formatted according to a
  format bytes.  See :ref:`about-io-formatted-io`.

  :arg this: the format bytes
  :arg args: the arguments to format
  :returns: the resulting bytes

  :throws SystemError: Thrown if the bytes could not be formatted.
 */
proc bytes.format(args ...?k): bytes throws {
  try {
    return chpl_do_format(this, (...args));
  } catch e: SystemError {
    try ioerror(e.err, "in bytes.format");
  } catch {
    try ioerror(EINVAL:syserr, "in bytes.format");
  }
  return b"";
}

private inline proc chpl_do_format(fmt:?t, args ...?k): t throws
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

  try r.readBytes(buf, offset:ssize_t);

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



use Regexp;

private extern proc qio_regexp_channel_match(const ref re:qio_regexp_t, threadsafe:c_int, ch:qio_channel_ptr_t, maxlen:int(64), anchor:c_int, can_discard:bool, keep_unmatched:bool, keep_whole_pattern:bool, submatch:_ddata(qio_regexp_string_piece_t), nsubmatch:int(64)):syserr;

pragma "no doc"
proc channel._extractMatch(m:reMatch, ref arg:reMatch, ref error:syserr) {
  // If the argument is a match record, just return it.
  arg = m;
}

pragma "no doc"
proc channel._extractMatch(m:reMatch, ref arg:bytes, ref error:syserr) {
  var cur:int(64);
  var target = m.offset:int;
  var len = m.size;

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
        qio_channel_read_string(false, iokind.native:c_int, stringStyleExactLen(len),
                                _channel_internal, ts, gotlen, len: ssize_t);
    s = createBytesWithOwnedBuffer(ts, length=gotlen);
  }

  if ! error {
    arg = s;
  } else {
    arg = b"";
  }
}

pragma "no doc"
proc channel._extractMatch(m:reMatch, ref arg:?t, ref error:syserr)
      where t != reMatch && t != bytes {
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

    :arg m: a :record:`Regexp.reMatch` storing a location that matched
    :arg arg: an argument to retrieve the match into. If it is not a string,
              the string match will be cast to arg.type.

    :throws SystemError: Thrown if a match could not be extracted.
 */
proc channel.extractMatch(m:reMatch, ref arg) throws {
  var err:syserr = ENOERR;
  on this.home {
    try this.lock(); defer { this.unlock(); }
    _extractMatch(m, arg, err);
  }
  if err {
    try this._ch_ioerror(err, "in channel.extractMatch(m:reMatch, ref " +
                              arg.type:string + ")");
  }
}

// documented in throws version
pragma "no doc"
proc channel.extractMatch(m:reMatch, ref arg, ref error:syserr) {
  on this.home {
    try! this.lock();
    _extractMatch(m, arg, error);
    this.unlock();
  }
}

// Assumes that the channel has been marked where the search began
// (or at least before the capture groups if discarding)
pragma "no doc"
proc channel._ch_handle_captures(matches:_ddata(qio_regexp_string_piece_t),
                                 nmatches:int,
                                 ref captures, ref error:syserr) {
  assert(nmatches >= captures.size);
  for param i in 1..captures.size {
    var m = _to_reMatch(matches[i]);
    _extractMatch(m, captures[i], error);
  }
}

// documented in the error= captures version
pragma "no doc"
proc channel.search(re:regexp, ref error:syserr):reMatch
{
  var m:reMatch;
  on this.home {
    try! this.lock();
    var nm = 1;
    var matches = _ddata_allocate(qio_regexp_string_piece_t, nm);
    error = qio_channel_mark(false, _channel_internal);
    if !error {
      error = qio_regexp_channel_match(re._regexp,
                                       false, _channel_internal, max(int(64)),
                                       QIO_REGEXP_ANCHOR_UNANCHORED,
                                       /* can_discard */ true,
                                       /* keep_unmatched */ false,
                                       /* keep_whole_pattern */ true,
                                       matches, nm);
    }
    // Don't report "didn't match" errors
    if error == EFORMAT || error == EEOF then error = ENOERR;
    if !error {
      m = _to_reMatch(matches[0]);
      if m.matched {
        // Advance to the match.
        qio_channel_revert_unlocked(_channel_internal);
        var cur = qio_channel_offset_unlocked(_channel_internal);
        var target = m.offset:int;
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


// documented in the error= version
pragma "no doc"
proc channel.search(re:regexp):reMatch throws
{
  var e:syserr = ENOERR;
  var ret = this.search(re, error=e);
  if e then try this._ch_ioerror(e, "in channel.search");
  return ret;
}

/*  Search for an offset in the channel matching the
    passed regular expression, possibly pulling out capture groups.
    If there is a match, leaves the channel position at the
    match. If there is no match, the channel position will be
    advanced to the end of the channel (or end of the file).

    Throws a SystemError if an error occurs.

    :arg re: a :record:`Regexp.regexp` record representing a compiled
             regular expression.
    :arg captures: an optional variable number of arguments in which to
                   store the regions of the file matching the capture groups
                   in the regular expression.
    :returns: the region of the channel that matched
 */
proc channel.search(re:regexp, ref captures ...?k): reMatch throws
{
  var m:reMatch;
  var err:syserr = ENOERR;
  on this.home {
    try this.lock(); defer { this.unlock(); }
    var nm = captures.size + 1;
    var matches = _ddata_allocate(qio_regexp_string_piece_t, nm);
    err = qio_channel_mark(false, _channel_internal);
    if ! err {
      err = qio_regexp_channel_match(re._regexp,
                                     false, _channel_internal, max(int(64)),
                                     QIO_REGEXP_ANCHOR_UNANCHORED,
                                     /* can_discard */ true,
                                     /* keep_unmatched */ false,
                                     /* keep_whole_pattern */ true,
                                     matches, nm);
    }
    if err == EFORMAT || err == EEOF then err = ENOERR;
    if !err {
      m = _to_reMatch(matches[0]);
      if m.matched {
        // Extract the capture groups.
        _ch_handle_captures(matches, nm, captures, err);

        // Advance to the match.
        qio_channel_revert_unlocked(_channel_internal);
        var cur = qio_channel_offset_unlocked(_channel_internal);
        var target = m.offset:int;
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

// documented in the capture group version
pragma "no doc"
proc channel.match(re:regexp, ref error:syserr):reMatch
{
  var m:reMatch;
  on this.home {
    try! this.lock();
    var nm = 1;
    var matches = _ddata_allocate(qio_regexp_string_piece_t, nm);
    error = qio_channel_mark(false, _channel_internal);
    if ! error {
      error = qio_regexp_channel_match(re._regexp,
                                       false, _channel_internal, max(int(64)),
                                       QIO_REGEXP_ANCHOR_START,
                                       /* can_discard */ true,
                                       /* keep_unmatched */ true,
                                       /* keep_whole_pattern */ true,
                                       matches, nm);
    }
    // Don't report "didn't match" errors
    if error == EFORMAT || error == EEOF then error = ENOERR;
    if !error {
      m = _to_reMatch(matches[0]);
      if m.matched {
        // Advance to the match.
        qio_channel_revert_unlocked(_channel_internal);
        var cur = qio_channel_offset_unlocked(_channel_internal);
        var target = m.offset:int;
        error = qio_channel_advance(false, _channel_internal, target - cur);
      } else {
        // If we didn't match... leave the channel position at start
        qio_channel_revert_unlocked(_channel_internal);
      }
    }
    _ddata_free(matches, nm);
    this.unlock();
  }
  return m;
}

// documented in the error= version
pragma "no doc"
proc channel.match(re:regexp):reMatch throws
{
  var e:syserr = ENOERR;
  var ret = this.match(re, error=e);
  if e then try this._ch_ioerror(e, "in channel.match");
  return ret;
}

/* Match, starting at the current position in the channel,
   against a regexp, possibly pulling out capture groups.
   If there was a match, leaves the channel position at
   the match. If there was no match, leaves the channel
   position where it was at the start of this call.

   :arg re: a :record:`Regexp.regexp` record representing a compiled
             regular expression.
   :arg captures: an optional variable number of arguments in which to
                  store the regions of the file matching the capture groups
                  in the regular expression.
   :returns: the region of the channel that matched
 */

proc channel.match(re:regexp, ref captures ...?k, ref error:syserr):reMatch
{
  var m:reMatch;
  on this.home {
    try! this.lock();
    var nm = 1 + captures.size;
    var matches = _ddata_allocate(qio_regexp_string_piece_t, nm);
    error = qio_channel_mark(false, _channel_internal);
    if !error {
      error = qio_regexp_channel_match(re._regexp,
                               false, _channel_internal, max(int(64)),
                               QIO_REGEXP_ANCHOR_START,
                               /* can_discard */ true,
                               /* keep_unmatched */ true,
                               /* keep_whole_pattern */ true,
                               matches, nm);
    }
    // Don't report "didn't match" errors
    if error == EFORMAT || error == EEOF then error = ENOERR;
    if !error {
      m = _to_reMatch(matches[0]);
      if m.matched {
        // Extract the capture groups.
        _ch_handle_captures(matches, nm, captures, error);

        // Advance to the match.
        qio_channel_revert_unlocked(_channel_internal);
        var cur = qio_channel_offset_unlocked(_channel_internal);
        var target = m.offset:int;
        error = qio_channel_advance(false, _channel_internal, target - cur);
      } else {
        // If we didn't match... leave the channel position at start
        qio_channel_revert_unlocked(_channel_internal);
      }
    }
    _ddata_free(matches, nm);
    this.unlock();
  }
  return m;
}
// documented in the error= version
pragma "no doc"
proc channel.match(re:regexp, ref captures ...?k):reMatch throws
{
  var e:syserr = ENOERR;
  var ret = this.match(re, (...captures), error=e);
  if e then try this._ch_ioerror(e, "in channel.match");
  return ret;
}



/* Enumerates matches in the string as well as capture groups.

   Yields tuples of :record:`Regexp.reMatch` objects, the 1st is always
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

   :arg re: a :record:`Regexp.regexp` record representing a compiled
            regular expression.
   :arg captures: an optional compile-time constant representing the number
                  of captures to be yielded in tuple elements.
   :arg maxmatches: the maximum number of matches to report.
   :yields: tuples of :record:`Regexp.reMatch` objects, where the first element
            is the whole pattern.  The tuples will have 1+captures elements.
 */
iter channel.matches(re:regexp, param captures=0, maxmatches:int = max(int))
// TODO: should be throws
{
  var m:reMatch;
  var go = true;
  var i = 0;
  var error:syserr = ENOERR;
  param nret = captures+1;
  var ret:nret*reMatch;

  // TODO should be try not try!  ditto try! _mark() below
  try! lock();
  on this.home do try! _mark();

  while go && i < maxmatches {
    on this.home {
      var nm = 1 + captures;
      var matches = _ddata_allocate(qio_regexp_string_piece_t, nm);
      if ! error {
        error = qio_regexp_channel_match(re._regexp,
                                 false, _channel_internal, max(int(64)),
                                 QIO_REGEXP_ANCHOR_UNANCHORED,
                                 /* can_discard */ true,
                                 /* keep_unmatched */ false,
                                 /* keep_whole_pattern */ true,
                                 matches, nm);
      }
      if !error {
        m = _to_reMatch(matches[0]);
        if m.matched {
          for param i in 1..nret {
            m = _to_reMatch(matches[i-1]);
            _extractMatch(m, ret[i], error);
          }
          // Advance to the start of the match.
          qio_channel_revert_unlocked(_channel_internal);
          error = qio_channel_mark(false, _channel_internal);
          if !error {
            var cur = qio_channel_offset_unlocked(_channel_internal);
            var target = m.offset:int;
            error = qio_channel_advance(false, _channel_internal, target - cur);
          }
        } else {
          // Stay at the end of the searched region.
        }
      }
      _ddata_free(matches, nm);
      if error then go = false;
    }
    if ! error then yield ret;
    i += 1;
  }
  _commit();
  unlock();
  // Don't report didn't find or end-of-file errors.
  if error == EFORMAT || error == EEOF then error = ENOERR;
  // TODO should be try not try!
  if error then try! this._ch_ioerror(error, "in channel.matches");
}

} /* end of FormattedIO module */

public use FormattedIO;

} /* end of IO module */
