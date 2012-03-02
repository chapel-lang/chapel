The new I/O system in Chapel offers additional flexibility while
providing a single interface to I/O calls that primarily differ
in performance.

There are 4 main types: file, channel, buffer, bytes.

Auxilary types include iostyle and err_t.

ERROR HANDLING
  I/O functions each have two versions; one takes a by-name error= argument,
  and the other does not. If you supply an error= argument, the error code
  will be returned in that argument. If not, the program will halt with a hopefully
  useful error message.

  Errors have type err_t, which supports some useful operations:
   var err:err_t;
   if err then ... // condition if there is an error
   if !err then ... // condition if there is no error
   var i = err:int(32) // turn the error into an int(32)
   var err2 = i:err_t // turn an int(32) into an err_t
   if err == EEOF then ... // check for a particular error
   if err != EEOF then ... // check for anything other than a particular error

  Hopefully, in the future, an exception facility can at least replace
  the versions that halt on an error.

FILES
  Create a file by calling one of the open() functions. The first form
  takes in a path string and an access mode, as well optional hints
  (the default for channels using that file) and an error object.
  The second form can open an existing file descriptor.

  An access mode is one of:
   mode.r
    like "r" in C, open an existing file for reading
   mode.w 
    like "w" in C, create a new file or truncate an existing one,
    and open it for writing.
   mode.rw
    like "r+" in C, open an existing file for reading and
    writing, but the file must exist.
   mode.wr
    like "w+" in C, open an existing file for reading and
    writing, creating or truncating the file if it exists.



  In addition, there is an opentmp() function, which will open
  a temporary file (ie one in /tmp) that will be deleted when
  the file is closed, and an openmem() function, which will open
  a buffer in memory but allow the usual I/O routines to work with it.

  Files are reference counted and will be closed eventually 
  when all references to a file are out of scope. If you need
  to close a file at a particular moment, there is a file.close()
  method available.

  It is possible - in some situations - for I/O to fail without
  returning an error. In cases where a programmer wants to be sure
  that there was no error writing the data to disk, it is important
  to call file.fsync() to make sure that data has arrived on disk without
  an error. Many errors can be reported with typical operation, but
  some errors can only be reported by the system during file.close
  or even file.fsync.

  It is not possible to seek, read, or write to a file directly.
  Create a channel to proceed.

  When opening a file, one can supply hints about how the file will
  be used. The hints= argument to an open function is a number constructed
  by bit-OR-ing any number of:
     HINT_CACHED - meaning data is or should be cached in memory
     HINT_RANDOM - meaning expect random access pattern
     HINT_SEQUENTIAL - meaning expect sequential access pattern

     Other hints are available (e.g. QIO_METHOD_PREADPWRITE) but
     will likely get more friendly names in the future, and new
     hints are likely to be added.

     The implementation will choose an I/O method (e.g. mmap, pwrite, ...)
     based on the file size and the supplied hints. It is possible to force
     a particular I/O method, but the means to do that is likely to change
     in the future.
   
     0 is always the default value to hint 'normal operation'.

  Lastly, one can supply an I/O style when opening a file. A style controls
  the formatting of basic types such as strings and integers. We support
  binary and text styles. See the STYLES section for more information.
  Generally, omitting the style= argument will get you a default style
  also obtainable with new iostyle().


CREATING CHANNELS
  Create a channel to read from or write to a particular region in
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
    kind -- param var for better-performing binary I/O.
            kind is either dynamic, native, big, or little.
            When this is 'dynamic', the style is consulted in all cases;
            otherwise it is a binary style and basic integral
            and floating point types are directly read and written.
            
    start -- start poisition within the file, default is 0
    end -- maximum end position within the file, default is max(int(64)).
           A channel will never read/write beyond its maximum end position,
           and it will extend the file only as necessary to store data
           written to the channel. In other words, specifying end here
           does not impact the file size; only the section of the file
           that this channel can write to. After all channels to a file
           are closed, that file will have a size equal to the last
           position of any channel.

    hints -- as when opening a file, it is possible to supply hints
             to the channel to direct its I/O. 

             If hints is 0, the hints value provided when opening
             the file will be used.
    
    style -- an I/O style; when omitted you get the style supplied when
             opening the file.
             See the STYLES section below for more information.

    error -- an out err_t argument. See ERROR HANDLING.

USING CHANNELS
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

STYLES 
  A style encapsulates how Chapel's basic types should be read or written.
  In particular, a style contains options for binary and text I/O, including
  byte order, how strings are encoded in binary channels, base for numeric
  text I/O, field widths, and precision.

  In most cases the defaults will serve well.

  It is possible to modify the style of a channel or a file with the
  modifyStyle() method, which takes in a function which is passed
  the style and should modify it, returning an error (0 for no error).

LOCKING
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
  guarantee that they are not called a channel without the appropriate
  locking.

BYTES
  A bytes object is just some data in memory along with a size. Bytes
  objects are reference counted, and the memory will be freed when
  nothing refers to the bytes object any more.

BUFFERS
  A buffer stores some number subsections of bytes objects. It is efficient
  to go to a particular offset in a buffer, and to push or pop bytes
  objects from the beginning or end of a buffer.

  Buffers are used internally in each channel.


