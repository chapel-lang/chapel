.. default-domain:: chpl

.. module:: Buffers
   :synopsis: Support for buffers - regions of memory without a particular interpretation.

Buffers
=======
**Usage**

.. code-block:: chapel

   use Buffers;

Support for buffers - regions of memory without a particular interpretation.

This module provides :record:`byteBuffer` and :record:`buffer` types which
can be used to manage memory regions.

These types should be safe to use in a multi-locale context. These types
should free their memory after the last user of that memory goes out of
scope. They are currently reference counted but that may not always be the
case.

Bytes Type
----------

A :record:`byteBuffer` is a contiguous memory region - really a data structure
containing a pointer, length, and also the information necessary to free the
memory region when it is no longer used.

Buffers
-------

A :record:`buffer` consists of a sequence of views into :record:`byteBuffer`
objects. A :record:`byteBuffer` object might be shared by several
:record:`buffer` objects.
It is efficient to go to a particular offset in a buffer, and to push or pop
bytes objects from the beginning or end of a buffer.



.. record:: byteBuffer

   This type represents a contiguous sequence of bytes.
   This sequence of bytes is represented with a C pointer and length and is
   currently reference counted.  Note that this record contains private
   fields in addition to the home field.
   
   


   .. attribute:: var home: locale

      The home locale storing the data 

.. method:: proc byteBuffer.init()

   Initialize an empty bytes object 

.. method:: proc byteBuffer.init(len: int(64), out error: syserr)

   Initialize a bytes object by allocating zero-filled memory.
   
   :arg len: the number of bytes to allocate
   :arg error: (optional) capture an error that was encountered instead of
               halting on error
   

.. method:: proc byteBuffer.ptr(): c_void_ptr

   
   .. note::
   
      The pointer returned by this method is only valid for the lifetime of
      the `byteBuffer` object and will be invalid if this memory is freed.
   
   :returns: a `c_void_ptr` to the internal byte array
   

.. method:: proc byteBuffer.len: int(64)

   :returns: the number of bytes stored in a :record:`byteBuffer` object 

.. record:: buffer_iterator

   This type represents a particular location with a buffer.
   Use buffer methods like :proc:`buffer.start` and :proc:`buffer.advance` to
   create and manipulate :record:`buffer_iterator` s.  Note that this record
   contains private fields in addition to the home field.
   
   


   .. attribute:: var home: locale

      The home locale storing the data 

.. record:: buffer_range

   A region within a buffer (indicated by two :record:`buffer_iterator` s ) 


   .. attribute:: var start: buffer_iterator

   .. attribute:: var end: buffer_iterator

.. method:: proc buffer_range.len: int(64)

   :returns: the number of bytes stored in a :record:`buffer_range` 

.. record:: buffer

   A buffer which can contain multiple memory regions
   (that is, multiple regions of :record:`byteBuffer` objects).  Note that this
   record contains private fields in addition to the home field.
   
   


   .. attribute:: var home: locale

      The home locale storing the data 

.. method:: proc buffer.init(out error: syserr)

   Create an empty buffer.
   
   :arg error: (optional) capture an error that was encountered instead of
                halting on error
   

.. method:: proc buffer.flatten(range: buffer_range) throws

   Flatten a buffer. Create a new :record:`byteBuffer` object and copy
   the buffer into it. This function should work even if buffer is
   remote.
   
   :arg range: the region of the buffer to copy, for example buffer.all()
   :returns: a newly initialized bytes object on the current locale
   

.. method:: proc buffer.len

   :returns: the number of bytes stored in a :record:`buffer` object 

.. method:: proc buffer.append(b: byteBuffer, skip_bytes: int(64) = 0, len_bytes: int(64) = b.len) throws

   Append a :record:`byteBuffer` object to a :record:`buffer`.
   This function might store the passed bytes object by reference instead of
   copying it. The current implementation will always do so and will always
   increase the reference count of the bytes object.  The version of this
   function called without the error argument will halt if an error is
   encountered.
   
   :arg b: the :record:`byteBuffer` object to append
   :arg skip_bytes: how many bytes at the front of b to skip
   :arg len_bytes: how many bytes to append to the buffer
   

.. method:: proc buffer.append(buf: buffer, part: buffer_range = AppendExpr.Call08) throws

   Append a :record:`buffer` object to a :record:`buffer`.
   This function might store a pointers to the bytes objects contained in
   `buf` instead of copying them. If that happens, the current implementation
   will increase their reference counts.  The version of this function called
   without the error argument will halt if an error is encountered.
   
   :arg buf: the :record:`buffer` object to append
   :arg part: a :record:`buffer_range` indicating which section of the
              buffer to copy. Defaults to all of the buffer.
   

.. method:: proc buffer.prepend(b: byteBuffer, skip_bytes: int(64) = 0, len_bytes: int(64) = b.len) throws

   Prepend a :record:`byteBuffer` object to a :record:`buffer`.
   This function might store the passed bytes object by reference instead of
   copying it. The current implementation will always do so and will always
   increase the reference count of the bytes object.  The version of this
   function called without the error argument will halt if an error is
   encountered.
   
   :arg b: the :record:`byteBuffer` object to prepend
   :arg skip_bytes: how many bytes at the front of b to skip
   :arg len_bytes: how many bytes to append to the buffer
   

.. method:: proc buffer.start(): buffer_iterator

   :return: a :record:`buffer_iterator` to the start of a buffer 

.. method:: proc buffer.end(): buffer_iterator

   :return: a :record:`buffer_iterator` to the end of a buffer 

.. method:: proc buffer.all(): buffer_range

   :return: a :record:`buffer_range` for the entirety of a buffer 

.. method:: proc buffer.next_part(ref it: buffer_iterator)

   Advance a :record:`buffer_iterator` to the next contiguous
   memory region stored therein
   
   :arg it: the buffer iterator to advance
   

.. method:: proc buffer.prev_part(ref it: buffer_iterator)

   Advance a :record:`buffer_iterator` to the previous contiguous
   memory region stored therein
   
   :arg it: the buffer iterator to advance
   

.. method:: proc buffer.advance(ref it: buffer_iterator, amount: int(64))

   Advance a :record:`buffer_iterator` by a particular number
   of bytes.
   
   :arg it: the buffer iterator to advance
   :arg amount: the number of bytes to advance
   

.. method:: proc buffer.copyout(it: buffer_iterator, out value: ?T): buffer_iterator throws

   Read a basic type (integral or floating point value) or `string`
   from a buffer.
   For basic types, this method reads the value by copying from memory -
   so it reads a binary value in native endianness. For strings, this method
   reads a string encoded as the string length (as `int`) followed by
   that number of bytes (as `uint(8)`).
   
   :arg it: a :record:`buffer_iterator` where reading will start
   :arg value: a basic type or `string`
   :returns: a buffer iterator storing the position immediately after
             the read value.
   

.. method:: proc buffer.copyin(it: buffer_iterator, value: ?T): buffer_iterator throws

   Write a basic type (integral or floating point value) or `string`
   to a buffer.
   For basic types, this method writes the value by copying to memory -
   so it writes a binary value in native endianness. For strings, this method
   writes a string encoded as the string length (as `int`) followed by
   that number of bytes (as `uint(8)`).
   
   :arg it: a :record:`buffer_iterator` where reading will start
   :arg value: a basic type or `string`
   :returns: a buffer iterator storing the position immediately after
             the written value.
   

