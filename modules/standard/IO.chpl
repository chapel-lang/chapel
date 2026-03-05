/*
 * Copyright 2020-2026 Hewlett Packard Enterprise Development LP
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

Input/output (I/O) facilities in Chapel include the types :record:`file`,
:record:`fileReader` and :record:`fileWriter`; the constants :record:`stdin`,
:record:`stdout` and :record:`stderr`; the functions :proc:`open`,
:proc:`file.close`, :proc:`file.reader`, :proc:`file.writer`,
:proc:`fileReader.read`, :proc:`fileWriter.write`, and many others.


Automatically Available Symbols
-------------------------------

.. note::

   These symbols can also be accessed using ``IO.`` as their qualified access
   prefix.

.. include:: ChapelIO.rst
  :start-line: 7
  :start-after: Automatically included IO symbols
  :end-before: .. function::

.. _about-io-overview:

I/O Overview
------------

A :record:`file` in Chapel identifies a file in the underlying operating system.
Reads to a file are done via one or more fileReaders associated with the file
and writes to a file are done via one or more fileWriters.  Each
:record:`fileReader` or :record:`fileWriter` uses a buffer to provide sequential
read or write access to its file.

For example, the following program opens a file and writes an integer to it:

.. literalinclude:: ../../../../test/library/standard/IO/doc-examples/example_open_read_write.chpl
 :language: chapel
 :start-after: START_EXAMPLE_1
 :end-before: STOP_EXAMPLE_1

Then, the following program can be used to read the integer:

.. literalinclude:: ../../../../test/library/standard/IO/doc-examples/example_open_read_write.chpl
 :language: chapel
 :start-after: START_EXAMPLE_2
 :end-before: STOP_EXAMPLE_2

The :proc:`~IO.read` functions allow one to read values into variables as
the following example demonstrates. It shows three ways to read values into
a pair of variables ``x`` and ``y``.

.. literalinclude:: ../../../../test/library/standard/IO/doc-examples/example_read_vars.chpl
 :language: chapel
 :start-after: START_EXAMPLE
 :end-before: STOP_EXAMPLE

Design Rationale
----------------

Since fileReaders and fileWriters operate independently, concurrent I/O to the
same open file is possible without contending for locks.  Furthermore, since the
fileReader or fileWriter (and not the file) stores the current file offset, it
is straightforward to create programs that access the same open file in
parallel. Note that such parallel access is not possible in C when multiple
threads are using the same ``FILE*`` to write to different regions of a file
because of the race condition between ``fseek`` and ``fwrite``. Because of these
issues, Chapel programmers wishing to perform I/O will need to know how to open
files as well as create fileReaders and fileWriters.

.. _serialize-deserialize:

The 'serialize' and 'deserialize' Methods
-----------------------------------------

A Chapel program can implement ``serialize`` and ``deserialize`` methods
on a user-defined data type to define how that type is deserialized from a
``fileReader`` or serialized to a ``fileWriter``. The method signatures for
non-class types are:

.. literalinclude:: ../../../../test/library/standard/IO/doc-examples/example_de-serialize_methods.chpl
 :language: chapel
 :start-after: START_EXAMPLE_1
 :end-before: STOP_EXAMPLE_1

The signatures for classes are slightly different:

.. literalinclude:: ../../../../test/library/standard/IO/doc-examples/example_de-serialize_methods.chpl
 :language: chapel
 :start-after: START_EXAMPLE_2
 :end-before: STOP_EXAMPLE_2

The ``serializer`` and ``deserializer`` arguments must satisfy the
:ref:`Serializer API<io-serializer-API>` and the
:ref:`Deserializer API<io-deserializer-API>`, respectively.

Basic Usage
~~~~~~~~~~~

Implementations of ``serialize`` and ``deserialize`` methods are not
necessarily required to utilize their ``serializer`` and ``deserializer``
arguments, and can instead trivially read and write from their ``fileReader``
and ``fileWriter`` arguments. For example:

.. literalinclude:: ../../../../test/library/standard/IO/doc-examples/example_basic_serializer.chpl
 :language: chapel
 :start-after: START_EXAMPLE
 :end-before: STOP_EXAMPLE

Using Serializers and Deserializers
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

:ref:`Serializers<io-serializer-user-API>` and
:ref:`Deserializers<io-deserializer-user-API>` support a variety of methods
to support serializing various kinds of types. These methods can be used
to serialize or deserialize a type in a format-agnostic way. For example,
consider a simple 'point' type:

.. literalinclude:: ../../../../test/library/standard/IO/doc-examples/example_point_de-serializer.chpl
 :language: chapel
 :start-after: START_EXAMPLE_1
 :end-before: STOP_EXAMPLE_1

The default implementation of ``point``'s ``serialize`` method will naturally
serialize ``point`` as a record. In the default serialization format, this
would look something like ``(x = 2, y = 4)``. In the JSON serialization format,
the output would instead be ``{"x":4, "y":2}``. While this may be perfectly
acceptable, what if the author of ``point`` wished to always serialize a
``point`` as a tuple?

Serializers and Deserializers have "start" methods that begin serialization
or deserialization of a type, and then return a helper object that implements
methods to continue the process. To begin serializing ``point`` as a tuple,
a user may invoke the ``startTuple`` method on the ``serializer``, passing in
the ``fileWriter`` to use when writing serialized output and the number of
elements in the tuple. The returned value from ``startTuple`` is a helper
object that implements ``writeElement`` and ``endTuple`` methods:

.. literalinclude:: ../../../../test/library/standard/IO/doc-examples/example_point_de-serializer.chpl
 :language: chapel
 :start-after: START_EXAMPLE_2
 :end-before: STOP_EXAMPLE_2

Now, when using different Serializers like the :type:`~IO.defaultSerializer` or
the :type:`~JSON.jsonSerializer`, the ``point`` type can be serialized without
introducing special cases for each format:

.. literalinclude:: ../../../../test/library/standard/IO/doc-examples/example_point_de-serializer.chpl
 :language: chapel
 :start-after: START_EXAMPLE_3
 :end-before: STOP_EXAMPLE_3

A similar API exists for deserialization that would allow for deserializing a
``point`` as a tuple. Please refer to the
:ref:`IO Serializers technote<ioSerializers>` for more detail on the various
kinds of types that can be serialized and deserialized. As of Chapel 1.32 the
supported type-kinds are Classes, Records, Tuples, Arrays, Lists, and Maps.

.. _about-io-generated-default-methods:

Compiler-Generated Default Methods
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Default ``serialize`` methods are created for all types for which a
user-defined ``serialize`` method is not provided.

Classes will be serialized as a 'Class' type-kind using the Serializer API,
and will invoke their parent ``serialize`` method before serializing their
own fields.

Records will be serialized as a 'Record' type-kind using the Serializer API,
and will serialize each field in the record.

Default ``deserialize`` methods are created for all types for which a
user-defined ``deserialize`` method is not provided.  The default
``deserialize`` methods will mirror the relevant API calls in the default
``serialize`` methods.

For more information on the default serialization format, please refer to the
:type:`~IO.defaultSerializer` and :type:`~IO.defaultDeserializer` types.

If the compiler sees a user-defined implementation of the ``serialize`` method,
the ``deserialize`` method, or the deserializing initializer, then the compiler
may choose to not automatically generate any of the other unimplemented
methods. This is out of concern that the user has intentionally deviated from
the default implementation of serialization and deserialization.

Types with compiler-generated versions of these methods do not need to
explicitly indicate that they satisfy any of the relevant serialization
interfaces (such as ``writeSerializable``).

.. note::

  Note that it is not currently possible to read and write circular
  data structures with these mechanisms.

.. _about-io-files:

Files
-----

There are several functions that open a file and return a :record:`file`
including :proc:`open`, :proc:`openTempFile`, :proc:`openMemFile`, the
:record:`file` initializer that takes an ``int`` argument, and the
:record:`file` initializer that takes a :type:`~CTypes.c_FILE` argument.

Once a file is open, it is necessary to create associated fileReader(s) and/or
fileWriter(s) - see :proc:`file.reader` and :proc:`file.writer` - to read from
and/or write to the file.

Use the :proc:`file.fsync` function to explicitly synchronize the file to
ensure that file data is committed to the file's underlying device for
persistence.

Files, fileReaders, and fileWriters will be kept alive while there are variables
referring to them and closed when all variables referring to them have gone out
of scope.  However, each can be closed explicitly with ``close`` methods.  Note
that :proc:`file.close` will not work if the file has open fileReaders and/or
fileWriters.

 .. note::

    :ref:`Escaped strings <string.nonunicode>` can be used for paths on systems
    where UTF-8 file names are not enforced.


.. _about-io-filereader-filewriter-creation:

Functions for fileReader and fileWriter Creation
------------------------------------------------

:proc:`file.writer` creates a :record:`fileWriter` for writing to a file, and
:proc:`file.reader` creates a :record:`fileReader` for reading from a file.

The helper functions :proc:`openReader` and :proc:`openWriter` can also be used
to open a file and create a ``fileReader``/``fileWriter`` to it in a
single step.

.. _about-io-filereader-filewriter-synchronization:

Synchronization of fileReader and fileWriter Data and Avoiding Data Races
-------------------------------------------------------------------------

FileReaders and fileWriters (and files) contain locks in order to keep their
operation safe for multiple tasks. When creating a fileReader or fileWriter, it
is possible to disable the lock (for performance reasons) by passing
``locking=false`` to e.g.  file.writer(), or by using
:proc:`openReader`/:proc:`openWriter`.  Some ``fileReader`` and ``fileWriter``
methods should only be called on locked fileReaders or fileWriters.  With these
methods, it is possible to perform I/O "transactions" (see
:proc:`fileWriter.mark`, e.g.). To use these methods, e.g., first lock the
fileWriter with :proc:`fileWriter.lock`, call the methods you need, then unlock
the fileWriter with :proc:`fileWriter.unlock`. Note that in the future, we may
move to alternative ways of calling these functions that guarantee that they are
not called on a fileReader or fileWriter without the appropriate locking.

Besides data races that can occur if locking is not used in fileWriters when it
should be, it is also possible for there to be data races on file data that is
buffered simultaneously in multiple fileReader/fileWriter combinations.  The
main way to avoid such data races is the :proc:`fileWriter.flush`
synchronization operation.  :proc:`fileWriter.flush` will make all writes to the
fileWriter, if any, available to concurrent viewers of its associated file, such
as other fileWriters, fileReaders or other applications accessing this file
concurrently. See the note below for more details on the situation in which this
kind of data race can occur.

.. note::

  Since fileWriters can buffer data until :proc:`fileWriter.flush` is called, it
  is possible to write programs that have undefined behavior because of race
  conditions on fileWriter buffers. In particular, the problem comes up for
  programs that make:

   * concurrent operations on multiple fileWriters and/or fileReaders that
     operate on overlapping regions of a file
   * where at least one fileWriter is used along with other fileWriters or
     fileReaders
   * and where data could be stored in more than one of the overlapping
     fileWriter's buffers at the same time (i.e., write and read ordering are
     not enforced through :proc:`fileWriter.flush` and other means such as
     sync variables).

  Note that it is possible in some cases to create a :record:`file` that does
  not allow multiple fileWriters and/or fileReaders at different
  offsets. FileWriters created on such files will not change the file's offset
  based on a ``region=`` offset argument. Instead, each read or write operation
  will use the file descriptor's current offset. Therefore, only one
  fileWriter or fileReader should be created for files created in the following
  situations:

    * with the file initializer that takes a :type:`~CTypes.c_FILE` argument
    * with the file initializer that takes an ``int`` argument, where the
      ``int`` represents a non-seekable system file descriptor


Performing I/O with FileReaders and FileWriters
-----------------------------------------------

FileReaders have a variety of read methods and fileWriters have a variety of
write methods. The most common variety of these are generic methods that can
read or write values of any type. For non-primitive types, the relevant
``deserialize`` or ``serialize`` method is used to control the I/O formatting;
see :ref:`serialize-deserialize`. These functions generally take any number of
arguments and `throw` if there was an error:

 * :proc:`fileWriter.write`
 * :proc:`fileWriter.writeln`
 * :proc:`FormattedIO.fileWriter.writef` (see also :ref:`about-io-formatted-io`)
 * :proc:`fileReader.read`
 * :proc:`fileReader.readln`
 * :proc:`FormattedIO.fileReader.readf` (see also :ref:`about-io-formatted-io`)

The ``fileWriter`` type also has the following methods for executing write
operations with more specific types. These methods can provide finer control
over the ``fileWriter``'s behavior as well as some performance advantages over
the generic `write` methods:

 * :proc:`fileWriter.writeBits`
 * :proc:`fileWriter.writeCodepoint`
 * :proc:`fileWriter.writeByte`
 * :proc:`fileWriter.writeString`
 * :proc:`fileWriter.writeBytes`
 * :proc:`fileWriter.writeBinary`

The ``fileReader`` type has similar methods for executing read operations with
more specific types, where the goal of these methods is also to provide finer
control over the ``fileReader``'s behavior and the potential for performance
advantages:

 * :proc:`fileReader.readBits`
 * :proc:`fileReader.readCodepoint`
 * :proc:`fileReader.readByte`
 * :proc:`fileReader.readString`
 * :proc:`fileReader.readBytes`
 * :proc:`fileReader.readBinary`

Additionally, the ``fileReader`` has the following methods which read arbitrary
amounts of data from the file until some stop condition is met. These methods
generally have multiple overloads for reading into values of different types:

 * :proc:`fileReader.readLine`
 * :proc:`fileReader.lines`
 * :proc:`fileReader.readThrough`
 * :proc:`fileReader.readTo`
 * :proc:`fileReader.readAll`

Sometimes it's important to flush the buffer in a fileWriter - to do that, use
the :proc:`fileWriter.flush()` method. Flushing the buffer will make all writes
available to other applications or other views of the file (e.g., it will call
the OS call ``pwrite()``).  It is also possible to close a fileWriter, which
will implicitly flush it and release any buffer memory used by the fileWriter.
Note that if you need to ensure that data from a fileWriter is on disk, you'll
have to call :proc:`fileWriter.flush` or :proc:`fileWriter.close` and then
:proc:`file.fsync` on the related file.

.. _about-io-closing-filereader-filewriter:

Functions for Closing FileReaders and FileWriters
-------------------------------------------------

A fileReader or fileWriter must be closed in order to free the resources
allocated for it, to ensure that data written to it is visible to other
fileReaders, or to allow the associated file to be closed.

See :proc:`fileReader.close` and :proc:`fileWriter.close`.

It is an error to perform any I/O operations on a fileReader or fileWriter
that has been closed.
It is an error to close a file when it has fileReaders and/or fileWriters that
have not been closed.

Files, fileReaders and fileWriters are reference counted. Each file, fileReader
and fileWriter is closed automatically when no references to it remain. For
example, if a local variable is the only reference to a fileReader, the
fileReader will be closed when that variable goes out of scope.  Programs may
also close a file, fileReader or fileWriter explicitly.

.. _stdin-stdout-stderr:

The ``stdin`` fileReader, and ``stdout`` and ``stderr`` fileWriters
-------------------------------------------------------------------

Chapel provides the predefined fileReader :var:`stdin`, and the predefined
fileWriters :var:`stdout`, and :var:`stderr` to access the corresponding
operating system streams standard input, standard output, and standard error.

:var:`stdin` supports reading;
:var:`stdout` and :var:`stderr` support writing.

All three are safe to use concurrently.

Unicode Support
---------------

Most I/O operations default to working with textual data in the UTF-8 encoding.
This choice of UTF-8 matches the encoding used by the ``string`` type (see
:ref:`Chapter-Strings`).

To work with non-UTF-8 data, it's necessary to use binary I/O routines (e.g.
:proc:`fileReader.readByte`, :proc:`fileReader.readBytes`,
:proc:`fileReader.readBinary` :proc:`fileReader.readBits`) or do I/O with a
serializer or deserializer that uses a binary format, such as
:record:`~IO.binaryDeserializer`.

Generally speaking, if invalid UTF-8 is encountered when reading textual data, a
``SystemError`` will be throw with ``EILSEQ`` and the channel position will be
left just after the first byte of UTF-8 that was determined to be invalid. Some
routines have other error handling behavior as described in their documentation
(for example, see :proc:`fileReader.readThrough`).

.. _about-io-error-handling:

Error Handling
--------------

Most I/O routines throw an :class:`~Errors.Error`, which can be handled
appropriately with ``try`` and ``catch`` (see the
:ref:`documentation<Handling_Errors>` for more detail).

Additionally, some subclasses of :class:`~Errors.Error` are commonly used within
the I/O implementation. These are:

 * :class:`OS.EofError` - the end of file was reached
 * :class:`OS.UnexpectedEofError` - a read or write only returned part of the requested data
 * :class:`OS.BadFormatError` - data read did not adhere to the requested format

.. _io-general-sys-error:

**System Errors:**

For other error cases, a general :class:`~OS.SystemError` is typically thrown.
These errors are often produced by less predictable circumstances that are
more challenging to recover from. For example, a :record:`fileReader` could run
out of memory when attempting to allocate more buffer space.

As such, it is typically recommended that more specific errors are caught and
recovered from separately from a ``SystemError``. See the following example:

.. literalinclude:: ../../../../test/library/standard/IO/doc-examples/example_error_handling.chpl
 :language: chapel
 :start-after: START_EXAMPLE
 :end-before: STOP_EXAMPLE

.. _io-transactions:

I/O Transactions
----------------

An *I/O transaction* is a common pattern afforded by the IO interface that
provides the ability to temporarily hold a particular region of a file in a
:record:`fileReader` or :record:`fileWriter`'s buffer. This allows I/O
operations within that region of the file to easily be undone in the event
of some unexpected data or other errors.

To support *I/O transactions*, each ``fileReader`` and ``fileWriter`` is fitted
with a *mark stack* which contains a series of file offsets. The region of the
file between the minimum and maximum offset on the *mark stack* will always be
retained in the buffer.

The steps of a typical *I/O transaction* are as follows:

* ``mark`` the current file offset with :proc:`fileReader.mark` or
  :proc:`fileWriter.mark`. This pushes the current offset onto the *mark stack*

* do a speculative I/O operation:

    * reading example: read 200 bytes followed by a `b`.
    * writing example: write 200 bytes without exceeding the ``fileWriter``'s
      region.

* if the operation fails, ``revert`` the operation by calling :proc:`fileReader.revert`
  or :proc:`fileWriter.revert`. Subsequent operations will continue from the
  originally marked offset as if nothing happened.

* if the operation is successful, call :proc:`fileReader.commit` or
  :proc:`fileWriter.commit` to pop the value from the *mark stack* and continue
  performing I/O operations from the current offset.

Note that when the mark stack is emptied, a ``fileWriter`` is allowed to flush
any portion of its buffer to its file and a ``fileReader`` is allowed to discard
any portion of its buffer.

See the following example of a simple I/O transaction:

.. literalinclude:: ../../../../test/library/standard/IO/doc-examples/example_transactions.chpl
 :language: chapel
 :start-after: START_EXAMPLE
 :end-before: STOP_EXAMPLE

.. _filereader-filewriter-regions:

Specifying the region of a fileReader or fileWriter
---------------------------------------------------

The :record:`fileReader` and :record:`fileWriter` types can be configured to
own a specific *region* of their associated file.

When a ``fileReader`` or ``fileWriter`` is initialized using one of the
following routines, the optional ``region`` argument can be set to designate
some region of the file (a zero-based :ref:`range<Chapter-Ranges>` of integers
in bytes) that can be read from or written to:

* :proc:`file.reader`
* :proc:`file.writer`
* :proc:`openReader`

I/O operations that fall outside of the *region* are illegal. The ``region``
argument defaults to ``0..``, meaning that the owned region starts at the 0th
byte, and extends indefinitely.

Note that :proc:`fileReader.seek` and :proc:`fileWriter.seek` can be used to
adjust a ``fileReader`` or ``fileWriter``'s region after initialization.

Creating a ``fileReader`` or ``fileWriter`` that points to a sub-region of
a file can be useful for concurrently reading from or writing to multiple
portions of a file from separate tasks. See the following example, which
uses multiple tasks to concurrently read bytes from a binary file into an
array of bytes:

.. literalinclude:: ../../../../test/library/standard/IO/doc-examples/example_read_region.chpl
 :language: chapel
 :start-after: START_EXAMPLE
 :end-before: STOP_EXAMPLE

.. _locking-filereaders-and-filewriters:

Locking Behavior of FileReaders and FileWriters
-----------------------------------------------

The :record:`fileReader` and :record:`fileWriter` types can be configured to
lock access to their file when executing I/O operations to avoid race conditions
with other ``fileReader`` or ``fileWriter`` instances that may be accessing the
same file.

The ``locking`` field is a ``param`` and is thus part of the ``fileReader``
and ``fileWriter`` type. As such, it is possible to use type constraints to
designate whether a reader or writer is locking. For example this could be
useful in a procedure that relies on a ``reader`` argument being locking:

.. literalinclude:: ../../../../test/library/standard/IO/doc-examples/example_lock_filereader.chpl
 :language: chapel
 :start-after: START_EXAMPLE
 :end-before: STOP_EXAMPLE

The ``locking`` field can be set by passing the desired value to one of the
following routines that create a :record:`fileReader` or :record:`fileWriter`:

* :proc:`file.reader`
* :proc:`file.writer`
* :proc:`openReader`
* :proc:`openWriter`

With a locking ``fileReader`` or ``fileWriter``, one can obtain a lock manually
by calling :proc:`fileReader.lock` or :proc:`fileWriter.lock`, and then release
a lock by calling :proc:`fileReader.unlock` or :proc:`fileWriter.unlock`.

.. note::
  The following methods will not automatically acquire/release a lock for
  ``locking=true``:

  * :proc:`fileReader.mark`
  * :proc:`fileWriter.mark`
  * :proc:`fileReader.commit`
  * :proc:`fileWriter.commit`
  * :proc:`fileReader.revert`
  * :proc:`fileWriter.revert`
  * :proc:`fileReader.offset`
  * :proc:`fileWriter.offset`


.. _about-io-ensuring-successful-io:

Ensuring Successful I/O
-----------------------

It is possible - in some situations - for I/O to fail without returning an
error. In cases where a programmer wants to be sure that there was no error
writing the data to disk, it is important to call :proc:`file.fsync` to make
sure that data has arrived on disk without an error. Many errors can be
reported with a typical operation, but some errors can only be reported by the
system during :proc:`file.close` or even :proc:`file.fsync`.

When a file (or fileWriter) is closed, data written to that file will be written
to disk eventually by the operating system. If an application needs to be sure
that the data is immediately written to persistent storage, it should use
:proc:`file.fsync` prior to closing the file.

Correspondence with C I/O
-------------------------

It is not possible to seek, read, or write to a file directly; fileReaders
and/or fileWriters must be created and used.

:proc:`fileWriter.flush` in Chapel has the same conceptual meaning as
``fflush()`` in C.  However, ``fflush()`` is not necessarily called in
:proc:`fileWriter.flush()`, unlike ``fsync()``, which is actually called by
:proc:`file.fsync()` in Chapel.

Automatically Included IO Functions
-----------------------------------

.. include:: ChapelIO.rst
  :start-after: begin ChapelIO docs

IO Functions and Types
----------------------

 */
module IO {
/* "fileReader" and "fileWriter" I/O contributed by Michael Ferguson

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
      as a result, current code might need to close fileReaders, fileWriters,
      and files explicitly and/or sharing of fileReaders, fileWriters and files
      between variables might not work correctly.
    - FileReaders and fileWriters have a lock, and some methods on a fileReader
      or fileWriter only make sense when the lock is held. In the future (again,
      once RAII makes sense), we might replace this with 'get locked fileWriter'
      that returns a different type that has the methods that can only be used
      on a locked fileWriter, e.g..
    - The error handling strategy here is a bit awkward for the library-writer
      (not so much for the user). A broader exceptions strategy would help
      quite a bit - and some of these interfaces will change when that happens.
    - Fancy features, like adding a bytes or buffer object to a fileReader or
      fileWriter (so that the fileReader or fileWriter just refers to it and
      does not copy it) are implemented but not well tested.
    - Cleaning up to reduce the number of exported symbols, and using enums for
      all constants once 'use enum' is available and we have a way to get
      C constants into Chapel enums.
    - Support for libevent/qthreads system call thread so that we have
      something suitable for writing (in a straightforward manner) a
      multithreaded webserver
    - Doing something reasonable with a file that moves from one node to
      another - namely, when a fileReader or fileWriter is created with a file
      that resides remotely, if that same file is accessible locally (e.g. with
      Lustre or NFS), we should open a local copy of that file and use that in
      the fileReader or fileWriter. (not sure how to avoid opening # fileReader/
      fileWriter copies of these files
      -- seems that we'd want some way to cache that...).
*/

import OS.POSIX.{ENOENT, ENOSYS, EINVAL, EILSEQ, EIO, ERANGE};
import OS.POSIX.{EBADF};
import OS.{errorCode};
use CTypes;
public use OS;
private use Reflection;
public use ChapelIO only write, writeln, writef;
use ByteBufferHelpers;

/*
The :type:`ioMode` type is an enum. When used as arguments when opening files, its
constants have the same meaning as the listed strings passed to ``fopen()`` in C.
However, :proc:`open()` in Chapel does not necessarily invoke ``fopen()`` in C.
*/
enum ioMode {
  /*
    Open an existing file for reading.
    (``fopen()`` string is "r")
  */
  r = 1,
  /*
    Create a new file for writing.
    If the file already exists, its contents are truncated.
    (``fopen()`` string is "w")
  */
  cw = 2,
  /*
    Open an existing file for reading and writing.
    (``fopen()`` string is "r+")
  */
  rw = 3,
  /*
    Same as :enumconstant:`ioMode.cw`, but reading from the file is also allowed.
    (``fopen()`` string is "w+")
  */
  cwr = 4,
  /*
    Open a file for appending, creating it if it does not exist.
    (``fopen()`` string is "a")
  */
  @unstable(":enumconstant:`ioMode.a` is unstable and subject to change. It currently only supports one :record:`fileWriter` at a time.")
  a = 5,
  // same as ``ioMode.a``, but reading from the file is also allowed.
  // ar, "a+"
  // open a file for writing, throwing an error if it already exists. (The test for file's existence and the file's creation are atomic on POSIX.)
  // cwx, "wx"
  // same as ``ioMode.cwx``, but reading from the file is also allowed.
  // cwrx, w+x

}

@chpldoc.nodoc
enum _iokind {
  // don't change these without updating qio_style.h QIO_NATIVE, etc
  // a default of 0 is always reasonable, but you can avoid some
  // branches to get faster I/O by setting this to native, big, or little.
  // In that case, the style is only consulted for text or string I/O.
  dynamic = 0, // look in iostyleInternal
  native = 1,
  big = 2, // aka "network"
  little = 3
}

/*
The :type:`endianness` type is an enum. When used as an argument to the
:record:`fileReader` or :record:`fileWriter` methods, its constants have the
following meanings:
*/
enum endianness {
  /* ``native`` means binary I/O is performed in the byte order that is native
  to the target platform. */
  native = 0,
  /* ``big`` means binary I/O is performed in big-endian byte order.*/
  big = 1,
  /* ``little`` means binary I/O is performed in little-endian byte order. */
  little = 2
}

/* Internal version of iostringstyle for interim use */
@chpldoc.nodoc
enum iostringstyleInternal {
  len1b_data = -1,
  len2b_data = -2,
  len4b_data = -4,
  len8b_data = -8,
  lenVb_data = -10,
  data_toeof = -0xff00,
  data_null = -0x0100,
}

/* Internal version of iostringformat for interim use */
@chpldoc.nodoc
enum iostringformatInternal {
  word = 0,
  basic = 1,
  chpl = 2,
  json = 3,
  toend = 4,
  toeof = 5,
}

@chpldoc.nodoc
extern const QIO_FDFLAG_UNK:c_int;
@chpldoc.nodoc
extern const QIO_FDFLAG_READABLE:c_int;
@chpldoc.nodoc
extern const QIO_FDFLAG_WRITEABLE:c_int;
@chpldoc.nodoc
extern const QIO_FDFLAG_SEEKABLE:c_int;

@chpldoc.nodoc
extern const QIO_METHOD_DEFAULT:c_int;
@chpldoc.nodoc
extern const QIO_METHOD_READWRITE:c_int;
@chpldoc.nodoc
extern const QIO_METHOD_PREADPWRITE:c_int;
@chpldoc.nodoc
extern const QIO_METHOD_FREADFWRITE:c_int;
@chpldoc.nodoc
extern const QIO_METHOD_MMAP:c_int;
@chpldoc.nodoc
extern const QIO_METHODMASK:c_int;
@chpldoc.nodoc
extern const QIO_HINT_RANDOM:c_int;
@chpldoc.nodoc
extern const QIO_HINT_SEQUENTIAL:c_int;
@chpldoc.nodoc
extern const QIO_HINT_LATENCY:c_int;
@chpldoc.nodoc
extern const QIO_HINT_BANDWIDTH:c_int;
@chpldoc.nodoc
extern const QIO_HINT_CACHED:c_int;
@chpldoc.nodoc
extern const QIO_HINT_PARALLEL:c_int;
@chpldoc.nodoc
extern const QIO_HINT_DIRECT:c_int;
@chpldoc.nodoc
extern const QIO_HINT_NOREUSE:c_int;
@chpldoc.nodoc
extern const QIO_HINT_OWNED:c_int;

// can be left opaque, but we need the correct C type name
@chpldoc.nodoc
extern record qio_file_t {}
@chpldoc.nodoc
extern type qio_file_ptr_t = c_ptr(qio_file_t);
private extern const QIO_FILE_PTR_NULL:qio_file_ptr_t;


@chpldoc.nodoc
extern record qiovec_t {
  var iov_base: c_ptr(void);
  var iov_len: c_size_t;
}

// opaque like qio_file_t
@chpldoc.nodoc
extern record qio_channel_t {}
@chpldoc.nodoc
extern type qio_channel_ptr_t = c_ptr(qio_channel_t);
private extern const QIO_CHANNEL_PTR_NULL:qio_channel_ptr_t;

// also the type for a buffer for qio_file_open_mem.
@chpldoc.nodoc
extern type qbuffer_ptr_t;
@chpldoc.nodoc
extern const QBUFFER_PTR_NULL:qbuffer_ptr_t;

@chpldoc.nodoc
extern type style_char_t = uint(8);

@chpldoc.nodoc
extern const QIO_STRING_FORMAT_WORD:uint(8);
@chpldoc.nodoc
extern const QIO_STRING_FORMAT_BASIC:uint(8);
@chpldoc.nodoc
extern const QIO_STRING_FORMAT_CHPL:uint(8);
@chpldoc.nodoc
extern const QIO_STRING_FORMAT_JSON:uint(8);
@chpldoc.nodoc
extern const QIO_STRING_FORMAT_TOEND:uint(8);
@chpldoc.nodoc
extern const QIO_STRING_FORMAT_TOEOF:uint(8);

/*

The :record:`iostyleInternal` type represents I/O styles
defining how Chapel's basic types should be read or written.

It replaces the now removed `iostyle` type, and will eventually
be migrated into a new strategy, likely involving serializers/deserializers
*/
@chpldoc.nodoc
extern record iostyleInternal { // aka qio_style_t
  /* Perform binary I/O? 1 - yes, 0 - no.
     This field is ignored for :type:`_iokind` values other than ``dynamic``.
   */
  var binary:uint(8) = 0;
  // binary style choices
  /* What byte order should we use when performing binary I/O?
     This field is ignored for :type:`_iokind` values other than ``dynamic``.
     It should be set to a value in :type:`_iokind`.
   */
  var byteorder:uint(8) = _iokind.native:uint(8);

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
  var string_start:style_char_t = '"'.toByte();
  /* What character do we end strings with, when appropriate? Default is ``"``. */
  var string_end:style_char_t = '"'.toByte();

  /* How should we format strings when performing text I/O?
     See :type:`iostringstyleInternal` for more information
     on what the values of ``str_style`` mean.
   */
  var string_format:uint(8) = iostringformatInternal.word:uint(8);

  /* What character do we start bytes with, when appropriate? Default is ``"b``. */
  var bytes_prefix:style_char_t = "b".toByte();

  // numeric scanning/printing choices
  /* When reading or writing a numeric value in a text mode fileReader or
     fileWriter, what base should be used for the number? Default of 0 means
     decimal. Bases 2, 8, 10, 16 are supported for integers. Bases 10 and 16
     are supported for real values.*/
  var base:uint(8) = 0;
  /* When reading or writing a numeric value in a text mode fileReader or
     fileWriter, how is the integer portion separated from the fractional
     portion? Default is ``.``. */
  var point_char:style_char_t = ".".toByte();
  /* When reading or writing a numeric value in a text mode fileReader or
     fileWriter, how is the exponent written? Default is ``e``. */
  var exponent_char:style_char_t = "e".toByte();
  /* When reading or writing a numeric value in a text mode fileReader or
     fileWriter, when base is > 10, how is the exponent written? Default is
     ``p``. */
  var other_exponent_char:style_char_t = "p".toByte();
  /* What character denotes a positive number? Default is ``+``. */
  var positive_char:style_char_t = "+".toByte();
  /* What character denotes a negative number? Default is ``-``. */
  var negative_char:style_char_t = "-".toByte();
  /* What character follows an the imaginary number? Default is ``i``. */
  var i_char:style_char_t = "i".toByte();
  /* When writing in a base other than 10, should the prefix be used
     (e.g. hexadecimal numbers are prefixed with 0x)? */
  var prefix_base:uint(8) = 1;
  // numeric printing choices
  /* When padding with spaces, which pad character to use? Default is ' '. */
  var pad_char:style_char_t = " ".toByte();
  /* When printing a positive numeric value, should the ``+`` be shown? */
  var showplus:uint(8) = 0;
  /* When printing a numeric value in hexadecimal, should it be
     uppercase? */
  var uppercase:uint(8) = 0;
  /* When printing a numeric value in a field of specified width, should
     the number be left, right, or center justified. The default
     is to right-justify the number.

              | 'leftjustify' | 'centjustify' |
    - right   |  0            |  0            |
    - left    |  1            |  0            |
    - center  |  0            |  1            |
  */
  var leftjustify:uint(8) = 0;
  var centjustify:uint(8) = 0;
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

// This class helps in implementing runtime calls.
// It represents a file as a pointer. C code can call Chapel
// code working with this file through the export procs below,
// e.g. chpl_qio_read_atleast. These work by casting to
// this type and then invoking the method with virtual dispatch.

@chpldoc.nodoc
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
     The caller has the responsibility to free the returned c_ptr(uint(8)).
   */
  // TODO: use Chapel strings for this, one day
  // This is using c_ptr(uint(8)) to match the qio function definition for chpl_qio_getpath
  proc getpath(out path:c_ptr(uint(8)), out len:int(64)):errorCode {
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
      localeNames:c_ptr(c_ptrConst(c_char)), ref nLocales:int(64)):errorCode {
    return ENOSYS;
  }

  /* Close the file. */
  proc close():errorCode {
    return ENOSYS;
  }
}

// This class helps with C runtime I/O plugins. It represents additional
// information for a channel.
@chpldoc.nodoc
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
export proc chpl_qio_setup_plugin_channel(file:c_ptr(void), ref plugin_ch:c_ptr(void), start:int(64), end:int(64), qio_ch:qio_channel_ptr_t):errorCode {
  var f=(file:unmanaged QioPluginFile?)!;
  var pluginChannel:unmanaged QioPluginChannel? = nil;
  var ret = f.setupChannel(pluginChannel, start, end, qio_ch);
  plugin_ch = c_ptrTo(pluginChannel);
  return ret;
}

export proc chpl_qio_read_atleast(ch_plugin:c_ptr(void), amt:int(64)) {
  var c=(ch_plugin:unmanaged QioPluginChannel?)!;
  return c.readAtLeast(amt);
}
export proc chpl_qio_write(ch_plugin:c_ptr(void), amt:int(64)) {
  var c=(ch_plugin:unmanaged QioPluginChannel?)!;
  return c.write(amt);
}
export proc chpl_qio_channel_close(ch:c_ptr(void)):errorCode {
  var c=(ch:unmanaged QioPluginChannel?)!;
  var err = c.close();
  delete c;
  return err;
}

export proc chpl_qio_filelength(file:c_ptr(void), ref length:int(64)):errorCode {
  var f=(file:unmanaged QioPluginFile?)!;
  return f.filelength(length);
}
export proc chpl_qio_getpath(file:c_ptr(void), ref str:c_ptr(uint(8)), ref len:int(64)):errorCode {
  var f=(file:unmanaged QioPluginFile?)!;
  return f.getpath(str, len);
}
export proc chpl_qio_fsync(file:c_ptr(void)):errorCode {
  var f=(file:unmanaged QioPluginFile?)!;
  return f.fsync();
}
export proc chpl_qio_get_chunk(file:c_ptr(void), ref length:int(64)):errorCode {
  var f=(file:unmanaged QioPluginFile?)!;
  return f.getChunk(length);
}
export proc chpl_qio_get_locales_for_region(file:c_ptr(void), start:int(64),
    end:int(64), ref localeNames:c_ptr(void), ref nLocales:int(64)):errorCode {
  var strPtr:c_ptr(c_ptrConst(c_char));
  var f=(file:unmanaged QioPluginFile?)!;
  var ret = f.getLocalesForRegion(start, end, strPtr, nLocales);
  localeNames = strPtr:c_ptr(void);
  return ret;
}
export proc chpl_qio_file_close(file:c_ptr(void)):errorCode {
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

private extern proc qio_file_init(ref file_out:qio_file_ptr_t, fp: c_ptr(c_FILE), fd:c_int, iohints:c_int, const ref style:iostyleInternal, usefilestar:c_int):errorCode;
private extern proc qio_file_open_access(ref file_out:qio_file_ptr_t, path:c_ptrConst(c_char), access:c_ptrConst(c_char), iohints:c_int, const ref style:iostyleInternal):errorCode;
private extern proc qio_file_open_tmp(ref file_out:qio_file_ptr_t, iohints:c_int, const ref style:iostyleInternal):errorCode;
private extern proc qio_file_open_mem(ref file_out:qio_file_ptr_t, buf:qbuffer_ptr_t, const ref style:iostyleInternal):errorCode;

@chpldoc.nodoc
extern proc qio_file_close(f:qio_file_ptr_t):errorCode;


private extern proc qio_file_lock(f:qio_file_ptr_t):errorCode;
private extern proc qio_file_unlock(f:qio_file_ptr_t);
private extern proc qio_file_isopen(f:qio_file_ptr_t):bool;

/* The general way to make sure data is written without error */
private extern proc qio_file_sync(f:qio_file_ptr_t):errorCode;

private extern proc qio_channel_end_offset_unlocked(ch:qio_channel_ptr_t):int(64);
private extern proc qio_channel_start_offset_unlocked(ch:qio_channel_ptr_t):int(64);
private extern proc qio_file_get_style(f:qio_file_ptr_t, ref style:iostyleInternal);
private extern proc qio_file_get_plugin(f:qio_file_ptr_t):c_ptr(void);
private extern proc qio_channel_get_plugin(ch:qio_channel_ptr_t):c_ptr(void);
private extern proc qio_file_length(f:qio_file_ptr_t, ref len:int(64)):errorCode;
private extern proc qio_file_length_guess(f:qio_file_ptr_t):int(64);

private extern proc qio_channel_create(ref ch:qio_channel_ptr_t, file:qio_file_ptr_t, hints:c_int, readable:c_int, writeable:c_int, start:int(64), end:int(64), const ref style:iostyleInternal, bufIoMax:int(64)):errorCode;

private extern proc qio_channel_path_offset(threadsafe:c_int, ch:qio_channel_ptr_t, ref path:c_ptrConst(c_char), ref offset:int(64)):errorCode;

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
@chpldoc.nodoc
extern proc qio_channel_isclosed(threadsafe:c_int, ch:qio_channel_ptr_t):bool;

private extern proc qio_channel_read(threadsafe:c_int, ch:qio_channel_ptr_t, ref ptr, len:c_ssize_t, ref amt_read:c_ssize_t):errorCode;
private extern proc qio_channel_read_amt(threadsafe:c_int, ch:qio_channel_ptr_t, ref ptr, len:c_ssize_t):errorCode;
// A specialization is needed for _ddata as the value is the pointer its memory
private extern proc qio_channel_read_amt(threadsafe:c_int, ch:qio_channel_ptr_t, ptr:_ddata, len:c_ssize_t):errorCode;
// and for c_ptr
private extern proc qio_channel_read_amt(threadsafe:c_int, ch:qio_channel_ptr_t, ptr:c_ptr, len:c_ssize_t):errorCode;
private extern proc qio_channel_read_byte(threadsafe:c_int, ch:qio_channel_ptr_t):int(32);

private extern proc qio_channel_write(threadsafe:c_int, ch:qio_channel_ptr_t, const ref ptr, len:c_ssize_t, ref amt_written:c_ssize_t):errorCode;
private extern proc qio_channel_write_amt(threadsafe:c_int, ch:qio_channel_ptr_t, const ref ptr, len:c_ssize_t):errorCode;
// A specialization is needed for _ddata as the value is the pointer its memory
private extern proc qio_channel_write_amt(threadsafe:c_int, ch:qio_channel_ptr_t, const ptr:_ddata, len:c_ssize_t):errorCode;
private extern proc qio_channel_write_byte(threadsafe:c_int, ch:qio_channel_ptr_t, byte:uint(8)):errorCode;

private extern proc qio_channel_offset_unlocked(ch:qio_channel_ptr_t):int(64);
private extern proc qio_channel_advance(threadsafe:c_int, ch:qio_channel_ptr_t, nbytes:int(64)):errorCode;
private extern proc qio_channel_advance_past_byte(threadsafe:c_int, ch:qio_channel_ptr_t, byte:c_int, max_bytes_to_advance:int(64), consume:c_int):errorCode;

private extern proc qio_channel_mark(threadsafe:c_int, ch:qio_channel_ptr_t):errorCode;
private extern proc qio_channel_revert_unlocked(ch:qio_channel_ptr_t);
private extern proc qio_channel_commit_unlocked(ch:qio_channel_ptr_t);

private extern proc qio_channel_seek(ch:qio_channel_ptr_t, start:int(64), end:int(64)):errorCode;

private extern proc qio_channel_write_bits(threadsafe:c_int, ch:qio_channel_ptr_t, v:uint(64), nbits:int(8)):errorCode;
private extern proc qio_channel_flush_bits(threadsafe:c_int, ch:qio_channel_ptr_t):errorCode;
private extern proc qio_channel_read_bits(threadsafe:c_int, ch:qio_channel_ptr_t, ref v:uint(64), nbits:int(8)):errorCode;

private extern proc qio_locales_for_region(fl:qio_file_ptr_t,
                                   start:int(64), end:int(64),
                                   loc_names_out:c_ptr(void),
                                   ref num_locs_out:c_int):errorCode;
private extern proc qio_get_chunk(fl:qio_file_ptr_t, ref len:int(64)):errorCode;
private extern proc qio_get_fs_type(fl:qio_file_ptr_t, ref tp:c_int):errorCode;

private extern proc qio_file_path_for_fd(fd:c_int, ref path:c_ptrConst(c_char)):errorCode;
private extern proc qio_file_path_for_fp(fp: c_ptr(c_FILE), ref path:c_ptrConst(c_char)):errorCode;
private extern proc qio_file_path(f:qio_file_ptr_t, ref path:c_ptrConst(c_char)):errorCode;
private extern proc qio_shortest_path(fl: qio_file_ptr_t, ref path_out:c_ptrConst(c_char), path_in:c_ptrConst(c_char)):errorCode;

private extern proc qio_get_fd(fl:qio_file_ptr_t, ref fd:int(32)):errorCode;
private extern proc qio_get_fp(fl:qio_file_ptr_t, ref fp:c_ptr(c_FILE)):errorCode;
private extern proc qio_channel_get_file_ptr(ch:qio_channel_ptr_t, ref file_out: qio_file_ptr_t):void;

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

private extern proc qio_channel_read_string(threadsafe:c_int, byteorder:c_int, str_style:int(64), ch:qio_channel_ptr_t, ref s:c_ptrConst(c_char), ref len:int(64), maxlen:c_ssize_t):errorCode;
private extern proc qio_channel_write_string(threadsafe:c_int, byteorder:c_int, str_style:int(64), ch:qio_channel_ptr_t, const s:c_ptrConst(c_char), len:c_ssize_t):errorCode;

private extern proc qio_channel_scan_int(threadsafe:c_int, ch:qio_channel_ptr_t, ref ptr, len:c_size_t, issigned:c_int):errorCode;
private extern proc qio_channel_print_int(threadsafe:c_int, ch:qio_channel_ptr_t, const ref ptr, len:c_size_t, issigned:c_int):errorCode;

private extern proc qio_channel_scan_float(threadsafe:c_int, ch:qio_channel_ptr_t, ref ptr, len:c_size_t):errorCode;
private extern proc qio_channel_print_float(threadsafe:c_int, ch:qio_channel_ptr_t, const ref ptr, len:c_size_t):errorCode;

// These are the same as scan/print float but they assume an 'i' afterwards.
private extern proc qio_channel_scan_imag(threadsafe:c_int, ch:qio_channel_ptr_t, ref ptr, len:c_size_t):errorCode;
private extern proc qio_channel_print_imag(threadsafe:c_int, ch:qio_channel_ptr_t, const ref ptr, len:c_size_t):errorCode;


private extern proc qio_channel_scan_complex(threadsafe:c_int, ch:qio_channel_ptr_t, ref re_ptr, ref im_ptr, len:c_size_t):errorCode;
private extern proc qio_channel_print_complex(threadsafe:c_int, ch:qio_channel_ptr_t, const ref re_ptr, const ref im_ptr, len:c_size_t, full_nan: c_int):errorCode;


private extern proc qio_channel_read_char(threadsafe:c_int, ch:qio_channel_ptr_t, ref char:int(32)):errorCode;
private extern proc qio_channel_read_chars(threadsafe:c_int, ch:qio_channel_ptr_t, ref ptr, maxBytes:c_ssize_t, maxCodepoints:c_ssize_t, ref readBytes: c_ssize_t, ref readCodepoints: c_ssize_t):errorCode;

private extern proc qio_nbytes_char(chr:int(32)):c_int;
private extern proc qio_encode_to_string(chr:int(32)):c_ptrConst(c_char);
private extern proc qio_decode_char_buf(ref chr:int(32), ref nbytes:c_int, buf:c_ptrConst(c_char), buflen:c_ssize_t):errorCode;
private extern proc qio_encode_char_buf(dst: c_ptr(void), chr: int(32)): errorCode;
private extern proc chpl_enc_utf8_decode(ref state: uint(32), ref codep:uint(32), byte: uint(32)): uint(32);

private extern proc qio_channel_write_char(threadsafe:c_int, ch:qio_channel_ptr_t, char:int(32)):errorCode;
private extern proc qio_channel_skip_past_newline(threadsafe:c_int, ch:qio_channel_ptr_t, skipOnlyWs:c_int):errorCode;
private extern proc qio_channel_write_newline(threadsafe:c_int, ch:qio_channel_ptr_t):errorCode;

private extern proc qio_channel_scan_string(threadsafe:c_int, ch:qio_channel_ptr_t, ref ptr:c_ptrConst(c_char), ref len:int(64), maxlen:c_ssize_t):errorCode;
private extern proc qio_channel_scan_bytes(threadsafe:c_int, ch:qio_channel_ptr_t, ref ptr:c_ptrConst(c_char), ref len:int(64), maxlen:c_ssize_t):errorCode;
private extern proc qio_channel_print_bytes(threadsafe:c_int, ch:qio_channel_ptr_t, const ptr:c_ptrConst(c_char), len:c_ssize_t):errorCode;
private extern proc qio_channel_print_string(threadsafe:c_int, ch:qio_channel_ptr_t, const ptr:c_ptrConst(c_char), len:c_ssize_t):errorCode;

private extern proc qio_channel_scan_literal(threadsafe:c_int, ch:qio_channel_ptr_t, const match:c_ptrConst(c_char), len:c_ssize_t, skipwsbefore:c_int):errorCode;
private extern proc qio_channel_scan_literal_2(threadsafe:c_int, ch:qio_channel_ptr_t, match:c_ptr(void), len:c_ssize_t, skipwsbefore:c_int):errorCode;
private extern proc qio_channel_print_literal(threadsafe:c_int, ch:qio_channel_ptr_t, const match:c_ptrConst(c_char), len:c_ssize_t):errorCode;
private extern proc qio_channel_print_literal_2(threadsafe:c_int, ch:qio_channel_ptr_t, match:c_ptr(void), len:c_ssize_t):errorCode;

private extern proc qio_channel_skip_json_field(threadsafe:c_int, ch:qio_channel_ptr_t):errorCode;

@chpldoc.nodoc
extern record qio_conv_t {
  var preArg1:uint(8);
  var preArg2:uint(8);
  var preArg3:uint(8);
  var argType:uint(8);
  var literal_is_whitespace:uint(8);
  var literal_length:uint(32);
  var literal:c_ptr(void);
  var regex_length:uint(32);
  var regex:c_ptr(void);
  var regex_flags_length:uint(32);
  var regex_flags:c_ptr(void);
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
private extern const QIO_CONV_ARG_TYPE_SERDE:c_int;
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
private extern proc qio_conv_parse(const fmt:c_ptrConst(c_char), start:c_size_t, ref end:uint(64), scanning:c_int, ref spec:qio_conv_t, ref style:iostyleInternal):errorCode;

private extern proc qio_format_error_too_many_args():errorCode;
private extern proc qio_format_error_too_few_args():errorCode;
private extern proc qio_format_error_arg_mismatch(arg:int):errorCode;
@chpldoc.nodoc
extern proc qio_format_error_bad_regex():errorCode;
private extern proc qio_format_error_write_regex():errorCode;

@chpldoc.nodoc
proc defaultIOStyleInternal(): iostyleInternal {
  var ret:iostyleInternal;
  qio_style_init_default(ret);
  return ret;
}

/* fdflag_t specifies how a file can be used. It can be:
  QIO_FDFLAG_UNK,
  QIO_FDFLAG_READABLE,
  QIO_FDFLAG_WRITEABLE,
  QIO_FDFLAG_SEEKABLE
*/
@chpldoc.nodoc
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

/* A value of the :record:`ioHintSet` type defines a set of hints to provide
  information about the operations that a :record:`file`, :record:`fileReader`
  or :record:`fileWriter` will perform. These hints may be used by the
  implementation to select optimized versions of the I/O operations.

  Most hints have POSIX equivalents associated with posix_fadvise() and
  posix_madvise().

  This example depicts how an ``ioHintSet`` might be used.

  .. literalinclude:: ../../../../test/library/standard/IO/doc-examples/example_ioHintSet.chpl
   :language: chapel
   :start-after: START_EXAMPLE
   :end-before: STOP_EXAMPLE
*/
record ioHintSet {
  @chpldoc.nodoc
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

  /*
    Suggests whether or not 'mmap' should be used to access the file contents.

     * when ``useMmap`` is ``true``, suggests that 'mmap' should be used
     * when ``useMmap`` is ``false``, suggests that 'mmap' should not be used and 'pread'/'pwrite' should be used instead

  */
  proc type mmap(useMmap = true) {
    return if useMmap
      then new ioHintSet(IOHINTS_MMAP)
      else new ioHintSet(IOHINTS_NOMMAP);
  }

  @chpldoc.nodoc
  proc type fromFlag(flag: c_int) { return new ioHintSet(flag); }
}

/* Compute the union of two hint sets
*/
operator ioHintSet.|(lhs: ioHintSet, rhs: ioHintSet) {
  return new ioHintSet(lhs._internal | rhs._internal);
}

/* Compute the intersection of two hint sets
*/
operator ioHintSet.&(lhs: ioHintSet, rhs: ioHintSet) {
  return new ioHintSet(lhs._internal & rhs._internal);
}

/* Compare two hint sets for equality
*/
operator ioHintSet.==(lhs: ioHintSet, rhs: ioHintSet) {
  return lhs._internal == rhs._internal;
}

/* Compare two hint sets for inequality
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
  @chpldoc.nodoc
  var _home: locale = here;
  @chpldoc.nodoc
  var _file_internal:qio_file_ptr_t = QIO_FILE_PTR_NULL;

  // INIT TODO: This would be a useful case for requesting a default initializer
  // be built even when handwritten initializers (copy init) exist.
  @chpldoc.nodoc
  proc init() {
  }
}

// TODO -- shouldn't have to write this this way!
@chpldoc.nodoc
proc file.init(x: file) {
  this._home = x._home;
  this._file_internal = x._file_internal;
  init this;
  on this._home {
    qio_file_retain(_file_internal);
  }
}

@chpldoc.nodoc
proc file.init=(x: file) {
  this.init(x);
}

@chpldoc.nodoc
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

/*
Create a Chapel :record:`file` that wraps around an open C file. A pointer to
a C ``FILE`` object can be obtained via Chapel's
:ref:`C Interoperability <primers-C-interop-using-C>` functionality.

.. note::

  This is an alternative way to create a :record:`file`.  The main way to do so
  is via the :proc:`open` function.

Once the Chapel file is created, you will need to use :proc:`file.reader` to
create a fileReader or :proc:`file.writer` to create a fileWriter to perform I/O
operations on the C file.

.. note::

  The resulting file value should only be used with one :record:`fileReader` or
  :record:`fileWriter` at a time. The I/O system will ignore the offsets when
  reading or writing to a file opened using this initializer.


:arg fp: a pointer to a C ``FILE``. See :type:`~CTypes.c_FILE`.
:arg hints: optional argument to specify any hints to the I/O system about
            this file. See :record:`ioHintSet`.
:arg own: set to indicate if the :type:`~CTypes.c_FILE` provided should be
          cleaned up when the ``file`` is closed.  Defaults to ``false``

:throws SystemError: If the C file could not be retrieved.
*/
proc file.init(fp: c_ptr(c_FILE), hints=ioHintSet.empty, own=false) throws {
  this.init();

  initHelper(this, fp, hints, own=own);
}

private proc initHelper(ref f: file, fp: c_ptr(c_FILE), hints=ioHintSet.empty,
                        own=false) throws {

  var local_style = defaultIOStyleInternal();
  f._home = here;
  var internalHints = hints._internal;
  if (own) {
    internalHints |= QIO_HINT_OWNED;
  }
  var err = qio_file_init(f._file_internal, fp, -1, internalHints, local_style,
                          1);

  // On exit either f._file_internal.ref_cnt == 1, or f._file_internal is NULL.
  // error should be nonzero in the latter case.
  if err {
    var path_cs:c_ptrConst(c_char);
    var path_err = qio_file_path_for_fp(fp, path_cs);
    var path = if path_err then "unknown"
                           else string.createCopyingBuffer(path_cs,
                                                          policy=decodePolicy.replace);
    deallocate(path_cs);
    try ioerror(err, "in init", path);
  }
}

/*

Create a Chapel file that works with a system file descriptor.  Note that once
the file is open, you will need to use a :proc:`file.reader` to create a
fileReader or :proc:`file.writer` to create a fileWriter to actually perform I/O
operations

.. note::

  This is an alternative way to create a :record:`file`.  The main way to do so
  is via the :proc:`open` function.

.. note::

  This function can be used to create Chapel files that refer to system file
  descriptors that do not support the ``seek`` functionality. For example, file
  descriptors that represent pipes or open socket connections have this
  property. In that case, the resulting file value should only be used with one
  :record:`fileReader` or :record:`fileWriter` at a time.  The I/O system will
  ignore the fileReader offsets when reading (or the fileWriter offsets when
  writing) to files backed by non-seekable file descriptors.


:arg fileDescriptor: a system file descriptor.
:arg hints: optional argument to specify any hints to the I/O system about
            this file. See :record:`ioHintSet`.
:arg own: set to indicate if the `fileDescriptor` provided should be cleaned up
          when the ``file`` is closed.  Defaults to ``false``

:throws SystemError: If the file descriptor could not be retrieved.
*/
proc file.init(fileDescriptor: int, hints=ioHintSet.empty, own=false) throws {
  this.init();

  initHelper2(this, fileDescriptor.safeCast(c_int), hints, own=own);
}

private proc initHelper2(ref f: file, fd: c_int, hints = ioHintSet.empty,
                         own=false) throws {

  var local_style = defaultIOStyleInternal();
  f._home = here;
  extern proc chpl_cnullfile():c_ptr(c_FILE);
  var internalHints = hints._internal;
  if (own) {
    internalHints |= QIO_HINT_OWNED;
  }
  var err = qio_file_init(f._file_internal, chpl_cnullfile(), fd, internalHints,
                          local_style, 0);

  // On return, either f._file_internal.ref_cnt == 1, or f._file_internal is
  // NULL.
  // err should be nonzero in the latter case.
  if err {
    var path_cs:c_ptrConst(c_char);
    var path_err = qio_file_path_for_fd(fd, path_cs);
    var path = if path_err then "unknown"
                           else string.createCopyingBuffer(path_cs,
                                                          policy=decodePolicy.replace);
    try ioerror(err, "in file.init", path);
  }
}

@chpldoc.nodoc
proc file.checkAssumingLocal() throws {
  if _file_internal == nil then
    throw createSystemError(EBADF, "Operation attempted on an invalid file");
  if !qio_file_isopen(_file_internal) then
    throw createSystemError(EBADF, "Operation attempted on closed file");
}

/* Indicates if the file is currently open.  Will return ``false`` for both
   closed and invalid files
*/
proc file.isOpen(): bool {
  if (_file_internal == nil) {
    return false;
  } else {
    return qio_file_isopen(_file_internal);
  }
}

@chpldoc.nodoc
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

@chpldoc.nodoc
proc file.filePlugin() : borrowed QioPluginFile? {
  var vptr = qio_file_get_plugin(this._file_internal);
  return vptr:borrowed QioPluginFile?;
}

// File style cannot be modified after the file is created;
// this prevents race conditions;
// fileReader or fileWriter style is protected by fileReader or fileWriter lock,
// can be modified.
@chpldoc.nodoc
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

   Files are automatically closed when the file variable goes out of scope and
   all fileReaders and fileWriters using that file are closed. Programs may also
   explicitly close a file using this method.

   It is an error to perform any I/O operations on a file that has been closed.
   It is an error to close a file when it has fileReaders and/or fileWriters
   that have not been closed.

   :throws SystemError: If the file could not be closed.
 */
proc file.close() throws {
  if _file_internal == nil then
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
Data written to the file by a fileWriter will only be guaranteed
committed if the fileWriter has been closed or flushed.

This function will typically call the ``fsync`` system call.

:throws SystemError: If the file could not be synced.
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

Get the absolute path to an open file.

Note that not all files have a path (e.g. files opened with :proc:`openMemFile`),
and that this procedure may not work on all operating systems.

The function :proc:`Path.realPath` is an alternative way
to get the path to a file.

:returns: the absolute path to the file
:rtype: ``string``

:throws SystemError: If the path could not be retrieved.
 */
proc file.path : string throws {
  var ret: string;
  var err:errorCode = 0;
  on this._home {
    try this.checkAssumingLocal();
    var tmp:c_ptrConst(c_char);
    err = qio_file_path(_file_internal, tmp);
    if !err {
      ret = string.createCopyingBuffer(tmp,
                                      policy=decodePolicy.escape);
    }
    deallocate(tmp);
  }
  if err then try ioerror(err, "in file.path");
  return ret;
}

// internal version of 'file.path' used to generate error messages in other IO methods
// produces a relative path when available
@chpldoc.nodoc
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
    var tmp:c_ptrConst(c_char);
    var tmp2:c_ptrConst(c_char);
    err = qio_file_path(f._file_internal, tmp);
    if !err {
      err = qio_shortest_path(f._file_internal, tmp2, tmp);
    }
    deallocate(tmp);
    if !err {
      ret = string.createCopyingBuffer(tmp2,
                                      policy=decodePolicy.escape);
    }
    deallocate(tmp2);
  }
  if err then try ioerror(err, "in file.path");
  return ret;
}

/*

Get the current size of an open file. Note that the size can always
change if other fileWriters, tasks or programs are writing to the file.

:returns: the current file size

:throws SystemError: If the size could not be retrieved.
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
private param _a = "a";

@chpldoc.nodoc
proc _modestring(mode:ioMode) {
  import HaltWrappers;
  select mode {
    when ioMode.r do return _r;
    when ioMode.rw do return _rw;
    when ioMode.cw do return _cw;
    when ioMode.cwr do return _cwr;
    when ioMode.a do return _a;
    otherwise do HaltWrappers.exhaustiveSelectHalt("Invalid ioMode");
  }
}

/*

Open a file on a filesystem. Note that once the file is open, you will need to
use a :proc:`file.reader` to create a fileReader or :proc:`file.writer` to
create a fileWriter to actually perform I/O operations

:arg path: which file to open (for example, "some/file.txt").
:arg mode: specify whether to open the file for reading or writing and
             whether or not to create the file if it doesn't exist.
             See :type:`ioMode`.
:arg hints: optional argument to specify any hints to the I/O system about
            this file. See :record:`ioHintSet`.
:returns: an open file to the requested resource.

:throws FileNotFoundError: If part of the provided path did not exist
:throws PermissionError: If part of the provided path had inappropriate
                         permissions
:throws NotADirectoryError: If part of the provided path was expected to
                            be a directory but was not
:throws SystemError: If the file could not be opened.
*/
proc open(path:string, mode:ioMode, hints=ioHintSet.empty): file throws {
  return openHelper(path, mode, hints);
}


private proc openHelper(path:string, mode:ioMode, hints=ioHintSet.empty): file throws {

  var local_style = defaultIOStyleInternal();
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

@chpldoc.nodoc
proc openplugin(pluginFile: QioPluginFile, mode:ioMode,
                seekable:bool, style:iostyleInternal) throws {
  import HaltWrappers;

  extern proc qio_file_init_plugin(ref file_out:qio_file_ptr_t,
                                   file_info:c_ptr(void), flags:c_int,
                                   const ref style:iostyleInternal):errorCode;

  var local_style = style;
  var ret:file;
  ret._home = here;

  var flags:c_int = 0;
  select mode {
    when ioMode.r {
      flags |= QIO_FDFLAG_READABLE;
    }
    when ioMode.rw {
      flags |= QIO_FDFLAG_READABLE;
      flags |= QIO_FDFLAG_WRITEABLE;
    }
    when ioMode.cw {
      flags |= QIO_FDFLAG_WRITEABLE;
    }
    when ioMode.cwr {
      flags |= QIO_FDFLAG_READABLE;
      flags |= QIO_FDFLAG_WRITEABLE;
    }
    otherwise do HaltWrappers.exhaustiveSelectHalt("Invalid ioMode");
  }

  if seekable then
    flags |= QIO_FDFLAG_SEEKABLE;

  var err = qio_file_init_plugin(ret._file_internal,
      c_ptrToConst(pluginFile), flags, style);
  if err {
    var path:string = "unknown";
    if pluginFile {
      var str:c_ptr(uint(8)) = nil;
      var len:int;
      var path_err = pluginFile.getpath(str, len);
      if path_err {
        path = "unknown";
      } else {
        // doesn't throw with decodePolicy.replace
        path = string.createCopyingBuffer(str, len,
                                         policy=decodePolicy.replace);
      }
    }

    try ioerror(err, "in openplugin", path);
  }

  return ret;
}

/*

Open a temporary file. Note that once the file is open, you will need to use a
:proc:`file.reader` to create a fileReader or :proc:`file.writer` to create a
fileWriter to actually perform I/O operations.

The temporary file will be created in an OS-dependent temporary directory,
for example "/tmp" is the typical location. The temporary file will be
deleted upon closing.

Temporary files are opened with :type:`ioMode` ``ioMode.cwr``; that is, a new
file is created that supports both writing and reading.  When possible, it may
be opened using OS support for temporary files in order to make sure that a new
file is created only for use by the current application.

:arg hints: optional argument to specify any hints to the I/O system about
            this file. See :record:`ioHintSet`.
:returns: an open temporary file.

:throws SystemError: If the temporary file could not be opened.
*/
proc openTempFile(hints=ioHintSet.empty):file throws {
  return opentmpHelper(hints);
}

private proc opentmpHelper(hints=ioHintSet.empty):file throws {
  var local_style = defaultIOStyleInternal();
  var ret:file;
  ret._home = here;

  // On return ret._file_internal.ref_cnt == 1.
  var err = qio_file_open_tmp(ret._file_internal, hints._internal, local_style);
  if err then try ioerror(err, "in opentmp");
  return ret;
}

/*

Open a file that is backed by a buffer in memory that will not persist when the
file is closed.  Note that once the file is open, you will need to use a
:proc:`file.reader` to create a fileReader or :proc:`file.writer` to create a
fileWriter to actually perform I/O operations.

The resulting file supports both reading and writing.

:returns: an open memory file.

:throws SystemError: If the memory buffered file could not be opened.
*/
proc openMemFile():file throws {
  return openMemFileHelper();
}

private proc openMemFileHelper():file throws {
  var local_style = defaultIOStyleInternal();
  var ret:file;
  ret._home = here;

  // On return ret._file_internal.ref_cnt == 1.
  var err = qio_file_open_mem(ret._file_internal, QBUFFER_PTR_NULL, local_style);
  if err then try ioerror(err, "in openMemFile");
  return ret;
}

// temporary config documented elsewhere
@chpldoc.nodoc
config param useIOSerializers = true;

private proc defaultSerializeType(param writing : bool) type {
  if !useIOSerializers then return nothing;

  if writing then return defaultSerializer;
  else return defaultDeserializer;
}

@chpldoc.nodoc
proc isDefaultSerializerType(type t) param : bool {
  import PrecisionSerializer;
  // the precisionSerializer should receive the same special-casing as the
  // defaultSerializer throughout the standard modules (e.g., dsiSerialWrite for assoc. domains)
  return t == defaultSerializer || t == PrecisionSerializer.precisionSerializer;
}

private proc defaultSerializeVal(param writing : bool) {
  if !useIOSerializers then return none;

  if writing then return new defaultSerializer();
  else return new defaultDeserializer();
}

@chpldoc.nodoc
class _serializeWrapper : writeSerializable {
  type T;
  var member: T;
  // TODO: Needed to avoid a weird memory error in the following test in
  // no-local configurations:
  // - library/draft/DistributedMap/v2/use-distributed-map
  //
  // Cause of bug likely involves the case when 'T' is nothing, and this
  // class has a zero-size allocation.
  var __dummy: int;

  override proc serialize(writer, ref serializer) throws {
  }
}

// Get the internal file, bump its reference count, and wrap it in a 'file'.
private inline
proc chpl_fileFromReaderOrWriter(readerOrWriter): file {
  var fp: qio_file_ptr_t;
  qio_channel_get_file_ptr(readerOrWriter._channel_internal, fp);
  qio_file_retain(fp);
  var ret: file;
  ret._home = readerOrWriter._home;
  ret._file_internal = fp;
  return ret;
}

/*

A ``fileReader`` supports sequential reading from an underlying :record:`file`
object. It can buffer data. Read operations on it might return old data.

The :record:`fileReader` type is implementation-defined.
A value of the :record:`fileReader` type refers to the state that is used
to implement the reading operations.

When a :record:`fileReader` formal argument has default intent, the actual is
passed by ``const ref`` to the formal upon a function call, and the formal
cannot be assigned within the function.

The default value of the :record:`fileReader` type is not associated
with any file, and so cannot be used to perform I/O.

The :record:`fileReader` type is generic.
*/
pragma "ignore noinit"
record fileReader {
  /*
     locking is a boolean indicating whether it is safe to use this
     fileReader concurrently (when `true`).
   */
  param locking:bool;

  /*
     deserializerType indicates the type of the deserializer that this
     fileReader will use to deserialize data.
   */
  type deserializerType = defaultSerializeType(/* writing= */ false);

  @chpldoc.nodoc
  var _home:locale = here;
  @chpldoc.nodoc
  var _channel_internal:qio_channel_ptr_t = QIO_CHANNEL_PTR_NULL;

  @chpldoc.nodoc
  var _deserializer : shared _serializeWrapper(deserializerType);

  // The member variable _readWriteThisFromLocale is used to support
  // writeThis needing to know where the I/O started. It is a member
  // variable on fileReader so that calls to writeln etc within writeThis
  // can preserve this information. Not used outside of
  // calling writeThis/readThis. If _readWriteThisFromLocale != nil, then
  // we are working on a fileReader created for running writeThis/readThis.
  // Therefore further locking by the same task is not necessary.
  @chpldoc.nodoc
  var _readWriteThisFromLocale = nilLocale;
}

/*
  Get the :record:`file` type underlying a :record:`fileReader`.
*/
@unstable("The 'fileReader.getFile()' method may change based on feedback")
proc fileReader.getFile() do return chpl_fileFromReaderOrWriter(this);

@chpldoc.nodoc
proc fileReader._writing param: bool do return false;

/*
  Return a mutable reference to this fileReader's deserializer.
*/
proc fileReader.deserializer ref : deserializerType {
  return _deserializer!.member;
}

/*

A ``fileWriter`` supports sequential writing to an underlying :record:`file`
object. A ``fileWriter`` can buffer data. Write operations might not have an
immediate effect. Use :proc:`fileWriter.flush` to control this buffering.

The :record:`fileWriter` type is implementation-defined.
A value of the :record:`fileWriter` type refers to the state that is used
to implement the writing operations.

When a :record:`fileWriter` formal argument has default intent, the actual is
passed by ``const ref`` to the formal upon a function call, and the formal
cannot be assigned within the function.

The default value of the :record:`fileWriter` type is not associated with any
file, and so cannot be used to perform I/O.

The :record:`fileWriter` type is generic.
*/
pragma "ignore noinit"
record fileWriter {
  /*
     locking is a boolean indicating whether it is safe to use this
     fileWriter concurrently (when `true`).
   */
  param locking:bool;

  /*
     serializerType indicates the type of the serializer that this fileWriter
     will use to serialize data.
   */
  type serializerType = defaultSerializeType(/* writing */ true);

  @chpldoc.nodoc
  var _home:locale = here;
  @chpldoc.nodoc
  var _channel_internal:qio_channel_ptr_t = QIO_CHANNEL_PTR_NULL;

  @chpldoc.nodoc
  var _serializer : shared _serializeWrapper(serializerType);

  // The member variable _readWriteThisFromLocale is used to support
  // writeThis needing to know where the I/O started. It is a member
  // variable on fileWriter so that calls to writeln etc within writeThis
  // can preserve this information. Not used outside of
  // calling writeThis/readThis. If _readWriteThisFromLocale != nil, then
  // we are working on a fileWriter created for running writeThis/readThis.
  // Therefore further locking by the same task is not necessary.
  @chpldoc.nodoc
  var _readWriteThisFromLocale = nilLocale;
}

/*
  Get the :record:`file` type underlying a :record:`fileWriter`.
*/
@unstable("The 'fileWriter.getFile()' method may change based on feedback")
proc fileWriter.getFile() do return chpl_fileFromReaderOrWriter(this);

@chpldoc.nodoc
proc fileWriter._writing param: bool do return true;

/*
  Return a mutable reference to this fileWriter's serializer.
*/
proc fileWriter.serializer ref : serializerType {
  return _serializer!.member;
}

/*
  The default Serializer used by ``fileWriter``.

  See :ref:`the serializers technote<ioSerializers>` for a general overview
  of Serializers and their usage.

  Otherwise, please refer to the individual methods in this type for a
  description of the default IO format.
*/
record defaultSerializer {

  @chpldoc.nodoc
  proc ref _serializeClassOrPtr(writer:fileWriter, x: ?t) : void throws {
    if x == nil {
      writer.writeLiteral("nil");
    } else if isClassType(t) {
      x!.serialize(writer=writer, serializer=this);
    } else if isProcedureType(t) {
      // The legacy procedure classes should have been handled by above.
      compilerAssert(!isClassType(t));

      // TODO: We might like to print the symbol name instead, here, and
      // that may be achievable since the names of loaded symbols are
      // stored in the binary cache (though they won't necessarily be
      // retrievable without comm). So we'd just have to arrange to store
      // the names of 'ftable' entries as well.
      const ptr = x : c_ptr(void);
      ptr.serialize(writer=writer, serializer=this);
    } else {
      x.serialize(writer=writer, serializer=this);
    }
  }

  /*
    Serialize ``val`` with ``writer``.

    Numeric values are serialized as though they were written with the format
    as ``%i`` for integers and ``%r`` for ``real`` numbers. Complex numbers are
    serialized as ``%z``. Please refer to :ref:`the section on Formatted
    IO<about-io-formatted-io>` for more information.

    Booleans are serialized as the literal strings ``true`` or ``false``.

    ``string`` values are serialized using the same format as ``%s`` — that is,
    literally and without quotes. ``bytes`` values are also serialized
    literally without extra formatting.

    Enums are serialized using the name of the corresponding value. For example
    with an enum like ``enum colors {red, green blue}``, the value ``red``
    would simply be serialized as ``red``.

    The ``nil`` value and nilable class variables storing ``nil`` will be
    serialized as the text ``nil``.

    Classes and records will have their ``serialize`` method invoked, passing
    in ``writer`` and this Serializer as arguments. Please see the
    :ref:`serializers technote<ioSerializers>` for more information.

    Classes and records are expected to implement the ``writeSerializable``
    or ``serializable`` interface.

    :arg writer: The ``fileWriter`` used to write serialized output.
    :arg val: The value to be serialized.
  */
  proc ref serializeValue(writer: fileWriter, const val: ?t) : void throws {
    if isNumericType(t) || isBoolType(t) || isEnumType(t) ||
       t == string || t == bytes {
      writer._writeOne(_iokind.dynamic, val, writer.getLocaleOfIoRequest());
    } else if t == _nilType {
      writer.writeLiteral("nil");
    } else if isClassType(t) || isAnyCPtr(t) || chpl_isDdata(t) ||
              isProcedureType(t) {
      _serializeClassOrPtr(writer, val);
    } else {
      val.serialize(writer=writer, serializer=this);
    }
  }

  /*
    Start serializing a class by writing the character ``{``.

    :arg writer: The ``fileWriter`` to be used when serializing.
    :arg name: The name of the class type.
    :arg size: The number of fields in the class.

    :returns: A new :record:`~IO.defaultSerializer.AggregateSerializer`
  */
  proc startClass(writer: fileWriter, name: string, size: int) throws {
    writer.writeLiteral("{");
    return new AggregateSerializer(writer, _ending="}");
  }

  /*
    Start serializing a record by writing the character ``(``.

    :arg writer: The ``fileWriter`` to be used when serializing.
    :arg name: The name of the record type.
    :arg size: The number of fields in the record.

    :returns: A new :type:`AggregateSerializer`
  */
  proc startRecord(writer: fileWriter, name: string, size: int) throws {
    writer.writeLiteral("(");
    return new AggregateSerializer(writer, _ending=")");
  }

  /*
    Returned by ``startClass`` or ``startRecord`` to provide the API for
    serializing classes or records.

    A ``class`` with integer fields 'x' and 'y' with values '0' and '5' would
    be serialized as:

    .. code-block:: text

      {x = 0, y = 5}

    A ``record`` with matching fields would be serialized in the same way, but
    would use ``(`` and ``)`` instead of ``{`` and ``}``.
  */
  record AggregateSerializer {
    @chpldoc.nodoc
    var writer;
    @chpldoc.nodoc
    var _parent = false;
    @chpldoc.nodoc
    var _first : bool = true;
    @chpldoc.nodoc
    const _ending : string;

    // pointer to child's 'first' field so that we can communicate back if/when
    // a field has already been written.
    @chpldoc.nodoc
    var _firstPtr : c_ptr(bool) = nil;

    /*
      Serialize ``field`` named ``name``.

      Serializes fields in the form '<name> = <field>'. Adds a comma before the
      name if this is not the first field.
    */
    proc ref writeField(name: string, const field: ?) throws {
      if !_first then writer.writeLiteral(", ");
      else _first = false;

      writer.writeLiteral(name);
      writer.writeLiteral(" = ");
      writer.write(field);
    }

    /*
      Start serializing a nested class inside the current class. In this format
      inheritance is not represented and parent fields are printed before child
      fields. For example, the following classes with values
      ``x=5`` and ``y=2.0``:

      .. literalinclude:: ../../../../test/library/standard/IO/doc-examples/example_serialized_class.chpl
       :language: chapel
       :start-after: START_EXAMPLE
       :end-before: STOP_EXAMPLE

      would be serialized as:

      .. literalinclude:: ../../../../test/library/standard/IO/doc-examples/example_serialized_class.good

      :arg writer: The ``fileWriter`` to be used when serializing. Must match
                   the writer used to create current AggregateSerializer.
      :arg name: The name of the class type.
      :arg size: The number of fields in the class.

      :returns: A new :record:`~IO.defaultSerializer.AggregateSerializer`
    */
    proc ref startClass(writer: fileWriter, name: string, size: int) throws {
      // Note: 'size' of parent might be zero, but 'size' of grandparent might
      // be non-zero.
      return new AggregateSerializer(writer, _parent=true,
                                     _firstPtr=c_addrOf(_first));
    }

    /*
      Ends serialization of the current class by writing the character ``}``

      .. note:: It is an error to call methods on an AggregateSerializer after
                invoking 'endClass'.
    */
    proc endClass() throws {
      if !_parent then
        writer.writeLiteral(_ending);
      else if _firstPtr != nil then
        _firstPtr.deref() = _first;
    }

    /*
      Ends serialization of the current record by writing the character ``)``

      .. note:: It is an error to call methods on an AggregateSerializer after
                invoking 'endRecord'.
    */
    proc endRecord() throws {
      writer.writeLiteral(_ending);
    }
  }

  /*
    Start serializing a tuple by writing the character ``(``.

    :arg writer: The ``fileWriter`` to be used when serializing.
    :arg size: The number of elements in the tuple.

    :returns: A new :record:`TupleSerializer`
  */
  proc startTuple(writer: fileWriter, size: int) throws {
    writer.writeLiteral("(");
    return new TupleSerializer(writer, size);
  }

  /*
    Returned by ``startTuple`` to provide the API for serializing tuples.

    A tuple will be serialized as a comma-separated list between two
    parentheses. For example, the tuple literal ``(1, 2, 3)`` would be
    serialized as:

    .. code-block:: text

      (1, 2, 3)

    A 1-tuple will be serialized with a trailing comma. For example, the literal
    ``(4,)`` would be serialized as ``(4,)``.

  */
  record TupleSerializer {
    @chpldoc.nodoc
    var writer;
    @chpldoc.nodoc
    const size : int;
    @chpldoc.nodoc
    var _first : bool = true;

    /*
      Serialize ``element``.

      Writes a leading comma before serializing the element if this is not the
      first element in the tuple.
    */
    proc ref writeElement(const element: ?) throws {
      if !_first then writer.writeLiteral(", ");
      else _first = false;

      writer.write(element);
    }

    /*
      Ends serialization of the current tuple by writing the character ``)``.

      Adds a comma between the last value and ``)`` if there was only one
      element.
    */
    proc endTuple() throws {
      if size == 1 then
        writer.writeLiteral(",)");
      else
        writer.writeLiteral(")");
    }
  }

  /*
    Start serializing a list by writing the character ``[``.

    :arg writer: The ``fileWriter`` to be used when serializing.
    :arg size: The number of elements in the list.

    :returns: A new :record:`ListSerializer`
  */
  proc startList(writer: fileWriter, size: int) throws {
    writer.writeLiteral("[");
    return new ListSerializer(writer);
  }

  /*
    Returned by ``startList`` to provide the API for serializing lists.

    A list will be serialized as a comma-separated series of serialized
    elements between two square brackets. For example, serializing a list
    with elements ``1``, ``2``, and ``3`` will produce the text:

    .. code-block:: text

      [1, 2, 3]

    Empty lists will be serialized as ``[]``.
  */
  record ListSerializer {
    @chpldoc.nodoc
    var writer;
    @chpldoc.nodoc
    var _first : bool = true;

    /*
      Serialize ``element``.

      Writes a leading comma before serializing the element if this is not the
      first element in the list.
    */
    proc ref writeElement(const element: ?) throws {
      if !_first then writer.writeLiteral(", ");
      else _first = false;

      writer.write(element);
    }

    /*
      Ends serialization of the current list by writing the character ``]``.
    */
    proc endList() throws {
      writer.writeLiteral("]");
    }
  }

  /*
    Start serializing an array.

    :arg writer: The ``fileWriter`` to be used when serializing.
    :arg size: The number of elements in the array.

    :returns: A new :record:`ArraySerializer`
  */
  proc startArray(writer: fileWriter, size: int) throws {
    return new ArraySerializer(writer);
  }

  /*
    Returned by :proc:`~IO.defaultSerializer.startArray` to provide the API for
    serializing arrays.

    In the default format, an array will be serialized as a
    whitespace-separated series of serialized elements.

    A 1D array is serialized simply using spaces:

    ::

      1 2 3 4

    A 2D array is serialized using spaces between elements in a row, and
    prints newlines for new rows:

    ::

      1 2 3
      4 5 6
      7 8 9

    Arrays with three or more dimensions will be serialized as a series of
    2D "panes", with multiple newlines separating new dimensions:

    ::

      1 2 3
      4 5 6
      7 8 9

      10 11 12
      13 14 15
      16 17 18

      19 20 21
      22 23 24
      25 26 27

    Empty arrays result in no output to the :record:`fileWriter`.
  */
  record ArraySerializer {
    @chpldoc.nodoc
    var writer;
    @chpldoc.nodoc
    var _arrayDim : int;
    @chpldoc.nodoc
    var _arrayMax : int;
    @chpldoc.nodoc
    var _first : bool = true;

    /*
      Inform the :record:`~IO.defaultSerializer.ArraySerializer` to start
      serializing a new dimension of size ``size``.
    */
    proc ref startDim(size: int) throws {
      _arrayDim += 1;

      if _arrayMax >= _arrayDim then
        writer.writeNewline();
      else
        _arrayMax = _arrayDim;
    }

    /*
      End the current dimension.
    */
    proc ref endDim() throws {
      _arrayDim -= 1;
      _first = true;
    }

    /*
      Serialize ``element``.

      Adds a space if this is not the first element in the row.
    */
    proc ref writeElement(const element: ?) throws {
      if !_first then writer.writeLiteral(" ");
      else _first = false;

      writer.write(element);
    }

    /*
      Ends serialization of the current array.
    */
    proc endArray() throws { }
  }

  /*
    Start serializing a map by writing the character ``{``.

    :arg writer: The ``fileWriter`` to be used when serializing.
    :arg size: The number of entries in the map.

    :returns: A new :record:`MapSerializer`
  */
  proc startMap(writer: fileWriter, size: int) throws {
    writer.writeLiteral("{");
    return new MapSerializer(writer);
  }

  /*
    Returned by :proc:`~IO.defaultSerializer.startMap` to provide the API for
    serializing maps.

    Maps are serialized as a comma-separated series of pairs between curly
    braces. Pairs are serialized with a ``:`` separating the key and value. For
    example, the keys ``1``, ``2``, and ``3`` with values corresponding to
    their squares would be serialized as:

    ::

      {1: 1, 2: 4, 3: 9}

    Empty maps will be serialized as ``{}``.
  */
  record MapSerializer {
    @chpldoc.nodoc
    var writer;
    @chpldoc.nodoc
    var _first : bool = true;

    /*
      Serialize ``key``.

      Adds a leading comma if this is not the first pair in the map.
    */
    proc ref writeKey(const key: ?) throws {
      if !_first then writer.writeLiteral(", ");
      else _first = false;

      writer.write(key);
    }

    /*
      Serialize ``val``, preceded by the character ``:``.
    */
    proc writeValue(const val: ?) throws {
      writer.writeLiteral(": ");
      writer.write(val);
    }

    /*
      Ends serialization of the current map by writing the character ``}``
    */
    proc endMap() throws {
      writer.writeLiteral("}");
    }
  }
}

/*
  The default Deserializer used by :record:`fileReader`.

  See :ref:`the serializers technote<ioSerializers>` for a general overview
  of Deserializers and their usage.

  Otherwise, please refer to :type:`defaultSerializer` for a description
  of the default IO format. Individual methods on this type may clarify
  behavior specific to deserialization.

  .. note::

    Prior to the 1.32 release and the advent of the 'serializers' feature, the
    default implementation for reading classes and records permitted reading
    fields out of order. This functionality is not supported by the
    ``defaultDeserializer``.

    For an unspecified amount of time this module will retain the ability to
    disable automatic use of the ``defaultDeserializer`` by recompiling
    programs with the config-param ``useIOSerializers`` set to ``false``.

    Eventually, however, users must update their programs to account for
    reading fields out of order.
*/
record defaultDeserializer {

  /*
    Deserialize type ``readType`` with ``reader``.

    Classes and records will be deserialized using an appropriate initializer,
    passing in ``reader`` and this Deserializer as arguments. If an
    initializer is unavailable, this method may invoke the class or record's
    ``deserialize`` method. Please see the
    :ref:`serializers technote<ioSerializers>` for more information.

    Classes and records are expected to implement either the
    ``initDeserializable`` or ``readDeserializable`` interfaces (or both).
    Alternatively, types implementing the entire ``serializable`` interface
    are also accepted.

    :arg reader: The :record:`fileReader` from which types are deserialized.
    :arg readType: The type to be deserialized.

    :returns: A value of type ``readType``.
  */
  proc ref deserializeType(reader:fileReader, type readType) : readType throws {
    if isNilableClassType(readType) {
      if reader.matchLiteral("nil") {
        return nil:readType;
      }
    }

    if isNumericType(readType) || isBoolType(readType) || isEnumType(readType) ||
       readType == string || readType == bytes {
      var x : readType;
      reader._readOne(_iokind.dynamic, x, here);
      return x;
    } else if canResolveTypeMethod(readType, "deserializeFrom", reader, this) ||
              isArrayType(readType) {
      // Always run 'deserializeFrom' on arrays, for now, to work around issues
      // where a compilerError might cause 'canResolveTypeMethod' to return
      // false.
      return readType.deserializeFrom(reader=reader, deserializer=this);
    } else {
      return new readType(reader=reader, deserializer=this);
    }
  }

  /*
    Deserialize from ``reader`` directly into ``val``.

    Like :proc:`deserializeType`, but reads into an initialized value rather
    than creating a new value. For classes and records, this method will first
    attempt to invoke a ``deserialize`` method. If the ``deserialize`` method
    is unavailable, this method may fall back on invoking a suitable
    initializer and assigning the resulting value into ``val``. Please see the
    :ref:`serializers technote<ioSerializers>` for more.

    Classes and records are expected to implement either the
    ``initDeserializable`` or ``readDeserializable`` interfaces (or both).
    Alternatively, types implementing the entire ``serializable`` interface
    are also accepted.

    :arg reader: The :record:`fileReader` from which values are deserialized.
    :arg val: The value into which this Deserializer will deserialize.
  */
  proc ref deserializeValue(reader: fileReader, ref val: ?readType) : void throws {
    if isNilableClassType(readType) {
      if reader.matchLiteral("nil") {
        val = nil;
        return;
      } else if val == nil {
        val = deserializeType(reader, readType);
      }
    }

    if isNumericType(readType) || isBoolType(readType) || isEnumType(readType) ||
       readType == string || readType == bytes {
      reader._readOne(_iokind.dynamic, val, here);
    } else {
      val.deserialize(reader=reader, deserializer=this);
    }
  }

  /*
    Start deserializing a class by reading the character ``{``.

    :arg reader: The :record:`fileReader` to use when deserializing.
    :arg name: The name of the class type

    :returns: A new :type:`AggregateDeserializer`
  */
  proc startClass(reader: fileReader, name: string) throws {
    reader.readLiteral("{");
    return new AggregateDeserializer(reader);
  }

  /*
    Start deserializing a record by reading the character ``(``.

    :arg reader: The :record:`fileReader` to use when deserializing.
    :arg name: The name of the record type

    :returns: A new :type:`AggregateDeserializer`
  */
  proc startRecord(reader: fileReader, name: string) throws {
    reader.readLiteral("(");
    return new AggregateDeserializer(reader);
  }

  /*
    Returned by :proc:`~IO.defaultDeserializer.startClass` or
    :proc:`~IO.defaultDeserializer.startRecord` to provide the API for
    deserializing classes or records.

    See :type:`~IO.defaultSerializer.AggregateSerializer` for details of the
    default format for classes and records.
  */
  record AggregateDeserializer {
    @chpldoc.nodoc
    var reader;
    @chpldoc.nodoc
    var _parent : bool = false;

    /*
      Deserialize a field named ``name`` of type ``fieldType``.

      :returns: A deserialized value of type ``fieldType``.
    */
    proc readField(name: string, type fieldType) : fieldType throws {
      reader.readLiteral(name);
      reader.readLiteral("=");

      var ret = reader.read(fieldType);
      reader.matchLiteral(",");
      return ret;
    }

    /*
      Deserialize a field named ``name`` in-place.
    */
    proc readField(name: string, ref field) throws {
      reader.readLiteral(name);
      reader.readLiteral("=");

      reader.read(field);
      reader.matchLiteral(",");
    }

    /*
      Start deserializing a nested class inside the current class.

      See :proc:`defaultSerializer.AggregateSerializer.startClass` for details
      on inheritance on the default format.

      :returns: A new AggregateDeserializer
    */
    proc startClass(reader: fileReader, name: string) throws {
      return new AggregateDeserializer(reader, _parent=true);
    }

    /*
      End deserialization of the current class by reading the character ``}``.
    */
    proc endClass() throws {
      if !_parent then
        reader.readLiteral("}");
    }

    /*
      End deserialization of the current record by reading the character ``)``.
    */
    proc endRecord() throws {
      reader.readLiteral(")");
    }
  }

  /*
    Start deserializing a tuple by reading the character ``(``.

    :arg reader: The ``fileReader`` to use when deserializing.

    :returns: A new :type:`TupleDeserializer`
  */
  proc startTuple(reader: fileReader) throws {
    reader.readLiteral("(");
    return new TupleDeserializer(reader);
  }

  /*
    Returned by ``startTuple`` to provide the API for deserializing tuples.

    See :record:`~IO.defaultSerializer.TupleSerializer` for details of the
    default format for tuples.
  */
  record TupleDeserializer {
    @chpldoc.nodoc
    var reader;

    /*
      Deserialize an element of the tuple.

      :returns: A deserialized value of type ``eltType``.
    */
    proc readElement(type eltType) : eltType throws {
      var ret = reader.read(eltType);
      reader.matchLiteral(",");
      return ret;
    }

    /*
      Deserialize ``element`` in-place as an element of the tuple.
    */
    proc readElement(ref element) throws {
      reader.read(element);
      reader.matchLiteral(",");
    }

    /*
      End deserialization of the current tuple by reading the character ``)``.
    */
    proc endTuple() throws {
      reader.readLiteral(")");
    }
  }

  /*
    Start deserializing a list by reading the character ``[``.

    :arg reader: The ``fileReader`` to use when deserializing.

    :returns: A new :type:`ListDeserializer`
  */
  proc ref startList(reader: fileReader) throws {
    reader.readLiteral("[");
    return new ListDeserializer(reader);
  }

  /*
    Returned by ``startList`` to provide the API for deserializing lists.

    See :record:`~IO.defaultSerializer.ListSerializer` for details of the
    default format for lists.
  */
  record ListDeserializer {
    @chpldoc.nodoc
    var reader;
    @chpldoc.nodoc
    var _first : bool = true;

    /*
      Deserialize an element of the list.

      :returns: A deserialized value of type ``eltType``.
    */
    proc ref readElement(type eltType) : eltType throws {
      if !_first then reader.readLiteral(",");
      else _first = false;

      return reader.read(eltType);
    }

    /*
      Deserialize ``element`` in-place as an element of the list.
    */
    proc ref readElement(ref element) throws {
      if !_first then reader.readLiteral(",");
      else _first = false;

      reader.read(element);
    }

    /*
      End deserialization of the current list by reading the character ``]``.
    */
    proc endList() throws {
      reader.readLiteral("]");
    }

    /*
      :returns: Returns ``true`` if there are more elements to read.
    */
    proc hasMore() : bool throws {
      reader.mark();
      defer reader.revert();
      return !reader.matchLiteral("]");
    }
  }

  /*
    Start deserializing an array.

    :arg reader: The ``fileReader`` to use when deserializing.

    :returns: A new :type:`ArrayDeserializer`
  */
  proc startArray(reader: fileReader) throws {
    return new ArrayDeserializer(reader);
  }

  /*
    Returned by ``startArray`` to provide the API for deserializing arrays.

    See :record:`~IO.defaultSerializer.ArraySerializer` for details of the
    default format for arrays.
  */
  record ArrayDeserializer {
    @chpldoc.nodoc
    var reader;
    @chpldoc.nodoc
    var _first : bool = true;
    @chpldoc.nodoc
    var _arrayDim : int;
    @chpldoc.nodoc
    var _arrayMax : int;

    /*
      Inform the :record:`~IO.defaultDeserializer.ArrayDeserializer` to start
      deserializing a new dimension.
    */
    proc ref startDim() throws {
      _arrayDim += 1;

      if _arrayMax >= _arrayDim {
        // use 'match' rather than 'read' to allow for reading in a non-shaped
        // sequence of numbers into an N-D array...
        reader.matchNewline();
      } else {
        _arrayMax = _arrayDim;
      }
    }

    /*
      End deserialization of the current dimension.
    */
    proc ref endDim() throws {
      _arrayDim -= 1;

      _first = true;
    }

    /*
      Deserialize an element of the array.

      :returns: A deserialized value of type ``eltType``.
    */
    proc ref readElement(type eltType) : eltType throws {
      if !_first then reader.readLiteral(" ");
      else _first = false;

      return reader.read(eltType);
    }

    /*
      Deserialize ``element`` in-place as an element of the array.
    */
    proc ref readElement(ref element) throws {
      if !_first then reader.readLiteral(" ");
      else _first = false;

      reader.read(element);
    }

    /*
      End deserialization of the current array.
    */
    proc endArray() throws {
    }
  }

  /*
    Start deserializing a map by reading the character ``{``.

    :arg reader: The ``fileReader`` to use when deserializing.

    :returns: A new :type:`MapDeserializer`
  */
  proc startMap(reader: fileReader) throws {
    reader.readLiteral("{");
    return new MapDeserializer(reader);
  }

  /*
    Returned by ``startMap`` to provide the API for deserializing maps.

    See :record:`~IO.defaultSerializer.MapSerializer` for details of the default
    format for map.
  */
  record MapDeserializer {
    @chpldoc.nodoc
    var reader;
    @chpldoc.nodoc
    var _first : bool = true;

    /*
      Deserialize and return a key of type ``keyType``.
    */
    proc ref readKey(type keyType) : keyType throws {
      if !_first then reader.readLiteral(", ");
      else _first = false;

      return reader.read(keyType);
    }

    /*
      Deserialize ``key`` in-place as a key of the map.
    */
    proc ref readKey(ref key) throws {
      if !_first then reader.readLiteral(", ");
      else _first = false;

      reader.read(key);
    }

    /*
      Deserialize and return a value of type ``valType``.
    */
    proc readValue(type valType) : valType throws {
      reader.readLiteral(": ");

      return reader.read(valType);
    }

    /*
      Deserialize ``value`` in-place as a value of the map.
    */
    proc readValue(ref value) throws {
      reader.readLiteral(": ");

      reader.read(value);
    }

    /*
      End deserialization of the current map by reading the character ``}``.
    */
    proc endMap() throws {
      reader.readLiteral("}");
    }

    /*
      :returns: Returns ``true`` if there are more elements to read.

      .. warning::

        Behavior of 'hasMore' is undefined when called between ``readKey`` and
        ``readValue``.
    */
    proc hasMore() : bool throws {
      reader.mark();
      defer reader.revert();
      return !reader.matchLiteral("}");
    }
  }
}


@unstable("This config param is unstable and may be removed without advance notice")
/*
  This config param allows users to disable a warning for reading and writing
  classes and strings with :record:`~IO.binarySerializer` and
  :record:`binaryDeserializer` following a format change in the 1.33 release.
*/
config param warnBinaryStructured : bool = true;

private proc warnBinary(type t, param depth : int) {
  if warnBinaryStructured {
    if t == string || t == bytes || isClassType(t) {
      param msg = "binary(De)Serializer's format for strings, bytes, and classes no longer includes length-bytes or nilability-bytes. Recompile with ``-swarnBinaryStructured=false`` to disable this warning. To utilize the old format, please use the unstable 'ObjectSerialization' package module.";
      compilerWarning(msg, depth);
    }
  }
}

private proc warnBinaryRead(type t, param depth : int) throws {
  if warnBinaryStructured {
    if isClassType(t) {
      param msg = "binary(De)Serializer's format for classes no longer includes nilability-bytes. Recompile with ``-swarnBinaryStructured=false`` to disable this warning. To utilize the old format, please use the unstable 'ObjectSerialization' package module.";
      compilerWarning(msg, depth);
    }
  }
  if t == string || t == bytes {
    throw new IllegalArgumentError("binaryDeserializer does not support reading 'string' or 'bytes'. Please use a method like 'fileReader.readBinary' instead.");
  }
}

/*
  A binary Serializer that implements a simple binary format.

  This Serializer supports an ``endian`` field which may be configured at
  execution time.

  See :ref:`the serializers technote<ioSerializers>` for a general overview
  of Serializers and their usage.

  .. warning::

    In the 1.32 release this format included bytes representing the length of
    a string. Also, classes were serialized beginning with a single byte to
    indicate whether the class value was ``nil``. This behavior was changed
    in the subsequent release to provide users with a more flexible
    serializer that did not insert bytes that the user did not request. A
    compile-time warning will be issued to indicate that this behavior has
    changed. Users can recompile with ``-swarnBinaryStructured=false`` to
    silence the warning.

    To mimic the old behavior, please use the unstable
    :mod:`ObjectSerialization` module.
*/
record binarySerializer {
  /*
    'endian' represents the endianness of the binary output produced by this
    Serializer.
  */
  const endian : endianness = endianness.native;

  @chpldoc.nodoc
  const _structured = false;

  // TODO: rewrite to use correct IO methods (e.g. writeBinary)
  // For now, this is just a helper to mirror the old behavior for basic
  // types
  @chpldoc.nodoc
  proc _oldWrite(ch: fileWriter(?), const val:?t) throws {
    var _def = new defaultSerializer();
    var dc = ch.withSerializer(_def);
    var st = dc._styleInternal();
    var orig = st; defer { dc._set_styleInternal(orig); }

    st.binary = 1;
    st.byteorder = 1 + endian:uint(8);
    if _structured then
      st.str_style = iostringstyleInternal.lenVb_data: int(64);
    else
      st.str_style = iostringstyleInternal.data_toeof: int(64);

    dc._set_styleInternal(st);
    dc._writeOne(_iokind.dynamic, val, here);
  }

  /*
    Serialize ``val`` with ``writer``.

    Numeric values like integers, real numbers, and complex numbers are
    serialized directly to the associated :record:`fileWriter` as binary data in
    the specified endianness.

    Booleans are serialized as single byte unsigned values of either ``0`` or
    ``1``.

    ``string`` values are serialized as a raw sequence of bytes that does not
    include a null terminator, nor any bytes representing length. This means
    that ``string`` values cannot be deserialized without manual intervention
    by users to decide how their strings should be stored such that they can
    be deserialized.

    The ``nil`` value is serialized as a single unsigned byte of value ``0``.

    Classes and records will have their ``serialize`` method invoked, passing
    in ``writer`` and this Serializer as arguments. Please see the
    :ref:`serializers technote<ioSerializers>` for more on the ``serialize``
    method.

    Classes and records are expected to implement the ``writeSerializable``
    interface. The ``serializable`` interface is also acceptable.

    .. note::

      Serializing and deserializing enums is not stable in this format.

    :arg writer: The ``fileWriter`` used to write serialized output.
    :arg val: The value to be serialized.
  */
  proc ref serializeValue(writer: fileWriter(serializerType=binarySerializer, locking=false, ?),
                      const val:?t) throws {
    if isNumericType(t) {
      select endian {
        when endianness.native do writer.writeBinary(val, endianness.native);
        when endianness.little do writer.writeBinary(val, endianness.little);
        when endianness.big do writer.writeBinary(val, endianness.big);
      }
    } else if t == string  || isEnumType(t) || t == bytes ||
              isBoolType(t) {
      _oldWrite(writer, val);
    } else if t == nothing {

    } else if isClassType(t) {
      // Classes always prefixed with a bit indicating nil-ness
      //
      // TODO: Should 'startClass' handle this case?
      if val == nil {
        if _structured then
          writer.writeByte(0);
      } else {
        if _structured then
          writer.writeByte(1);
        val!.serialize(writer=writer, serializer=this);
      }
    } else {
      val.serialize(writer=writer, serializer=this);
    }
  }

  /*
    Start serializing a class and return a new ``AggregateSerializer``.

    :arg writer: The ``fileWriter`` to be used when serializing.
    :arg name: The name of the class type.
    :arg size: The number of fields in the class.

    :returns: A new :type:`AggregateSerializer`
  */
  proc startClass(writer: fileWriter(?), name: string, size: int) throws {
    return new AggregateSerializer(writer);
  }

  /*
    Start serializing a record and return a new ``AggregateSerializer``.

    :arg writer: The ``fileWriter`` to be used when serializing.
    :arg name: The name of the record type.
    :arg size: The number of fields in the class.

    :returns: A new :type:`AggregateSerializer`
  */
  proc startRecord(writer: fileWriter(?), name: string, size: int) throws {
    return new AggregateSerializer(writer);
  }

  /*
    Returned by :proc:`~IO.binarySerializer.startClass` or
    :proc:`~IO.binarySerializer.startRecord` to provide the API for serializing
    classes or records.

    In this simple binary format, classes and records do not begin or end with
    any bytes indicating size, and instead serialize their field values in
    ``binarySerializer``'s format.

    For example, a record with two ``uint(8)`` fields with values ``1`` and
    ``2`` would be serialized as ``0x01`` followed by ``0x02`` (in raw binary).
  */
  record AggregateSerializer {
    @chpldoc.nodoc
    var writer : fileWriter(false, binarySerializer);

    /*
      Serialize ``field`` in :record:`binarySerializer`'s format.
    */
    proc writeField(name: string, const field: ?T) throws {
      writer.write(field);
    }

    /*
      Start serializing a nested class inside the current class. In this
      binary format, this has no impact on the serialized output.
    */
    proc startClass(writer, name: string, size: int) throws {
      return this;
    }

    /*
      End deserialization of this class.
    */
    proc endClass() throws {
    }

    /*
      End deserialization of this record.
    */
    proc endRecord() throws {
    }
  }

  /*
    Start serializing a tuple and return a new :record:`TupleSerializer`.

    :arg writer: The :record:`fileWriter` to be used when serializing.
    :arg size: The number of elements in the tuple.

    :returns: A new TupleSerializer
  */
  proc startTuple(writer: fileWriter(?), size: int) throws {
    return new TupleSerializer(writer);
  }

  /*
    Returned by :proc:`~IO.binarySerializer.startTuple` to provide the API for
    serializing tuples.

    In this simple binary format, tuples do not begin or end with any bytes
    indicating size, and instead serialize their elements sequentially in
    :record:`binarySerializer`'s format.
  */
  record TupleSerializer {
    @chpldoc.nodoc
    var writer : fileWriter(false, binarySerializer);

    /*
      Serialize ``element`` in :record:`binarySerializer`'s format.
    */
    proc writeElement(const element: ?T) throws {
      writer.write(element);
    }

    /*
      Ends serialization of the current tuple.
    */
    proc endTuple() throws {
    }
  }

  /*
    Start serializing a list by serializing ``size``.

    :arg writer: The :record:`fileWriter` to be used when serializing.
    :arg size: The number of elements in the list.

    :returns: A new :record:`ListSerializer`
  */
  proc startList(writer: fileWriter(?), size: int) throws {
    writer.write(size);
    return new ListSerializer(writer);
  }

  /*
    Returned by :proc:`~IO.binarySerializer.startList` to provide the API for
    serializing lists.

    In this simple binary format, lists begin with the serialization of an
    ``int`` representing the size of the list. This data is then followed by
    the binary serialization of the specified number of elements.
  */
  record ListSerializer {
    @chpldoc.nodoc
    var writer : fileWriter(false, binarySerializer);

    /*
      Serialize ``element`` in :record:`binarySerializer`'s format.
    */
    proc writeElement(const element: ?) throws {
      writer.write(element);
    }

    /*
      Ends serialization of  the current list.
    */
    proc endList() throws {
    }
  }

  /*
    Start serializing an array and return a new :record:`ArraySerializer`.

    :arg writer: The :record:`fileWriter` to be used when serializing.
    :arg size: The number of elements in the array.

    :returns: A new ArraySerializer
  */
  proc startArray(writer: fileWriter(?), size: int) throws {
    return new ArraySerializer(writer, endian);
  }

  /*
    Returned by :proc:`~IO.binarySerializer.startArray` to provide the API for
    serializing arrays.

    In this simple binary format, arrays are serialized element by element
    in the order indicated by the caller of :proc:`writeElement`. Dimensions and
    the start or end of the array are not represented.
  */
  record ArraySerializer {
    @chpldoc.nodoc
    var writer : fileWriter(false, binarySerializer);
    @chpldoc.nodoc
    const endian : endianness;

    /*
      Start serializing a new dimension of the array.
    */
    proc startDim(size: int) throws {
    }

    /*
      Ends serialization of this dimension.
    */
    proc endDim() throws {
    }

    /*
      Serialize ``element`` in :record:`binarySerializer`'s format.
    */
    proc writeElement(const element: ?) throws {
      writer.write(element);
    }

    /*
      Serialize ``numElements`` number of elements in ``data``, provided that
      the element type of ``data`` is a numeric type.

      This performance-motivated implementation of the optional
      ``writeBulkElements`` will write the elements of ``data`` in the order
      in which they are represented in memory.

      .. note::

        This method is only optimized for the case where the
        :record:`binarySerializer` has been configured for ``native``
        endianness.

      .. warning::

        This method should only be called when the ``data`` argument is located
        on the same locale as the underlying ``file`` of this serializer.
        Otherwise the ``c_ptr`` will be invalid.
    */
    proc writeBulkElements(data: c_ptr(?eltType), numElements: int) throws
    where isNumericType(eltType) {
      if isNativeEndianness(endian) {
        const n = c_sizeof(eltType)*numElements;
        writer.writeBinary(data, n.safeCast(int));
      } else {
        for i in 0..<numElements do writer.write(data[i]);
      }
    }

    /*
      Ends serialization of the current array.
    */
    proc endArray() throws {
    }
  }

  /*
    Start serializing a map by serializing ``size``.

    :arg writer: The :record:`fileWriter` to be used when serializing.
    :arg size: The number of entries in the map.

    :returns: A new :record:`MapSerializer`
  */
  proc startMap(writer: fileWriter(?), size: int) throws {
    writer.write(size);
    return new MapSerializer(writer);
  }

  /*
    Returned by :proc:`~IO.binarySerializer.startMap` to provide the API for
    serializing maps.

    In this simple binary format, maps begin with the serialization of an
    ``int`` representing the size of the map. This data is then followed by the
    binary serialization of the specified number of key-value pairs. The binary
    serialization of a key-value pair has no structure, and simply consists of
    the serialization of the key followed by the serialization of the value.
  */
  record MapSerializer {
    @chpldoc.nodoc
    var writer : fileWriter(false, binarySerializer);

    /*
      Serialize ``key`` in :record:`binarySerializer`'s format.
    */
    proc writeKey(const key: ?) throws {
      writer.write(key);
    }

    /*
      Serialize ``val`` in :record:`binarySerializer`'s format.
    */
    proc writeValue(const val: ?) throws {
      writer.write(val);
    }

    /*
      Ends serialization of the current map.
    */
    proc endMap() throws {
    }
  }
}


/*
  A binary Deserializer that implements a simple binary format.

  This Deserializer supports an ``endian`` field which may be configured at
  execution time.

  See :ref:`the serializers technote<ioSerializers>` for a general overview
  of Deserializers and their usage.

  Otherwise, please refer to :type:`binarySerializer` for a description of the
  binary format. Individual methods on this type may clarify relevant behavior
  specific to deserialization

  .. note::

    Deserializing :type:`~String.string` or :type:`~Bytes.bytes` types will
    result in an :type:`~Errors.IllegalArgumentError` because these types cannot
    currently be deserialized with the raw nature of the format.

  .. warning::

    In the 1.32 release this format included bytes representing the length of
    a string. Also, classes were serialized beginning with a single byte to
    indicate whether the class value was ``nil``. This behavior was changed
    in the subsequent release to provide users with a more flexible
    deserializer that did not read bytes that the user did not request. A
    compile-time warning will be issued to indicate that this behavior has
    changed. Users can recompile with ``-swarnBinaryStructured=false`` to
    silence the warning.

    To mimic the old behavior, please use the unstable
    :mod:`ObjectSerialization` module.

*/
record binaryDeserializer {
  /*
    'endian' represents the endianness that this Deserializer should use when
    deserializing input.
  */
  const endian : IO.endianness = IO.endianness.native;

  @chpldoc.nodoc
  var _structured = false;

  @chpldoc.nodoc
  proc init(endian: IO.endianness = IO.endianness.native, _structured : bool = false) {
    this.endian = endian;
    this._structured = _structured;
    init this;
  }

  // TODO: rewrite in terms of writef, or something
  @chpldoc.nodoc
  proc _oldRead(ch: fileReader(?), ref val:?t) throws {
    var _def = new defaultDeserializer();
    var dc = ch.withDeserializer(_def);
    var st = dc._styleInternal();
    var orig = st; defer { dc._set_styleInternal(orig); }

    st.binary = 1;
    st.byteorder = 1 + endian:uint(8);
    if _structured then
      st.str_style = iostringstyleInternal.lenVb_data: int(64);
    else
      st.str_style = iostringstyleInternal.data_toeof: int(64);

    dc._set_styleInternal(st);
    dc._readOne(_iokind.dynamic, val, here);
  }

  @chpldoc.nodoc
  proc _checkClassNil(reader:fileReader(?), type readType) : bool throws {
    const notNil = if _structured then reader.readByte() else 1;
    if notNil == 0 {
      if isNilableClassType(readType) then
        return true;
      else
        throw new BadFormatError("cannot read nil class into non-nilable class");
    } else if notNil != 1 {
      throw new BadFormatError("invalid binary format found for class nilability");
    }

    return false;
  }

  /*
    Deserialize type ``readType`` with ``reader``.

    Classes and records will be deserialized using an appropriate initializer,
    passing in ``reader`` and this Deserializer as arguments. If an
    initializer is unavailable, this method may invoke the class or record's
    ``deserialize`` method. Please see the :ref:`serializers technote<ioSerializers>` for more.

    Classes and records are expected to implement either the
    ``initDeserializable`` or ``readDeserializable`` interfaces (or both). The
    ``serializable`` interface is also acceptable.

    :arg reader: The :record:`fileReader` from which types are deserialized.
    :arg readType: The type to be deserialized.

    :returns: A value of type ``readType``.
  */
  proc ref deserializeType(reader:fileReader(?), type readType) : readType throws {
    if isClassType(readType) {
      var isNil = _checkClassNil(reader, readType);
      if isNilableClassType(readType) && isNil then
        return nil:readType;
    }
    // else: not nil, proceed to try initializing

    if isNumericType(readType) {
      var x : readType;
      var ret : bool;
      select endian {
        when endianness.native do ret = reader.readBinary(x, endianness.native);
        when endianness.little do ret = reader.readBinary(x, endianness.little);
        when endianness.big    do ret = reader.readBinary(x, endianness.big);
      }
      if !ret then
        throw new EofError();
      else
        return x;
    } else if isBoolType(readType) ||
              isEnumType(readType) || isStringType(readType) ||
              isBytesType(readType) {
      var x : readType;
      _oldRead(reader, x);
      return x;
    } else if readType == nothing {
      // nothing...
    } else if canResolveTypeMethod(readType, "deserializeFrom", reader, this) ||
              isArrayType(readType) {
      return readType.deserializeFrom(reader=reader, deserializer=this);
    } else {
      return new readType(reader=reader, deserializer=this);
    }
  }

  /*
    Deserialize from ``reader`` directly into ``val``.

    Like :proc:`deserializeType`, but reads into an initialized value rather
    than creating a new value. For classes and records, this method will first
    attempt to invoke a ``deserialize`` method. If the ``deserialize`` method
    is unavailable, this method may fall back on invoking a suitable
    initializer and assigning the resulting value into ``val``. Please see the
    :ref:`serializers technote<ioSerializers>` for more.

    Classes and records are expected to implement either the
    ``readDeserializable`` or ``initDeserializable`` interfaces (or both). The
    ``serializable`` interface is also acceptable.

    :arg reader: The :record:`fileReader` from which values are deserialized.
    :arg val: The value into which this Deserializer will deserialize.
  */
  proc ref deserializeValue(reader: fileReader(?), ref val: ?readType) : void throws {
    if isClassType(readType) {
      var isNil = _checkClassNil(reader, readType);
      if isNilableClassType(readType) && isNil then
        val = nil;
    }

    if canResolveMethod(val, "deserialize", reader, this) {
      val.deserialize(reader=reader, deserializer=this);
    } else {
      val = deserializeType(reader, readType);
    }
  }

  /*
    Start deserializing a class by returning an :record:`AggregateDeserializer`.

    :arg reader: The :record:`fileReader` to use when deserializing.
    :arg name: The name of the class type.

    :returns: A new :type:`AggregateDeserializer`
  */
  proc startClass(reader: fileReader(?), name: string) throws {
    return new AggregateDeserializer(reader);
  }

  /*
    Start deserializing a record by returning an
    :record:`AggregateDeserializer`.

    :arg reader: The :record:`fileReader` to use when deserializing.
    :arg name: The name of the record type.

    :returns: A new :type:`AggregateDeserializer`
  */
  proc startRecord(reader: fileReader(?), name: string) throws {
    return new AggregateDeserializer(reader);
  }

  /*
    Returned by :proc:`~IO.binaryDeserializer.startClass` or
    :proc:`~IO.binaryDeserializer.startRecord` to provide the API for
    deserializing classes or records.

    See :record:`binarySerializer.AggregateSerializer` for details of the
    binary format for classes and records.
  */
  record AggregateDeserializer {
    @chpldoc.nodoc
    var reader : fileReader(false, binaryDeserializer);

    /*
      Deserialize and return a value of type ``fieldType``.
    */
    proc readField(name: string, type fieldType) : fieldType throws {
      return reader.read(fieldType);
    }

    /*
      Deserialize ``field`` in-place.
    */
    proc readField(name: string, ref field) throws {
      reader.read(field);
    }

    /*
      Start deserializing a nested class inside the current class.

      See :proc:`binarySerializer.AggregateSerializer.startClass` for details
      on inheritance on the binary format.

      :returns: A new :record:`~IO.binaryDeserializer.AggregateDeserializer`
    */
    proc startClass(reader, name: string) throws {
      return this;
    }

    /*
      End deserialization of the current class.
    */
    proc endClass() throws {
    }

    /*
      End deserialization of the current record.
    */
    proc endRecord() throws {
    }
  }

  /*
    Start deserializing a tuple by returning a :record:`TupleDeserializer`.

    :arg reader: The :record:`fileReader` to use when deserializing.

    :returns: A new :type:`TupleDeserializer`
  */
  proc startTuple(reader: fileReader(?)) throws {
    return new TupleDeserializer(reader);
  }

  /*
    Returned by :proc:`~IO.binaryDeserializer.startTuple` to provide the API for
    deserializing tuples.

    See :record:`binarySerializer.TupleSerializer` for details of the binary
    format for tuples.
  */
  record TupleDeserializer {
    @chpldoc.nodoc
    var reader : fileReader(false, binaryDeserializer);

    /*
      Deserialize an element of the tuple.

      :returns: A deserialized value of type ``eltType``.
    */
    proc readElement(type eltType) : eltType throws {
      return reader.read(eltType);
    }

    /*
      Deserialize ``element`` in-place as an element of the tuple.
    */
    proc readElement(ref element) throws {
      reader.read(element);
    }

    /*
      End deserialization of the current tuple.
    */
    proc endTuple() throws {
    }
  }

  /*
    Start deserializing a list by returning a :record:`ListDeserializer`.

    :arg reader: The :record:`fileReader` to use when deserializing.

    :returns: A new :record:`ListDeserializer`
  */
  proc startList(reader: fileReader(?)) throws {
    return new ListDeserializer(reader, reader.read(uint));
  }

  /*
    Returned by :proc:`~IO.binaryDeserializer.startList` to provide the API for
    deserializing lists.

    See :record:`binarySerializer.ListSerializer` for details of the binary
    format for lists.
  */
  record ListDeserializer {
    @chpldoc.nodoc
    var reader : fileReader(false, binaryDeserializer);
    @chpldoc.nodoc
    var _numElements : uint;

    /*
      Deserialize an element of the list.

      :returns: A deserialized value of type ``eltType``.
    */
    proc ref readElement(type eltType) : eltType throws {
      if _numElements <= 0 then
        throw new BadFormatError("no more list elements remain");

      _numElements -= 1;

      return reader.read(eltType);
    }

    /*
      Deserialize ``element`` in-place as an element of the list.
    */
    proc ref readElement(ref element) throws {
      if _numElements <= 0 then
        throw new BadFormatError("no more list elements remain");

      _numElements -= 1;

      reader.read(element);
    }

    /*
      End deserialization of the current list.

      :throws: A :type:`~OS.BadFormatError` if there are remaining elements.
    */
    proc endList() throws {
      if _numElements != 0 then
        throw new BadFormatError("read too few elements for list");
    }

    /*
      :returns: Returns ``true`` if there are more elements to read.
    */
    proc hasMore() : bool throws {
      return _numElements > 0;
    }
  }

  /*
    Start deserializing an array by returning an :record:`ArrayDeserializer`.

    :arg reader: The :record:`fileReader` to use when deserializing.

    :returns: A new :record:`ArrayDeserializer`
  */
  proc startArray(reader: fileReader(?)) throws {
    return new ArrayDeserializer(reader, endian);
  }

  /*
    Returned by :proc:`~IO.binaryDeserializer.startArray` to provide the API for
    deserializing arrays.

    See :record:`binarySerializer.ArraySerializer` for details of the binary
    format for arrays.
  */
  record ArrayDeserializer {
    @chpldoc.nodoc
    var reader : fileReader(false, binaryDeserializer);
    @chpldoc.nodoc
    const endian : endianness;

    /*
      Inform the :record:`~IO.binaryDeserializer.ArrayDeserializer` to start
      deserializing a new dimension.
    */
    proc startDim() throws {
    }

    /*
      End deserialization of the current dimension.
    */
    proc endDim() throws {
    }

    /*
      Deserialize an element of the list.

      :returns: A deserialized value of type ``eltType``.
    */
    proc readElement(type eltType) : eltType throws {
      return reader.read(eltType);
    }

    /*
      Deserialize ``element`` in-place as an element of the array.
    */
    proc readElement(ref element) throws {
      reader.read(element);
    }

    /*
      Deserialize ``numElements`` number of elements into ``data``, provided
      that the element type of ``data`` is a numeric type.

      This performance-motivated implementation of the optional
      ``readBulkElements`` will read the elements of ``data`` in the order in
      which they are represented in memory.

      .. note::

        This method is only optimized for the case where the
        :record:`binaryDeserializer` has been configured for ``native``
        endianness.

      .. warning::

        This method should only be called when the ``data`` argument is located
        on the same locale as the underlying ``file`` of this deserializer.
        Otherwise the ``c_ptr`` will be invalid.
    */
    proc readBulkElements(data: c_ptr(?eltType), numElements: int) throws
    where isNumericType(eltType) {
      if isNativeEndianness(endian) {
        const n = c_sizeof(eltType)*numElements;
        const got = reader.readBinary(data, n.safeCast(int));
        if got < n then throw createSystemOrChplError(EEOF);
      } else {
        for i in 0..<numElements {
          try {
            if !reader.read(data[i]) then
              throw createSystemOrChplError(EEOF);
          } catch e: UnexpectedEofError {
            // Match behavior of ``readBinary``, where we tolerate such partial
            // reads.
            throw createSystemOrChplError(EEOF);
          }
        }
      }
    }

    /*
      End deserialization of the current array.
    */
    proc endArray() throws {
    }
  }


  /*
    Start deserializing a map by returning a ``MapDeserializer``.

    :arg reader: The :record:`fileReader` to use when deserializing.

    :returns: A new :type:`MapDeserializer`
  */
  proc startMap(reader: fileReader(?)) throws {
    return new MapDeserializer(reader, reader.read(uint));
  }

  /*
    Returned by :proc:`~IO.binaryDeserializer.startMap` to provide the API for
    deserializing maps.

    See :record:`binarySerializer.MapSerializer` for details of the binary
    format for map.
  */
  record MapDeserializer {
    @chpldoc.nodoc
    var reader;
    @chpldoc.nodoc
    var _numElements : uint;

    /*
      Deserialize and return a key of type ``keyType``.
    */
    proc ref readKey(type keyType) : keyType throws {
      if _numElements <= 0 then
        throw new BadFormatError("no more map elements remain!");

      _numElements -= 1;

      return reader.read(keyType);
    }

    /*
      Deserialize ``key`` in-place as a key of the map.
    */
    proc ref readKey(ref key) throws {
      if _numElements <= 0 then
        throw new BadFormatError("no more map elements remain!");

      _numElements -= 1;

      reader.read(key);
    }

    /*
      Deserialize and return a value of type ``valType``.
    */
    proc readValue(type valType) : valType throws {
      return reader.read(valType);
    }

    /*
      Deserialize ``value`` in-place as a value of the map.
    */
    proc readValue(ref value) throws {
      reader.read(value);
    }

    /*
      End deserialization of the current map.

      :throws: A :type:`~OS.BadFormatError` if there are entries remaining.
    */
    proc endMap() throws {
      if _numElements != 0 then
        throw new BadFormatError("failed to read all expected elements in map");
    }

    /*
      :returns: Returns ``true`` if there are more elements to read.

      .. warning::

        Behavior of 'hasMore' is undefined when called between :proc:`readKey`
        and :proc:`readValue`.
    */
    proc hasMore() : bool throws {
      return _numElements > 0;
    }
  }
}


@chpldoc.nodoc
operator fileReader.=(ref lhs:fileReader, rhs:fileReader) {
  // retain -- release
  on rhs._home {
    qio_channel_retain(rhs._channel_internal);
  }

  on lhs._home {
    qio_channel_release(lhs._channel_internal);
  }

  lhs._home = rhs._home;
  lhs._channel_internal = rhs._channel_internal;
  lhs._deserializer = rhs._deserializer;
}

@chpldoc.nodoc
operator fileWriter.=(ref lhs:fileWriter, rhs:fileWriter) {
  // retain -- release
  on rhs._home {
    qio_channel_retain(rhs._channel_internal);
  }

  on lhs._home {
    qio_channel_release(lhs._channel_internal);
  }

  lhs._home = rhs._home;
  lhs._channel_internal = rhs._channel_internal;
  lhs._serializer = rhs._serializer;
}

@chpldoc.nodoc
proc fileReader.init(param locking:bool, type deserializerType) {
  this.locking = locking;
  this.deserializerType = deserializerType;
  this._deserializer = new shared _serializeWrapper(deserializerType);
}

@chpldoc.nodoc
proc fileWriter.init(param locking:bool, type serializerType) {
  this.locking = locking;
  this.serializerType = serializerType;
  this._serializer = new shared _serializeWrapper(serializerType);
}

@chpldoc.nodoc
proc fileReader.init=(x: fileReader) {
  // allow locking field to be modified in initialization
  this.locking = if this.type.locking != ?
                 then this.type.locking
                 else x.locking;

  this.deserializerType = x.deserializerType;
  this._home = x._home;
  this._channel_internal = x._channel_internal;
  this._deserializer = x._deserializer;
  this._readWriteThisFromLocale = x._readWriteThisFromLocale;
  init this;
  on x._home {
    qio_channel_retain(x._channel_internal);
  }
}

@chpldoc.nodoc
proc fileWriter.init=(x: fileWriter) {
  // allow locking field to be modified in initialization
  this.locking = if this.type.locking != ?
                 then this.type.locking
                 else x.locking;

  this.serializerType = x.serializerType;
  this._home = x._home;
  this._channel_internal = x._channel_internal;
  this._serializer = x._serializer;
  this._readWriteThisFromLocale = x._readWriteThisFromLocale;
  init this;
  on x._home {
    qio_channel_retain(x._channel_internal);
  }
}

@chpldoc.nodoc
operator :(rhs: fileReader, type t: fileReader) {
  var tmp: t = rhs; // just run init=
  return tmp;
}

@chpldoc.nodoc
operator :(rhs: fileWriter, type t: fileWriter) {
  var tmp: t = rhs; // just run init=
  return tmp;
}

@chpldoc.nodoc
proc fileReader.init(param locking:bool,
                     home: locale, _channel_internal:qio_channel_ptr_t,
                     _readWriteThisFromLocale: locale,
                     _deserializer: shared _serializeWrapper(?dt)) {
  this.locking = locking;
  this.deserializerType = dt;
  this._home = home;
  this._channel_internal = _channel_internal;
  this._deserializer = _deserializer;
  this._readWriteThisFromLocale = _readWriteThisFromLocale;
}

@chpldoc.nodoc
proc fileReader.init(param locking:bool, in deserializer:?,
                     f:file, out error:errorCode, hints: ioHintSet,
                     start:int(64), end:int(64),
                     in local_style:iostyleInternal) {
  this.init(locking, deserializer.type);
  on f._home {
    this._deserializer = new shared _serializeWrapper(deserializer.type, deserializer);
    this._home = f._home;
    error = qio_channel_create(this._channel_internal, f._file_internal,
                               hints._internal, true, false,
                               start, end, local_style, 64*1024);
    // On return this._channel_internal.ref_cnt == 1.
    // Failure to check the error return code may result in a double-deletion error.
  }
}

// Used to create a non-locking alias of an existing channel
@chpldoc.nodoc
proc fileWriter.init(param locking:bool,
                     home: locale, _channel_internal:qio_channel_ptr_t,
                     _readWriteThisFromLocale: locale,
                     _serializer: shared _serializeWrapper(?st)) {
  this.locking = locking;
  this.serializerType = st;
  this._home = home;
  this._channel_internal = _channel_internal;
  this._serializer = _serializer;
  this._readWriteThisFromLocale = _readWriteThisFromLocale;
}

@chpldoc.nodoc
proc fileWriter.init(param locking:bool, in serializer:?,
                     f:file, out error:errorCode, hints: ioHintSet,
                     start:int(64), end:int(64),
                     in local_style:iostyleInternal) {
  this.init(locking, serializer.type);
  on f._home {
    this._serializer = new shared _serializeWrapper(serializer.type, serializer);
    this._home = f._home;
    error = qio_channel_create(this._channel_internal, f._file_internal,
                               hints._internal, false, true,
                               start, end, local_style, 64*1024);
    // On return this._channel_internal.ref_cnt == 1.
    // Failure to check the error return code may result in a double-deletion error.
  }
}

@chpldoc.nodoc
proc ref fileReader.deinit() {
  on this._home {
    qio_channel_release(_channel_internal);
    this._channel_internal = QIO_CHANNEL_PTR_NULL;
  }
}

@chpldoc.nodoc
proc ref fileWriter.deinit() {
  on this._home {
    qio_channel_release(_channel_internal);
    this._channel_internal = QIO_CHANNEL_PTR_NULL;
  }
}

/*
  Create and return an alias of this :record:`fileReader` configured to use
  ``deserializerType`` for deserialization. The provided ``deserializerType``
  must be able to be default-initialized.

  .. warning::

    It is an error for the returned alias to outlive the original
    :record:`fileReader`.
*/
proc fileReader.withDeserializer(type deserializerType) :
  fileReader(this.locking, deserializerType) {
  var des : deserializerType;
  return withDeserializer(des);
}

/*
  Create and return an alias of this :record:`fileReader` configured to use
  ``deserializer`` for deserialization.

  .. warning::

    It is an error for the returned alias to outlive the original
    :record:`fileReader`.
*/
proc fileReader.withDeserializer(in deserializer: ?dt) : fileReader(this.locking, dt) {
  var ret = new fileReader(this.locking, dt);
  ret._deserializer = new shared _serializeWrapper(dt, deserializer);
  ret._channel_internal = this._channel_internal;
  ret._home = _home;
  ret._readWriteThisFromLocale = _readWriteThisFromLocale;
  on ret._home {
    qio_channel_retain(ret._channel_internal);
  }
  return ret;
}

/*
  Create and return an alias of this :record:`fileWriter` configured to use
  ``serializerType`` for serialization. The provided ``serializerType`` must be
  able to be default-initialized.

  .. warning::

    It is an error for the returned alias to outlive the original
    :record:`fileWriter`.
*/
proc fileWriter.withSerializer(type serializerType) :
  fileWriter(this.locking, serializerType) {
  var ser : serializerType;
  return withSerializer(ser);
}

/*
  Create and return an alias of this :record:`fileWriter` configured to use
  ``serializer`` for serialization.

  .. warning::

    It is an error for the returned alias to outlive the original
    :record:`fileWriter`.
*/
proc fileWriter.withSerializer(in serializer: ?st) : fileWriter(this.locking, st) {
  var ret = new fileWriter(this.locking, st);
  ret._serializer = new shared _serializeWrapper(st, serializer);
  ret._channel_internal = this._channel_internal;
  ret._home = _home;
  ret._readWriteThisFromLocale = _readWriteThisFromLocale;
  on ret._home {
    qio_channel_retain(ret._channel_internal);
  }
  return ret;
}

// represents a Unicode codepoint
// used to pass codepoints to read and write to avoid duplicating code
@chpldoc.nodoc
record _internalIoChar : writeSerializable {
  /* The codepoint value */
  var ch:int(32);
  @chpldoc.nodoc
  proc serialize(writer, ref serializer) throws {
    // ioChar.writeThis should not be called;
    // I/O routines should handle ioChar directly
    assert(false);
  }
}

@chpldoc.nodoc
inline operator :(x: _internalIoChar, type t:string) {
  var csc: c_ptrConst(c_char) =  qio_encode_to_string(x.ch);
  // The caller has responsibility for freeing the returned string.
  try! {
    return string.createAdoptingBuffer(csc);
  }
}

@chpldoc.nodoc
proc fileReader._getFp(): (bool, c_ptr(c_FILE)) {
  extern proc fdopen(fd: int(32), mode: c_ptrConst(c_char)): c_ptr(c_FILE);

  var f: qio_file_ptr_t,
      fd: c_int,
      fp: c_ptr(c_FILE);

  qio_channel_get_file_ptr(this._channel_internal, f);

  if qio_get_fp(f, fp) == 0 {
    return (true, fp);
  } else {
    if qio_get_fd(f, fd) == 0 {
      fp = fdopen(fd, "r");
      return (true, fp);
    } else {
      return (false, fp);
    }
  }
}


@chpldoc.nodoc
record chpl_ioNewline : writeSerializable {
  /*
    Normally, we will skip anything at all to get to a ``\n``,
    but if skipWhitespaceOnly is set, it will be an error
    if we run into non-space characters other than ``\n``.
   */
  var skipWhitespaceOnly: bool = false;

  @chpldoc.nodoc
  proc serialize(writer: fileWriter, ref serializer: writer.serializerType) throws {
    writer.writeNewline();
  }
}

@chpldoc.nodoc
inline operator :(x: chpl_ioNewline, type t:string) {
  return "\n";
}


@chpldoc.nodoc
record chpl_ioLiteral : writeSerializable {
  /* The value of the literal */
  var val: string;
  /* Should read operations using this literal ignore and consume
     whitespace before the literal?
   */
  var ignoreWhiteSpace: bool = true;
  @chpldoc.nodoc
  proc serialize(writer, ref serializer) throws {
    // Normally this is handled explicitly in read/write.
    writer.write(val);
  }
}

@chpldoc.nodoc
inline operator :(x: chpl_ioLiteral, type t:string) {
  return x.val;
}

@chpldoc.nodoc
record _internalIoBits {
  /* The bottom ``numBits`` of x will be read or written */
  var x:uint(64);
  /* How many of the low-order bits of ``x`` should we read or write? */
  var numBits:int(8);

  // keep the old names for compatibility with old ioBits
  proc v: x.type {return x;}
  proc nbits:numBits.type {return numBits;}
}

/*
 EEOF, ESHORT, and EFORMAT are internal, Chapel-specific IO error codes.
 */

private extern proc chpl_macro_int_EEOF():c_int;
/* An error code indicating the end of file has been reached (Chapel specific)
 */
@chpldoc.nodoc
private inline proc EEOF do return chpl_macro_int_EEOF():c_int;

private extern proc chpl_macro_int_ESHORT():c_int;
/* An error code indicating that the end of file or the end of the
   input was reached before the requested amount of data could be read.
   (Chapel specific)
  */
@chpldoc.nodoc
private inline proc ESHORT do return chpl_macro_int_ESHORT():c_int;

private extern proc chpl_macro_int_EFORMAT():c_int;
/* An error code indicating a format error; for example when reading a quoted
   string literal, this would be returned if we never encountered the
   opening quote. (Chapel specific)
  */
@chpldoc.nodoc
private inline proc EFORMAT do return chpl_macro_int_EFORMAT():c_int;


@chpldoc.nodoc
proc fileReader._ch_ioerror(error:errorCode, msg:string) throws {
  var path:string = "unknown";
  var offset:int(64) = -1;
  on this._home {
    var tmp_path:c_ptrConst(c_char);
    var tmp_offset:int(64);
    var err:errorCode = 0;
    err = qio_channel_path_offset(locking, _channel_internal, tmp_path, tmp_offset);
    if !err {
      // shouldn't throw
      path = string.createCopyingBuffer(tmp_path,
                                       policy=decodePolicy.replace);
      deallocate(tmp_path);
    } else {
      tmp_offset = qio_channel_offset_unlocked(_channel_internal);
    }
    offset = tmp_offset;
  }
  try ioerror(error, msg, path, offset);
}

// this version accepts an integral error code
@chpldoc.nodoc
proc fileReader._ch_ioerror(error:c_int, msg:string) throws {
  extern proc qio_int_to_err(a:int(32)):errorCode;
  try _ch_ioerror(qio_int_to_err(error), msg);
}

@chpldoc.nodoc
proc fileReader._ch_ioerror(errstr:string, msg:string) throws {
  var path:string = "unknown";
  var offset:int(64) = -1;
  on this._home {
    var tmp_path:c_ptrConst(c_char);
    var tmp_offset:int(64);
    var err:errorCode = 0;
    err = qio_channel_path_offset(locking, _channel_internal, tmp_path, tmp_offset);
    if !err {
      // shouldn't throw
      path = string.createCopyingBuffer(tmp_path,
                                       policy=decodePolicy.replace);
      deallocate(tmp_path);
    } else {
      tmp_offset = qio_channel_offset_unlocked(_channel_internal);
    }
    offset = tmp_offset;
  }
  try ioerror(errstr, msg, path, offset);
}

@chpldoc.nodoc
proc fileWriter._ch_ioerror(error:errorCode, msg:string) throws {
  var path:string = "unknown";
  var offset:int(64) = -1;
  on this._home {
    var tmp_path:c_ptrConst(c_char);
    var tmp_offset:int(64);
    var err:errorCode = 0;
    err = qio_channel_path_offset(locking, _channel_internal, tmp_path, tmp_offset);
    if !err {
      // shouldn't throw
      path = string.createCopyingBuffer(tmp_path,
                                       policy=decodePolicy.replace);
      deallocate(tmp_path);
    } else {
      tmp_offset = qio_channel_offset_unlocked(_channel_internal);
    }
    offset = tmp_offset;
  }
  try ioerror(error, msg, path, offset);
}

@chpldoc.nodoc
proc fileWriter._ch_ioerror(errstr:string, msg:string) throws {
  var path:string = "unknown";
  var offset:int(64) = -1;
  on this._home {
    var tmp_path:c_ptrConst(c_char);
    var tmp_offset:int(64);
    var err:errorCode = 0;
    err = qio_channel_path_offset(locking, _channel_internal, tmp_path, tmp_offset);
    if !err {
      // shouldn't throw
      path = string.createCopyingBuffer(tmp_path,
                                       policy=decodePolicy.replace);
      deallocate(tmp_path);
    } else {
      tmp_offset = qio_channel_offset_unlocked(_channel_internal);
    }
    offset = tmp_offset;
  }
  try ioerror(errstr, msg, path, offset);
}

/*
   Acquire a fileReader's lock. See :ref:`locking-filereaders-and-filewriters`
   for more details.

   :throws SystemError: If the lock could not be acquired.
 */
inline proc fileReader.lock() throws {
  var err:errorCode = 0;

  if _channel_internal == nil then
    throw createSystemError(EINVAL,
                            "Operation attempted on an invalid fileReader");

  if locking {
    on this._home {
      err = qio_channel_lock(_channel_internal);
    }
  }
  if err then try this._ch_ioerror(err, "in lock");
}

/*
   Acquire a fileWriter's lock. See :ref:`locking-filereaders-and-filewriters`
   for more details.

   :throws SystemError: If the lock could not be acquired.
 */
inline proc fileWriter.lock() throws {
  var err:errorCode = 0;

  if _channel_internal == nil then
    throw createSystemError(EINVAL,
                            "Operation attempted on an invalid fileWriter");

  if locking {
    on this._home {
      err = qio_channel_lock(_channel_internal);
    }
  }
  if err then try this._ch_ioerror(err, "in lock");
}

/*
   Release a fileReader's lock. See :ref:`locking-filereaders-and-filewriters`
   for more details.
 */
inline proc fileReader.unlock() {
  if locking {
    on this._home {
      qio_channel_unlock(_channel_internal);
    }
  }
}

/*
   Release a fileWriter's lock. See :ref:`locking-filereaders-and-filewriters`
   for more details.
 */
inline proc fileWriter.unlock() {
  if locking {
    on this._home {
      qio_channel_unlock(_channel_internal);
    }
  }
}

/*
   Return the current offset of a :record:`fileReader`.

   If the fileReader can be used by multiple tasks, take care when doing
   operations that rely on the fileReader's current offset. To prevent race
   conditions, lock the fileReader with :proc:`fileReader.lock` before calling
   :proc:`fileReader.offset`, then unlock it afterwards with
   :proc:`fileReader.unlock`.

   :returns: the current offset of the fileReader
 */
proc fileReader.offset(): int(64) {
  var ret:int(64);
  ret = qio_channel_offset_unlocked(this._channel_internal);
  return ret;
}

/*
   Return the current offset of a :record:`fileWriter`.

   If the fileWriter can be used by multiple tasks, take care when doing
   operations that rely on the fileWriter's current offset. To prevent race
   conditions, lock the fileWriter with :proc:`fileWriter.lock` before calling
   :proc:`fileWriter.offset`, then unlock it afterwards with
   :proc:`fileWriter.unlock`.

   :returns: the current offset of the fileWriter
 */
proc fileWriter.offset():int(64) {
  var ret:int(64);
  ret = qio_channel_offset_unlocked(this._channel_internal);
  return ret;
}

/*
   Move a :record:`fileReader` offset forward.

   This routine will consume the next ``amount`` bytes from the file, storing
   them in the ``fileReader``'s buffer. This can be useful for advancing to some
   known offset in the file before reading.

   Note that calling :proc:`fileReader.mark` before advancing will cause at
   least ``amount`` bytes to be retained in memory until
   :proc:`~fileReader.commit` or :proc:`~fileReader.revert` are called. As such,
   it is typical to advance by a small number of bytes during an I/O transaction.

   To make large adjustments to the offset, consider creating a new
   ``fileReader`` or using :proc:`~fileReader.seek` instead.

   :throws EofError: If EOF is reached before the requested number of bytes can
                     be consumed. The offset will be left at EOF.
   :throws SystemError: For other failures, for which fileReader offset is not
                        moved.
 */
proc fileReader.advance(amount:int(64)) throws {
  var err:errorCode = 0;
  on this._home {
    try this.lock(); defer { this.unlock(); }
    err = qio_channel_advance(false, _channel_internal, amount);
  }
  if err then try this._ch_ioerror(err, "in advance");
}

/*
   Move a :record:`fileWriter` offset forward.

   This routine will populate the ``fileWriter``'s buffer as the offset is moved
   forward by ``amount`` bytes. The buffer can be populated with any of the
   following data depending on the ``fileWriter``'s configuration and whether
   it was marked before advancing:

   * zeros
   * bytes directly from the file
   * bytes from a previously buffered portion of the file

   The contents of the buffer will subsequently be written to the file by the
   buffering mechanism.

   Note that calling :proc:`fileWriter.mark` before advancing will cause at
   least ``amount`` bytes to be retained in memory until
   :proc:`~fileWriter.commit` or :proc:`~fileWriter.revert` are called. As such,
   it is typical to advance by a small number of bytes during an I/O transaction.

   To make large adjustments to the offset, consider creating a new
   ``fileWriter`` or using :proc:`~fileWriter.seek` instead.

   :throws EofError: If EOF is reached before the offset can be advanced by the
                     requested number of bytes. The offset will be left at EOF.
   :throws SystemError: For other failures, for which fileWriter offset is not
                        moved.
 */
proc fileWriter.advance(amount:int(64)) throws {
  var err:errorCode = 0;
  on this._home {
    try this.lock(); defer { this.unlock(); }
    err = qio_channel_advance(false, _channel_internal, amount);
  }
  if err then try this._ch_ioerror(err, "in advance");
}

/*
   Read until a separator is found, leaving the :record:`fileReader` offset just
   after it.

   If the separator cannot be found, the ``fileReader`` offset is left at EOF
   and an ``UnexpectedEofError`` is thrown.

   .. note::

    The implementation is faster for single-byte ``string`` or ``bytes``
    separators.

   :arg separator: The separator to match with. Must be a :type:`~String.string`
    or :type:`~Bytes.bytes`.

   :throws IllegalArgumentError: If the separator is empty
   :throws EofError: If the ``fileReader`` offset was already at EOF.
   :throws UnexpectedEofError: If the requested ``separator`` could not
                               be found.
   :throws SystemError: If data could not be read from the ``file``.
                        In that event, the fileReader's offset will be
                        left near the position where the error occurred.
*/
proc fileReader.advanceThrough(separator: ?t) throws where t==string || t==bytes {
  on this._home {
    try this.lock(); defer { this.unlock(); }
    var err: errorCode = 0;

    if separator.numBytes == 1 {
      // fast advance to the single-byte separator
      err = qio_channel_advance_past_byte(false, this._channel_internal, separator.toByte():c_int, max(int(64)), true);
      if err {
        if err == EEOF {
          try this._ch_ioerror(err, "in advanceThrough(" + t:string + ")");
        } else if err == ESHORT {
          throw new UnexpectedEofError("separator not found in advanceThrough(" + t:string + ")");
        } else {
          try this._ch_ioerror(err, "in advanceThrough(" + t:string + ")");
        }
      }
    } else if separator.numBytes == 0 {
      throw new IllegalArgumentError("advanceThrough(" + t:string + ") called with empty separator");
    } else {
      // slow advance to multi-byte separator
      const (readError, found, bytesRead) = _findSeparator(separator, -1, this._channel_internal);
      // handle system errors
      if readError != 0 &&
         readError != EEOF && readError != ESHORT && readError != EFORMAT {
        try this._ch_ioerror(readError, "in advanceThrough(" + t:string + ")");
      }

      // advance past the separator
      err = qio_channel_advance(false, this._channel_internal, bytesRead + separator.numBytes);
      // handle system errors
      if err != 0 && err != EEOF then try this._ch_ioerror(err, "in advanceThrough(" + t:string + ")");

      if bytesRead == 0 {
        // throw EofError
        try this._ch_ioerror(EEOF, "in advanceThrough(" + t:string + ")");
      } else if !found {
        // throw UnexpectedEofError
        try this._ch_ioerror(ESHORT, "separator not found in advanceThrough(" + t:string + ")");
      }
    }
  }
}

/*
  Read until a newline is found, leaving the :record:`fileReader` offset just
  after it.

  If a newline cannot be found, the ``fileReader`` offset is left at EOF and
  an ``UnexpectedEofError`` is thrown.

  :throws EofError: If the ``fileReader`` offset was already at EOF.
  :throws UnexpectedEofError: A newline couldn't be found before the end of the
                              file.
  :throws SystemError: If data could not be read from the ``file``.
                       In that event, the fileReader's offset will be
                       left near the position where the error occurred.
*/
proc fileReader.advanceThroughNewline() throws {
  on this._home {
    param nl = "\n".toByte():c_int;

    try this.lock(); defer { this.unlock(); }
    var err: errorCode = 0;

    err = qio_channel_advance_past_byte(false, this._channel_internal, nl, max(int(64)), true);
    if err {
      if err == EEOF {
        try this._ch_ioerror(err, "in advanceThroughNewline)");
      } else if err == ESHORT {
        throw new UnexpectedEofError("newline not found in advanceThroughNewline");
      } else {
        try this._ch_ioerror(err, "in advanceThroughNewline");
      }
    }
  }
}

/*
   Read until a separator is found, leaving the :record:`fileReader` offset just
   before it.

   If the separator cannot be found, the ``fileReader`` offset is left at EOF
   and an ``UnexpectedEofError`` is thrown.

   .. note::

    The implementation is faster for single-byte ``string`` or ``bytes``
    separators.

   :arg separator: The separator to match with. Must be a :type:`~String.string` or
    :type:`~Bytes.bytes`.

   :throws IllegalArgumentError: If the separator is empty
   :throws EofError: If the ``fileReader`` offset is already at EOF.
   :throws UnexpectedEofError: If the requested ``separator`` could not be found.
   :throws SystemError: If data could not be read from the ``fileReader``.
                        In that event, the fileReader's offset will be
                        left near the position where the error occurred.
*/
proc fileReader.advanceTo(separator: ?t) throws where t==string || t==bytes {
  on this._home {
    try this.lock(); defer { this.unlock(); }
    var err: errorCode = 0;

    if separator.numBytes == 1 {
      // fast advance to the single-byte separator
      err = qio_channel_advance_past_byte(false, this._channel_internal, separator.toByte():c_int, max(int(64)), false);
      if err {
        if err == ESHORT || err == EFORMAT {
          throw new UnexpectedEofError("separator not found in advanceTo(" + t:string + ")");
        } else {
          try this._ch_ioerror(err, "in advanceTo(" + t:string + ")");
        }
      }
    } else if separator.numBytes == 0 {
      throw new IllegalArgumentError("advanceTo(" + t:string + ") called with empty separator");
    } else {
      // slow advance to multi-byte separator or EOF
      const (readError, found, bytesRead) = _findSeparator(separator, -1, this._channel_internal);
      if readError != 0 &&
         readError != EEOF && readError != ESHORT && readError != EFORMAT {
        try this._ch_ioerror(readError, "in advanceTo(" + t:string + ")");
      }

      // advance to separator, or to EOF if not found
      err = qio_channel_advance(
        false, this._channel_internal,
        bytesRead + if found then 0 else separator.numBytes
      );
      if err != 0 && err != EEOF then try this._ch_ioerror(err, "in advanceTo(" + t:string + ")");

      if bytesRead == 0 {
        // throw EofError
        try this._ch_ioerror(EEOF, "in advanceTo(" + t:string + ")");
      } else if !found {
        // throw UnexpectedEofError
        try this._ch_ioerror(ESHORT, "separator not found in advanceTo(" + t:string + ")");
      }
    }
  }
}

@chpldoc.nodoc
private inline proc markHelper(fileRW) throws {
  const offset = fileRW.offset();
  const err = qio_channel_mark(false, fileRW._channel_internal);

  if err then
    throw createSystemError(err);

  return offset;
}

/*
   *Mark* a :record:`fileReader` - that is, save the current offset of the
   ``fileReader`` on its *mark stack*.

   The *mark stack* stores several file offsets. The ``fileReader`` will keep
   the region of the file between its minimum and maximum *mark stack* values
   buffered in memory so that IO operations can be undone. As a result, it is
   possible to perform *I/O transactions* on a ``fileReader``. The basic steps
   for an *I/O transaction* are:

    * *mark* the current offset with :proc:`fileReader.mark`
    * do something speculative (e.g. try to read 200 bytes of anything followed
      by a 'B')
    * if the speculative operation was successful, commit the changes by
      calling :proc:`fileReader.commit`
    * if the speculative operation was not successful, go back to the *mark* by
      calling :proc:`fileReader.revert`. Subsequent I/O operations will work
      as though nothing happened.

   If a fileReader has ``locking==true``, :proc:`~fileReader.mark` should only
   be called once it has been locked with :proc:`fileReader.lock`. The
   fileReader should not be unlocked with :proc:`fileReader.unlock` until
   after the mark has been committed with :proc:`~fileReader.commit` or reverted
   with :proc:`~fileReader.revert`.

   See :ref:`io-transactions` for more.

  .. note::

    Note that it is possible to request an entire file be buffered in memory
    using this feature, for example by *marking* at offset=0 and then
    advancing to the end of the file. It is important to be aware of these
    memory space requirements.

  :returns: The offset that was marked
  :throws SystemError: if marking the ``fileReader`` failed
 */
proc fileReader.mark() throws do return markHelper(this);
// TODO (8/29/23): In the future, we'd like to support IO transactions using
// context managers. Under this proposal, entering a transaction context would
// mark the channel, leaving the context normally would commit, and leaving via
// throwing an error would revert.
// See: #19611

/*
   *Mark* a :record:`fileWriter` - that is, save the current offset of the
   ``fileWriter`` on its *mark stack*.

   The *mark stack* stores several file offsets. The ``fileWriter`` will keep
   the region of the file between its minimum and maximum *mark stack* values
   buffered in memory so that IO operations can be undone. As a result, it
   is possible to perform *I/O transactions* on a ``fileWriter``. The basic
   steps for an *I/O transaction* are:

    * *mark* the current offset with :proc:`fileWriter.mark`
    * do something speculative (e.g. try to write 200 bytes)
    * if the speculative operation was successful, commit the changes by
      calling :proc:`fileWriter.commit`
    * if the speculative operation was not successful, go back to the *mark* by
      calling :proc:`fileWriter.revert`. Subsequent I/O operations will work
      as though nothing happened.

   If a fileWriter has ``locking==true``, :proc:`~fileWriter.mark` should only
   be called once it has been locked with :proc:`fileWriter.lock`. The
   fileWriter should not be unlocked with :proc:`fileWriter.unlock` until
   after the mark has been committed with :proc:`~fileWriter.commit` or reverted
   with :proc:`~fileWriter.revert`.

   See :ref:`io-transactions` for more.

  .. note::

    Note that it is possible to request an entire file be buffered in memory
    using this feature, for example by *marking* at offset=0 and then
    advancing to the end of the file. It is important to be aware of these
    memory space requirements.

  :returns: The offset that was marked
  :throws SystemError: if marking the ``fileWriter`` failed
 */
proc fileWriter.mark() throws do return markHelper(this);
// TODO (8/29/23): In the future, we'd like to support IO transactions using
// context managers. Under this proposal, entering a transaction context would
// mark the channel, leaving the context normally would commit, and leaving via
// throwing an error would revert.
// See: #19611

/*
   Abort an *I/O transaction* by popping from the ``fileReader``'s *mark stack*
   and adjusting its position to that offset. See :ref:`io-transactions` for
   more.

   This routine should only be called on a fileReader that has already
   been marked. If called on a fileReader with ``locking=true``, the fileReader
   should have already been locked manually with :proc:`~fileReader.lock` before
   :proc:`~fileReader.mark` was called.
*/
inline proc fileReader.revert() {
  qio_channel_revert_unlocked(_channel_internal);
}

/*
   Abort an *I/O transaction* by popping from the ``fileWriter``'s *mark stack*
   and adjusting its position to that offset. See :ref:`io-transactions` for
   more.

   This routine should only be called on a fileWriter that has already
   been marked. If called on a fileWriter with ``locking=true``, the fileWriter
   should have already been locked manually with :proc:`~fileWriter.lock` before
   :proc:`~fileWriter.mark` was called.
*/
inline proc fileWriter.revert() {
  qio_channel_revert_unlocked(_channel_internal);
}

/*
   Commit an *I/O transaction* by popping from the ``fileReader``'s *mark stack*
   and leaving its position in the file unchanged. See :ref:`io-transactions` for
   more.

   This routine should only be called on a fileReader that has already
   been marked. If called on a fileReader with ``locking=true``, the fileReader
   should have already been locked manually with :proc:`~fileReader.lock` before
   :proc:`~fileReader.mark` was called.
*/
inline proc fileReader.commit() {
  qio_channel_commit_unlocked(_channel_internal);
}

/*
   Commit an *I/O transaction* by popping from the ``fileWriter``'s *mark stack*
   and leaving its position in the file unchanged. See :ref:`io-transactions` for
   more.

   This routine should only be called on a fileWriter that has already
   been marked. If called on a fileWriter with ``locking=true``, the fileWriter
   should have already been locked manually with :proc:`~fileWriter.lock` before
   :proc:`~fileWriter.mark` was called.
*/
inline proc fileWriter.commit() {
  qio_channel_commit_unlocked(_channel_internal);
}

/*
   Adjust a :record:`fileReader`'s region. The ``fileReader``'s buffer will be
   discarded.

   This routine has the following constraints:

    * the underlying file must be seekable (sockets and pipes are not seekable)
    * the ``fileReader`` must be non-locking (to avoid race conditions if two
      tasks seek and read simultaneously)
    * the ``fileReader`` must not be marked (see: :proc:`fileReader.mark`)

   If the ``fileReader`` offset needs to be updated during an I/O transaction
   or if discarding the buffer will incur a performance penalty, consider using
   :proc:`fileReader.advance` instead.

   :arg region: the new region, measured in bytes and counting from 0. An upper
                bound can be omitted (e.g., ``r.seek(range=42..)``). See
                :ref:`region <filereader-filewriter-regions>` for more.

   .. warning::

      The region argument will ignore any specified stride other than 1.

   :throws SystemError: if seeking failed. Possible reasons include
                         that the file is not seekable, or that the
                         fileReader is marked.
   :throws IllegalArgumentError: if region argument did not have a lower bound
 */
proc fileReader.seek(region: range(?)) throws {

  if this.locking then
    compilerError("Cannot seek on a locking fileReader");

  if (!region.hasLowBound()) {
    throw new IllegalArgumentError("illegal argument 'region': must have a lower bound");

  } else {
    on this._home {
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
}

/*
   Adjust a :record:`fileWriter`'s region. The ``fileWriter``'s buffer will be
   discarded.

   This routine has the following constraints:

    * the underlying file must be seekable (sockets and pipes are not seekable)
    * the ``fileWriter`` must be non-locking (to avoid race conditions if two
      tasks seek and read simultaneously)
    * the ``fileWriter`` must not be marked (see: :proc:`fileWriter.mark`)

   If the ``fileWriter`` offset needs to be updated during an I/O transaction
   or if discarding the buffer will incur a performance penalty, consider using
   :proc:`fileWriter.advance` instead.

   :arg region: the new region, measured in bytes and counting from 0. An upper
                bound can be omitted (e.g., ``w.seek(range=42..)``). See
                :ref:`region <filereader-filewriter-regions>` for more.

   .. warning::

      The region argument will ignore any specified stride other than 1.

   :throws SystemError: if seeking failed. Possible reasons include
                         that the file is not seekable, or that the
                         fileReader is marked.
   :throws IllegalArgumentError: if region argument did not have a lower bound
 */
proc fileWriter.seek(region: range(?)) throws {

  if this.locking then
    compilerError("Cannot seek on a locking fileWriter");

  if (!region.hasLowBound()) {
    throw new IllegalArgumentError("illegal argument 'region': must have a lower bound");

  } else {
    on this._home {
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
}

// Replacement for fileReader._style(), but it really shouldn't be used by
// users, it will get replaced at some point.
@chpldoc.nodoc
proc fileReader._styleInternal(): iostyleInternal {
  var ret:iostyleInternal;
  on this._home {
    var local_style:iostyleInternal;
    qio_channel_get_style(_channel_internal, local_style);
    ret = local_style;
  }
  return ret;
}

// Replacement for fileWriter._style(), but it really shouldn't be used by
// users, it will get replaced at some point.
@chpldoc.nodoc
proc fileWriter._styleInternal(): iostyleInternal {
  var ret:iostyleInternal;
  on this._home {
    var local_style:iostyleInternal;
    qio_channel_get_style(_channel_internal, local_style);
    ret = local_style;
  }
  return ret;
}

// Replacement for fileReader._set_style(), but it really shouldn't be used by
// users, it will get replaced at some point.
@chpldoc.nodoc
proc fileReader._set_styleInternal(style: iostyleInternal) {
  on this._home {
    var local_style:iostyleInternal = style;
    qio_channel_set_style(_channel_internal, local_style);
  }
}

// Replacement for fileWriter._set_style(), but it really shouldn't be used by
// users, it will get replaced at some point.
@chpldoc.nodoc
proc fileWriter._set_styleInternal(style: iostyleInternal) {
  on this._home {
    var local_style:iostyleInternal = style;
    qio_channel_set_style(_channel_internal, local_style);
  }
}

/*

   Return the locale on which an ongoing I/O was started with a fileReader.
   This method will return ``nilLocale`` unless it is called on a fileReader
   that is the formal argument to a `readThis` method.

 */
@unstable("'readWriteThisFromLocale' is unstable and may be removed or modified in a future release")
inline
proc fileReader.readWriteThisFromLocale() {
  return _readWriteThisFromLocale;
}

/*

   Return the locale on which an ongoing I/O was started with a fileWriter.
   This method will return ``nilLocale`` unless it is called on a fileWriter
   that is the formal argument to a `writeThis` method.

 */
@unstable("'readWriteThisFromLocale' is unstable and may be removed or modified in a future release")
inline
proc fileWriter.readWriteThisFromLocale() {
  return _readWriteThisFromLocale;
}

// Returns the original locale that the I/O started on
// Uses readWriteThisFromLocale in order to propagate that
// information across readThis/writeThis calls.
// If readWriteThisFromLocale returns nil, that means the fileReader
// was not created to call readThis/writeThis and
// so the original locale of the I/O is `here`.
@chpldoc.nodoc
inline
proc fileReader.getLocaleOfIoRequest() {
  var ret = this._readWriteThisFromLocale;
  if ret == nilLocale then
    ret = here;
  return ret;
}

// Returns the original locale that the I/O started on
// Uses readWriteThisFromLocale in order to propagate that
// information across readThis/writeThis calls.
// If readWriteThisFromLocale returns nil, that means the fileWriter
// was not created to call readThis/writeThis and
// so the original locale of the I/O is `here`.
@chpldoc.nodoc
inline
proc fileWriter.getLocaleOfIoRequest() {
  var ret = this._readWriteThisFromLocale;
  if ret == nilLocale then
    ret = here;
  return ret;
}

// QIO plugins don't have stable interface yet, hence no-doc
// only works when called on locale owning fileReader.
@chpldoc.nodoc
proc fileReader.channelPlugin() : borrowed QioPluginChannel? {
  var vptr = qio_channel_get_plugin(this._channel_internal);
  return vptr:borrowed QioPluginChannel?;
}
// only works when called on locale owning fileWriter.
@chpldoc.nodoc
proc fileWriter.channelPlugin() : borrowed QioPluginChannel? {
  var vptr = qio_channel_get_plugin(this._channel_internal);
  return vptr:borrowed QioPluginChannel?;
}

@chpldoc.nodoc
proc fileReader.filePlugin() : borrowed QioPluginFile? {
  var vptr = qio_file_get_plugin(qio_channel_get_file(this._channel_internal));
  return vptr:borrowed QioPluginFile?;
}
@chpldoc.nodoc
proc fileWriter.filePlugin() : borrowed QioPluginFile? {
  var vptr = qio_file_get_plugin(qio_channel_get_file(this._channel_internal));
  return vptr:borrowed QioPluginFile?;
}


// We can simply call fileReader.close() on these, since the underlying file
// will be closed once we no longer have any references to it (which in this
// case, since we only will have one reference, will be right after we close
// this fileReader presumably).

/*

Open a file at a particular path and return a :record:`fileReader` for it.
This function is equivalent to calling :proc:`open` and then
:proc:`file.reader` on the resulting file.

:arg path: which file to open (for example, "some/file.txt").
:arg locking: compile-time argument to determine whether or not the
              fileReader should use locking; sets the
              corresponding parameter of the :record:`fileReader` type.
              Defaults to ``false``
:arg region: zero-based byte offset indicating where in the file the
            fileReader should start and stop reading. Defaults to
            ``0..``, meaning from the start of the file to no specified end
            point.
:arg hints: optional argument to specify any hints to the I/O system about
            this file. See :record:`ioHintSet`.
:arg deserializer: deserializer to use when reading.
:returns: an open fileReader to the requested resource.

.. Note::

  ``locking=true`` should only be used when a fileReader will be used by
  multiple tasks concurrently.

.. warning::

   The region argument will ignore any specified stride other than 1.

:throws FileNotFoundError: If part of the provided path did not exist
:throws PermissionError: If part of the provided path had inappropriate
                         permissions
:throws NotADirectoryError: If part of the provided path was expected to
                            be a directory but was not
:throws SystemError: If a fileReader could not be returned.
:throws IllegalArgumentError: If trying to read explicitly prior to byte
                              0.
 */
proc openReader(path:string, param locking = false,
                region: range(?) = 0.., hints=ioHintSet.empty,
                in deserializer: ?dt = defaultSerializeVal(false))
    : fileReader(locking, dt) throws {

  var fl:file = try open(path, ioMode.r);
  return try fl.readerHelper(locking, region, hints, defaultIOStyleInternal(),
                             deserializer=deserializer);
}

/*
  Create a :record:`fileReader` around a :type:`~String.string`

  Note that the string is copied into a local memory file, so it can be modified
  after the ``fileReader`` is created without affecting the contents of the
  ``fileReader``.

  :arg s: the ``string`` to read from
  :arg deserializer: deserializer to use when reading.

  :returns: a ``fileReader`` reading from the string

*/
@unstable("'openStringReader' is an experimental feature; its name and behavior are subject to change")
proc openStringReader(const s: string, in deserializer: ?dt = defaultSerializeVal(false)): fileReader(false, dt) throws {
  // populate a memory file with the contents of the string
  const slocal = s.localize();
  var f = openMemFile(),
      w = f.writer(locking=false);
  w.write(slocal);
  w.close();

  // create a fileReader for the memory file
  var err: errorCode = 0,
      fr = new fileReader(
        false, deserializer, f, err, ioHintSet.empty,
        0, f.size, defaultIOStyleInternal()
      );

  if err then try fr._ch_ioerror(err, "in openStringReader");
  return fr;
}

/*
  Create a :record:`fileReader` around a :type:`~Bytes.bytes`

  Note that the bytes is copied into a local memory file, so it can be modified
  after the ``fileReader`` is created without affecting the contents of the
  ``fileReader``.

  :arg b: the ``bytes`` to read from
  :arg deserializer: deserializer to use when reading.

  :returns: a ``fileReader`` reading from the string

*/
@unstable("'openBytesReader' is an experimental feature; its name and behavior are subject to change")
proc openBytesReader(const b: bytes, in deserializer: ?dt = defaultSerializeVal(false)): fileReader(false, dt) throws {
  // populate a memory file with the contents of the bytes
  const blocal = b.localize();
  var f = openMemFile(),
      w = f.writer(locking=false);
  w.write(blocal);
  w.close();

  // create a fileReader for the memory file
  var err: errorCode = 0,
      fr = new fileReader(
        false, deserializer, f, err, ioHintSet.empty,
        0, f.size, defaultIOStyleInternal()
      );

  if err then try fr._ch_ioerror(err, "in openBytesReader");
  return fr;
}

/*

Open a file at a particular path and return a :record:`fileWriter` for it.
This function is equivalent to calling :proc:`open` with ``ioMode.cwr`` and then
:proc:`file.writer` on the resulting file.

:arg path: which file to open (for example, "some/file.txt").
:arg locking: compile-time argument to determine whether or not the
              fileWriter should use locking; sets the
              corresponding parameter of the :record:`fileWriter` type.
              Defaults to ``false``
:arg hints: optional argument to specify any hints to the I/O system about
            this file. See :record:`ioHintSet`.
:arg serializer: serializer to use when writing.
:returns: an open fileWriter to the requested resource.

.. Note::

  ``locking=true`` should only be used when a fileWriter will be used by
  multiple tasks concurrently.

:throws FileNotFoundError: If part of the provided path did not exist
:throws PermissionError: If part of the provided path had inappropriate
                         permissions
:throws NotADirectoryError: If part of the provided path was expected to
                            be a directory but was not
:throws SystemError: If a fileWriter could not be returned.
:throws IllegalArgumentError: If trying to write explicitly prior to byte
                              0.
*/
proc openWriter(path:string, param locking = false,
                hints = ioHintSet.empty,
                in serializer: ?st = defaultSerializeVal(true))
    : fileWriter(locking, st) throws {

  var fl:file = try open(path, ioMode.cw);
  return try fl.writerHelper(locking, 0..max(int(64)), hints, defaultIOStyleInternal(),
                             serializer=serializer);
}

/*
   Create a :record:`fileReader` that supports reading from a file. See
   :ref:`about-io-overview`.

   The ``region=`` argument defines the portion of the file that the fileReader
   will read from.  This is a byte offset; the beginning of the file is at the
   offset 0.  The default for this argument enables the fileReader to access the
   entire file.

   A fileReader will never read beyond its maximum end offset. In addition,
   reading from a fileReader beyond the end of the underlying file will not
   extend that file.  Reading beyond the end of the file or beyond the end
   offset of the fileReader will produce the error ``OS.EofError`` (or just
   return `false` in many cases such as :proc:`fileReader.read`) to indicate
   that the end was reached.

   :arg locking: compile-time argument to determine whether or not the
                 fileReader should use locking; sets the
                 corresponding parameter of the :record:`fileReader` type.
                 Defaults to ``false``.
   :arg region: zero-based byte offset indicating where in the file the
               fileReader should start and stop reading. Defaults to
               ``0..`` - meaning from the start of the file to no end point.
   :arg hints: provide hints about the I/O that this fileReader will perform.
               See :record:`ioHintSet`. The default value of `ioHintSet.empty`
               will cause the fileReader to use the hints provided when the
               file was opened.
   :arg deserializer: deserializer to use when reading.

   .. warning::

      The region argument will ignore any specified stride other than 1.


   :throws SystemError: If a fileReader could not be returned.
   :throws IllegalArgumentError: If trying to read explicitly prior to
                                 byte 0.
 */
proc file.reader(param locking = false,
                 region: range(?) = 0.., hints = ioHintSet.empty,
                 in deserializer: ?dt = defaultSerializeVal(false))
  : fileReader(locking, dt) throws {
  return this.readerHelper(locking, region, hints,
                           deserializer=deserializer);
}

@chpldoc.nodoc
proc file.readerHelper(param locking=true,
                       region: range(?) = 0.., hints = ioHintSet.empty,
                       style:iostyleInternal = this._style,
                       in deserializer: ?dt = defaultSerializeVal(false))
  : fileReader(locking, dt) throws {
  if (region.hasLowBound() && region.low < 0) {
    throw new IllegalArgumentError("illegal argument 'region': file region's lowest accepted bound is 0");
  }

  // It is the responsibility of the caller to release the returned fileReader
  // if the error code is nonzero.
  // The return error code should be checked to avoid double-deletion errors.
  var ret : fileReader(locking, dt);
  var err:errorCode = 0;
  on this._home {
    var start : region.idxType;
    var end : region.idxType;
    try this.checkAssumingLocal();
    if (region.hasLowBound() && region.hasHighBound()) {
      start = region.low;
      end = if region.high == max(region.idxType) then max(region.idxType) else region.high + 1;

    } else if (region.hasLowBound()) {
      start = region.low;
      end = max(region.idxType);

    } else if (region.hasHighBound()) {
      start = 0;
      end = if region.high == max(region.idxType) then max(region.idxType) else region.high + 1;

    } else {
      start = 0;
      end = max(region.idxType);
    }

    ret = new fileReader(locking, deserializer, this, err, hints,
                         start, end, style);
  }
  if err then try ioerror(err, "in file.reader", this._tryGetPath());

  return ret;
}

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

   A fileWriter will never write beyond its maximum end offset.  It will
   extend the file only as necessary to store data written to the fileWriter. In
   other words, specifying the high bound of the region argument here does not
   impact the file size directly; it impacts only the section of the file that
   this fileWriter can write to. After all fileWriters to a file are closed,
   that file will have a size equal to the last offset written to by any
   fileWriter.

   :arg locking: compile-time argument to determine whether or not the
                 fileWriter should use locking; sets the
                 corresponding parameter of the :record:`fileWriter` type.
                 Defaults to ``false``.
   :arg region: zero-based byte offset indicating where in the file the
               fileWriter should start and stop writing. Defaults to
               ``0..`` - meaning from the start of the file to no specified end
               point.
   :arg hints: provide hints about the I/O that this fileWriter will perform.
               See :record:`ioHintSet`. The default value of `ioHintSet.empty`
               will cause the fileWriter to use the hints provided when the
               file was opened.
   :arg serializer: serializer to use when writing.

   .. warning::

      The region argument will ignore any specified stride other than 1.

   :throws SystemError: If a fileWriter could not be returned.
   :throws IllegalArgumentError: If trying to write explicitly prior to
                                 byte 0.
 */
proc file.writer(param locking = false,
                 region: range(?) = 0.., hints = ioHintSet.empty,
                 in serializer:?st = defaultSerializeVal(true))
    : fileWriter(locking, st) throws {
  return this.writerHelper(locking, region, hints, serializer=serializer);
}

@chpldoc.nodoc
proc file.writerHelper(param locking=false,
                       region: range(?) = 0.., hints = ioHintSet.empty,
                       style:iostyleInternal = this._style,
                       in serializer:?st = defaultSerializeVal(true)):
  fileWriter(locking,st) throws {

  if (region.hasLowBound() && region.low < 0) {
    throw new IllegalArgumentError("illegal argument 'region': file region's lowest accepted bound is 0");
  }

  // It is the responsibility of the caller to retain and release the returned
  // fileWriter.
  // If the return error code is nonzero, the ref count will be 0 not 1.
  // The error code should be checked to avoid double-deletion errors.
  var ret : fileWriter(locking, st);
  var err:errorCode = 0;
  on this._home {
    var start : region.idxType;
    var end : region.idxType;
    try this.checkAssumingLocal();
    if (region.hasLowBound() && region.hasHighBound()) {
      start = region.low;
      end = if region.high == max(region.idxType) then max(region.idxType) else region.high + 1;

    } else if (region.hasLowBound()) {
      start = region.low;
      end = max(region.idxType);

    } else if (region.hasHighBound()) {
      start = 0;
      end = if region.high == max(region.idxType) then max(region.idxType) else region.high + 1;

    } else {
      start = 0;
      end = max(region.idxType);
    }

    ret = new fileWriter(locking, serializer, this, err, hints,
                         start, end, style);
  }
  if err then try ioerror(err, "in file.writer", this._tryGetPath());

  return ret;
}

@chpldoc.nodoc
proc _isSimpleIoType(type t) param do return
  isBoolType(t) || isNumericType(t) || isEnumType(t);

@chpldoc.nodoc
proc _isIoPrimitiveType(type t) param do return
  _isSimpleIoType(t) || (t == string) || (t == bytes);

@chpldoc.nodoc
 proc _isIoPrimitiveTypeOrNewline(type t) param do return
  _isIoPrimitiveType(t) || t == chpl_ioNewline || t == chpl_ioLiteral || t == _internalIoChar || t == _internalIoBits;

// Read routines for all primitive types.
private proc _read_text_internal(_channel_internal:qio_channel_ptr_t,
    ref x:?t):errorCode where _isIoPrimitiveType(t) {
  if isBoolType(t) {
    var err:errorCode = 0;
    var got:bool = false;

    err = qio_channel_scan_literal(false, _channel_internal, "true", "true".numBytes:c_ssize_t, 1);
    if !err {
      got = true;
    } else if err == EFORMAT {
      // try reading false instead.
      err = qio_channel_scan_literal(false, _channel_internal, "false", "false".numBytes:c_ssize_t, 1);
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
    var tx: c_ptrConst(c_char);
    var ret = qio_channel_scan_string(false, _channel_internal, tx, len, -1);
    x = try! string.createAdoptingBuffer(tx, length=len);
    return ret;
  } else if t == bytes {
    // handle _bytes
    var len:int(64);
    var tx: c_ptrConst(c_char);
    var ret = qio_channel_scan_bytes(false, _channel_internal, tx, len, -1);
    x = bytes.createAdoptingBuffer(tx, length=len);
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
      return qio_channel_print_literal(false, _channel_internal, "true", "true".numBytes:c_ssize_t);
    } else {
      return qio_channel_print_literal(false, _channel_internal, "false", "false".numBytes:c_ssize_t);
    }
  } else if isIntegralType(t) {
    // handles int types
    return qio_channel_print_int(false, _channel_internal, x, numBytes(t), isIntType(t));

  } else if isRealType(t) {
    // handles real
    return qio_channel_print_float(false, _channel_internal, x, numBytes(t));
  } else if isImagType(t) {
    return qio_channel_print_imag(false, _channel_internal, x, numBytes(t));
  } else if isComplexType(t) {
    // handle complex types
    return chpl_print_complex(_channel_internal, x);
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

@edition(first="preview")
private proc chpl_print_complex(_channel_internal, x) {
  var re = x.re;
  var im = x.im;
  return qio_channel_print_complex(threadsafe=false, _channel_internal, re, im,
                                   numBytes(re.type), full_nan=false);
}

@edition(last="2.0")
private proc chpl_print_complex(_channel_internal, x) {
  var re = x.re;
  var im = x.im;
  return qio_channel_print_complex(threadsafe=false, _channel_internal, re, im,
                                   numBytes(re.type), full_nan=true);
}

config param chpl_testReadBinaryInternalEIO = false;

private proc _read_binary_internal(_channel_internal:qio_channel_ptr_t, param byteorder:_iokind, ref x:?t):errorCode where _isIoPrimitiveType(t) {
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
    var tx: c_ptrConst(c_char);
    var ret = qio_channel_read_string(false, byteorder:c_int,
                                      qio_channel_str_style(_channel_internal),
                                      _channel_internal, tx, len, -1);
    x = try! string.createAdoptingBuffer(tx, length=len);
    return ret;
  } else if t == bytes {
    // handle _bytes (nothing special for bytes vs string in this case)
    var len:int(64);
    var tx: c_ptrConst(c_char);
    var ret = qio_channel_read_string(false, byteorder:c_int,
                                      qio_channel_str_style(_channel_internal),
                                      _channel_internal, tx, len, -1);
    x = bytes.createAdoptingBuffer(tx, length=len);
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

config param chpl_testWriteBinaryInternalEIO = false;

private proc _write_binary_internal(_channel_internal:qio_channel_ptr_t, param byteorder:_iokind, x:?t):errorCode where _isIoPrimitiveType(t) {
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

@chpldoc.nodoc
proc fileReader._constructIoErrorMsg(const x:?t): string {
  var result: string = "while reading ";
  result += t:string;

  select t {
    when chpl_ioNewline do result += " " + "newline";
    when chpl_ioLiteral do result += " " + "\"" + x:string + "\"";
  }

  return result;
}

@chpldoc.nodoc
proc fileWriter._constructIoErrorMsg(const x:?t): string {
  var result: string = "while writing ";
  result += t:string;

  select t {
    when chpl_ioNewline do result += " " + "newline";
    when chpl_ioLiteral do result += " " + "\"" + x:string + "\"";
  }

  return result;
}

@chpldoc.nodoc
proc fileReader._deserializeOne(type readType, loc:locale) throws {
  // TODO: Investigate overhead of initializer when in a loop.
  pragma "no init"
  pragma "no auto destroy"
  var reader: fileReader(locking=false, deserializerType);
  reader._channel_internal = _channel_internal;
  __primitive("=", reader._deserializer, _deserializer);
  reader._home = _home;
  reader._readWriteThisFromLocale = loc;

  return reader.deserializer.deserializeType(reader, readType);
}

@chpldoc.nodoc
proc fileReader._deserializeOne(ref x:?t, loc:locale) throws {
  // TODO: Investigate overhead of initializer when in a loop.
  pragma "no init"
  pragma "no auto destroy"
  var reader: fileReader(locking=false, deserializerType);
  reader._channel_internal = _channel_internal;
  __primitive("=", reader._deserializer, _deserializer);
  reader._home = _home;
  reader._readWriteThisFromLocale = loc;

  if t == chpl_ioLiteral || t == chpl_ioNewline || t == _internalIoBits || t == _internalIoChar {
    reader._readOne(_iokind.dynamic, x, reader.getLocaleOfIoRequest());
    return;
  }

  reader.deserializer.deserializeValue(reader, x);
}

//
// The fileReader must be locked and running on this._home.
// The intent of x is ref (vs out) because it might contain a string literal.
//
@chpldoc.nodoc
proc fileReader._readOne(param kind: _iokind, ref x:?t,
                             loc:locale) throws {
  // TODO: Make _read_one_internal(s) a method instead.
  var err = try _read_one_internal(_channel_internal, kind, x, loc);

  if err != 0 {
    const msg = _constructIoErrorMsg(x);
    try _ch_ioerror(err, msg);
  }
}

private proc escapedNonUTF8ErrorMessage() : string {
  const ret = "Strings with escaped non-UTF8 bytes cannot be used with I/O. " +
        "Try using string.encode(encodePolicy.unescape) first.\n";
  return ret;
}

@chpldoc.nodoc
proc fileWriter._serializeOne(const x:?t, loc:locale) throws {
  // TODO: Investigate overhead of initializer when in a loop.
  //
  // TODO: provide a way to get a non-locking alias
  // (it shouldn't release anything since it's a local copy).
  pragma "no init"
  pragma "no auto destroy"
  var writer : fileWriter(locking=false, serializerType);
  writer._channel_internal = _channel_internal;
  __primitive("=", writer._serializer, _serializer);
  writer._home = _home;
  writer._readWriteThisFromLocale = loc;

  if t == chpl_ioLiteral || t == chpl_ioNewline || t == _internalIoBits || t == _internalIoChar {
    writer._writeOne(_iokind.dynamic, x, writer.getLocaleOfIoRequest());
    return;
  }

  try writer.serializer.serializeValue(writer, x);
}

//
// The fileWriter must be locked and running on this._home.
//
@chpldoc.nodoc
proc fileWriter._writeOne(param kind: _iokind, const x:?t, loc:locale) throws {
  // TODO: Make _write_one_internal(s) a method instead.
  var err = _write_one_internal(_channel_internal, kind, x, loc);

  if err != 0 {
    var msg = _constructIoErrorMsg(x);
    if err == EILSEQ {
      // TODO: Is this error tested?
      msg = escapedNonUTF8ErrorMessage() + msg;
    }
    try _ch_ioerror(err, msg);
  }
}
private proc _read_io_type_internal(_channel_internal:qio_channel_ptr_t,
                                    param kind:_iokind,
                                    ref x:?t,
                                    loc:locale): errorCode throws {
  var e:errorCode = 0;
  if t == chpl_ioNewline {
    return qio_channel_skip_past_newline(false, _channel_internal, x.skipWhitespaceOnly);
  } else if t == _internalIoChar {
    return qio_channel_read_char(false, _channel_internal, x.ch);
  } else if t == chpl_ioLiteral {
    return qio_channel_scan_literal(false, _channel_internal,
                                    x.val.localize().c_str(),
                                    x.val.numBytes: c_ssize_t, x.ignoreWhiteSpace);
  } else if t == _internalIoBits {
    return qio_channel_read_bits(false, _channel_internal, x.x, x.numBits);
  } else if kind == _iokind.dynamic {
    var binary:uint(8) = qio_channel_binary(_channel_internal);
    var byteorder:uint(8) = qio_channel_byteorder(_channel_internal);
    if binary {
      select byteorder:_iokind {
        when _iokind.big    do e = _read_binary_internal(_channel_internal, _iokind.big, x);
        when _iokind.little do e = _read_binary_internal(_channel_internal, _iokind.little, x);
        otherwise             e = _read_binary_internal(_channel_internal, _iokind.native, x);
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
                                param kind:_iokind,
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
                                        param kind:_iokind,
                                        const x:?t,
                                        loc:locale): errorCode throws where _isIoPrimitiveTypeOrNewline(t) {
  var e:errorCode = 0;
  if t == chpl_ioNewline {
    return qio_channel_write_newline(false, _channel_internal);
  } else if t == _internalIoChar {
    return qio_channel_write_char(false, _channel_internal, x.ch);
  } else if t == chpl_ioLiteral {
    return qio_channel_print_literal(false, _channel_internal, x.val.localize().c_str(), x.val.numBytes:c_ssize_t);
  } else if t == _internalIoBits {
    return qio_channel_write_bits(false, _channel_internal, x.x, x.numBits);
  } else if kind == _iokind.dynamic {
    var binary:uint(8) = qio_channel_binary(_channel_internal);
    var byteorder:uint(8) = qio_channel_byteorder(_channel_internal);
    if binary {
      select byteorder:_iokind {
        when _iokind.big    do e = try _write_binary_internal(_channel_internal, _iokind.big, x);
        when _iokind.little do e = try _write_binary_internal(_channel_internal, _iokind.little, x);
        otherwise             e = try _write_binary_internal(_channel_internal, _iokind.native, x);
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
                                       param kind:_iokind,
                                       ref x:?t,
                                       loc:locale): errorCode throws {
  // Create a new fileReader that borrows the pointer in the
  // existing fileReader so we can avoid locking (because we
  // already have the lock)
  var temp : shared _serializeWrapper(nothing);
  var reader = new fileReader(locking=false,
                              _deserializer=temp,
                              home=here,
                              _channel_internal=_channel_internal,
                              _readWriteThisFromLocale=loc);

  // Set the fileReader pointer to NULL to make the
  // destruction of the local reader record safe
  // (it shouldn't release anything since it's a local copy).
  defer { reader._channel_internal = QIO_CHANNEL_PTR_NULL; }

  if isNilableClassType(t) {
    // future - write class IDs, have serialization format, handle binary
    var st = reader.styleElement(QIO_STYLE_ELEMENT_AGGREGATE);
    var iolit:chpl_ioLiteral;
    if st == QIO_AGGREGATE_FORMAT_JSON {
      iolit = new chpl_ioLiteral("null");
    } else {
      iolit = new chpl_ioLiteral("nil");
    }
    var e:errorCode = 0;
    e = try _read_one_internal(_channel_internal, _iokind.dynamic, iolit, loc);
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
                                        param kind:_iokind,
                                        const x:?t,
                                        loc:locale): errorCode throws {
  // Create a new fileWriter that borrows the pointer in the
  // existing fileWriter so we can avoid locking (because we
  // already have the lock)
  var temp : shared _serializeWrapper(nothing);
  var writer = new fileWriter(locking=false,
                              _serializer=temp,
                              home=here,
                              _channel_internal=_channel_internal,
                              _readWriteThisFromLocale=loc);

  // Set the fileWriter pointer to NULL to make the
  // destruction of the local writer record safe
  // (it shouldn't release anything since it's a local copy).
  defer { writer._channel_internal = QIO_CHANNEL_PTR_NULL; }

  var err: errorCode = 0;

  if isClassType(t) || chpl_isDdata(t) || isAnyCPtr(t) {
    if x == nil {
      // future - write class IDs, have serialization format, handle binary
      var st = writer.styleElement(QIO_STYLE_ELEMENT_AGGREGATE);
      var iolit:chpl_ioLiteral;
      if st == QIO_AGGREGATE_FORMAT_JSON {
        iolit = new chpl_ioLiteral("null");
      } else {
        iolit = new chpl_ioLiteral("nil");
      }
      err = try _write_one_internal(_channel_internal, _iokind.dynamic,
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

private proc literalErrorHelper(x: ?t, action: string,
                                isLiteral: bool): string {
  // Error message construction is handled here so that messages are
  // consistent across the cross product of:
  //   {read,write,match} x {literal,newline} x {string, bytes}
  //
  // Note that newlines do not involve strings or bytes

  var msg: string = "while " + action + " ";

  if isLiteral {
    if t == string then
      msg += "string literal \"" + x + "\"";
    else
      msg += "bytes literal";
  } else {
    msg += "newline";
  }

  return msg;
}

@chpldoc.nodoc
inline proc fileReader._checkLiteralError(x:?t, err:errorCode, action:string,
                                          isLiteral:bool) : void throws {
  if err != 0 {
    var msg: string = literalErrorHelper(x, action, isLiteral);
    if (err == EILSEQ) {
      // TODO: Is this error tested?
      msg = escapedNonUTF8ErrorMessage() + "Error: " + msg;
    }

    try _ch_ioerror(err, msg);
  }
}

@chpldoc.nodoc
inline proc fileWriter._checkLiteralError(x:?t, err:errorCode, action:string,
                                          isLiteral:bool) : void throws {
  if err != 0 {
    var msg: string = literalErrorHelper(x, action, isLiteral);
    if (err == EILSEQ) {
      // TODO: Is this error tested?
      msg = escapedNonUTF8ErrorMessage() + "Error: " + msg;
    }

    try _ch_ioerror(err, msg);
  }
}

@chpldoc.nodoc
inline proc fileReader._readLiteralCommon(x:?t, ignore:bool,
                                          param isMatch:bool) throws {
  if t != string && t != bytes then
    compilerError("expecting string or bytes");

  on this._home {
    try! this.lock(); defer { this.unlock(); }
    const ref cstr = x.localize().c_str();
    const err = qio_channel_scan_literal(false, _channel_internal,
                                         cstr, x.numBytes:c_ssize_t,
                                         ignore);

    const action = if isMatch then "matching" else "reading";
    try _checkLiteralError(x, err, action, isLiteral=true);
  }
}

/*
  Advances the offset of a :record:`fileReader` within the file by reading the
  exact text of the given string ``literal`` from the fileReader.

  If the string is not matched exactly, then the fileReader's offset is
  unchanged. In such cases a :class:`OS.BadFormatError` will be thrown, unless
  the end of the fileReader is encountered in which case an :class:`OS.EofError`
  will be thrown.

  By default this method will ignore leading whitespace in the file when
  attempting to read a literal (leading whitespace in the ``literal`` itself
  is still matched against whitespace in the file).

  :arg literal: the string to be matched.
  :arg ignoreWhitespace: determines whether leading whitespace is ignored.

  :throws BadFormatError: If literal could not be matched.
  :throws EofError: If end of fileReader is encountered.

*/
inline
proc fileReader.readLiteral(literal:string,
                            ignoreWhitespace=true) : void throws {
  _readLiteralCommon(literal, ignoreWhitespace, isMatch=false);
}

/*
  Advances the offset of a fileReader by reading the exact bytes of the given
  ``literal`` from the :record:`fileReader`.

  If the bytes are not matched exactly, then the fileReader's offset is
  unchanged. In such cases a :class:`OS.BadFormatError` will be thrown, unless
  the end of the ``fileReader`` is encountered in which case an
  :class:`OS.EofError` will be thrown.

  By default this method will ignore leading whitespace in the file when
  attempting to read a literal (leading whitespace in the ``literal`` itself
  is still matched against whitespace in the file).

  :arg literal: the bytes to be matched.
  :arg ignoreWhitespace: determines whether leading whitespace is ignored.

  :throws BadFormatError: If literal could not be matched.
  :throws EofError: If end of the ``fileReader`` is encountered.

*/
inline
proc fileReader.readLiteral(literal:bytes,
                            ignoreWhitespace=true) : void throws {
  _readLiteralCommon(literal, ignoreWhitespace, isMatch=false);
}

// TODO: Don't we need an option to ignore whitespace or not?
// Note: We don't want to allow skipping over non-whitespace.
//
// Note: We can add an 'ignoreWhitespace' optional argument that defaults
// to 'true' without changing behavior in existing programs.
@chpldoc.nodoc
inline
proc fileReader._readNewlineCommon(param isMatch:bool) throws {
  on this._home {
    try! this.lock(); defer { this.unlock(); }
    const err = qio_channel_skip_past_newline(false, _channel_internal,
                                              /*skipWhitespaceOnly=*/ true);

    const action = if isMatch then "matching" else "reading";
    try _checkLiteralError("", err, action, isLiteral=false);
  }
}

// TODO: How does this differ from readln() ?
/*
  Advances the offset of the :record:`fileReader` by reading a newline.

  If a newline is not matched exactly, then the fileReader's offset is
  unchanged. In such cases a :class:`OS.BadFormatError` will be thrown, unless
  the end of the ``fileReader`` is encountered in which case an
  :class:`OS.EofError` will be thrown. By default this method will ignore
  leading whitespace when attempting to read a newline.

  :throws BadFormatError: If a newline could not be matched.
  :throws EofError: If end of the ``fileReader`` is encountered.
*/
inline
proc fileReader.readNewline() : void throws {
  _readNewlineCommon(isMatch=false);
}

@chpldoc.nodoc
inline
proc fileReader._matchLiteralCommon(literal, ignore : bool) : bool throws {
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
  Advances the offset of a :record:`fileReader` by reading the exact text of the
  given string ``literal`` from the fileReader.

  If the string is not matched exactly, then the fileReader's offset is
  unchanged and this method will return ``false``. In other words, this
  fileReader will return ``false`` in the cases where
  :proc:`fileReader.readLiteral` would throw a :class:`OS.BadFormatError` or an
  :class:`OS.EofError`.

  By default this method will ignore leading whitespace in the file when
  attempting to read a literal (leading whitespace in the ``literal`` itself
  is still matched against whitespace in the file).

  :arg literal: the string to be matched.
  :arg ignoreWhitespace: determines whether leading whitespace is ignored.

  :returns: ``true`` if the read succeeded, and ``false`` on end of file or if
            the literal could not be matched.
*/
inline
proc fileReader.matchLiteral(literal:string,
                             ignoreWhitespace=true) : bool throws {
  return _matchLiteralCommon(literal, ignoreWhitespace);
}

/*
  Advances the offset of a :record:`fileReader` by reading the exact bytes of
  the given ``literal`` from the ``fileReader``.

  If the bytes are not matched exactly, then the fileReader's offset is
  unchanged and this method will return ``false``. In other words, this
  fileReader will return ``false`` in the cases where
  :proc:`fileReader.readLiteral` would throw a :class:`OS.BadFormatError` or an
  :class:`OS.EofError`.

  By default this method will ignore leading whitespace in the file when
  attempting to read a literal (leading whitespace in the ``literal`` itself
  is still matched against whitespace in the file).

  :arg literal: the bytes to be matched.
  :arg ignoreWhitespace: determines whether leading whitespace is ignored.

  :returns: ``true`` if the read succeeded, and ``false`` on end of file or if
            the literal could not be matched.
*/
inline
proc fileReader.matchLiteral(literal:bytes,
                             ignoreWhitespace=true) : bool throws {
  return _matchLiteralCommon(literal, ignoreWhitespace);
}

/*
  Advances the offset of the :record:`fileReader` by reading a newline.

  If a newline is not matched exactly, then the fileReader's offset is
  unchanged and this method will return ``false``. In other words, this
  fileReader will return ``false`` in the cases where
  :proc:`fileReader.readNewline` would throw a :class:`OS.BadFormatError` or an
  :class:`OS.EofError`.

  By default this method will ignore leading whitespace when attempting to
  read a newline.

  :returns: ``true`` if the read succeeded, and ``false`` on end of file or if
            the newline could not be matched.
*/
inline
proc fileReader.matchNewline() : bool throws {
  try {
    _readNewlineCommon(isMatch=true);
  } catch e : BadFormatError {
    return false;
  } catch e : EofError {
    return false;
  }

  return true;
}

@chpldoc.nodoc
inline
proc fileWriter._writeLiteralCommon(x:?t) : void throws {
  if t != string && t != bytes then
    compilerError("expecting string or bytes");

  on this._home {
    try! this.lock(); defer { this.unlock(); }
    const ref cstr = x.localize().c_str();
    const err = qio_channel_print_literal(false, _channel_internal, cstr,
                                          x.numBytes:c_ssize_t);
    try _checkLiteralError(x, err, "writing", isLiteral=true);
  }
}

/*
  Writes a string to the :record:`fileWriter`, ignoring any formatting
  configured for this ``fileWriter``.
*/
inline
proc fileWriter.writeLiteral(literal:string) : void throws {
  _writeLiteralCommon(literal);
}

/*
  Writes bytes to the :record:`fileWriter`, ignoring any formatting configured
  for this ``fileWriter``.
*/
inline
proc fileWriter.writeLiteral(literal:bytes) : void throws {
  _writeLiteralCommon(literal);
}

// TODO: How does this differ from writeln() ?
/*
  Writes a newline to the :record:`fileWriter`, ignoring any formatting
  configured for this ``fileWriter``.
*/
inline
proc fileWriter.writeNewline() : void throws {
  on this._home {
    try! this.lock(); defer { this.unlock(); }
    const err = qio_channel_write_newline(false, _channel_internal);
    try _checkLiteralError("", err, "writing", isLiteral=false);
  }
}

@chpldoc.nodoc
proc fileReader._binary():bool {
  var ret:uint(8);
  on this._home {
    ret = qio_channel_binary(_channel_internal);
  }
  return ret != 0;
}

@chpldoc.nodoc
proc fileWriter._binary():bool {
  var ret:uint(8);
  on this._home {
    ret = qio_channel_binary(_channel_internal);
  }
  return ret != 0;
}

/* return other style elements. */
@chpldoc.nodoc
proc fileReader.styleElement(element:int):int {
  var ret:int = 0;
  on this._home {
    ret = qio_channel_style_element(_channel_internal, element);
  }
  return ret;
}

/* return other style elements. */
@chpldoc.nodoc
proc fileWriter.styleElement(element:int):int {
  var ret:int = 0;
  on this._home {
    ret = qio_channel_style_element(_channel_internal, element);
  }
  return ret;
}

/*
  Iterate over all of the lines ending in ``\n`` in a :record:`fileReader` - the
  fileReader lock will be held while iterating over the lines.

  This iterator will halt on internal :ref:`system errors<io-general-sys-error>`.
  In the future, iterators are intended to be able to throw in such cases.

  **Example:**

  .. literalinclude:: ../../../../test/library/standard/IO/doc-examples/example_read_lines.chpl
   :language: chapel
   :start-after: START_EXAMPLE
   :end-before: STOP_EXAMPLE

  .. warning::

    This iterator executes on the current locale. This may impact performance if
    the current locale is not the same locale on which the fileReader was created.

  .. warning::

    Parallel iteration is not supported for sockets, pipes, or other
    non-file-based sources

  .. warning::

    Zippered parallel iteration is not yet supported for this iterator.

  :arg stripNewline: Whether to strip the trailing ``\n`` from the line —
                     defaults to false
  :arg t: The type of the lines to read (either :type:`~String.string` or
          :type:`~Bytes.bytes`) — defaults to ``string``
  :yields: lines from the fileReader, by default with a trailing ``\n``

*/
iter fileReader.lines(
  stripNewline = false,
  type t = string
): t
  where t == string || t == bytes
{
  try! this.lock();
  for line in this._lines_serial(stripNewline, t) do yield line;
  this.unlock();
}

/*
  Iterate over all of the lines ending in ``\n`` in a :record:`fileReader` using
  multiple locales - the fileReader lock will be held while iterating over the lines.

  This iterator will halt on internal :ref:`system errors<io-general-sys-error>`.
  In the future, iterators are intended to be able to throw in such cases.

  **Example:**

  .. literalinclude:: ../../../../test/library/standard/IO/doc-examples/example_read_lines_dist.chpl
   :language: chapel
   :start-after: START_EXAMPLE
   :end-before: STOP_EXAMPLE

  .. warning::

    Parallel iteration is not supported for sockets, pipes, or other
    non-file-based sources

  .. warning::

    This procedure does not support reading from a memory-file (e.g.,
    files opened with :proc:`openMemFile`)

  .. warning::

    Zippered parallel iteration is not yet supported for this iterator.

  :arg stripNewline: Whether to strip the trailing ``\n`` from the line —
                     defaults to false
  :arg targetLocales: The locales on which to read the lines (parallel
                      iteration only)
  :arg t: The type of the lines to read (either :type:`~String.string` or
          :type:`~Bytes.bytes`) — defaults to ``string``
  :yields: lines from the fileReader, by default with a trailing ``\n``

*/
iter fileReader.lines(
  stripNewline = false,
  type t = string,
  targetLocales: [] locale
): t
  where t == string || t == bytes
{
  try! this.lock();
  for line in this._lines_serial(stripNewline, t) do yield line;
  this.unlock();
}

@chpldoc.nodoc
iter fileReader._lines_serial(
  stripNewline = false,
  type t = string
): t
  where t == string || t == bytes
{
  // Save iostyleInternal
  const saved_style: iostyleInternal = this._styleInternal();
  // Update iostyleInternal
  var newline_style: iostyleInternal = this._styleInternal();

  param newlineChar = '\n'.toByte();

  newline_style.string_format = QIO_STRING_FORMAT_TOEND;
  newline_style.string_end = newlineChar;
  this._set_styleInternal(newline_style);

  // Iterate over lines
  var itemReader = new itemReaderInternal(t, locking, deserializerType, this);
  for line in itemReader {
    if !stripNewline then yield line;
    else {
      var lastCharIdx = line.size-1;
      if !line.isEmpty() && line.byte(lastCharIdx) == newlineChar
        then yield line[..<lastCharIdx];
        else yield line;
    }
  }

  // Set the iostyle back to original state
  this._set_styleInternal(saved_style);
}

// single locale parallel 'lines'
@chpldoc.nodoc
iter fileReader.lines(
  param tag: iterKind,
  stripNewline = false,
  type t = string
): t
  where tag == iterKind.standalone && (t == string || t == bytes)
{
  on this._home {
    try! this.lock();

    const f = chpl_fileFromReaderOrWriter(this),
          myStart = qio_channel_start_offset_unlocked(this._channel_internal),
          myEnd = qio_channel_end_offset_unlocked(this._channel_internal),
          myBounds = myStart..min(myEnd, try! f.size),
          nTasks = if dataParTasksPerLocale>0 then dataParTasksPerLocale
                                              else here.maxTaskPar;
    try {
      // try to break the file into chunks and read the lines in parallel
      // can fail if the file is too small to break into 'nTasks' chunks
      const byteOffsets = findFileChunks(f, nTasks, myBounds);

      coforall tid in 0..<nTasks {
        const taskBounds = byteOffsets[tid]..<byteOffsets[tid+1],
              r = try! f.reader(region=taskBounds);

        var line: t;
        while (try! r.readLine(line, stripNewline=stripNewline)) do
          yield line;
      }
    } catch {
      // fall back to serial iteration if 'findFileChunks' fails
      for line in this._lines_serial(stripNewline, t) do yield line;
    }

    this.unlock();
  }
}

// multi-locale parallel 'lines'
@chpldoc.nodoc
iter fileReader.lines(
  param tag: iterKind,
  stripNewline = false,
  type t = string,
  targetLocales: [?tld] locale
): t
  where tag == iterKind.standalone && (t == string || t == bytes)
{
  on this._home {
    try! this.lock();

    const f = chpl_fileFromReaderOrWriter(this),
          myStart = qio_channel_start_offset_unlocked(this._channel_internal),
          myEnd = qio_channel_end_offset_unlocked(this._channel_internal),
          myBounds = myStart..min(myEnd, try! f.size),
          fpath = try! f.path;

    try {
      // try to break the file into chunks and read the lines in parallel
      // can fail if the file is too small to break into 'targetLocales.size' chunks
      const byteOffsets = findFileChunks(f, targetLocales.size, myBounds);
      coforall lid in 0..<targetLocales.size do on targetLocales[tld.orderToIndex(lid)] {
        const locBounds = byteOffsets[lid]..byteOffsets[lid+1];

        // if byteOffsets looks like [0, 10, 10, 14, 21], then don't try to read 10..10 (locale 1)
        if locBounds.size > 1 {
          const locFile = try! open(fpath, ioMode.r),
                nTasks = if dataParTasksPerLocale>0 then dataParTasksPerLocale
                                                    else here.maxTaskPar;
          try {
            // try to break this locale's chunk into 'nTasks' chunks and read the lines in parallel
            const locByteOffsets = findFileChunks(locFile, nTasks, locBounds);
            coforall tid in 0..<nTasks {
              const taskBounds = locByteOffsets[tid]..<locByteOffsets[tid+1],
                    r = try! locFile.reader(region=taskBounds);

              if taskBounds.size > 0 {
                var line: t;
                while (try! r.readLine(line, stripNewline=stripNewline)) do
                  yield line;
              }
            }
          } catch {
            // fall back to serial iteration for this locale if 'findFileChunks' fails
            for line in locFile.reader(region=locBounds)._lines_serial(stripNewline, t) do yield line;
          }
        }
      }
    } catch {
      // fall back to serial iteration if 'findFileChunks' fails
      for line in this._lines_serial(stripNewline, t) do yield line;
    }

    this.unlock();
  }
}

/*
  Get an array of ``n+1`` byte offsets that divide the file ``f`` into ``n``
  roughly equally sized chunks, where each byte offset comes immediately after
  a newline.

  :arg f: the file to search
  :arg n: the number of chunks to find
  :arg bounds: a range of byte offsets to break into chunks

  :returns: a length ``n+1`` array of byte offsets (the first offset is
            ``bounds.low`` and the last is ``bounds.high``)

  :throws: if a valid byte offset cannot be found for one or more chunks
*/
@chpldoc.nodoc
private proc findFileChunks(const ref f: file, n: int, bounds: range): [] int throws {
  const nDataBytes = bounds.high - bounds.low,
        approxBytesPerChunk = nDataBytes / n;

  var chunkOffsets: [0..n] int;
  chunkOffsets[0] = bounds.low;
  chunkOffsets[n] = bounds.high;

  forall i in 1..<n with (ref chunkOffsets) {
    const estOffset = bounds.low + i * approxBytesPerChunk,
          r = f.reader(region=estOffset..);

    try {
      r.advanceThroughNewline(); // advance past the next newline
    } catch e {
      // there wasn't an offset in this chunk
      throw new Error(
        "Failed to find byte offset in the range (" + bounds.low:string + ".." + bounds.high:string +
        ") for chunk " + i:string + " of " + n:string + ". Try using fewer tasks."
      );
    }
    chunkOffsets[i] = r.offset(); // record the offset for this chunk
  }

  return chunkOffsets;
}

public use ChapelIOStringifyHelper;

@chpldoc.nodoc
proc chpl_stringify(const args ...?k):string {
  if _can_stringify_direct(args) {
    return stringify_simple((...args));
  } else {
    // otherwise, write it using the I/O system.
    try! {
      // Open a memory buffer to store the result
      var f = openMemFile();
      defer try! f.close();

      var w = f.writer(locking=false);
      defer try! w.close();

      w.write((...args));

      try! w.lock();
      var offset = w.offset();
      w.unlock();

      var buf = allocate(uint(8), offset:c_size_t+1);

      var r = f.reader(locking=false);
      defer try! r.close();

      r.readBinary(buf, offset);
      // Add the terminating NULL byte to make C string conversion easy.
      buf[offset] = 0;

      const ret = string.createCopyingBuffer(buf, offset, offset+1,
                                            decodePolicy.replace);
      deallocate(buf);
      return ret;
    }
  }
}

@chpldoc.nodoc
inline proc fileReader._readInner(ref args ...?k):void throws {
  const origLocale = this.getLocaleOfIoRequest();

  on this._home {
    try this.lock(); defer { this.unlock(); }
    for param i in 0..k-1 {
      if deserializerType != nothing {
        if deserializerType == binaryDeserializer {
          warnBinaryRead(args[i].type, 3);
        }
        _deserializeOne(args[i], origLocale);
      } else {
        _readOne(_iokind.dynamic, args[i], origLocale);
      }
    }
  }
}

/*
   Read one or more values from a :record:`fileReader`. The ``fileReader``'s
   lock will be held while reading the values — this protects against
   interleaved reads.

   :arg args: a series of variables to read into. Basic types are handled
              internally, but for other types this function will call
              value.deserialize() with a `fileReader` argument as described
              in :ref:`serialize-deserialize`.
   :returns: `true` if the read succeeded, and `false` on end of file.

   :throws UnexpectedEofError: If an EOF occurred while reading an item.
   :throws SystemError: If data could not be read from the ``fileReader``
                        for :ref:`another reason<io-general-sys-error>`.
 */
inline proc fileReader.read(ref args ...?k):bool throws {
  try {
    this._readInner((...args));
  } catch err: EofError {
    return false;
  }

  return true;
}

/*
  Read a line into an array of bytes.

  Reads bytes from the :record:`fileReader` until a ``\n`` is reached. Values
  are read in binary format (i.e., this method is not aware of UTF-8 encoding).

  The array's size is not changed to accommodate bytes. If a newline is not
  found before the array is filled, or ``maxSize`` bytes are read, a
  :class:`~OS.BadFormatError` is thrown and the ``fileReader`` offset is
  returned to the position it had when this routine was called.

  :arg a: A 1D DefaultRectangular non-strided array storing ``int(8)`` or
          ``uint(8)``. Values are overwritten.
  :arg maxSize: The maximum number of bytes to store into the ``a`` array.
                Defaults to the size of the array.
  :arg stripNewline: Whether to strip the trailing ``\n`` from the line. If
                     ``true``, the newline isn't counted in the number of
                     bytes read.
  :returns: The number of array elements set by this call, or ``0`` otherwise
            (i.e., the ``fileReader`` was already at EOF).

  :throws IllegalArgumentError: If ``maxSize > a.size``
  :throws BadFormatError: If the line is longer than ``maxSize``. The
                          fileReader's offset is not moved in that case.
  :throws SystemError: If data could not be read from the ``fileReader``
                       due to a :ref:`system error<io-general-sys-error>`.
                       In that event, the fileReader's offset is not moved
                       by this routine.
 */
proc fileReader.readLine(ref a: [] ?t, maxSize=a.size,
                         stripNewline=false): int throws
      where a.rank == 1 && a.isRectangular() && a.strides == strideKind.one &&
            (t == uint(8) || t == int(8))
{
  if maxSize > a.size
    then throw new IllegalArgumentError("'maxSize' argument exceeds size of array in readLine");
  if maxSize == 0 then return 0;

  var err:errorCode = 0;
  var numRead:int;
  on this._home {
    try this.lock(); defer { this.unlock(); }
    this.mark();
    param newLineChar = '\n'.toByte();
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
        this.revert();
        var err:errorCode = -got;
        try this._ch_ioerror(err, "in fileReader.readLine(a : [] uint(8))");
      }
      if got == newLineChar {
        foundNewline = true;
      }
      if i > maxIdx {
        if foundNewline && stripNewline {
          // don't worry about it since we wouldn't return the newline
        } else {
          // The line is longer than was specified so we throw an error
          this.revert();
          try this._ch_ioerror(EFORMAT:errorCode, "line longer than maxSize in fileReader.readLine(a : [] uint(8))");
        }
      }
      if !(foundNewline && stripNewline) {
        a[i] = got:uint(8);
        i += 1;
      }
    }
    numRead = i - a.domain.lowBound;
    if i == a.domain.lowBound && got < 0 then err = (-got):errorCode;
    this.commit();
  }

  if !err {
    return numRead;
  } else if err == EEOF {
    return 0;
  } else {
    try this._ch_ioerror(err, "in fileReader.readLine(a : [] uint(8))");
  }
  return 0;

}

pragma "last resort"
@chpldoc.nodoc
inline proc fileReader.readLine(ref a: [] ?t, maxSize=a.size,
                                stripNewline=false): int throws {
  compilerError("'readLine()' is currently only supported for non-strided 1D rectangular arrays");
}

// Helper function to replace the contents of a string or bytes
// by reading up to a fixed number of bytes / codepoints.
// Returns an error code, and ESHORT if less than that number of
// bytes was read.
// Does not validate that the string has valid encoding -- the call
// site should do that.
// Assumes we are already on the locale with the fileReader and that
// it is already locked.
// Passing -1 to 'nCodepoints' tells this function to compute the number
// of codepoints itself, and store the result in 'cachedNumCodepoints';
// additionally, it will check that the string is valid UTF-8.
@chpldoc.nodoc
proc readStringBytesData(ref s: ?t /*: string or bytes*/,
                         _channel_internal:qio_channel_ptr_t,
                         nBytes: int,
                         nCodepoints: int): errorCode {
  import BytesStringCommon;
  var sLoc: t;
  ref sLocal = if s.locale == here then s else sLoc;

  BytesStringCommon.resizeBuffer(sLocal, nBytes);

  // TODO: if the fileReader is working with non-UTF-8 data
  // (which is a feature not yet implemented at all)
  // this would need to call a read that can do character set conversion
  // in the event that s.type == string.

  var len:c_ssize_t = nBytes.safeCast(c_ssize_t);
  var err;
  if len != 0 {
    err = qio_channel_read_amt(false, _channel_internal, sLocal.buff, len);
  } else {
    err = 0:errorCode;
  }
  if !err {
    sLocal.buffLen = nBytes;
    if nBytes != 0 then sLocal.buff[nBytes] = 0; // include null-byte
    if t == string {
      if nCodepoints == -1 {
        // validate the string
        var byteI: c_ssize_t = 0;
        var codepointI: c_ssize_t = 0;
        while byteI < nBytes {
          var codepoint: int(32);
          var gotbytes: c_int;
          err = qio_decode_char_buf(codepoint, gotbytes,
                                    (sLocal.buff + byteI):c_ptrConst(c_char),
                                    len - byteI);
          if err then break;
          codepointI += 1;
          byteI += gotbytes;
        }
        sLocal.cachedNumCodepoints = codepointI;
      } else {
        sLocal.cachedNumCodepoints = nCodepoints;
      }
      sLocal.hasEscapes = false;
    }
  }

  if err {
    sLocal.buffLen = 0;
    if t == string {
      sLocal.cachedNumCodepoints = 0;
      sLocal.hasEscapes = false;
    }
  }

  if s.locale != here then s <=> sLoc;
  return err;
}

/*
  Read a line into a :type:`~String.string`. Reads until a ``\n`` is reached.

  :arg s: the :type:`~String.string` to read into. Contents are overwritten.
  :arg maxSize: The maximum number of codepoints to store into ``s``. The
                default of -1 means to read an unlimited number of codepoints.
  :arg stripNewline: Whether to strip the trailing ``\n`` from the line.
  :returns: ``true`` if a line was read without error, ``false`` upon EOF

  :throws BadFormatError: If the line is longer than `maxSize`. The
                          :record:`fileReader` offset is not moved.
  :throws SystemError: If data could not be read from the ``fileReader``
                       due to a :ref:`system error<io-general-sys-error>`.
                       In that event, the fileReader's offset is not moved
                       by this routine.
*/
proc fileReader.readLine(ref s: string,
                         maxSize=-1,
                         stripNewline=false): bool throws {
  const origLocale = this.getLocaleOfIoRequest();
  var ret: bool = false;

  on this._home {
    try this.lock(); defer { this.unlock(); }
    param newLineChar = '\n'.toByte();
    var maxCodepoints = if maxSize < 0 then max(int) else maxSize;
    var nCodepoints: int = 0; // num codepoints, including newline
    var chr : int(32);
    var err: errorCode = 0;
    var foundNewline = false;
    // use the fileReader's buffering to compute how many bytes/codepoints
    // we are reading
    this.mark();
    while !foundNewline {
      // read a single codepoint
      err = qio_channel_read_char(false, this._channel_internal, chr);
      if err == EEOF {
        // encountered EOF
        break;
      } else if err {
        // encountered an error so throw
        this.revert();
        try this._ch_ioerror(err, "in fileReader.readLine(ref s: string)");
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
          this.revert();
          try this._ch_ioerror(EFORMAT:errorCode,
               "line longer than maxSize in fileReader.readLine(ref s: string)");
        }
      }
    }
    var endOffset = this.offset();
    this.revert();
    var nBytes:int = endOffset - this.offset();
    // now, nCodepoints and nBytes include the newline
    if foundNewline && stripNewline {
      // but we don't want to read the newline if stripNewline=true.
      nBytes -= 1;
      nCodepoints -= 1;
    }

    // now read the data into the string
    // readStringBytesData will advance the fileReader by exactly `nBytes`.
    // This may consume or leave the newline based on the logic above.
    err = readStringBytesData(s, this._channel_internal, nBytes, nCodepoints);
    if foundNewline && stripNewline && !err {
      // pass the newline in the input
      err = qio_channel_read_char(false, this._channel_internal, chr);
    }

    if err != 0 && err != EEOF {
      try this._ch_ioerror(err, "in fileReader.readLine(ref s: string)");
    }

    // return 'true' if we read anything
    ret = foundNewline || nBytes > 0;
  }

  return ret;
}

/*
  Read a line into a :type:`~Bytes.bytes`. Reads until a ``\n`` is reached.

  :arg b: the :type:`~Bytes.bytes` to receive the line. Contents are overwritten.
  :arg maxSize: The maximum number of bytes to store into ``b``. The default of
                -1 means to read an unlimited number of bytes.
  :arg stripNewline: Whether to strip the trailing ``\n`` from the line.
  :returns: ``true`` if a line was read without error, ``false`` upon EOF

  :throws BadFormatError: If the line is longer than `maxSize`. The file
                          offset is not moved.
  :throws SystemError: If data could not be read from the :record:`fileReader`
                       due to a :ref:`system error<io-general-sys-error>`.
                       In that event, the fileReader's offset is not moved
                       by this routine.
*/
proc fileReader.readLine(ref b: bytes,
                         maxSize=-1,
                         stripNewline=false): bool throws {
  const origLocale = this.getLocaleOfIoRequest();
  var ret: bool = false;

  on this._home {
    try this.lock(); defer { this.unlock(); }
    param newLineChar = '\n'.toByte();
    var maxBytes = if maxSize < 0 then max(int) else maxSize;
    var nBytes: int = 0;
    // use the fileReader's buffering to compute how many bytes we are reading
    this.mark();
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
        this.revert();
        err = -got;
        try this._ch_ioerror(err, "in fileReader.readLine(ref b: bytes)");
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
          this.revert();
          try this._ch_ioerror(EFORMAT:errorCode,
                   "line longer than maxSize in fileReader.readLine(ref b: bytes)");
        }
      }
    }
    this.revert();
    // now, nBytes includes the newline
    if foundNewline && stripNewline {
      // but we don't want to read the newline if stripNewline=true.
      nBytes -= 1;
    }

    // now read the data into the bytes
    // readStringBytesData will advance the fileReader by exactly `nBytes`.
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
      try this._ch_ioerror(err, "in fileReader.readLine(ref b: bytes)");
    }

    // return 'true' if we read anything
    ret = foundNewline || nBytes > 0;
  }

  return ret;
}

/*
  Read a line. Reads until a ``\n`` is reached.

  :arg t: the type of data to read, which must be :type:`~String.string` or
          :type:`~Bytes.bytes`. Defaults to ``string`` if not specified.
  :arg maxSize: The maximum number of codepoints to read. The default of -1
                means to read an unlimited number of codepoints.
  :arg stripNewline: Whether to strip the trailing ``\n`` from the line.
  :returns: A ``string`` or ``bytes`` with the contents of the
            :record:`fileReader` up to (and possibly including) the newline.

  :throws EofError: If nothing could be read because the ``fileReader``
                    was already at EOF.
  :throws BadFormatError: If the line is longer than `maxSize`. The file
                          offset is not moved.
  :throws SystemError: If data could not be read from the ``fileReader``
                       due to a :ref:`system error<io-general-sys-error>`.
                       In that event, the fileReader's offset is not moved
                       by this routine.
*/
proc fileReader.readLine(type t=string, maxSize=-1,
                         stripNewline=false): t throws where t==string || t==bytes {
  var retval: t;
  if !this.readLine(retval, maxSize, stripNewline)
    then throw new EofError("Encountered EOF in readLine");
  return retval;
}


/*
  Read until the given separator is found, returning the contents of the
  :record:`fileReader` through that point.

  If the separator is found, the ``fileReader`` offset is left immediately
  after it. If the separator could not be found in the next ``maxSize`` bytes,
  a :type:`~OS.BadFormatError` is thrown and the ``fileReader``'s offset is not
  changed. Otherwise, if EOF is reached before finding the separator, the
  remainder of the ``fileReader``'s contents are returned and the offset is
  left at EOF.

  To match with multiple separators, or a more complex separator, use the
  overload of :proc:`~Regex.fileReader.readThrough` that accepts a
  :type:`~Regex.regex` separator.

  :arg separator: The separator to match with. Must be a :type:`~String.string`
                  or :type:`~Bytes.bytes`.
  :arg maxSize: The maximum number of bytes (for t==bytes) or codepoints (for
                t==string) to read. For the default value of
                ``-1``, this method can read until EOF.
  :arg stripSeparator: Whether to strip the separator from the returned
                        ``string`` or ``bytes``. If ``true``, the returned
                        value will not include the separator.
  :returns: A ``string`` or ``bytes`` with the contents of the ``fileReader``
            up to (and possibly including) the separator.

  :throws IllegalArgumentError: If the separator is empty
  :throws EofError: If nothing could be read because the ``fileReader``
                    was already at EOF.
  :throws BadFormatError: If the separator was not found in the next
                          `maxSize` bytes. The fileReader offset is not moved.
  :throws SystemError: If data could not be read from the ``fileReader``
                       due to a :ref:`system error<io-general-sys-error>`.
                       In that event, the fileReader's offset is not moved
                       by this routine.
*/
proc fileReader.readThrough(separator: ?t, maxSize=-1, stripSeparator=false): t throws
  where t==string || t==bytes
{
  var ret: t;
  if !this.readThrough(separator, ret, maxSize, stripSeparator)
    then throw new EofError("Encountered EOF in readThrough(" + t:string + ")");
  return ret;
}

/*
  Read until the given separator is found, returning the contents of the
  :record:`fileReader` through that point.

  See the above :proc:`overload <fileReader.readThrough>` of this method for
  more details.

  :arg separator: The separator to match with.
  :arg s: The :type:`~String.string` to read into. Contents will be overwritten.
  :arg maxSize: The maximum number of codepoints to read. For the default value
                of ``-1``, this method can read until EOF.
  :arg stripSeparator: Whether to strip the separator from the returned ``string``.
                       If ``true``, the separator will not be included in ``s``.
  :returns: ``true`` if something was read, and ``false`` otherwise (i.e., the
            ``fileReader`` was already at EOF).

  :throws IllegalArgumentError: If the separator is empty
  :throws BadFormatError: If the separator was not found in the next
                          `maxSize` bytes. The fileReader offset is not moved.
  :throws SystemError: If data could not be read from the ``fileReader``
                       due to a :ref:`system error<io-general-sys-error>`.
                       In that event, the fileReader's offset is not moved
                       by this routine.
*/
proc fileReader.readThrough(separator: string, ref s: string, maxSize=-1, stripSeparator=false): bool throws {
  on this._home {
    try this.lock(); defer { this.unlock(); }

    if separator.numBytes == 0 {
      throw new IllegalArgumentError("readThrough(string) called with empty separator");
    }

    // performance TODO: investigate using qio_channel_read_string as a fast path for single-byte separators
    //  (this would be a single pass and would not require retroactive codepoint checking)

    // find the byte offset to the start of the separator, 'maxSize' bytes, or EOF (whichever comes first)
    const (searchErr, found, bytesRead) = _findSeparator(separator, 4*maxSize, this._channel_internal);
    // handle system error & not found within maxSize
    if searchErr != 0 && searchErr != EEOF && searchErr != ESHORT {
      try this._ch_ioerror(searchErr, "in readThrough(string)");
    }

    // compute the number of bytes to read into 's'
    const bytesToRead = if found then bytesRead + separator.numBytes else bytesRead;

    // read the given number of bytes into 's', advancing the pointer that many bytes
    // then, ensure the number of codepoints does not exceed the specified maxSize
    qio_channel_mark(false, this._channel_internal);
    const err = readStringBytesData(s, this._channel_internal, bytesToRead, -1);
    if err {
      qio_channel_revert_unlocked(this._channel_internal);
      try this._ch_ioerror(err, "in readThrough(string)");
    } else {
      if maxSize >= 0 && s.numCodepoints > maxSize {
        qio_channel_revert_unlocked(this._channel_internal);
        try this._ch_ioerror(EFORMAT:errorCode, "in readThrough(string)");
      } else {
        qio_channel_commit_unlocked(this._channel_internal);
      }
    }

    // remove the separator from the returned string if necessary
    // TODO: Do this more efficiently in 'readStringBytesData'
    if found && stripSeparator then s = s[0..<s.numCodepoints-separator.numCodepoints];
  }
  return s.size > 0;
}

/*
  Read until the given separator is found, returning the contents of the
  :record:`fileReader` through that point.

  See the above :proc:`overload <fileReader.readThrough>` of this method for
  more details.

  :arg separator: The separator to match with.
  :arg b: The :type:`~Bytes.bytes` to read into. Contents will be overwritten.
  :arg maxSize: The maximum number of bytes to read. For the default value
                of ``-1``, this method can read until EOF.
  :arg stripSeparator: Whether to strip the separator from the returned ``bytes``.
                       If ``true``, the separator will not be included in ``b``.
  :returns: ``true`` if something was read, and ``false`` otherwise (i.e., the
            ``fileReader`` was already at EOF).

  :throws IllegalArgumentError: If the separator is empty
  :throws BadFormatError: If the separator was not found in the next
                          ``maxSize`` bytes. The fileReader offset is not moved.
  :throws SystemError: If data could not be read from the ``fileReader``
                       due to a :ref:`system error<io-general-sys-error>`.
                       In that event, the fileReader's offset is not moved
                       by this routine.
*/
proc fileReader.readThrough(separator: bytes, ref b: bytes, maxSize=-1, stripSeparator=false): bool throws {
  on this._home {
    try this.lock(); defer { this.unlock(); }

    if separator.numBytes == 0 {
      throw new IllegalArgumentError("readThrough(bytes) called with empty separator");
    }

    // find the byte offset to the start of the separator, 'maxSize' bytes, or EOF (whichever comes first)
    const (searchErr, found, bytesRead) = _findSeparator(separator, maxSize, this._channel_internal);
    if searchErr != 0 && searchErr != EEOF && searchErr != ESHORT {
      try this._ch_ioerror(searchErr, "in readThrough(bytes)");
    }

    // compute the number of bytes to read into 'b'
    const bytesToRead = if found then bytesRead + separator.numBytes else bytesRead;

    // read the given number of bytes into 'b'
    const err = readStringBytesData(b, this._channel_internal, bytesToRead, 0);
    if err then try this._ch_ioerror(err, "in readThrough(bytes)");

    // remove the separator from the returned string if necessary
    // TODO: Do this more efficiently in 'readStringBytesData'
    if found && stripSeparator then b = b[0..<b.numBytes-separator.numBytes];
  }
  return b.size > 0;
}

/*
  Read until the given separator is found, returning the contents of the
  :record:`fileReader` up to that point.

  If the separator is found, the ``fileReader`` offset is left immediately
  before it. If the separator could not be found in the next ``maxSize`` bytes,
  a :type:`~OS.BadFormatError` is thrown and the ``fileReader``'s offset is not
  changed. Otherwise, if EOF is reached before finding the separator, the
  remainder of the ``fileReader``'s contents are returned and the offset is
  left at EOF.

  To match with multiple separators, or a more complex separator, use the
  overload of :proc:`~Regex.fileReader.readTo` that accepts a
  :type:`~Regex.regex` separator.

  :arg separator: The separator to match with. Must be a :type:`~String.string`
                  or :type:`~Bytes.bytes`.
  :arg maxSize: The maximum number of bytes (for t==bytes) or codepoints (for
                t==string) to read. For the default
                value of ``-1``, this method can read until EOF.
  :returns: A ``string`` or ``bytes`` with the contents of the ``fileReader``
            up to the ``separator``.

  :throws IllegalArgumentError: If the separator is empty
  :throws EofError: If nothing could be read because the ``fileReader``
                    was already at EOF.
  :throws BadFormatError: If the separator was not found in the next
                          ``maxSize`` bytes. The ``fileReader`` offset is not
                          moved.
  :throws SystemError: If data could not be read from the ``fileReader``
                       due to a :ref:`system error<io-general-sys-error>`.
                       In that event, the fileReader's offset is not moved
                       by this routine.
*/
proc fileReader.readTo(separator: ?t, maxSize=-1): t throws
  where t==string || t==bytes
{
  var ret: t;
  if !this.readTo(separator, ret, maxSize)
    then throw new EofError("Encountered EOF in readTo(" + t:string + ")");
  return ret;
}


/*
  Read until the given separator is found, returning the contents of the
  :record:`fileReader` up to that point.

  See the above :proc:`overload <fileReader.readTo>` of this method for
  more details.

  :arg separator: The separator to match with.
  :arg s: The :type:`~String.string` to read into. Contents will be overwritten.
  :arg maxSize: The maximum number of codepoints to read. For the default value
                of ``-1``, this method will read until EOF.
  :returns: ``true`` if something was read, and ``false`` otherwise (i.e., the
            ``fileReader`` was already at EOF).

  :throws IllegalArgumentError: If the separator is empty
  :throws BadFormatError: If the separator was not found in the next
                          `maxSize` bytes. The ``fileReader`` offset is not
                          moved.
  :throws SystemError: If data could not be read from the ``fileReader``
                       due to a :ref:`system error<io-general-sys-error>`.
                       In that event, the fileReader's offset is not moved
                       by this routine.
*/
proc fileReader.readTo(separator: string, ref s: string, maxSize=-1): bool throws {
  var atEof = false;
  on this._home {
    try this.lock(); defer { this.unlock(); }

    if separator.numBytes == 0 {
      throw new IllegalArgumentError("readTo(string) called with empty separator");
    }

    // performance TODO: investigate using qio_channel_read_string as a fast path for single-byte separators
    //  (this would be a single pass and would not require retroactive codepoint checking)

    const (searchErr, _, bytesRead) = _findSeparator(separator, 4*maxSize, this._channel_internal);
    if searchErr != 0 && searchErr != EEOF && searchErr != ESHORT {
      try this._ch_ioerror(searchErr, "in fileReader.readTo(string)");
    }
    atEof = searchErr == EEOF && bytesRead == 0;

    // read the given number of bytes into 's', advancing the pointer that many bytes
    // then, ensure the number of codepoints does not exceed the specified maxSize
    qio_channel_mark(false, this._channel_internal);
    const err = readStringBytesData(s, this._channel_internal, bytesRead, -1);
    if err {
      qio_channel_revert_unlocked(this._channel_internal);
      try this._ch_ioerror(err, "in fileReader.readTo(string)");
    } else {
      if maxSize >= 0 && s.numCodepoints >= maxSize {
        qio_channel_revert_unlocked(this._channel_internal);
        try this._ch_ioerror(EFORMAT:errorCode, "in fileReader.readTo(string)");
      } else {
        qio_channel_commit_unlocked(this._channel_internal);
      }
    }
  }
  return !atEof;
}

/*
  Read until the given separator is found, returning the contents of the
  :record:`fileReader` up to that point.

  See the above :proc:`overload <fileReader.readTo>` of this method for
  more details.

  :arg separator: The separator to match with.
  :arg b: The :type:`~Bytes.bytes` to read into. Contents will be overwritten.
  :arg maxSize: The maximum number of bytes to read. For the default value
                of ``-1``, this method will read until EOF.
  :returns: ``true`` if something was read, and ``false`` otherwise (i.e., the
            ``fileReader`` was already at EOF).

  :throws IllegalArgumentError: If the separator is empty
  :throws BadFormatError: If the separator was not found in the next
                          ``maxSize`` bytes. The ``fileReader`` offset is not
                          moved.
  :throws SystemError: If data could not be read from the ``fileReader``
                       due to a :ref:`system error<io-general-sys-error>`.
                       In that event, the fileReader's offset is not moved
                       by this routine.
*/
proc fileReader.readTo(separator: bytes, ref b: bytes, maxSize=-1): bool throws {
  var atEof = false;
  on this._home {
    try this.lock(); defer { this.unlock(); }

    if separator.numBytes == 0 {
      throw new IllegalArgumentError("readTo(bytes) called with empty separator");
    }

    const (searchErr, _, bytesRead) = _findSeparator(separator, maxSize, this._channel_internal);
    if searchErr != 0 && searchErr != EEOF && searchErr != ESHORT {
      try this._ch_ioerror(searchErr, "in fileReader.readTo(bytes)");
    }
    atEof = searchErr == EEOF && bytesRead == 0;

    const err = readStringBytesData(b, this._channel_internal, bytesRead, 0);
    if err then try this._ch_ioerror(err, "in fileReader.readTo(bytes)");
  }
  return !atEof;
}

/* helper for: readTo, readThrough, advanceThrough, advanceTo

  looks for a sequence of bytes matching 'separator' in the
  next 'maxSize' bytes in the channel

  does not move the channel's pointer

 returns: (0, true, bytes_to_sep) if found
          (EFORMAT, false, maxBytes) if not found within maxBytes
          (EFORMAT, false, 0) if separator is empty
          (EEOF, false, 0) if the channel was already at EOF
          (ESHORT, false, bytes_to_eof) if reached EOF before the pattern
          (error_code, false, 0) system error
*/
private proc _findSeparator(separator: ?t, maxBytes=-1, ch_internal): (errorCode, bool, int)
  where t==string || t==bytes
{
  if separator.isEmpty() then return (EFORMAT:errorCode, false, 0);

  const maxToRead = if maxBytes < 0 then max(int) else maxBytes,
        sepLocal = separator.localize(),
        numSepBytes = sepLocal.numBytes,
        firstByte = sepLocal.byte(0);

  var nextByte: int,
      err: errorCode = 0,
      foundSeparator = false,
      numMatched = 0;

  qio_channel_mark(false, ch_internal); // A
  while true {
    // advance to the the first byte in the separator
    //  (separator's first byte is intentionally not consumed here
    //   so that reverting B puts the pointer **before** the separator)
    err = qio_channel_advance_past_byte(false, ch_internal, firstByte,
                                        maxToRead, /* consume */ false);
    if err == EEOF || err == ESHORT {
      // do further checking for the rest of the separator below
      break;
    } else if err == EFORMAT {
      qio_channel_revert_unlocked(ch_internal); // A
      return (err, false, maxBytes);
    } else if err {
      qio_channel_revert_unlocked(ch_internal); // A
      return (err, false, 0);
    }

    // try to match the entire separator if we aren't at EOF already
    if err == 0 {
      qio_channel_mark(false, ch_internal); // B
      numMatched = 0;
      for i in 0..<numSepBytes {
        // read a byte to match with
        nextByte = qio_channel_read_byte(false, ch_internal);

        // check for errors
        if nextByte < 0 {
          err = -nextByte;
          if err == EEOF {
            break;
          } else {
            qio_channel_revert_unlocked(ch_internal); // B
            qio_channel_revert_unlocked(ch_internal); // A
            return (err, false, 0);
          }
        }

        if nextByte == sepLocal.byte(i)
          then numMatched += 1;
          else break;
      }
      qio_channel_revert_unlocked(ch_internal); // B

      if numMatched == numSepBytes {
        foundSeparator = true;
        break;
      }

      // consume an additional byte to move the search forward
      nextByte = qio_channel_read_byte(false, ch_internal);
      if nextByte < 0 {
        err = -nextByte;
        if err == EEOF {
          break;
        } else {
          qio_channel_revert_unlocked(ch_internal); // A
          return(err, false, 0);
        }
      }
    }
  }
  // move the channel pointer back to its starting position (A)
  // compute the number of bytes from A to the start of the separator (or to EOF/maxBytes if it wasn't found)
  const endOffset = qio_channel_offset_unlocked(ch_internal);
  qio_channel_revert_unlocked(ch_internal); // A
  const numBytesRead: int = endOffset - qio_channel_offset_unlocked(ch_internal);

  // should return EFORMAT if separator wasn't found before maxBytes bytes were read
  // (if reading a string, the number of codepoints will also need to be checked later)
  if !foundSeparator {
    if numBytesRead >= maxToRead {
      err = EFORMAT:errorCode;
    } else if err == EEOF || err == ESHORT {
      // choose between EEOF and ESHORT according to if some data was read
      if numBytesRead > 0 then err = ESHORT:errorCode;
      else err = EEOF:errorCode;
    }
  } else err = 0; // such errors should have been returned already above

  return (err, foundSeparator, numBytesRead);
}


/*
  Read the remaining contents of the :record:`fileReader` into an instance of
  the specified type

  :arg t: the type to read into; must be :type:`~String.string` or
          :type:`~Bytes.bytes`. Defaults to ``bytes`` if not specified.
  :returns: the contents of the ``fileReader`` as a ``t``

  :throws EofError: If nothing could be read because the ``fileReader``
                    was already at EOF.
  :throws SystemError: If data could not be read from the ``fileReader``
                       due to a :ref:`system error<io-general-sys-error>`.
                       In that event, the fileReader's offset will be
                       left near the position where the error occurred.
*/
proc fileReader.readAll(type t=bytes): t throws
  where t==string || t==bytes
{
  var out_var : t,
      num_read = 0;

  if t == bytes {
    out_var = b"";
    num_read = this.readAll(out_var);
  } else {
    out_var = "";
    num_read = this.readAll(out_var);
  }

  if num_read == 0 then throw new EofError("EOF encountered in readAll");
  return out_var;
}

/*
  Read the remaining contents of the :record:`fileReader` into a ``string``.

  Note that any existing contents of the ``string`` are overwritten.

  :arg s: the :type:`~String.string` to read into
  :returns: the number of codepoints that were stored in ``s``, or 0 if
            the ``fileReader`` is at EOF.
  :rtype: int

  :throws SystemError: If data could not be read from the ``fileReader``
                       due to a :ref:`system error<io-general-sys-error>`.
                       In that event, the fileReader's offset will be
                       left near the position where the error occurred.
*/
proc fileReader.readAll(ref s: string): int throws {
  const (err, lenread) = readStringImpl(this, s, -1);

  if err != 0 && err != EEOF {
    try this._ch_ioerror(err, "in fileReader.readAll(ref s: string)");
  }

  return lenread;
}

/*
  Read the remaining contents of the :record:`fileReader` into a ``bytes``.

  Note that any existing contents of the ``bytes`` are overwritten.

  :arg b: the :type:`~Bytes.bytes` to read into
  :returns: the number of bytes that were stored in ``b``, or 0 if
            the ``fileReader`` is at EOF.
  :rtype: int

  :throws SystemError: If data could not be read from the ``fileReader``
                       due to a :ref:`system error<io-general-sys-error>`.
                       In that event, the fileReader's offset will be
                       left near the position where the error occurred.
*/
proc fileReader.readAll(ref b: bytes): int throws {
  const (err, lenread) = readBytesImpl(this, b, -1);

  if err != 0 && err != EEOF {
    try this._ch_ioerror(err, "in fileReader.readAll(ref b: bytes)");
  }

  return lenread;
}

/*
  Read the remaining contents of the :record:`fileReader` into an array of
  bytes.

  Note that this routine currently requires a 1D rectangular non-strided array.

  If the remaining contents of the fileReader exceed the size of ``a``, the
  first ``a.size`` bytes will be read into ``a``, and then an
  :class:`~OS.InsufficientCapacityError` will be thrown. In such a case, the
  ``fileReader`` offset is advanced ``a.size`` bytes from its original
  position.

  :arg a: the array of bytes to read into
  :returns: the number of bytes that were stored in ``a``
  :rtype: int

  :throws InsufficientCapacityError: If the fileReader's contents do not
                                     fit into ``a``.
  :throws SystemError: If data could not be read from the ``fileReader``
                       due to a :ref:`system error<io-general-sys-error>`.
                       In that event, the fileReader's offset will be
                       left near the position where the error occurred.
*/
proc fileReader.readAll(ref a: [?d] ?t): int throws
  where a.rank == 1 && a.isRectangular() && a.strides == strideKind.one &&
        (t == uint(8) || t == int(8))
{
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
        try this._ch_ioerror((-got):errorCode,
                             "in fileReader.readAll(ref a: [])");
      } else {
        // got a byte, store it
        a[i] = got:t;
        i += 1;
      }
    }

    // if a is full, but we haven't reached EOF, throw
    if i-1 == d.high {
      var has_more = false;

      this.mark();
      got = qio_channel_read_byte(false, this._channel_internal);
      has_more = (got >= 0);
      this.revert();

      if has_more {
        const sz = qio_channel_get_size(this._channel_internal);
        const err_msg = "FileReader's contents" + (if sz == -1 then " " else " (" + sz:string + " bytes) ") +
          "exceeded capacity of array argument (" + a.size:string + " bytes) in 'readAll'";

        throw new owned InsufficientCapacityError(err_msg);
      }
    }
  }

  return (i - d.low);
}

/*
  Read a given number of codepoints from a :record:`fileReader`, returning a new
  :type:`~String.string`.

  The ``string``'s length may be less than ``maxSize`` if EOF is reached while
  reading. If nothing is read, the empty string (``""``) will be returned.

  :arg maxSize: the maximum number of codepoints to read from the ``fileReader``
  :returns: a new ``string`` containing up to the next ``maxSize`` codepoints
              from the ``fileReader``

  :throws EofError: If the ``fileReader`` offset was already at EOF.
  :throws SystemError: If data could not be read from the ``fileReader``
                       due to a :ref:`system error<io-general-sys-error>`.
                       In that event, the fileReader's offset will be
                       left near the position where the error occurred.
*/
proc fileReader.readString(maxSize: int): string throws {
  var ret: string = "";
  var (e, numRead) = readStringImpl(this, ret, maxSize);

  if e != 0 && e != EEOF then throw createSystemError(e);
  else if e == EEOF && numRead == 0 then
    throw new EofError("EOF encountered in readString");

  return ret;
}

/*
  Read a given number of codepoints from a :record:`fileReader` into a
  :type:`~String.string`.

  The updated ``string``'s length may be less than ``maxSize`` if EOF is
  reached while reading. If nothing is read, it will be set to the empty
  string (``""``).

  :arg s: the ``string`` to read into — contents will be overwritten
  :arg maxSize: the maximum number of codepoints to read from the ``fileReader``
  :returns: ``true`` if something was read, and ``false`` otherwise (i.e., the
            ``fileReader`` was already at EOF).

  :throws SystemError: If data could not be read from the ``fileReader``
                       due to a :ref:`system error<io-general-sys-error>`.
                       In that event, the fileReader's offset will be
                       left near the position where the error occurred.
*/
proc fileReader.readString(ref s: string, maxSize: int): bool throws {
  var (e, lenRead) = readStringImpl(this, s, maxSize);

  if e != 0 && e != EEOF then throw createSystemError(e);

  return lenRead > 0;
}

/*
  Read a given number of bytes from a :record:`fileReader`, returning a new
  :type:`~Bytes.bytes`.

  The ``bytes``'s length may be less than ``maxSize`` if EOF is reached while
  reading. If nothing is read, the empty bytes (``b""``) will be returned.

  :arg maxSize: the maximum number of bytes to read from the ``fileReader``
  :returns: a new ``bytes`` containing up to the next ``maxSize`` bytes
            from the ``fileReader``

  :throws EofError: If the ``fileReader`` offset was already at EOF.
  :throws SystemError: If data could not be read from the ``fileReader``
                       due to a :ref:`system error<io-general-sys-error>`.
                       In that event, the fileReader's offset will be
                       left near the position where the error occurred.
*/
proc fileReader.readBytes(maxSize: int): bytes throws {
  var ret: bytes = b"";
  var (e, numRead) = readBytesImpl(this, ret, maxSize);

  if e != 0 && e != EEOF then throw createSystemError(e);
  else if e == EEOF && numRead == 0 then
    throw new EofError("EOF encountered in readBytes");

  return ret;
}

/*
  Read a given number of bytes from a :record:`fileReader` into a
  :type:`~Bytes.bytes`.

  The updated ``bytes``'s length may be less than ``maxSize`` if EOF is
  reached while reading. If nothing is read, it will be set to the empty
  bytes (``b""``).

  :arg b: the ``bytes`` to read into — contents will be overwritten
  :arg maxSize: the maximum number of bytes to read from the ``fileReader``
  :returns: ``true`` if something was read, and ``false`` otherwise (i.e., the
            ``fileReader`` was already at EOF).

  :throws SystemError: If data could not be read from the ``fileReader``
                       due to a :ref:`system error<io-general-sys-error>`.
                       In that event, the fileReader's offset will be
                       left near the position where the error occurred.
*/
proc fileReader.readBytes(ref b: bytes, maxSize: int): bool throws {
  var (e, lenRead) = readBytesImpl(this, b, maxSize);

  if e != 0 && e != EEOF then throw createSystemError(e);

  return lenRead > 0;
}

// helper function to compute the length to read (in bytes)
// assumes that the fileReader is already locked
private proc computeMaxBytesToRead(ch: fileReader,
                                   len: int,
                                   pos: int,
                                   type t): c_ssize_t {
  var uselen: c_ssize_t;

  if len < 0 {
    uselen = max(c_ssize_t);
  } else {
    uselen = len:c_ssize_t;
    if c_ssize_t != int(64) {
      assert( len == uselen );
    }
    if t == string {
      // len is in codepoints, but each codepoint could be 4 bytes
      uselen = 4*uselen;
    }
  }

  // adjust uselen according to the channel's region,
  // which cannot change during this read
  var end = qio_channel_end_offset_unlocked(ch._channel_internal);
  if end != max(int(64)) {
    // if the channel had an end position set, compute distance to it
    var channelLen = (end - pos):c_ssize_t;
    if channelLen < uselen {
      uselen = channelLen + 1; // +1 to get an EOF back even if already there
    }
  }

  assert(uselen >= 0);

  return uselen;
}

// helper function to compute the initial buffer size when reading
// to a string/bytes
private
proc computeGuessReadSize(ch: fileReader, maxChars: c_ssize_t, pos: int): c_ssize_t {
  var guessReadSize:c_ssize_t = 0;
  var fp: qio_file_ptr_t = nil;
  qio_channel_get_file_ptr(ch._channel_internal, fp);
  var fileLen:int(64) = -1;
  if fp {
    if maxChars == max(c_ssize_t) {
      // try to find the file size with stat etc when doing readAll
      var err:errorCode = qio_file_length(fp, fileLen);
      // if there was an error, ignore it, but don't use the file length
      if err then fileLen = 0;
    } else {
      // use the file length from when it was opened to avoid overhead
      fileLen = qio_file_length_guess(fp);
    }
  }
  if pos >= 0 && fileLen >= 1 && fileLen > pos {
    guessReadSize = (fileLen - pos):c_ssize_t;
  }

  // limit the size to read by maxBytes
  if guessReadSize > maxChars {
    guessReadSize = maxChars;
  }

  assert(guessReadSize >= 0);

  return guessReadSize;
}

private proc readBytesImpl(ch: fileReader, ref out_var: bytes, len: int(64)) : (errorCode, int(64))
  throws
{
  var err:errorCode = 0;
  var lenread:int(64);

  on ch._home {
    try ch.lock(); defer { ch.unlock(); }

    // note the current channel position
    var pos = qio_channel_offset_unlocked(ch._channel_internal);

    // Compute the maximum amount we could read as a 'c_ssize_t'
    // based upon 'len' and the channel's region.
    // This handles len==-1 as well as a channel with a bounded region.
    const maxBytes:c_ssize_t = computeMaxBytesToRead(ch, len, pos, bytes);

    // Compute a guess as to the size to read based on the file length.
    // This is only a guess & it's possible it will be out of date
    // by the time we actually read.
    // We'll use this guess to decide how much to allocate up-front.
    // We don't want to allocate all of 'len' up front if it's bigger
    // than the observed channel size or the initial file size.
    const guessReadSize:c_ssize_t =
      computeGuessReadSize(ch, maxBytes, pos)+1; // +1 for trailing \0

    // proactively allocate 'guessReadSize'
    var buff: bufferType = nil;
    var buffSz = 0;
    var n:c_ssize_t = 0; // how many bytes have we read into buff?
    (buff, buffSz) = bufferAlloc(guessReadSize); // room for trailing \0

    // then try to read repeatedly until we have read 'maxBytes' or reach EOF
    while n < maxBytes {
      var locErr:errorCode = 0;
      var amtRead:c_ssize_t = 0;
      if n >= buffSz {
        // if we need more room in the buffer, grow it
        // this will happen if we have not read all of 'maxBytes' yet
        // but there is more data in the file (as when guessReadSize
        // was inaccurate for one reason or another)
        var requestSz = 2*buffSz;
        // make sure to at least request 16 bytes
        if requestSz < n + 16 then requestSz = n + 16;
        // but don't ever ask for more bytes than maxBytes + 1
        if maxBytes < max(c_ssize_t) && requestSz > maxBytes + 1 then
           requestSz = maxBytes + 1;
        (buff, buffSz) = bufferEnsureSize(buff, buffSz, requestSz);
        assert(n < buffSz);
      }
      const readN = min(maxBytes - n,           // Don't exceed max byte count
                        buffSz:c_ssize_t - n);  // Or allocated buffer space

      locErr = qio_channel_read(false, ch._channel_internal,
                                buff[n], // read starting with data here
                                readN, amtRead);

      n += amtRead;

      if locErr {
        // reached EOF or other error so we need to stop
        err = locErr;
        break;
      }
    }

    // add the trailing \0
    (buff, buffSz) = bufferEnsureSize(buff, buffSz, n+1);
    buff[n] = 0;

    var tmp: bytes = bytes.createAdoptingBuffer(buff, length=n, size=buffSz);
    out_var <=> tmp;
    lenread = n;
  }

  return (err, lenread);
}

// read up to 'len' codepoints of string data (less if we reach EOF)
// if 'len' is negative, read until EOF
// stores the result in 'out_var'.
private proc readStringImpl(ch: fileReader, ref out_var: string, len: int(64)) : (errorCode, int(64))
  throws
{
  var err:errorCode = 0;
  var lenread:int(64);

  on ch._home {
    try ch.lock(); defer { ch.unlock(); }

    // note the current channel position
    var pos = qio_channel_offset_unlocked(ch._channel_internal);

    // Compute the maximum amount we could read as a 'c_ssize_t'
    // based upon 'len' and the channel's region.
    // This is an amount in bytes.
    const maxBytes:c_ssize_t = computeMaxBytesToRead(ch, len, pos, string);
    // Compute the maximum number of codepoints we could read
    const maxChars:c_ssize_t = if len < 0 then max(c_ssize_t) else len:c_ssize_t;

    // Compute a guess as to the size to read based on the file length,
    // assuming 1-byte-per-codepoint.
    const guessReadSize:c_ssize_t = computeGuessReadSize(ch, maxChars, pos)+5;
          // +5 -- room for 4 bytes per codepoint + 1 byte for trailing \0

    // proactively allocate 'guessReadSize'
    var buff: bufferType = nil;
    var buffSz = 0;
    var n:c_ssize_t = 0; // how many bytes have we read into buff?
    var nChars:c_ssize_t = 0; // how many codepoints have we read?
    (buff, buffSz) = bufferAlloc(guessReadSize);

    // then try to read repeatedly until we have read 'maxChars' or reach EOF
    while nChars < maxChars {
      var locErr:errorCode = 0;

      if n + 5 > buffSz {
        var requestSz = 2*buffSz;
        // make sure to at least request 16 bytes
        if requestSz < n + 16 then requestSz = n + 16;
        // but don't ever ask for more bytes than maxBytes + 5
        if maxBytes < max(c_ssize_t) && requestSz > maxBytes + 5 then
          requestSz = maxBytes + 5;
        (buff, buffSz) = bufferEnsureSize(buff, buffSz, requestSz);
        assert(n + 5 < buffSz);
      }

      const bytesRemaining = buffSz:c_ssize_t - n;
      const charsRemaining = if maxChars < max(c_ssize_t)
                             then maxChars - nChars
                             else max(c_ssize_t);
      var readCodepoints:c_ssize_t = 0;
      var readBytes:c_ssize_t = 0;
      locErr = qio_channel_read_chars(false, ch._channel_internal,
                                      buff[n], // store starting here
                                      bytesRemaining,
                                      charsRemaining,
                                      readBytes,
                                      readCodepoints);

      nChars += readCodepoints;
      n += readBytes;

      if locErr {
        // reached EOF or other error so we need to stop
        err = locErr;
        break;
      }

      // should have read something if there was no error
      assert(readBytes > 0);
    }

    // add the trailing \0
    (buff, buffSz) = bufferEnsureSize(buff, buffSz, n+1);
    buff[n] = 0;

    var tmp: string =
      NVStringFactory.chpl_createStringWithOwnedBufferNV(buff,
                                                         length=n,
                                                         size=buffSz,
                                                         numCodepoints=nChars);

    out_var <=> tmp;
    lenread = n;
  }

  return (err, lenread);
}

/*
   Read bits with binary I/O

   :arg x: where to store the read bits. This value will have its *numBits*
           least-significant bits set.
   :arg numBits: how many bits to read
   :returns: ``true`` if the bits were read, and ``false`` otherwise (i.e., the
             :record:`fileReader` was already at EOF).

   :throws UnexpectedEofError: If EOF was encountered before ``numBits``
                               could be read.
   :throws SystemError: If data could not be read from the ``fileReader``
                        due to a :ref:`system error<io-general-sys-error>`.
 */
proc fileReader.readBits(ref x:integral, numBits:int):bool throws {
  if castChecking {
    // Error if reading more bits than fit into x
    if Types.numBits(x.type) < numBits then
      throw new owned IllegalArgumentError("readBits numBits=" + numBits:string +
                                                 " > bits in x:" + x.type:string);
    // Error if reading negative number of bits
    if isIntType(numBits.type) && numBits < 0 then
      throw new owned IllegalArgumentError("readBits numBits=" + numBits:string + " < 0");
  }

  var tmp:_internalIoBits;
  tmp.numBits = numBits:int(8);
  var ret = try this.read(tmp);
  x = tmp.x:x.type;
  return ret;
}

/*
    Read bits with binary I/O

    :arg resultType: type of the value returned
    :arg numBits: how many bits to read
    :returns: bits read. This value will have its *numBits* least-significant
              bits set

    :throws EofError: If the :record:`fileReader` offset was already at EOF.
    :throws UnexpectedEofError: If EOF was encountered before ``numBits``
                                could be read.
    :throws SystemError: If data could not be read from the ``fileReader``
                         due to a :ref:`system error<io-general-sys-error>`.
*/
proc fileReader.readBits(type resultType, numBits:int):resultType throws {
  var tmp:resultType;
  var ret = try this.readBits(tmp, numBits);
  if !ret then throw new EofError("EOF Encountered in readBits");
  return tmp;
}

/*
  Write bits with binary I/O

  :arg x: a value containing *numBits* bits to write the least-significant bits
  :arg numBits: how many bits to write

  :throws EofError: If the :record:`fileWriter` offset was already at EOF.
  :throws UnexpectedEofError: If the write operation exceeds the
                              ``fileWriter``'s specified range.
  :throws IllegalArgumentError: If writing more bits than fit into `x`.
  :throws SystemError: If data could not be written to the ``fileWriter``
                       due to a :ref:`system error<io-general-sys-error>`.
*/
proc fileWriter.writeBits(x: integral, numBits: int) : void throws {
  if castChecking {
    // Error if writing more bits than fit into x
    if Types.numBits(x.type) < numBits then
      throw new owned IllegalArgumentError("writeBits numBits=" +
        numBits:string + " > bits in x:" + x.type:string);
    // Error if writing negative number of bits
    if isIntType(numBits.type) && numBits < 0 then
      throw new owned IllegalArgumentError("writeBits numBits=" +
        numBits:string + " < 0");
  }

  try this.write(new _internalIoBits(x:uint(64), numBits:int(8)));
}

/*
  Write a single Unicode codepoint to a :record:`fileWriter`

  :arg codepoint: Unicode codepoint to write

  :throws EofError: If the ``fileWriter`` offset was already at EOF.
  :throws UnexpectedEofError: If the write operation exceeds the
                              ``fileWriter``'s specified range.
  :throws SystemError: If data could not be written to the ``fileWriter``
                       due to a :ref:`system error<io-general-sys-error>`.
*/
proc fileWriter.writeCodepoint(codepoint: int) throws {
  try this.write(new _internalIoChar(codepoint.safeCast(int(32))));
}

/*
  Read a single Unicode codepoint from a :record:`fileReader`

  :returns: Unicode codepoint read

  :throws EofError: If the ``fileReader`` offset was already at EOF.
  :throws UnexpectedEofError: If EOF was encountered while reading a
                              codepoint.
  :throws SystemError: If data could not be read from the ``fileReader``
                       due to a :ref:`system error<io-general-sys-error>`.
*/
proc fileReader.readCodepoint(): int throws {
  var tmp:int;
  var ret = try this.readCodepoint(tmp);
  if !ret then throw new EofError("Encountered EOF in readCodepoint");
  return tmp;
}

/*
  Read a single Unicode codepoint from a :record:`fileReader`

  :arg codepoint: where to store the read codepoint
  :returns: ``true`` if the codepoint was read, and ``false`` otherwise (i.e.,
            the ``fileReader`` was already at EOF).

  :throws UnexpectedEofError: If EOF was encountered while reading a
                              codepoint.
  :throws SystemError: If data could not be read from the ``fileReader``
                       due to a :ref:`system error<io-general-sys-error>`.
*/
proc fileReader.readCodepoint(ref codepoint: int):bool throws {
  var tmp:_internalIoChar;
  var ret = try this.read(tmp);
  codepoint = tmp.ch.safeCast(codepoint.type);
  return ret;
}

/*
  Write a single byte to a :record:`fileWriter`

  :arg byte: the byte to write

  :throws EofError: If the ``fileWriter`` offset was already at EOF.
  :throws UnexpectedEofError: If the write operation exceeds the
                              ``fileWriter``'s specified range.
  :throws SystemError: If data could not be written to the ``fileWriter``
                       due to a :ref:`system error<io-general-sys-error>`.
*/
proc fileWriter.writeByte(byte: uint(8)) throws {
  var err:errorCode = 0;
  on this._home {
    try this.lock(); defer { this.unlock(); }
    err = qio_channel_write_uint8(false, this._channel_internal, byte);
  }

  if err == EEOF {
    throw new UnexpectedEofError("Encountered EOF in writeByte");
  }
  else if err {
    try this._ch_ioerror(err, "in fileWriter.writeByte");
  }
}

/*
  Read a single byte from a :record:`fileReader`

  :returns: the byte read

  :throws EofError: If the ``fileReader`` offset was already at EOF.
  :throws SystemError: If data could not be read from the ``fileReader``
                       due to a :ref:`system error<io-general-sys-error>`.
*/
proc fileReader.readByte(): uint(8) throws {
  var tmp:uint(8);
  var ret = try this.readByte(tmp);
  if !ret then throw new EofError("Encountered EOF in readByte");
  return tmp;
}

/*
  Read a single byte from a :record:`fileReader`

  :arg byte: where to store the read byte
  :returns: ``true`` if the byte was read, and ``false`` otherwise (i.e.,
            the ``fileReader`` was already at EOF).

  :throws SystemError: If data could not be read from the ``fileReader``
                       due to a :ref:`system error<io-general-sys-error>`.
*/
proc fileReader.readByte(ref byte: uint(8)): bool throws {
  var err:errorCode = 0;
  var x: uint(8);
  on this._home {
    try this.lock(); defer { this.unlock(); }
    err = qio_channel_read_uint8(false, this._channel_internal, x);
  }
  byte = x;

  if !err {
    return true;
  }
  else if err == EEOF {
    return false;
  } else {
    try this._ch_ioerror(err, "in fileReader.readByte");
  }
  return false;
}

/*
  Controll whether large read/write operations can bypass the IO runtime's
  buffering mechanism.

  This optimization is on by default as it can improve performance for large
  operations where buffering doesn't significantly reduce the number of system
  I/O calls and thus adds unnecessary overhead.

  To disable the optimization, compile with ``-sIOSkipBufferingForLargeOps=false``.

  Note that this flag controls an implementation-specific feature and
  thus is not part of the Chapel language specification.
*/
@unstable("IOSkipBufferingForLargeOps is unstable and could change or be removed in the future")
config param IOSkipBufferingForLargeOps = true;
private extern var qio_write_unbuffered_threshold: c_ssize_t;
private extern var qio_read_unbuffered_threshold: c_ssize_t;

if !IOSkipBufferingForLargeOps {
  qio_write_unbuffered_threshold = max(c_ssize_t);
  qio_read_unbuffered_threshold = max(c_ssize_t);
}

/*
  Write ``size`` codepoints from a :type:`~String.string` to a
  :record:`fileWriter`

  :arg s: the ``string`` to write
  :arg size: the number of codepoints to write from the ``string``

  :throws EofError: If the ``fileWriter`` offset was already at EOF.
  :throws UnexpectedEofError: If the write operation exceeds the
                              ``fileWriter``'s specified range.
  :throws SystemError: If data could not be written to the ``fileWriter``
                       due to a :ref:`system error<io-general-sys-error>`.
  :throws IllegalArgumentError: If ``size`` is larger than ``s.size``
*/
proc fileWriter.writeString(s: string, size = s.size) throws {
  // TODO: use a separate implementation when `fileWriter`s start supporting
  //        non UTF-8 character encodings
  try this.writeBinary(s, size);
}

/*
  Write ``size`` bytes from a :type:`~Bytes.bytes` to a :record:`fileWriter`

  :arg b: the ``bytes`` to write
  :arg size: the number of bytes to write from the ``bytes``

  :throws EofError: If the ``fileWriter`` offset was already at EOF.
  :throws UnexpectedEofError: If the write operation exceeds the
                              ``fileWriter``'s specified range.
  :throws SystemError: If data could not be written to the ``fileWriter``
                       due to a :ref:`system error<io-general-sys-error>`.
  :throws IllegalArgumentError: If ``size`` is larger than ``b.size``
*/
proc fileWriter.writeBytes(b: bytes, size = b.size) throws {
  try this.writeBinary(b, size);
}

private proc sysEndianness() {
  var x: int(16) = 1;
  // if the initial byte is 0, this is a big-endian system
  if (c_addrOf(x): c_ptr(void): c_ptr(uint(8))).deref() == 0 then
    return endianness.big;
  else
    return endianness.little;
}

private proc isNativeEndianness(endian) {
  return endian == endianness.native || endian == sysEndianness();
}

private proc endianToIoKind(param e: endianness) param {
  if e == endianness.native then
    return _iokind.native;
  else if e == endianness.big then
    return _iokind.big;
  else if e == endianness.little then
    return _iokind.little;
  else
    compilerError("Unexpected value in chpl_endianToIoKind(): ", e);
}

/*
  Write ``numBytes`` of data from a :class:`~CTypes.c_ptr` to a
  :record:`fileWriter`

  Note that native endianness is always used.

  If ``numBytes`` is not evenly divisible by the size of ``t``, the remaining
  bytes will be ignored. For example, if the ``c_ptr``'s internal type is 4
  bytes in length, and ``numBytes=17``, only 16 bytes will be written.

  .. warning::
    This method provides no protection against attempting to access invalid
    memory

  :arg ptr: a :class:`~CTypes.c_ptr` to some valid memory
  :arg numBytes: the number of bytes to write

  :throws EofError: If the ``fileWriter`` offset was already at EOF.
  :throws UnexpectedEofError: If the write operation exceeds the
                              ``fileWriter``'s specified region.
  :throws SystemError: If data could not be written to the ``fileWriter``
                       due to a :ref:`system error<io-general-sys-error>`.
*/
proc fileWriter.writeBinary(ptr: c_ptr(?t), numBytes: int) throws
{
  var e:errorCode = 0;
  const t_size = c_sizeof(t),
        numBytesToWrite = (numBytes / t_size) * t_size;

  e = try qio_channel_write_amt(false, this._channel_internal, ptr[0], numBytesToWrite:c_ssize_t);

  if (e != 0) {
    throw createSystemOrChplError(e);
  }
}

/*
  Write ``numBytes`` of data from a ``CTypes.c_ptr(void)`` to a
  :record:`fileWriter`

  The data are written to the file one byte at a time.

  .. warning::
    This method provides no protection against attempting to access invalid
    memory

  :arg ptr: a ``c_ptr(void)`` to some valid memory
  :arg numBytes: the number of bytes to write

  :throws EofError: If the ``fileWriter`` offset was already at EOF.
  :throws UnexpectedEofError: If the write operation exceeds the
                              ``fileWriter``'s specified region.
  :throws SystemError: If data could not be written to the ``fileWriter``
                       due to a :ref:`system error<io-general-sys-error>`.
*/
proc fileWriter.writeBinary(ptr: c_ptr(void), numBytes: int) throws {
  var e:errorCode = 0;

  var byte_ptr = ptr : c_ptr(uint(8));
  e = try qio_channel_write_amt(false, this._channel_internal, byte_ptr[0], numBytes:c_ssize_t);

  if e != 0 then throw createSystemOrChplError(e);
}

/*
  Write a binary number to the :record:`fileWriter`

  :arg arg: number to be written
  :arg endian: :type:`endianness` compile-time argument that specifies the byte
               order in which to write the number. Defaults to
               :enumconstant:`endianness.native`.

  :throws EofError: If the ``fileWriter`` offset was already at EOF.
  :throws UnexpectedEofError: If the write operation exceeds the
                              ``fileWriter``'s specified region.
  :throws SystemError: If data could not be written to the ``fileWriter``
                       due to a :ref:`system error<io-general-sys-error>`.
 */
proc fileWriter.writeBinary(arg:numeric,
                            param endian:endianness = endianness.native) throws {
  const e: errorCode = try _write_binary_internal(_channel_internal,
                                                  endianToIoKind(endian),
                                                  arg);
  if (e != 0) {
    throw createSystemOrChplError(e);
  }
}

/*
  Write a binary number to the :record:`fileWriter`

  :arg arg: number to be written
  :arg endian: :type:`endianness` specifies the byte order in which
               to write the number.

  :throws EofError: If the ``fileWriter`` offset was already at EOF.
  :throws UnexpectedEofError: If the write operation exceeds the
                              ``fileWriter``'s specified region.
  :throws SystemError: If data could not be written to the ``fileWriter``
                       due to a :ref:`system error<io-general-sys-error>`.
*/
proc fileWriter.writeBinary(arg:numeric, endian:endianness) throws {
  select (endian) {
    when endianness.native {
      this.writeBinary(arg, endianness.native);
    }
    when endianness.big {
      this.writeBinary(arg, endianness.big);
    }
    when endianness.little {
      this.writeBinary(arg, endianness.little);
    }
  }
}

/*
  Write a :type:`~String.string` to a :record:`fileWriter` in binary format

  :arg s: the ``string`` to write
  :arg size: the number of codepoints to write from the ``string``

  :throws EofError: If the ``fileWriter`` offset was already at EOF.
  :throws UnexpectedEofError: If the write operation exceeds the
                              ``fileWriter``'s specified region.
  :throws SystemError: If data could not be written to the ``fileWriter``
                       due to a :ref:`system error<io-general-sys-error>`.
  :throws IllegalArgumentError: If ``size`` is larger than ``s.size``.
*/
proc fileWriter.writeBinary(s: string, size: int = s.size) throws {
  // handle bad arguments
  if size > s.size then
    throw new owned IllegalArgumentError("illegal argument 'size': cannot exceed length of provided string");
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
      _iokind.native: c_int,
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
  Write a :type:`~Bytes.bytes` to a :record:`fileWriter` in binary format

  :arg b: the ``bytes`` to write
  :arg size: the number of bytes to write from the ``bytes``

  :throws EofError: If the ``fileWriter`` offset was already at EOF.
  :throws UnexpectedEofError: If the write operation exceeds the
                              ``fileWriter``'s specified region.
  :throws SystemError: If data could not be written to the ``fileWriter``
                       due to a :ref:`system error<io-general-sys-error>`.
  :throws IllegalArgumentError: If ``size`` is larger than ``b.size``.
*/
proc fileWriter.writeBinary(b: bytes, size: int = b.size) throws {
  // handle bad arguments
  if size > b.size then
    throw new owned IllegalArgumentError("illegal argument 'size': cannot exceed length of provided bytes");

  on this._home {
    // write the first size bytes to the fileWriter
    var bLocal = b.localize();
    var e: errorCode = qio_channel_write_string(
      false,
      _iokind.native: c_int,
      qio_channel_str_style(this._channel_internal),
      this._channel_internal,
      bLocal.c_str(),
      size: c_ssize_t
    );

    if e != 0 then
      throw createSystemOrChplError(e);
  }
}

@chpldoc.nodoc
private proc isSuitableForBinaryReadWrite(arr: _array) param {
  return chpl__isDROrDRView(arr);
}

/*
  Write an array of binary numbers to a :record:`fileWriter`

  Note that this routine currently requires a local rectangular non-strided
  array.

  :arg data: an array of numbers to write to the fileWriter
  :arg endian: :type:`endianness` compile-time argument that specifies the byte
               order in which to read the numbers. Defaults to
               :enumconstant:`endianness.native`.

  :throws EofError: If the ``fileWriter`` offset was already at EOF.
  :throws UnexpectedEofError: If the write operation exceeds the
                              ``fileWriter``'s specified region.
  :throws SystemError: If data could not be written to the ``fileWriter``
                       due to a :ref:`system error<io-general-sys-error>`.
*/
proc fileWriter.writeBinary(const ref data: [?d] ?t, param endian:endianness = endianness.native) throws
  where isSuitableForBinaryReadWrite(data) && data.strides == strideKind.one && (
    isIntegralType(t) || isRealType(t) || isImagType(t) || isComplexType(t) )
{
  var err : string,      // errors from invalid invocation of this function
      e : errorCode = 0; // errors from performing the IO

  on this._home {
    try this.lock(); defer { this.unlock(); }
    const tSize = c_sizeof(t) : c_ssize_t;

    // Allow either DefaultRectangular arrays or dense slices of DR arrays
    if !data._value.isDataContiguous(d._value) {
      err = "writeBinary() array data must be contiguous";
    } else if data.locale.id != this._home.id {
      err = "writeBinary() array data must be on same locale as 'fileWriter'";
    } else if isNativeEndianness(endian) {
      if data.size > 0 {
        e = try qio_channel_write_amt(false, this._channel_internal, data[d.low], data.size:c_ssize_t * tSize);
      } // else no-op, writing a 0-element array writes nothing
    } else {
      for b in data {
        e = try _write_binary_internal(this._channel_internal,
                                       endianToIoKind(endian), b);
        if e != 0 then break;
      }
    }
  }

  // Throwing moved outside of the 'on' block due to a bug in throwing methods
  // under GASNET.
  //
  // https://github.com/chapel-lang/chapel/issues/23400
  if !err.isEmpty() then throw new IllegalArgumentError(err);
  if e != 0 then throw createSystemOrChplError(e);
}


@chpldoc.nodoc
proc fileWriter.writeBinary(const ref data: [?d] ?t, param endian:endianness = endianness.native) throws {
  compilerError("writeBinary() only supports local, rectangular, non-strided arrays of simple types");
}


/*
  Write an array of binary numbers to a :record:`fileWriter`

  Note that this routine currently requires a local rectangular non-strided array.

  :arg data: an array of numbers to write to the fileWriter
  :arg endian: :type:`endianness` specifies the byte order in which
               to write the number.

  :throws EofError: If the ``fileWriter`` offset was already at EOF.
  :throws UnexpectedEofError: If the write operation exceeds the
                              ``fileWriter``'s specified region.
  :throws SystemError: If data could not be written to the ``fileWriter``
                       due to a :ref:`system error<io-general-sys-error>`.
*/
proc fileWriter.writeBinary(const ref data: [] ?t, endian:endianness) throws
  where isSuitableForBinaryReadWrite(data) && data.strides == strideKind.one && (
    isIntegralType(t) || isRealType(t) || isImagType(t) || isComplexType(t) )
{
  select (endian) {
    when endianness.native {
      this.writeBinary(data, endianness.native);
    }
    when endianness.big {
      this.writeBinary(data, endianness.big);
    }
    when endianness.little {
      this.writeBinary(data, endianness.little);
    }
  }
}

@chpldoc.nodoc
proc fileWriter.writeBinary(const ref data: [] ?t, endian:endianness) throws
{
  compilerError("writeBinary() only supports local, rectangular, non-strided arrays of simple types");
}

/*
  Read a binary number from the :record:`fileReader`

  :arg arg: number to be read
  :arg endian: :type:`endianness` compile-time argument that specifies the byte
               order in which to read the number. Defaults to
               :enumconstant:`endianness.native`.
  :returns: ``true`` if the number was read, and ``false`` otherwise (i.e.,
            the ``fileReader`` was already at EOF).

  :throws UnexpectedEofError: If EOF was encountered while reading the
                              number.
  :throws SystemError: If data could not be read from the ``fileReader``
                       due to a :ref:`system error<io-general-sys-error>`.
 */
proc fileReader.readBinary(ref arg:numeric, param endian:endianness = endianness.native):bool throws {
  const e:errorCode = try _read_binary_internal(_channel_internal,
                                                endianToIoKind(endian),
                                                arg);

  if (e == EEOF) {
    return false;
  } else if (e != 0) {
    throw createSystemOrChplError(e);
  }
  return true;
}

/*
  Read a binary number from the :record:`fileReader`

  :arg arg: number to be read
  :arg endian: :type:`endianness` specifies the byte order in which
               to read the number.
  :returns: ``true`` if the number was read, and ``false`` otherwise (i.e.,
            the ``fileReader`` was already at EOF).

  :throws UnexpectedEofError: If EOF was encountered while reading the
                              number.
  :throws SystemError: If data could not be read from the ``fileReader``
                       due to a :ref:`system error<io-general-sys-error>`.
 */
proc fileReader.readBinary(ref arg:numeric, endian: endianness):bool throws {
  var rv: bool = false;

  select (endian) {
    when endianness.native {
      rv = this.readBinary(arg, endianness.native);
    }
    when endianness.big {
      rv = this.readBinary(arg, endianness.big);
    }
    when endianness.little {
      rv = this.readBinary(arg, endianness.little);
    }
  }
  return rv;
}

/*
  Read a specified number of codepoints into a :type:`~String.string`

  The resulting string ``s`` may be smaller than ``maxSize`` if EOF is reached
  before reading the specified number of codepoints. Additionally, if nothing is
  read from the :record:`fileReader`, ``s`` will be set to ``""`` (the empty
  string) and the method will return ``false``.

  .. note::

    This method always uses UTF-8 encoding regardless of the fileReader's
    configuration

  :arg s: the string to read into — this value is overwritten
  :arg maxSize: the number of codepoints to read from the ``fileReader``
  :returns: ``true`` if some codepoints were read, or ``false`` on EOF

  :throws SystemError: If data could not be read from the ``fileReader``
                       due to a :ref:`system error<io-general-sys-error>`.
                       In that event, the fileReader's offset will be
                       left near the position where the error occurred.
*/
proc fileReader.readBinary(ref s: string, maxSize: int): bool throws {
  var (e, lenRead) = readStringImpl(this, s, maxSize);

  if e != 0 && e != EEOF then throw createSystemError(e);

  return lenRead > 0;
}

/*
  Read a specified number of bytes into a :type:`~Bytes.bytes`

  The bytes ``b`` may be smaller than ``maxSize`` if EOF is reached before
  reading the specified number of bytes. Additionally, if nothing is read from
  the :record:`fileReader`, ``b`` will be set to ``b""`` (the empty bytes) and
  the method will return ``false``.

  :arg b: the bytes to read into — this value is overwritten
  :arg maxSize: the number of bytes to read from the ``fileReader``
  :returns: ``true`` if some bytes were read, or ``false`` on EOF

  :throws SystemError: If data could not be read from the ``fileReader``
                       due to a :ref:`system error<io-general-sys-error>`.
                       In that event, the fileReader's offset will be
                       left near the position where the error occurred.
*/
proc fileReader.readBinary(ref b: bytes, maxSize: int): bool throws {
  var (e, lenRead) = readBytesImpl(this, b, maxSize);

  if e != 0 && e != EEOF then throw createSystemError(e);

  return lenRead > 0;
}


/*
  Read an array of binary numbers from a :record:`fileReader`

  Binary values of the type ``data.eltType`` are consumed from the fileReader
  until ``data`` is full or EOF is reached.

  Note that this routine currently requires a local rectangular non-strided array.

  :arg data: an array to read into – existing values are overwritten.
  :arg endian: :type:`endianness` compile-time argument that specifies the byte
               order in which to read the numbers in. Defaults to
               :enumconstant:`endianness.native`.
  :returns: the number of values that were read into the array. This can be
            less than ``data.size`` if EOF was reached, or an error occurred,
            before filling the array.

  :throws SystemError: If data could not be read from the ``fileReader``
                       due to a :ref:`system error<io-general-sys-error>`.
                       In that event, the fileReader's offset will be
                       left near the position where the error occurred.
*/
proc fileReader.readBinary(ref data: [?d] ?t, param endian = endianness.native): int throws
  where isSuitableForBinaryReadWrite(data) && data.strides == strideKind.one && (
        isIntegralType(t) || isRealType(t) || isImagType(t) || isComplexType(t) )
{
  var err : string,       // errors from invalid invocation of this function
      e : errorCode = 0,  // errors from perform the IO
      numRead : c_ssize_t = 0;

  on this._home {
    try this.lock(); defer { this.unlock(); }

    if !data._value.isDataContiguous(d._value) {
      err = "readBinary() array data must be contiguous";
    } else if data.locale.id != this._home.id {
      err = "readBinary() array data must be on same locale as 'fileReader'";
    } else if isNativeEndianness(endian) {
      if data.size > 0 {
        e = qio_channel_read(false, this._channel_internal, data[d.low], (data.size * c_sizeof(t)) : c_ssize_t, numRead);
        // Note: partial reads of values are possible and are currently ignored.
        numRead /= c_sizeof(t): numRead.type;  // convert from #bytes to #elts
      } // else no-op, reading a 0-element array reads nothing
    } else {
      for (_, b) in zip(data.domain, data) {
        e = try _read_binary_internal(this._channel_internal,
                                      endianToIoKind(endian), b);

        if e == EEOF {
          break;
        } else if e != 0 {
          break;
        } else {
          numRead += 1;
        }
      }
    }
  }

  // Throwing moved outside of the 'on' block due to a bug in throwing methods
  // under GASNET.
  //
  // https://github.com/chapel-lang/chapel/issues/23400
  if !err.isEmpty() then throw new IllegalArgumentError(err);
  // Tolerate EOFs or ESHORTs since we are returning the number of elements
  // that were read.
  if e != 0 && e != EEOF && e != ESHORT then throw createSystemOrChplError(e);

  return numRead : int;
}

/*
   Read an array of binary numbers from a :record:`fileReader`

   Binary values of the type ``data.eltType`` are consumed from the fileReader
   until ``data`` is full or EOF is reached.

   Note that this routine currently requires a local rectangular non-strided
   array.

   :arg data: an array to read into – existing values are overwritten.
   :arg endian: :type:`endianness` specifies the byte order in which
                to read the number.
   :returns: the number of values that were read into the array. This can be
             less than ``data.size`` if EOF was reached, or an error occurred,
             before filling the array.

   :throws SystemError: If data could not be read from the ``fileReader``
                        due to a :ref:`system error<io-general-sys-error>`.
                        In that event, the fileReader's offset will be
                        left near the position where the error occurred.
*/
proc fileReader.readBinary(ref data: [] ?t, endian: endianness):int throws
  where isSuitableForBinaryReadWrite(data) && data.strides == strideKind.one && (
        isIntegralType(t) || isRealType(t) || isImagType(t) || isComplexType(t) )
{
  var nr: int = 0;

  select (endian) {
    when endianness.native {
      nr = this.readBinary(data, endianness.native);
    }
    when endianness.big {
      nr = this.readBinary(data, endianness.big);
    }
    when endianness.little {
      nr = this.readBinary(data, endianness.little);
    }
  }

  return nr;
}

@chpldoc.nodoc
proc fileReader.readBinary(ref data: [] ?t, endian: endianness):int throws
{
  compilerError("readBinary() only supports local, rectangular, non-strided ",
                  "arrays of simple types");
}

@chpldoc.nodoc
proc fileReader.readBinary(ref data: [] ?t, param endian = endianness.native): bool throws
{
  compilerError("readBinary() only supports local, rectangular, non-strided ",
                  "arrays of simple types");
}

/*
   Read up to ``maxBytes`` bytes from a :record:`fileReader` into a
   :class:`~CTypes.c_ptr`

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

   :throws SystemError: If data could not be read from the ``fileReader``
                        due to a :ref:`system error<io-general-sys-error>`.
                        In that event, the fileReader's offset will be
                        left near the position where the error occurred.
*/
proc fileReader.readBinary(ptr: c_ptr(?t), maxBytes: int): int throws {
  var e: errorCode = 0,
      numRead: c_ssize_t = 0;
  const t_size = c_sizeof(t),
        numBytesToRead = (maxBytes:uint / t_size) * t_size;

  e = qio_channel_read(false, this._channel_internal, ptr[0], numBytesToRead: c_ssize_t, numRead);

  if e != 0 && e != EEOF then throw createSystemOrChplError(e);
  return numRead;
}

/*
   Read up to ``maxBytes`` bytes from a :record:`fileReader` into a
   ``CTypes.c_ptr(void)``

   Note that data are read from the file one byte at a time.

   :arg ptr: a ``c_ptr(void)`` to some memory — existing
             values will be overwritten
   :arg maxBytes: the maximum number of bytes to read from the ``fileReader``
   :returns: the number of bytes that were read. this can be less than
             ``maxBytes`` if EOF was reached before reading the specified number
             of bytes

   :throws SystemError: If data could not be read from the ``fileReader``
                        due to a :ref:`system error<io-general-sys-error>`.
                        In that event, the fileReader's offset will be
                        left near the position where the error occurred.
*/
proc fileReader.readBinary(ptr: c_ptr(void), maxBytes: int): int throws {
  var e: errorCode = 0,
      numRead: c_ssize_t = 0;
  var bytes_ptr = ptr: c_ptr(uint(8));

  e = qio_channel_read(false, this._channel_internal, bytes_ptr[0], maxBytes: c_ssize_t, numRead);

  if e != 0 && e != EEOF then throw createSystemOrChplError(e);
  return numRead;
}


// Documented in the varargs version
@chpldoc.nodoc
@unstable("'readln' is unstable and may be removed or modified in a future release")
proc fileReader.readln():bool throws {
  var nl = new chpl_ioNewline();
  return try this.read(nl);
}

/*
   Read values from a :record:`fileReader` and then consume any bytes until
   newline is reached. The input will be consumed atomically - the
   fileReader lock will be held while reading all of the passed values.

   :arg args: a list of arguments to read. This routine can be called
              with zero or more such arguments. Basic types are handled
              internally, but for other types this function will call
              value.deserialize() with a ``fileReader`` argument as described
              in :ref:`serialize-deserialize`.
   :returns: `true` if the read succeeded, and `false` upon end of file.

   :throws UnexpectedEofError: If EOF was encountered before data could
                               be read.
   :throws SystemError: If data could not be read from the ``fileReader``
                        due to a :ref:`system error<io-general-sys-error>`.
 */
@unstable("'readln' is unstable and may be removed or modified in a future release")
proc fileReader.readln(ref args ...?k):bool throws {
  var nl = new chpl_ioNewline();
  return try this.read((...args), nl);
}

/*
   Read a value of passed type.

   For example, the following line of code reads a value of type `int`
   from :var:`stdin` and uses it to initialize a variable ``x``:

   .. literalinclude:: ../../../../test/library/standard/IO/doc-examples/example_read_stdin.chpl
    :language: chapel
    :start-after: START_EXAMPLE
    :end-before: STOP_EXAMPLE

   :arg t: the type to read
   :returns: the value read

   :throws EofError: If the :record:`fileReader` is already at EOF.
   :throws UnexpectedEofError: If EOF was encountered before data could
                               be fully read.
   :throws SystemError: If data could not be read from the ``fileReader``
                        due to a :ref:`system error<io-general-sys-error>`.
 */
proc fileReader.read(type t) throws {
  const origLocale = this.getLocaleOfIoRequest();

  if isGenericType(t) then
    compilerError("reading generic types is not supported: '" + t:string + "'");
  if isBorrowedClass(t) then
    compilerError("reading borrowed class types is not supported: '" + t:string + "'");

  // Need 'do not RVF' here so that 'ret' is passed by reference across the
  // on-stmt. Otherwise it would be serialized/bit-copied and we couldn't
  // return the value that we just read.
  //
  // Currently need to use nilable for unmanaged classes due to unknown
  // "dead value" error in compiler.
  param specialUnmanaged = isUnmanagedClassType(t) && isNonNilableClassType(t);
  type retType = if specialUnmanaged then _to_nilable(t) else t;
  pragma "no init"
  pragma "do not RVF"
  var ret : retType;

  on this._home {
    try this.lock(); defer { this.unlock(); }

    if deserializerType != nothing {
      if deserializerType == binaryDeserializer {
        warnBinaryRead(t, 2);
      }
      __primitive("move", ret, _deserializeOne(t, origLocale));
    } else {
      pragma "no auto destroy"
      var tmp : t;
      _readOne(_iokind.dynamic, tmp, origLocale);
      __primitive("=", ret, tmp);
    }
  }

  if specialUnmanaged then return ret!;
  else return ret;
}

/*
   Read a value of passed type followed by a newline.

   :arg t: the type to read
   :returns: the value read

   :throws EofError: If the :record:`fileReader` is at already EOF.
   :throws UnexpectedEofError: If EOF was encountered before data could
                               be fully read.
   :throws SystemError: If data could not be read from the ``fileReader``
                        due to a :ref:`system error<io-general-sys-error>`.
 */
@unstable("'readln' is unstable and may be removed or modified in a future release")
proc fileReader.readln(type t) throws {
  var tmp:t;
  var nl = new chpl_ioNewline();
  this._readInner(tmp, nl);
  return tmp;
}

/*
   Read values of passed types followed by a newline
   and return a tuple containing the read values.

   :arg t: more than one type to read
   :returns: a tuple of the read values

   :throws EofError: If the :record:`fileReader` is already at EOF.
   :throws UnexpectedEofError: If EOF was encountered before data could
                               be fully read.
   :throws SystemError: If data could not be read from the ``fileReader``
                        due to a :ref:`system error<io-general-sys-error>`.
 */
@unstable("'readln' is unstable and may be removed or modified in a future release")
proc fileReader.readln(type t ...?numTypes) throws where numTypes > 1 {
  var tupleVal: t;
  var nl = new chpl_ioNewline();
  this._readInner((...tupleVal), nl);
  return tupleVal;
}

/*
   Read values of passed types and return a tuple containing the read values.
   The :record:`fileReader`'s lock will be held while reading — this protects
   against interleaved reads.

   :arg t: more than one type to read
   :returns: a tuple of the read values

   :throws EofError: If the ``fileReader`` is already at EOF.
   :throws UnexpectedEofError: If EOF was encountered while more data was
                               expected.
   :throws SystemError: If data could not be read from the ``fileReader``
                        due to a :ref:`system error<io-general-sys-error>`.
 */
proc fileReader.read(type t ...?numTypes) throws where numTypes > 1 {
  // TODO: better IO-specific error message if type is an array or tuple...
  var tupleVal: t;
  this._readInner((...tupleVal));
  return tupleVal;
}


/*
   Write values to a :record:`fileWriter`. The output will be produced
   atomically - the ``fileWriter`` lock will be held while writing all of the
   passed values.

   :arg args: a list of arguments to write. Basic types are handled
              internally, but for other types this function will call
              value.serialize() with the ``fileWriter`` as an argument.
   :arg sep: a string separator that is printed in between each argument.
             Defaults to the empty string. Note that specifying ``sep`` is
             currently an unstable feature pending further design.

   :throws EofError: If EOF is reached before all the arguments could be
                     written.
   :throws UnexpectedEofError: If EOF is encountered while writing one of
                               the arguments.
   :throws SystemError: If data could not be written to the ``fileWriter``
                        due to a :ref:`system error<io-general-sys-error>`.
 */
pragma "last resort"
pragma "fn exempt instantiation limit"
inline proc fileWriter.write(const args ...?k, sep: string = "") throws {
  if chpl_warnUnstable then
    compilerWarning("specifying 'sep' is an unstable feature");
  this.writeHelper(none, sep, (...args));
}

pragma "fn exempt instantiation limit"
@chpldoc.nodoc
inline proc fileWriter.write(const args ...?k) throws {
  this.writeHelper(none, none, (...args));
}


// documented in varargs version
@chpldoc.nodoc
proc fileWriter.writeln() throws {
  try this.write(new chpl_ioNewline());
}

/*

   Write values to a :record:`fileWriter` followed by a newline.  The output
   will be produced atomically - the ``fileWriter`` lock will be held while
   writing all of the passed values.

   :arg args: a variable number of arguments to write. This method can be
              called with zero or more arguments. Basic types are handled
              internally, but for other types this function will call
              value.serialize() with the fileWriter as an argument.
   :arg sep: a string separator that is printed in between each argument.
             Defaults to the empty string. Note that specifying ``sep`` is
             currently an unstable feature pending further design.

   :throws EofError: If EOF is reached before all the arguments
                     could be written.
   :throws UnexpectedEofError: If EOF is encountered while writing one of
                               the arguments.
   :throws SystemError: If data could not be written to the ``fileWriter``
                        due to a :ref:`system error<io-general-sys-error>`.
 */
pragma "last resort"
proc fileWriter.writeln(const args ...?k, sep:string="") throws {
  if chpl_warnUnstable then
    compilerWarning("specifying 'sep' is an unstable feature");
  this.writeHelper(new chpl_ioNewline(), sep, (...args));
}

@chpldoc.nodoc
proc fileWriter.writeln(const args ...?k) throws {
  this.writeHelper(new chpl_ioNewline(), none, (...args));
}

@chpldoc.nodoc
inline proc fileWriter.writeHelper(endl: ?endlType, sep: ?sepType, const args...) throws {
  const origLocale = this.getLocaleOfIoRequest();
  on this._home {
    try this.lock(); defer { this.unlock(); }

    for param i in 0..<args.size {
      if i != 0 && sepType != nothing then
        try _writeOne(_iokind.dynamic, sep, origLocale);

      if serializerType != nothing {
        if serializerType == binarySerializer {
          warnBinary(args(i).type, 2);
        }
        this._serializeOne(args(i), origLocale);
      } else {
        try _writeOne(_iokind.dynamic, args(i), origLocale);
      }
    }
    if endlType != nothing then
      try _writeOne(_iokind.dynamic, endl, origLocale);
  }
}

/*

  Makes all writes to the :record:`fileWriter`, if any, available to concurrent
  viewers of its associated file, such as other fileWriters/fileReader or other
  applications accessing this file concurrently.

  Unlike :proc:`file.fsync`, this does not commit the written data
  to the file's device.

  :throws SystemError: If the flush fails.
*/
proc fileWriter.flush() throws {
  var err:errorCode = 0;
  on this._home {
    err = qio_channel_flush(locking, _channel_internal);
  }
  if err then try this._ch_ioerror(err, "in fileWriter.flush");
}

// documented in throws version
@chpldoc.nodoc
proc fileWriter.flush(out error:errorCode) {
  error = 0;
  try {
    this.flush();
  } catch e: SystemError {
    error = e.err;
  } catch {
    error = EINVAL;
  }
}

/* Assert that a :record:`fileReader` has reached end-of-file and that there was
   no error doing the read.
*/
@unstable("'assertEOF' is unstable and may be removed or modified in a future release")
proc fileReader.assertEOF(errStr: string = "- Not at EOF") {
  var isEOF = try! this.atEOF();
  if !isEOF then
    try! this._ch_ioerror("assert failed", errStr);
}

/* Returns ``true`` if a fileReader has reached end-of-file, ``false`` if not.
  Throws an error if there was an error doing the read.

   Inherently racy for channels, hence no doc.
 */
@chpldoc.nodoc
proc fileReader.atEOF(): bool throws {
  var tmp:uint(8);
  return !(try this.readByte(tmp));
}

/*
  Close a :record:`fileReader`

  :throws SystemError: If the ``fileReader`` is not successfully closed.
*/
proc fileReader.close() throws {
  var err:errorCode = 0;

  if _channel_internal == nil then
    throw createSystemOrChplError(EINVAL, "cannot close invalid fileReader");

  on this._home {
    err = qio_channel_close(locking, _channel_internal);
  }
  if err then try this._ch_ioerror(err, "in fileReader.close");
}

/*
  Close a :record:`fileWriter`. Implicitly performs the :proc:`fileWriter.flush`
  operation (see :ref:`about-io-filereader-filewriter-synchronization`).

  :throws SystemError: If the ``fileWriter`` is not successfully closed.
*/
proc fileWriter.close() throws {
  var err:errorCode = 0;

  if _channel_internal == nil then
    throw createSystemOrChplError(EINVAL, "cannot close invalid fileWriter");

  on this._home {
    err = qio_channel_close(locking, _channel_internal);
  }
  if err then try this._ch_ioerror(err, "in fileWriter.close");
}

/*
   Return ``true`` if a :record:`fileReader` is currently closed.
 */
proc fileReader.isClosed() : bool {
  var ret:bool;
  on this._home {
    ret = qio_channel_isclosed(locking, _channel_internal);
  }
  return ret;
}

/*
   Return ``true`` if a :record:`fileWriter` is currently closed.
 */
proc fileWriter.isClosed() : bool {
  var ret:bool;
  on this._home {
    ret = qio_channel_isclosed(locking, _channel_internal);
  }
  return ret;
}

@chpldoc.nodoc
record itemReaderInternal {
  /* What type do we read and yield? */
  type ItemType;
  /* the locking field for our fileReader */
  param locking:bool;
  /* the deserializer for this fileReader */
  type deserializerType;
  /* our fileReader */
  var ch:fileReader(locking,deserializerType);

  /* read a single item, throwing on error */
  proc read(out arg:ItemType):bool throws {
    return ch.read(arg);
  }

  /* iterate through all items of that type read from the fileReader */
  iter these() { // TODO: this should throw
    while true {
      var x:ItemType;
      var gotany:bool;
      try! { // TODO this should by a 'try' (once #7134 is fixed)
        gotany = ch.read(x);
      }
      if ! gotany then break;
      yield x;
    }
  }
}

// And now, the toplevel items.

/* A locking :record:`fileReader` instance that reads from standard input. */
const stdin:fileReader(true);
stdin = try! (new file(0)).reader(locking=true);

extern proc chpl_cstdout(): c_ptr(c_FILE);
/* A locking :record:`fileWriter` instance that writes to standard output. */
const stdout:fileWriter(true);
stdout = try! (new file(chpl_cstdout())).writer(locking=true);


extern proc chpl_cstderr(): c_ptr(c_FILE);
/* A locking :record:`fileWriter` instance that writes to standard error. */
const stderr:fileWriter(true);
stderr = try! (new file(chpl_cstderr())).writer(locking=true);

/* Equivalent to ``stdin.read``. See :proc:`fileReader.read` */
proc read(ref args ...?n):bool throws {
  return stdin.read((...args));
}
/* Equivalent to ``stdin.read``. See :proc:`fileReader.read` for types */
proc read(type t ...?numTypes) throws {
  return stdin.read((...t));
}

/* Equivalent to ``stdin.readLine``.  See :proc:`fileReader.readLine` */
proc readLine(ref a: [] ?t, maxSize=a.size, stripNewline=false): int throws
  where a.rank == 1 && a.isRectangular() && a.strides == strideKind.one &&
        (t == uint(8) || t == int(8))
{
  return stdin.readLine(a, maxSize, stripNewline);
}

pragma "last resort"
@chpldoc.nodoc
proc readLine(ref a: [] ?t, maxSize=a.size, stripNewline=false): int throws
      where (t == uint(8) || t == int(8)) {
  compilerError("'readLine()' is currently only supported for non-strided 1D rectangular arrays");
}

/* Equivalent to ``stdin.readLine``.  See :proc:`fileReader.readLine` */
proc readLine(ref s: string, maxSize=-1, stripNewline=false): bool throws{
  return stdin.readLine(s, maxSize, stripNewline);
}

/* Equivalent to ``stdin.readLine``.  See :proc:`fileReader.readLine` */
proc readLine(ref b: bytes, maxSize=-1, stripNewline=false): bool throws{
  return stdin.readLine(b, maxSize, stripNewline);
}

/* Equivalent to ``stdin.readLine``.  See :proc:`fileReader.readLine` */
proc readLine(type t=string, maxSize=-1, stripNewline=false): t throws where t==string || t==bytes {
  return stdin.readLine(t, maxSize, stripNewline);
}

/* Equivalent to ``stdin.readln``. See :proc:`fileReader.readln` */
@unstable("'readln' is unstable and may be removed or modified in a future release")
proc readln(ref args ...?n):bool throws {
  return stdin.readln((...args));
}
// documented in the arguments version.
@chpldoc.nodoc
@unstable("'readln' is unstable and may be removed or modified in a future release")
proc readln():bool throws {
  return stdin.readln();
}

/* Equivalent to ``stdin.readln``. See :proc:`fileReader.readln` for types */
@unstable("'readln' is unstable and may be removed or modified in a future release")
proc readln(type t ...?numTypes) throws {
  return stdin.readln((...t));
}


/************** Distributed File Systems ***************/

private extern const FTYPE_NONE   : c_int;
private extern const FTYPE_LUSTRE : c_int;

@chpldoc.nodoc
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
:proc:`fileWriter.writef` and :proc:`fileReader.readf` functions. These
functions take in a format string and some arguments. The :proc:`string.format`
method is also available and is loosely equivalent to C's 'sprintf'. For
example, one might do:

.. literalinclude:: ../../../../../test/library/standard/IO/doc-examples/FormattedIO/example_string_format.chpl
 :language: chapel
 :start-after: START_EXAMPLE
 :end-before: STOP_EXAMPLE

The following sections offer a tour through the conversions to illustrate the
common cases. A more precise definition follows in the
:ref:`about-io-formatted-io-in-detail` section below.

In this file, we use "integral" to refer to the Chapel types int or uint and
"floating-point" to refer to real, imaginary, or complex, of any bit width.

Formatted I/O for C Programmers
+++++++++++++++++++++++++++++++

This table is designed to help C programmers understand the equivalent
Chapel format specifiers.

========  ===========  ==========================================
C         Chapel       Meaning
========  ===========  ==========================================
%i        %i           a signed integer in decimal
%d        %i           a signed integer in decimal
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
example printing 2 (an ``int``) with ``%.2dr`` will result in ``2.00``.  Note
that Chapel's ``%n`` and ``%?`` are equivalent to ``%r`` for real conversions
and ``%i`` for numeric conversions; so these are also equivalent to ``%i``
``%d`` or ``%g`` in C. Also note that Chapel format strings include many
capabilities not available with C formatted I/O routines - including quoted
strings, binary numbers, and complex numbers.


Generic Numeric Conversions
+++++++++++++++++++++++++++

``%{##.###}``
  decimal number padded on the left with spaces to 2 digits before
  the point, rounded to 3 after. Works with integral, real, imaginary,
  or complex arguments.

  In all cases, the output is padded on the left to the total length
  of the conversion specifier (6 in this example).  The output
  can be longer, when needed to accommodate the number.

``%{#####}``
  integral value padded out to 5 digits. Also works with real, imaginary
  or complex numbers by rounding them to integers. Numbers with more
  digits will take up more space instead of being truncated.

In both conversions above, an imaginary argument gets an 'i' afterwards
and the entire expression is padded out to the width of ##### digits.
For example:

.. literalinclude:: ../../../../../test/library/standard/IO/doc-examples/FormattedIO/example_generic_numeric_conversions.chpl
 :language: chapel
 :start-after: START_EXAMPLE_1
 :end-before: STOP_EXAMPLE_1

Complex arguments are printed in the format a + bi, where each of a and b is
rounded individually as if printed under that conversion on its own. Then, the
formatted complex number is padded to the requested size. For example:

.. literalinclude:: ../../../../../test/library/standard/IO/doc-examples/FormattedIO/example_generic_numeric_conversions.chpl
 :language: chapel
 :start-after: START_EXAMPLE_2
 :end-before: STOP_EXAMPLE_2

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
``%<17i``
 a decimal integer left-justified (padded on the right) to 17 columns
``%^17i``
 a decimal integer center-justified (padded equally on the left and right) to 17 columns
``%>17i``
 a decimal integer right-justified (padded on the left) to 17 columns — equivalent to ``%17i``
``%+i``
 a decimal integer showing ``+`` for positive numbers
``% i``
 a decimal integer with a space for positive numbers

Real Conversions
++++++++++++++++

``%r``
 a real number with decimal or exponential notation, where
 exponential is chosen if the decimal version would be too long

``%6r``
 as with ``%r`` but padded on the left to 6 columns (i.e., right-justified)
``%<6r``
 as with ``%r`` but padded on the right to 6 columns (i.e., left-justified)
``%^6r``
 as with ``%r`` but padded equally on the left and right to 6 columns (i.e., center-justified)
``%>6r``
 equivalent to ``%6r``
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
 print a and b with ``%dr`` (using decimal notation)
``%ez``
 print a and b with ``%er`` (using exponential notation)

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
``%<17s``
  * when writing - a string right padded (left justified) to 17 columns
``%^17s``
  * when writing - a string equally left and right padded (center justified) to 17 columns
``%>17s``
  * when writing - a string left padded (right justified) to 17 columns
  * when reading - read up to 17 bytes or a whitespace, whichever comes
    first, rounding down to whole characters
``%.17s``
 * when writing - a string truncated to 17 columns. When combined
   with quoting strings, for example ``%.17"S``, the conversion
   will print ... after a string if it was truncated. The
   truncation includes leaving room for the quotes and -
   if needed - the periods, so the shortest truncated
   string is ``""...``  Generally, you won't be able to read
   these back in.
 * when reading - read exactly 17 Unicode codepoints
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

General Conversion
++++++++++++++++++

``%?``
  Use the :record:`~IO.fileWriter`/:record:`~IO.fileReader`'s associated
  serializer/deserializer to write or read a value.

  For example, read and write a record in JSON format:

  .. literalinclude:: ../../../../../test/library/standard/IO/doc-examples/FormattedIO/example_read_write_json.chpl
   :language: chapel
   :start-after: START_EXAMPLE
   :end-before: STOP_EXAMPLE

Note About Whitespace
+++++++++++++++++++++

When reading, ``\n`` in a format string matches any zero or more space
characters other than newline and then exactly one newline character. In
contrast, ``" "`` matches at least one space character of any kind.

When writing, whitespace is printed from the format string just like any
other literal would be.

Finally, space characters after a revcom will be ignored, so
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

.. literalinclude:: ../../../../../test/library/standard/IO/doc-examples/FormattedIO/example_hashtag_specifiers.chpl
 :language: chapel
 :start-after: START_EXAMPLE_1
 :end-before: STOP_EXAMPLE_1

This syntax also works for numbers without a decimal point by rounding them
appropriately.

A # specifier may start with a ``.``.

.. literalinclude:: ../../../../../test/library/standard/IO/doc-examples/FormattedIO/example_hashtag_specifiers.chpl
 :language: chapel
 :start-after: START_EXAMPLE_2
 :end-before: STOP_EXAMPLE_2

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
  ``<``
   left-justify the converted value instead of right-justifying.
   Note, if both ``0`` and ``<`` are given, the effect is as if only ``<``
   were given.
  ``^``
   center-justify the converted value instead of right-justifying. Note, if
   both ``0`` and ``^`` are given, zero-padding will only be applied to the
   left of the numerical value
  ``>``
   explicitly denote right-justification


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

   Providing a precision argument to an integral specifier (``%i``, ``%u``, etc)
   will emit a warning. Using ``%n`` to specify precision for an integer is
   preferred.

   The precision is silently ignored for for ``%//`` conversions.

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
   ``n``
    means type-based number, allowing width and precision (size is not
    mandatory)
   ``i``
    means integral conversion. Note that the size is mandatory for binary
    integral conversions
   ``u``
    means unsigned integral conversion. Note that the size is mandatory for
    binary integral conversions
   ``r``
    means real conversion (e.g. ``12.23``). Note that the size is mandatory for
    binary real conversions
   ``m``
    means imaginary conversion with an ``i`` after it (e.g. ``12.23i``)
   ``z``
    means complex conversion. Note that the size is mandatory for binary complex
    conversions
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


.. _about-io-formatted-io-examples:

Formatted I/O Examples
++++++++++++++++++++++

.. literalinclude:: ../../../../../test/library/standard/IO/doc-examples/FormattedIO/example_formatted_io.chpl
 :language: chapel
 :start-after: START_EXAMPLE_1
 :end-before: STOP_EXAMPLE_1

.. literalinclude:: ../../../../../test/library/standard/IO/doc-examples/FormattedIO/example_formatted_io.chpl
 :language: chapel
 :start-after: START_EXAMPLE_2
 :end-before: STOP_EXAMPLE_2

.. literalinclude:: ../../../../../test/library/standard/IO/doc-examples/FormattedIO/example_formatted_io.chpl
 :language: chapel
 :start-after: START_EXAMPLE_3
 :end-before: STOP_EXAMPLE_3

.. literalinclude:: ../../../../../test/library/standard/IO/doc-examples/FormattedIO/example_formatted_io_regex.chpl
 :language: chapel
 :start-after: START_EXAMPLE_1
 :end-before: STOP_EXAMPLE_1

.. literalinclude:: ../../../../../test/library/standard/IO/doc-examples/FormattedIO/example_formatted_io_regex.chpl
 :language: chapel
 :start-after: START_EXAMPLE_2
 :end-before: STOP_EXAMPLE_2

FormattedIO Functions and Types
-------------------------------

 */
module FormattedIO {
  use IO;
  use CTypes;
  use OS.POSIX;
  use OS;

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
  if isPrimitiveType(t) {
    return (x:string, true);
  } else {
    return (chpl_stringify(x), true);
  }
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
  lhs = new _internalIoChar(rhs):string;
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

@chpldoc.nodoc
class _channel_regex_info : writeSerializable {
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
  override proc serialize(writer, ref serializer) throws {
    writer.write("{hasRegex = " + hasRegex: string);
    writer.write(", matchedRegex = " + matchedRegex: string);
    writer.write(", releaseRegex = " + releaseRegex: string);
    writer.write(", ... capturei = " + capturei: string);
    writer.write(", ncaptures = " + ncaptures: string + "}");
  }
}

@chpldoc.nodoc
proc fileReader._match_regex_if_needed(cur:c_size_t, len:c_size_t,
                                       ref error:errorCode,
                                       ref style:iostyleInternal,
                                       r:unmanaged _channel_regex_info)
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
      // And, advance the fileReader to the end of the match.
      var cur = qio_channel_offset_unlocked(_channel_internal);
      var target = (r.matches[0].offset + r.matches[0].len):int;
      error = qio_channel_advance(false, _channel_internal, target - cur);
    } else {
      // otherwise, clear out caps...
      for j in 0..#ncaps {
        r.capArr[j] = b"";
      }
      // ... and put the fileReader before the match.
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
// Assumes, for a fileReader, that we are within a mark/revert/commit
//  in readf. (used in the regex handling here).
@chpldoc.nodoc
proc fileReader._format_reader(
    fmtStr:?fmtType, ref cur:c_size_t, len:c_size_t, ref error:errorCode,
    ref conv:qio_conv_t, ref gotConv:bool, ref style:iostyleInternal,
    ref r:unmanaged _channel_regex_info?)
{
  if r != nil then r!.hasRegex = false;
  if !error {
    const ref fmt = fmtStr.localize().c_str();
    while cur < len {
      gotConv = false;
      if error then break;
      var end:uint(64);
      error = qio_conv_parse(fmt, cur, end, true, conv, style);
      if error {
      }
      cur = end:c_size_t;
      if error then break;
      if conv.argType == QIO_CONV_ARG_TYPE_NONE_LITERAL {
        // Print whitespace or I/O literal.
        // literal string in conv
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
      } else if conv.argType == QIO_CONV_ARG_TYPE_NONE_REGEX_LITERAL {
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
      } else {
        // Some other kind of format specifier... we
        // will return to handle.
        gotConv = true;
        break;
      }
    }
  }
}

// Reads the next format string that will require argument handling.
// Handles literals and regexes itself; everything else will
// be returned in conv and with gotConv = true.
// (used in the regex handling here).
@chpldoc.nodoc
proc fileWriter._format_reader(
    fmtStr:?fmtType, ref cur:c_size_t, len:c_size_t, ref error:errorCode,
    ref conv:qio_conv_t, ref gotConv:bool, ref style:iostyleInternal)
{
  if !error {
    const ref fmt = fmtStr.localize().c_str();
    while cur < len {
      gotConv = false;
      if error then break;
      var end:uint(64);
      error = qio_conv_parse(fmt, cur, end, false, conv, style);
      if error {
      }
      cur = end:c_size_t;
      if error then break;
      if conv.argType == QIO_CONV_ARG_TYPE_NONE_LITERAL {
        // Print whitespace or I/O literal.
        // literal string in conv
        // when printing we don't care if it's just whitespace.
        error = qio_channel_print_literal_2(false, _channel_internal, conv.literal, conv.literal_length:c_ssize_t);
      } else if conv.argType == QIO_CONV_ARG_TYPE_NONE_REGEX_LITERAL {
        // It's not so clear what to do when printing
        // a regex. So we just don't handle it.
        error = qio_format_error_write_regex();
      } else {
        // Some other kind of format specifier... we
        // will return to handle.
        gotConv = true;
        break;
      }
    }
  }
}

@chpldoc.nodoc
private proc _conv_helper(
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

@chpldoc.nodoc
private proc _conv_sethandler(
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

@chpldoc.nodoc
proc fileWriter._write_signed(width:uint(32), t:int, i:int):errorCode
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

@chpldoc.nodoc
proc fileReader._read_signed(width:uint(32), out t:int, i:int):errorCode
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

@chpldoc.nodoc
proc fileWriter._write_unsigned(width:uint(32), t:uint, i:int)
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
@chpldoc.nodoc
proc fileReader._read_unsigned(width:uint(32), out t:uint, i:int)
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


@chpldoc.nodoc
proc fileWriter._write_real(width:uint(32), t:real, i:int)
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
@chpldoc.nodoc
proc fileReader._read_real(width:uint(32), out t:real, i:int)
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


@chpldoc.nodoc
proc fileWriter._write_complex(width:uint(32), t:complex, i:int)
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

@chpldoc.nodoc
proc fileReader._read_complex(width:uint(32), out t:complex, i:int)
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
@chpldoc.nodoc
proc fileWriter._writefOne(fmtStr, const ref arg, i: int,
                           ref cur: c_size_t, ref j: int,
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
    this._format_reader(fmtStr, cur, len, err,
                        conv, gotConv, style);
  }

  _conv_helper(err, conv, gotConv, j, argType, argTypeLen);

  var domore = _conv_sethandler(err, argType(i), style, i,arg,false);

  if domore {
    this._set_styleInternal(style);
    // otherwise we will consume at least one argument.
    select argType(i) {
      when QIO_CONV_ARG_TYPE_SIGNED, QIO_CONV_ARG_TYPE_BINARY_SIGNED {
        if style.precision != -1 then
          warning("Setting precision for integer format specifiers is not supported; use '%n' instead");

        var (t,ok) = _toSigned(arg);
        if ! ok {
          err = qio_format_error_arg_mismatch(i);
        } else {
          if argType(i) == QIO_CONV_ARG_TYPE_BINARY_SIGNED then
            err = _write_signed(style.max_width_bytes, t, i);
          else
            try _writeOne(_iokind.dynamic, t, origLocale);
        }
      } when QIO_CONV_ARG_TYPE_UNSIGNED, QIO_CONV_ARG_TYPE_BINARY_UNSIGNED {
        if style.precision != -1 then
          warning("Setting precision for integer format specifiers is not supported; use '%n' instead");

        var (t,ok) = _toUnsigned(arg);
        if ! ok {
          err = qio_format_error_arg_mismatch(i);
        } else {
          if argType(i) == QIO_CONV_ARG_TYPE_BINARY_UNSIGNED then
            err = _write_unsigned(style.max_width_bytes, t, i);
          else
            try _writeOne(_iokind.dynamic, t, origLocale);
        }
      } when QIO_CONV_ARG_TYPE_REAL, QIO_CONV_ARG_TYPE_BINARY_REAL {
        var (t,ok) = _toReal(arg);
        if ! ok {
          err = qio_format_error_arg_mismatch(i);
        } else {
          if argType(i) == QIO_CONV_ARG_TYPE_BINARY_REAL then
            err = _write_real(style.max_width_bytes, t, i);
          else
            try _writeOne(_iokind.dynamic, t, origLocale);
        }
      } when QIO_CONV_ARG_TYPE_IMAG, QIO_CONV_ARG_TYPE_BINARY_IMAG {
        var (t,ok) = _toImag(arg);
        if ! ok {
          err = qio_format_error_arg_mismatch(i);
        } else {
          if argType(i) == QIO_CONV_ARG_TYPE_BINARY_IMAG then
            err = _write_real(style.max_width_bytes, t:real, i);
          else
            try _writeOne(_iokind.dynamic, t, origLocale);
        }
      } when QIO_CONV_ARG_TYPE_COMPLEX, QIO_CONV_ARG_TYPE_BINARY_COMPLEX {
        var (t,ok) = _toComplex(arg);
        if ! ok {
          err = qio_format_error_arg_mismatch(i);
        } else {
          if argType(i) == QIO_CONV_ARG_TYPE_BINARY_COMPLEX then
            err = _write_complex(style.max_width_bytes, t, i);
          else try _writeOne(_iokind.dynamic, t, origLocale);
        }
      } when QIO_CONV_ARG_TYPE_NUMERIC {
        var (t,ok) = _toNumeric(arg);
        if ! ok {
          err = qio_format_error_arg_mismatch(i);
        } else try _writeOne(_iokind.dynamic, t, origLocale);
      } when QIO_CONV_ARG_TYPE_CHAR {
        var (t,ok) = _toChar(arg);
        if ! ok {
          err = qio_format_error_arg_mismatch(i);
        } else try _writeOne(_iokind.dynamic, new _internalIoChar(t), origLocale);
      } when QIO_CONV_ARG_TYPE_BINARY_STRING {
        var (t,ok) = _toBytes(arg);
        if ! ok {
          err = qio_format_error_arg_mismatch(i);
        } else try _writeOne(_iokind.dynamic, t, origLocale);
      } when QIO_CONV_ARG_TYPE_STRING { // can only happen with string
        var (t,ok) = _toString(arg);
        if ! ok {
          err = qio_format_error_arg_mismatch(i);
        } else try _writeOne(_iokind.dynamic, t, origLocale);
      } when QIO_CONV_ARG_TYPE_REGEX { // It's not so clear what to do when printing
        // a regex. So we just don't handle it.
        err = qio_format_error_write_regex();
      } when QIO_CONV_ARG_TYPE_SERDE {
        if serializerType != nothing {
          if serializerType == binarySerializer {
            warnBinary(arg.type, 3);
          }
          this._serializeOne(arg, origLocale);
        } else {
          try _writeOne(_iokind.dynamic, arg, origLocale);
        }
      } otherwise {
        // Unhandled argument type!
        throw new owned IllegalArgumentError(
          "illegal argument 'args(" + i:string + ")': writef internal error " + argType(i):string
        );
      }
    }
  }
}

/*

   Write arguments according to a format. See
   :ref:`about-io-formatted-io`.

   :arg fmt: the format as string or bytes

   :arg args: 0 or more arguments to write

   :throws IllegalArgumentError: if an unsupported argument type is encountered.
   :throws SystemError: if the arguments could not be written.
 */
proc fileWriter.writef(fmtStr: ?t, const args ...?k) throws
  where isStringType(t) || isBytesType(t)
{
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

    for i in 0..argType.size-1 {
      argType(i) = QIO_CONV_UNK;
    }

    var j = 0;

    for param i in 0..k-1 {
      _writefOne(fmtStr, args(i), i, cur, j, c_ptrTo(argType[0]), argTypeLen,
                 conv, gotConv, style, err, origLocale, len);
    }

    if ! err {
      if cur < len {
        var dummy:c_int;
        this._format_reader(fmtStr, cur, len, err,
                            conv, gotConv, style);
      }

      if cur < len {
        // Mismatched number of arguments!
        err = qio_format_error_too_few_args();
      }
    }
  }

  if err then try this._ch_ioerror(err, "in fileWriter.writef(fmt:string)");
}

// documented in varargs version
@chpldoc.nodoc
proc fileWriter.writef(fmtStr:?t) throws
  where isStringType(t) || isBytesType(t)
{
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

    _format_reader(fmtStr, cur, len, err,
                   conv, gotConv, style);

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

  if err then try this._ch_ioerror(err, "in fileWriter.writef(fmt:string, ...)");
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

   :throws UnexpectedEofError: If EOF was encountered before data could be read.
   :throws SystemError: If the arguments could not be read.
 */
proc fileReader.readf(fmtStr:?t, ref args ...?k): bool throws
    where isStringType(t) || isBytesType(t) {

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
                         conv, gotConv, style, r);

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
                  try _readOne(_iokind.dynamic, ti, origLocale);
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
                  try _readOne(_iokind.dynamic, ti, origLocale);
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
                  try _readOne(_iokind.dynamic, ti, origLocale);
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
                  try _readOne(_iokind.dynamic, ti, origLocale);
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
                  try _readOne(_iokind.dynamic, ti, origLocale);
                if ! err then err = _setIfPrimitive(args(i),ti,i);
              }
            } when QIO_CONV_ARG_TYPE_NUMERIC {
              var (t,ok) = _toNumeric(args(i));
              if !ok {
                err = qio_format_error_arg_mismatch(i);
              } else {
                var ti = t;
                try _readOne(_iokind.dynamic, ti, origLocale);
                if !err then err = _setIfPrimitive(args(i),ti,i);
              }
            } when QIO_CONV_ARG_TYPE_CHAR {
              var (t,ok) = _toChar(args(i));
              var chr = new _internalIoChar(t);
              if ! ok {
                err = qio_format_error_arg_mismatch(i);
              } else try _readOne(_iokind.dynamic, chr, origLocale);
              if ! err then _setIfChar(args(i),chr.ch);
            } when QIO_CONV_ARG_TYPE_BINARY_STRING {
              var (t,ok) = _toBytes(args(i));
              if ! ok {
                err = qio_format_error_arg_mismatch(i);
              } else try _readOne(_iokind.dynamic, t, origLocale);
              if ! err then err = _setIfPrimitive(args(i),t,i);
            } when QIO_CONV_ARG_TYPE_STRING {
              var (t,ok) = _toString(args(i));
              if ! ok {
                err = qio_format_error_arg_mismatch(i);
              } else try _readOne(_iokind.dynamic, t, origLocale);
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
            } when QIO_CONV_ARG_TYPE_SERDE {
              if deserializerType != nothing {
                if deserializerType == binaryDeserializer {
                  warnBinaryRead(args(i).type, 4);
                }
                this._deserializeOne(args(i), origLocale);
              } else {
                try _readOne(_iokind.dynamic, args(i), origLocale);
              }
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
              throw new owned IllegalArgumentError(
                "illegal argument 'args(" + i:string + ")': readf internal error " + argType(i):string
              );
            }
          }
        }
      }

      if ! err {
        if cur < len {
          var dummy:c_int;
          _format_reader(fmtStr, cur, len, err,
                         conv, gotConv, style, r);
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
    try this._ch_ioerror(err, "in fileReader.readf(fmt:string, ...)");
  }

  return false;
}

// documented in varargs version
@chpldoc.nodoc
proc fileReader.readf(fmtStr:?t) throws
    where isStringType(t) || isBytesType(t) {

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
                     conv, gotConv, style, r);
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
    try this._ch_ioerror(err, "in fileReader.readf(fmt:string)");
    return false;
  }
}

/* Call ``stdin.readf``; see :proc:`fileReader.readf`. */
proc readf(fmt:string, ref args ...?k):bool throws {
  return try stdin.readf(fmt, (...args));
}
// documented in varargs version
@chpldoc.nodoc
proc readf(fmt:string):bool throws {
  return try stdin.readf(fmt);
}

/*

  Return a new string consisting of values formatted according to a
  format string.  See :ref:`about-io-formatted-io`.

  :arg this: the format string
  :arg args: the arguments to format
  :returns: the resulting string

  :throws UnexpectedEofError: The size of the temporary buffer was exceeded
                              while writing the string.
  :throws BadFormatError: Improperly formatted values.
  :throws SystemError: If the string could not be formatted for another reason.
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

  :throws UnexpectedEofError: The size of the temporary buffer was exceeded
                              while writing the bytes.
  :throws BadFormatError: Improperly formatted values.
  :throws SystemError: If the bytes could not be formatted for another reason.
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

pragma "no inline"
private proc chpl_do_format_read(f, size: int) throws {
  var buf = allocate(uint(8), (size+1).safeCast(c_size_t));
  var r = f.reader(locking=false);
  defer {
    try {
      r.close();
    } catch { /* ignore deferred close error */ }
  }

  r.readBinary(buf, size);

  // close errors are thrown instead of ignored
  r.close();
  f.close();

  // Add the terminating NULL byte to make C string conversion easy.
  buf[size] = 0;

  return buf;
}

pragma "no inline"
private proc chpl_do_format(fmt:?t, args ...?k): t throws
    where isStringType(t) || isBytesType(t) {

  // Open a memory buffer to store the result
  var f = openMemFile();
  defer {
    try {
      f.close();
    } catch { /* ignore deferred close error */ }
  }

  var offset:int = 0;
  {
    var w = f.writer(locking=false);
    defer {
      try {
        w.close();
      } catch { /* ignore deferred close error */ }
    }
    w.writef(fmt, (...args));
    try! w.lock();
    offset = w.offset();
    w.unlock();

    // close error is thrown instead of ignored
    w.close();
  }

  return t.createAdoptingBuffer(chpl_do_format_read(f, size=offset), offset, offset+1);
}


// ---------------------------------------------------------------
// ---------------------------------------------------------------
// Starting support for regular expression search on channels
// ---------------------------------------------------------------
// ---------------------------------------------------------------



use Regex;

private extern proc qio_regex_channel_match(const ref re:qio_regex_t, threadsafe:c_int, ch:qio_channel_ptr_t, maxlen:int(64), anchor:c_int, can_discard:bool, keep_unmatched:bool, keep_whole_pattern:bool, submatch:_ddata(qio_regex_string_piece_t), nsubmatch:int(64)):errorCode;

@chpldoc.nodoc
proc fileReader._extractMatch(m:regexMatch, ref arg:regexMatch,
                              ref error:errorCode) {
  // If the argument is a match record, just return it.
  arg = m;
}

@chpldoc.nodoc
proc fileReader._extractMatch(m:regexMatch, ref arg:bytes,
                              ref error:errorCode) {
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
    var ts: c_ptrConst(c_char);
    error =
        qio_channel_read_string(false, _iokind.native:c_int, len: int(64),
                                _channel_internal, ts, gotlen, len: c_ssize_t);
    s = bytes.createAdoptingBuffer(ts, length=gotlen);
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

@chpldoc.nodoc
proc fileReader._extractMatch(m:regexMatch, ref arg:?t, ref error:errorCode)
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
      try {
        arg = s:arg.type;
      } catch {
        error = EFORMAT;
      }
    }
  } else {
    var empty:arg.type;
    arg = empty;
  }
}


/*  Sets arg to the string of a match.

    Assumes that the :record:`~IO.fileReader` has been marked before where
    the captures are being returned. Will change the fileReader
    offset to just after the match. Will not do anything
    if error is set.

    :arg m: a :record:`Regex.regexMatch` storing a location that matched
    :arg arg: an argument to retrieve the match into. If it is not a string,
              the string match will be cast to arg.type.

    :throws SystemError: If a match could not be extracted.
 */
proc fileReader.extractMatch(m:regexMatch, ref arg) throws {
  var err:errorCode = 0;
  on this._home {
    try this.lock(); defer { this.unlock(); }
    _extractMatch(m, arg, err);
  }
  if err {
    try this._ch_ioerror(err, "in fileReader.extractMatch(m:regexMatch, ref " +
                              arg.type:string + ")");
  }
}

/*  Returns the value of a match

    Assumes that the :record:`~IO.fileReader` has been marked before where
    the captures are being returned. Will change the fileReader
    offset to just after the match. Will not do anything
    if error is set.

    :arg m: a :record:`Regex.regexMatch` storing a location that matched
    :arg t: the type of the value to return, defaults to string

    :throws SystemError: If a match could not be extracted.
 */
@unstable("this overload of extractMatch is unstable pending a design discussion")
proc fileReader.extractMatch(m: regexMatch, type t = string): t throws {
  var arg:t;
  try this.extractMatch(m, arg);
  return arg;
}

// Assumes that the fileReader has been marked where the search began
// (or at least before the capture groups if discarding)
@chpldoc.nodoc
proc fileReader._ch_handle_captures(matches:_ddata(qio_regex_string_piece_t),
                                    nmatches:int,
                                    ref captures, ref error:errorCode) {
  assert(nmatches >= captures.size);
  for param i in 0..captures.size-1 {
    var m = _to_regexMatch(matches[i]);
    _extractMatch(m, captures[i], error);
  }
}

// helper for _searchHelp
private proc advanceToEof(fr: fileReader): errorCode {
  var error:errorCode = 0;
  // advance max(int(64)) bytes to reach EOF for sure
  error = qio_channel_advance(false, fr._channel_internal, max(int(64)));
  // don't worry about EOF since that is expected
  if error == EEOF then error = 0;
  return error;
}

// Private implementation helper for fileReader.search(re:regex(?))
@chpldoc.nodoc
private inline proc _searchHelp(ref fr: fileReader,
                                re:regex(?),
                                ref error:errorCode,
                                param numMatches,
                                param doCapture: bool,
                                ref captures):regexMatch
{
  var m:regexMatch;
  on fr._home {
    try! fr.lock(); defer { fr.unlock(); }
    param nm = numMatches;
    var matches = _ddata_allocate(qio_regex_string_piece_t, nm);
    error = qio_channel_mark(false, fr._channel_internal);
    if !error {
      error = qio_regex_channel_match(re._regex,
                                     false, fr._channel_internal, max(int(64)),
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
        if doCapture {
          // Extract the capture groups.
          fr._ch_handle_captures(matches, nm, captures, error);
        }

        // Advance to the match.
        qio_channel_revert_unlocked(fr._channel_internal);
        var cur = qio_channel_offset_unlocked(fr._channel_internal);
        var target = m.byteOffset:int;
        // TODO: this can be qio_channel_advance_unlocked since we have already locked
        error = qio_channel_advance(false, fr._channel_internal, target - cur);
      } else {
        // If we didn't match... advance the fileReader position to EOF
        qio_channel_commit_unlocked(fr._channel_internal);
        error = advanceToEof(fr);
      }
    }
    _ddata_free(matches, nm);
  }
  return m;
}



// documented in the version with captures
@chpldoc.nodoc
proc ref fileReader.search(re:regex(?)):regexMatch throws
{
  var e:errorCode = 0;
  var dummy:int;
  var ret = _searchHelp(this, re, e, 1, false, dummy);
  if e then try this._ch_ioerror(e, "in fileReader.search");
  return ret;
}

/* Search for an offset in the :record:`~IO.fileReader` from the current offset
    matching the passed regular expression, possibly pulling out capture
    groups. If there is a match, leaves the fileReader offset at the beginning
    of the match. If there is no match, the fileReader offset will be advanced
    to the end of the fileReader (or end of the file).

    Throws a SystemError if an error occurs.

    :arg re: a :record:`Regex.regex` record representing a compiled
             regular expression.
    :arg captures: an optional variable number of arguments in which to
                   store the regions of the file matching the capture groups
                   in the regular expression.
    :returns: the region of the fileReader that matched
 */
proc ref fileReader.search(re:regex(?), ref captures ...?k): regexMatch throws
{
  var e:errorCode = 0;
  var ret = _searchHelp(this, re, e, captures.size + 1, true, captures);
  if e then try this._ch_ioerror(e, "in fileReader.search");
  return ret;
}

/* Enumerates matches in the string as well as capture groups.

   Yields tuples of :record:`Regex.regexMatch` objects, the 1st is always
   the match for the whole pattern.

   At the time each match is returned, the :record:`~IO.fileReader` offset is at
   the start of that match. Note though that you would have to use
   :proc:`IO.fileReader.advance` to get to the offset of a capture group.

   After yielding each match, advances to just after that
   match and looks for another match. Thus, it will not return
   overlapping matches.

   In the end, leaves the fileReader offset at the end of the
   last reported match (if we ran out of maxmatches)
   or at the end of the fileReader (if we no longer matched).

   Holds the fileReader lock for the duration of the search.

   :arg re: a :record:`Regex.regex` record representing a compiled
            regular expression.
   :arg captures: an optional compile-time constant representing the number
                  of captures to be yielded in tuple elements.
   :arg maxmatches: the maximum number of matches to report.
   :yields: tuples of :record:`Regex.regexMatch` objects, where the first
            element is the whole pattern.  The tuples will have 1+captures
            elements.
 */
iter fileReader.matches(re:regex(?), param captures=0,
                        maxmatches:int = max(int))
// TODO: should be throws
{
  var go = true;
  var i = 0;
  var error:errorCode = 0;
  param nret = captures+1;
  var ret:nret*regexMatch;

  // TODO should be try not try!  ditto try! mark() below
  try! lock();
  on this._home do try! mark();

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
  commit();
  if i < maxmatches {
    // we stopped because eof, move to end
    error = advanceToEof(this);
  }
  unlock();
  // Don't report didn't find or end-of-file errors.
  if error == EFORMAT || error == EEOF then error = 0;
  // TODO should be try not try!
  if error then try! this._ch_ioerror(error, "in fileReader.matches");
}

} /* end of FormattedIO module */

public use FormattedIO;

} /* end of IO module */
