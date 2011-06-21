The new I/O system in Chapel offers additional flexibility while
providing a single interface to I/O calls that primarily differ
in performance.

There are 4 main types: file, channel, buffer, bytes.

Auxilary types include style and error.

ERROR HANDLING
  I/O functions use ErrorHandler objects for error handling. If no ErrorHandler is
  passed, and an error occurs, halt() is called to terminate the program with
  an error. If an ErrorHandler is provided, the onError method in the handler
  object will be called. The ErrorSaver is an ErrorHandler provided for saving
  I/O errors (so that they can be checked by the caller of an I/O function).
  Hopefully this system can one day be replaced by exceptions or
  another language-supported error-handling facility.

FILES
  Create a file by calling one of the open() functions. The first form
  takes in a path string and an access string, as well optional hints
  (the default for channels using that file) and an error object.
  The second form can open an existing file descriptor.

  In addition, there is an opentmp() function, which will open
  a temporary file (ie one in /tmp) that will be deleted when
  the file is closed.

  Files are reference counted and will be closed eventually 
  when all references to a file are out of scope. If you need
  to close a file at a particular moment, there is a .close()
  method available.

  It is possible - in some situations - for I/O to fail without
  returning an error. In cases where a programmer wants to be sure
  that there was no error writing the data, it is important to call
  file.fsync() to make sure that data has arrived on disk without
  an error.

  It is not possible to seek, read, or write to a file directly.
  Create a channel to proceed.

CREATING CHANNELS
  Create a channel to read from or write to a particular position in
  a file. To do that, use file.reader() and file.writer() methods.

  For example, if you have a file called myfile,
    var writer = myfile.writer();

  A channel can read or write only to a limited region of a file. That way,
  it is possible to write parallel programs in which different tasks write
  to different regions of a file. Channels buffer data, and if a writing
  channel is using the same region of a file as another channel at the same
  time, there will be a race condition and undefined behavior. Since channels
  allow specification of a region of a file, these race conditions can
  be easily avoided. You are ON YOUR OWN if you create channels writing
  simultaneously to overlapping regions of a file.

  Reader and writer methods take the following optional arguments:
    kind -- param var for better-performing binary I/O
            kind is either dynamic, native, big, or little.
            When this is 'dynamic', the style is consulted.
            
    start -- start poisition within the file, default is 0
    end -- maximum end position within the file, default is max(int(64)).
           A channel will never read/write beyond its maximum end position,
           and it will extend the file only as necessary to store data
           written to the channel. In other words, specifying end here
           does not impact the file size; only the section of the file
           that this channel can write to.

    hints -- a number constructed with one of:
              QIO_METHOD_DEFAULT == 0
              QIO_METHOD_READWRITE
              QIO_METHOD_P_READWRITE
              QIO_METHOD_MMAP
             masked with any number of
              QIO_HINT_RANDOM
              QIO_HINT_SEQUENTIAL
              QIO_HINT_LATENCY
              QIO_HINT_BANDWIDTH
              QIO_HINT_CACHED
              QIO_HINT_NOREUSE


             The implementation will choose an I/O method based on
             the file size and the supplied hints. To force a particular
             I/O method, use one of the QIO_METHOD_... values.

             If hints is 0==QIO_METHOD_DEFAULT, the hints value from the
             file object will be used (a hint can be provided to
             the file open function).
    
   style -- an I/O style, or nil to use a default style.
            See the STYLES section below for more information.

   err -- an ErrorHandler object. See ERROR HANDLING.

USING CHANNELS
  Channels contain read_one and write_one methods, which are generic
  methods that can write one of anything, and also take additional
  arguments (kind, style, error object).

  In addition, channels have read() and write() methods that take any
  number of arguments, and will halt if there was an error.

  Sometimes it's important to flush the buffer in a channel -
  to do that, use the .flush() method. Flushing the buffer will
  make all writes available to other applications or other views
  of the file (ie, it will call e.g. the OS call pwrite). Note
  that if you need to ensure that data from a channel is on disk,
  you'll have to call .flush() on the channel and .fsync on the file.

STYLES 
  A style encapsulates how Chapel's basic types should be read or written.
  In particular, a style contains options for binary and text I/O, including
  byte order, how strings are encoded in binary channels, base for numeric
  text I/O, field widths, precision; also how arrays, tuples, records, and
  classes are written or read.

  In most cases the defaults will serve well.

  It is possible to modify the style of a channel or a file with the
  modifyStyle() method, which takes in a function which is passed
  the style and should modify it, returning an error (0 for no error).

BYTES
  A bytes object is just some data in memory along with a size. Bytes
  objects are reference counted, and the memory will be freed when
  nothing refers to the bytes object any more.

BUFFERS
  A buffer stores some number subsections of bytes objects. It is efficient
  to go to a particular offset in a buffer, and to push or pop bytes
  objects from the beginning or end of a buffer.

  Buffers are used internally in each channel.


