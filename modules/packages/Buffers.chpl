/*
 * Copyright 2004-2018 Cray Inc.
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

/* Support for buffers - regions of memory without a particular interpretation.

   This module provides :record:`bytes` and :record:`buffer` types which
   can be used to manage memory regions.

   These types should be safe to use in a multi-locale context. These types
   should free their memory after the last user of that memory goes out of
   scope. They are currently reference counted but that may not always be the
   case.

   Bytes Type
   ----------

   A :record:`bytes` is a contiguous memory region - really a data structure
   containing a pointer, length, and also the information necessary to free the
   memory region when it is no longer used.

   Buffers
   -------

   A :record:`buffer` consists of a sequence of views into :record:`bytes`
   objects. A :record:`bytes` object might be shared by several
   :record:`buffer` objects.
   It is efficient to go to a particular offset in a buffer, and to push or pop
   bytes objects from the beginning or end of a buffer.

 */
module Buffers {
  use SysBasic;
  use SysError;

  pragma "no doc"
  extern type qbytes_ptr_t;
  pragma "no doc"
  extern type qbuffer_ptr_t;
  pragma "no doc"
  extern type qbuffer_iter_t;
  private extern const QBYTES_PTR_NULL:qbytes_ptr_t;
  private extern const QBUFFER_PTR_NULL:qbuffer_ptr_t;


  private extern proc qbytes_retain(qb:qbytes_ptr_t);
  private extern proc qbytes_release(qb:qbytes_ptr_t);
  private extern proc qbytes_len(qb:qbytes_ptr_t):int(64);
  private extern proc qbytes_data(qb:qbytes_ptr_t):c_void_ptr;

  private extern proc qbytes_create_iobuf(out ret:qbytes_ptr_t):syserr;
  private extern proc qbytes_create_calloc(out ret:qbytes_ptr_t, len:int(64)):syserr;

  private extern proc qbuffer_iter_null():qbuffer_iter_t;

  private extern proc qbuffer_create(out buf:qbuffer_ptr_t):syserr;
  private extern proc qbuffer_retain(buf:qbuffer_ptr_t);
  private extern proc qbuffer_release(buf:qbuffer_ptr_t);

  private extern proc qbuffer_append(buf:qbuffer_ptr_t, bytes:qbytes_ptr_t, skip_bytes:int(64), len_bytes:int(64)):syserr;
  private extern proc qbuffer_append_buffer(buf:qbuffer_ptr_t, src:qbuffer_ptr_t, src_start:qbuffer_iter_t, src_end:qbuffer_iter_t):syserr;
  private extern proc qbuffer_prepend(buf:qbuffer_ptr_t, bytes:qbytes_ptr_t, skip_bytes:int(64), len_bytes:int(64)):syserr;
  private extern proc qbuffer_flatten(buf:qbuffer_ptr_t, start:qbuffer_iter_t, end:qbuffer_iter_t, out bytes_out):syserr;
  private extern proc qbuffer_copyout(buf:qbuffer_ptr_t, start:qbuffer_iter_t, end:qbuffer_iter_t, ref x, size):syserr;
  private extern proc qbuffer_copyout(buf:qbuffer_ptr_t, start:qbuffer_iter_t, end:qbuffer_iter_t, x: c_ptr, size):syserr;
  private extern proc qbuffer_copyout(buf:qbuffer_ptr_t, start:qbuffer_iter_t, end:qbuffer_iter_t, x: c_void_ptr, size):syserr;
  private extern proc qbuffer_copyin(buf:qbuffer_ptr_t, start:qbuffer_iter_t, end:qbuffer_iter_t, ref x, size):syserr;
  private extern proc qbuffer_copyin(buf:qbuffer_ptr_t, start:qbuffer_iter_t, end:qbuffer_iter_t, x: c_ptr, size):syserr;
  private extern proc qbuffer_copyin(buf:qbuffer_ptr_t, start:qbuffer_iter_t, end:qbuffer_iter_t, x: c_void_ptr, size):syserr;

  private extern proc qbuffer_begin(buf:qbuffer_ptr_t):qbuffer_iter_t;
  private extern proc qbuffer_end(buf:qbuffer_ptr_t):qbuffer_iter_t;
  private extern proc qbuffer_iter_next_part(buf:qbuffer_ptr_t, ref it:qbuffer_iter_t);
  private extern proc qbuffer_iter_prev_part(buf:qbuffer_ptr_t, ref it:qbuffer_iter_t);
  private extern proc qbuffer_iter_advance(buf:qbuffer_ptr_t, ref it:qbuffer_iter_t, amt:int(64));

  private extern proc qbuffer_iter_get(it: qbuffer_iter_t, end:qbuffer_iter_t,
                                out bytes_out:qbytes_ptr_t,
                                out skip_out:int(64),
                                out len_out:int(64));
  private extern proc qbuffer_iter_num_bytes(start:qbuffer_iter_t, end:qbuffer_iter_t):int(64);

  private extern proc qbuffer_len(buf:qbuffer_ptr_t):int(64);

  private extern proc debug_print_qbuffer_iter(/*const*/ ref it:qbuffer_iter_t);


  private extern proc qbuffer_start_offset(buf:qbuffer_ptr_t):int(64);
  private extern proc qbuffer_end_offset(buf:qbuffer_ptr_t):int(64);
  private extern proc qbuffer_reposition(buf:qbuffer_ptr_t, new_offset_start:int(64));

  private extern proc bulk_get_bytes(src_locale:int, src_addr:qbytes_ptr_t):qbytes_ptr_t;

  private extern proc bulk_put_buffer(dst_locale:int, dst_addr:c_void_ptr, dst_len:int(64), buf:qbuffer_ptr_t, start:qbuffer_iter_t, end:qbuffer_iter_t):syserr;

  // Now define the Chapel types using the originals..

  /* This type represents a contiguous sequence of bytes.
     This sequence of bytes is represented with a C pointer and length and is
     currently reference counted.  Note that this record contains private
     fields in addition to the home field.

   */
  pragma "ignore noinit"
  record bytes {
    /* The home locale storing the data */
    var home: locale;
    pragma "no doc"
    var _bytes_internal:qbytes_ptr_t = QBYTES_PTR_NULL;
  }

  // bytes methods.
  /* Construct an empty bytes object */
  proc bytes.init() {
    this.home = here;
    this._bytes_internal = QBYTES_PTR_NULL;
  }
  /*

     defaultOf appears to be unnecessary since the initializer
     in the class takes care of it.

  inline proc _defaultOf(type t): t where t == bytes {
    var ret:bytes = noinit;
    ret.home = here;
    ret._bytes_internal = QBYTES_PTR_NULL;
    return ret;
  }*/

  /* Construct a bytes object by allocating zero-filled memory.

     :arg len: the number of bytes to allocate
     :arg error: (optional) capture an error that was encountered instead of
                 halting on error
   */
  proc bytes.init(len:int(64), out error:syserr) {
    this.home = here;
    this.complete();
    error = qbytes_create_calloc(this._bytes_internal, len);
    // The buffer is "retained" internally on creation, but only on success.
  }
  pragma "no doc"
  proc bytes.init(len:int(64)) {
    this.home = here;
    this.complete();
    var error:syserr = qbytes_create_calloc(this._bytes_internal, len);
    if error then try! ioerror(error, "in bytes constructor");
    // The buffer is retained internally on construction, but only on success.
  }


  pragma "no doc"
  proc bytes.init(x: bytes) {
    this.home = here;
    if x.home == here {
      qbytes_retain(x._bytes_internal);
      this._bytes_internal = x._bytes_internal;
    } else {
      // The initial ref count is 1, so no need to call qbytes_retain here.
      this._bytes_internal = bulk_get_bytes(x.home.id, x._bytes_internal);
    }
  }


  pragma "no doc"
  private proc create_iobuf(out error:syserr):bytes {
    var ret:bytes;
    error = qbytes_create_iobuf(ret._bytes_internal);
    // The buffer is "retained" internally on creation, but only on success.
    ret.home = here;
    return ret;
  }
  pragma "no doc"
  private proc create_iobuf():bytes throws {
    var err:syserr = ENOERR;
    var ret = create_iobuf(err);
    if err then try ioerror(err, "in create_iobuf");
    return ret;
  }

  pragma "no doc"
  proc =(ref ret:bytes, x:bytes) {
    // retain -- release
    if( x.home == here ) {
      on x.home {
        qbytes_retain(x._bytes_internal);
      }

      on ret.home {
        qbytes_release(ret._bytes_internal);
      }

      ret.home = x.home;
      ret._bytes_internal = x._bytes_internal;
    } else {
      on ret.home {
        qbytes_release(ret._bytes_internal);
      }
      ret.home = here;
      ret._bytes_internal = bulk_get_bytes(x.home.id, x._bytes_internal);
      // On return from bulk_get_bytes, the ref count in ret._bytes_internal
      // should be 1.
      // Note that the error case is not handled (bulk_get_bytes must succeed).
    }
  }

  pragma "no doc"
  proc bytes.deinit() {
    on this.home {
      qbytes_release(this._bytes_internal);
      this._bytes_internal = QBYTES_PTR_NULL;
    }
  }

  /*
    .. note::

       The pointer returned by this method is only valid for the lifetime of
       the `bytes` object and will be invalid if this memory is freed.

    :returns: a `c_void_ptr` to the internal byte array
   */
  proc bytes.ptr(): c_void_ptr {
    return qbytes_data(this._bytes_internal);
  }

  /* :returns: the number of bytes stored in a :record:`bytes` object */
  proc bytes.len:int(64) {
    var ret:int(64);
    on home {
     ret = qbytes_len(this._bytes_internal);
    }
    return ret;
  }


  /* This type represents a particular location with a buffer.
     Use buffer methods like :proc:`buffer.start` and :proc:`buffer.advance` to
     create and manipulate :record:`buffer_iterator` s.  Note that this record
     contains private fields in addition to the home field.

   */
  pragma "ignore noinit"
  record buffer_iterator {
    /* The home locale storing the data */
    var home: locale;
    pragma "no doc"
    var _bufit_internal:qbuffer_iter_t = qbuffer_iter_null();
  }

  /* Create a :record:`buffer_iterator` that points nowhere */
  pragma "no doc"
  proc buffer_iterator.init() {
    this.home = here;
    this._bufit_internal = qbuffer_iter_null();
  }

  /* A region within a buffer (indicated by two :record:`buffer_iterator` s ) */
  pragma "ignore noinit"
  record buffer_range {
    var start:buffer_iterator;
    var end:buffer_iterator;
  }

  /* :returns: the number of bytes stored in a :record:`buffer_range` */
  proc buffer_range.len:int(64) {
    var ret:int(64);
    on this.start.home {
      ret = qbuffer_iter_num_bytes(this.start._bufit_internal,
                                   this.end._bufit_internal);
    }
    return ret;
  }

  // FUTURE -- we could create const buffers
  // with a boolean type param

  /* A buffer which can contain multiple memory regions
     (that is, multiple regions of :record:`bytes` objects).  Note that this
     record contains private fields in addition to the home field.

   */
  pragma "ignore noinit"
  record buffer {
    /* The home locale storing the data */
    var home:locale;
    pragma "no doc"
    var _buf_internal:qbuffer_ptr_t = QBUFFER_PTR_NULL;
  }


  /* buffer methods. */

  /* Create an empty buffer.

     :arg error: (optional) capture an error that was encountered instead of
                  halting on error
   */
  proc buffer.init(out error:syserr) {
    this.home = here;
    this.complete();
    error = qbuffer_create(this._buf_internal);
  }
  pragma "no doc"
  proc buffer.init() /*throws*/ {
    var error:syserr = ENOERR;
    this.home = here;
    this.complete();
    error = qbuffer_create(this._buf_internal);
    // TODO: really want the following to be `try` once we can throw from
    // initializers
    if error then try! ioerror(error, "in buffer constructor");
  }
  pragma "no doc"
  proc buffer.init(x: buffer) {
    if x.home == here {
      qbuffer_retain(x._buf_internal);
      this.home = here;
      this._buf_internal = x._buf_internal;
      this.complete();
    } else {
      var error: syserr = ENOERR;
      this.init(error);
      if error then halt("Got an error when initializing a new buffer.");
      var start_offset:int(64);
      var end_offset:int(64);

      on x.home {
        start_offset = qbuffer_start_offset(x._buf_internal);
        end_offset = qbuffer_end_offset(x._buf_internal);
      }

      var allocErr:syserr = ENOERR;
      var b = new bytes(end_offset - start_offset, error=allocErr);
      if allocErr then
        try! ioerror(allocErr, "could not allocate bytes in buffer copy");

      var len = qbytes_len(b._bytes_internal);
      var ptr = qbytes_data(b._bytes_internal);

      // Now create a local buffer with the right data
      // in it starting at the right position.
      qbuffer_reposition(this._buf_internal, start_offset);

      var there_uid = here.id;

      on x.home {
        var err:syserr = ENOERR;
        err = bulk_put_buffer(there_uid, ptr, len, x._buf_internal,
                              qbuffer_begin(x._buf_internal),
                              qbuffer_end(x._buf_internal));
        if err then try! ioerror(err, "put failed in buffer copy");
      }

      var appendErr:syserr = ENOERR;
      this.append(b, error=appendErr);
      if appendErr then
        try! ioerror(appendErr, "append failed in buffer copy");
    }
  }
  /*
     defaultOf appears to be unnecessary since the initializer
     in the class takes care of it.

  inline proc _defaultOf(type t): t where t == buffer {
    var ret:buffer = noinit;
    ret.home = here;
    ret._buf_internal = QBUFFER_PTR_NULL;
    return ret;
  }*/



  /* Flatten a buffer. Create a new :record:`bytes` object and copy
     the buffer into it. This function should work even if buffer is
     remote.

     :arg range: the region of the buffer to copy, for example buffer.all()
     :arg error: (optional) capture an error that was encountered instead of
                 halting on error
     :returns: a newly constructed bytes object on the current locale
   */
  proc buffer.flatten(range:buffer_range, out error:syserr) {
  //proc buffer.flatten(range:buffer_range, out error:syserr):bytes {
    var ret:bytes = new bytes();
    if this.home == here {
      error = qbuffer_flatten(this._buf_internal, range.start._bufit_internal, range.end._bufit_internal, ret._bytes_internal);
    } else {
      var dst_locale = here;
      var dst_len:int(64) = range.len;
      ret = new bytes(dst_len, error=error);
      if error then return ret;
      var dst_addr = qbytes_data(ret._bytes_internal);
      on this.home {
        // Copy the buffer to the bytes...
        error = bulk_put_buffer(dst_locale.id, dst_addr, dst_len,
                                this._buf_internal,
                                range.start._bufit_internal,
                                range.end._bufit_internal);
      }
    }
    return ret;
  }

  pragma "no doc"
  proc buffer.flatten(range:buffer_range) throws {
  //proc buffer.flatten(range:buffer_range):bytes {
    var error:syserr = ENOERR;
    var ret:bytes = new bytes();
    ret = this.flatten(range,error);
    if error then try ioerror(error, "in buffer.flatten");
    return ret;
  }

  pragma "no doc"
  proc =(ref ret:buffer, x:buffer) throws {
    ret.home = here;
    // retain -- release
    if( x.home == ret.home ) {
      on x.home {
        qbuffer_retain(x._buf_internal);
      }

      on ret.home {
        qbuffer_release(ret._buf_internal);
      }

      ret.home = x.home;
      ret._buf_internal = x._buf_internal;
    } else {
      on ret.home {
        qbuffer_release(ret._buf_internal);
      }

      var start_offset:int(64);
      var end_offset:int(64);

      on x.home {
        start_offset = qbuffer_start_offset(x._buf_internal);
        end_offset = qbuffer_end_offset(x._buf_internal);
      }

      var b = new bytes(end_offset - start_offset);
      var len = qbytes_len(b._bytes_internal);
      var ptr = qbytes_data(b._bytes_internal);

      // Now create a local buffer with the right data
      // in it starting at the right position.
      qbuffer_reposition(ret._buf_internal, start_offset);

      var there_uid = here.id;

      on x.home {
        var err:syserr = ENOERR;
        err = bulk_put_buffer(there_uid, ptr, len, x._buf_internal,
                              qbuffer_begin(x._buf_internal),
                              qbuffer_end(x._buf_internal));
        if err then try ioerror(err, "in buffer assignment");
      }

      ret.append(b);
    }
  }


  pragma "no doc"
  proc buffer.deinit() {
    on this.home {
      qbuffer_release(this._buf_internal);
      this._buf_internal = QBUFFER_PTR_NULL;
    }
  }

  /* :returns: the number of bytes stored in a :record:`buffer` object */
  proc buffer.len {
    var ret:int(64);
    on this.home {
      ret = qbuffer_len(this._buf_internal);
    }
    return ret;
  }

  /* Append a :record:`bytes` object to a :record:`buffer`.
     This function might store the passed bytes object by reference instead of
     copying it. The current implementation will always do so and will always
     increase the reference count of the bytes object.  The version of this
     function called without the error argument will halt if an error is
     encountered.

     :arg b: the :record:`bytes` object to append
     :arg skip_bytes: how many bytes at the front of b to skip
     :arg len_bytes: how many bytes to append to the buffer
     :arg error: (optional) capture an error that was encountered instead of
           halting on error
  */
  proc buffer.append(b:bytes, skip_bytes:int(64) = 0, len_bytes:int(64) = b.len, out error:syserr) {
    on this.home {
      error = qbuffer_append(this._buf_internal, b._bytes_internal, skip_bytes, len_bytes);
    }
  }

  pragma "no doc"
  proc buffer.append(b:bytes, skip_bytes:int(64) = 0, len_bytes:int(64) = b.len) throws {
    var err:syserr = ENOERR;
    this.append(b, skip_bytes, len_bytes, err);
    if err then try ioerror(err, "in buffer.append");
  }

  /* Append a :record:`buffer` object to a :record:`buffer`.
     This function might store a pointers to the bytes objects contained in
     `buf` instead of copying them. If that happens, the current implementation
     will increase their reference counts.  The version of this function called
     without the error argument will halt if an error is encountered.

     :arg buf: the :record:`buffer` object to append
     :arg part: a :record:`buffer_range` indicating which section of the
                buffer to copy. Defaults to all of the buffer.
     :arg error: (optional) capture an error that was encountered instead of
                 halting on error
   */
  proc buffer.append(buf:buffer, part:buffer_range = buf.all(), out error:syserr) {
    on this.home {
      error = qbuffer_append_buffer(this._buf_internal, buf._buf_internal, part.start._bufit_internal, part.end._bufit_internal);
    }
  }
  pragma "no doc"
  proc buffer.append(buf:buffer, part:buffer_range = buf.all()) throws {
    var err:syserr = ENOERR;
    this.append(buf, part, err);
    if err then try ioerror(err, "in buffer.append");
  }

  /* Prepend a :record:`bytes` object to a :record:`buffer`.
     This function might store the passed bytes object by reference instead of
     copying it. The current implementation will always do so and will always
     increase the reference count of the bytes object.  The version of this
     function called without the error argument will halt if an error is
     encountered.

     :arg b: the :record:`bytes` object to prepend
     :arg skip_bytes: how many bytes at the front of b to skip
     :arg len_bytes: how many bytes to append to the buffer
     :arg error: (optional) capture an error that was encountered instead of
           halting on error
  */
  proc buffer.prepend(b:bytes, skip_bytes:int(64) = 0, len_bytes:int(64) = b.len, out error:syserr) {
    on this.home {
      qbuffer_prepend(this._buf_internal, b._bytes_internal, skip_bytes, len_bytes);
    }
  }
  pragma "no doc"
  proc buffer.prepend(b:bytes, skip_bytes:int(64) = 0, len_bytes:int(64) = b.len) throws {
    var err:syserr = ENOERR;
    this.prepend(b, skip_bytes, len_bytes, err);
    if err then try ioerror(err, "in buffer.prepend");
  }

  /* :return: a :record:`buffer_iterator` to the start of a buffer */
  proc buffer.start():buffer_iterator {
    var ret:buffer_iterator;
    ret.home = this.home;
    on this.home {
      ret._bufit_internal = qbuffer_begin(this._buf_internal);
    }
    return ret;
  }
  /* :return: a :record:`buffer_iterator` to the end of a buffer */
  proc buffer.end():buffer_iterator {
    var ret:buffer_iterator;
    ret.home = this.home;
    on this.home {
      ret._bufit_internal = qbuffer_end(this._buf_internal);
    }
    return ret;
  }
  /* :return: a :record:`buffer_range` for the entirety of a buffer */
  proc buffer.all():buffer_range {
    return new buffer_range(this.start(), this.end());
  }

  /* Advance a :record:`buffer_iterator` to the next contiguous
     memory region stored therein

     :arg it: the buffer iterator to advance
   */
  proc buffer.next_part(ref it:buffer_iterator) {
    on this.home {
      var x = it;
      qbuffer_iter_next_part(this._buf_internal, x._bufit_internal);
      it = x;
    }
  }

  /* Advance a :record:`buffer_iterator` to the previous contiguous
     memory region stored therein

     :arg it: the buffer iterator to advance
   */
  proc buffer.prev_part(ref it:buffer_iterator) {
    on this.home {
      var x = it;
      qbuffer_iter_prev_part(this._buf_internal, x._bufit_internal);
      it = x;
    }
  }

  /* Advance a :record:`buffer_iterator` by a particular number
     of bytes.

     :arg it: the buffer iterator to advance
     :arg amount: the number of bytes to advance
   */
  proc buffer.advance(ref it:buffer_iterator, amount:int(64)) {
    on this.home {
      var x = it;
      qbuffer_iter_advance(this._buf_internal, x._bufit_internal, amount);
      it = x;
    }
  }

  pragma "no doc"
  proc buffer_iterator.debug_print()
  {
    on this.home {
      debug_print_qbuffer_iter(this._bufit_internal);
    }
  }

  /* methods to read/write basic types. */

  /* Read a basic type (integral or floating point value) or `string`
     from a buffer.
     For basic types, this method reads the value by copying from memory -
     so it reads a binary value in native endianness. For strings, this method
     reads a string encoded as the string length (as `int`) followed by
     that number of bytes (as `uint(8)`).

     :arg it: a :record:`buffer_iterator` where reading will start
     :arg value: a basic type or `string`
     :arg error: (optional) capture an error that was encountered instead of
                 halting on error
     :returns: a buffer iterator storing the position immediately after
               the read value.
  */
  proc buffer.copyout(it:buffer_iterator, out value: ?T, out error:syserr):buffer_iterator where isNumericType(T) {
    var ret:buffer_iterator;
    ret.home = this.home;
    on this.home {
      var end:buffer_iterator = it;
      var tmp:value.type;
      var sz = numBytes(value.type);
      this.advance(end, sz);
      error = qbuffer_copyout(this._buf_internal, it._bufit_internal, end._bufit_internal, tmp, sz);
      value = tmp;
      ret = end;
    }
    return ret;
  }

  pragma "no doc"
  proc buffer.copyout(it:buffer_iterator, out value: string, out error:syserr):buffer_iterator {
    var ret:buffer_iterator;
    ret.home = this.home;
    on this.home {
      var start = it;
      var end = it;
      // Read string length
      var len: int;
      this.advance(end, numBytes(int));
      error = qbuffer_copyout(this._buf_internal,
                              start._bufit_internal, end._bufit_internal,
                              len, numBytes(int));
      ret = end;
      // Read byte array
      if !error {
        this.advance(start, numBytes(int));
        this.advance(end, len);
        var buf = c_calloc(uint(8), (len+1):size_t);
        error = qbuffer_copyout(this._buf_internal,
                                start._bufit_internal, end._bufit_internal,
                                buf, len);
        value = new string(buff=buf, length=len, size=len+1,
                             owned=true, needToCopy=false);
        ret = end;
      }
    }
    return ret;
  }

  pragma "no doc"
  proc buffer.copyout(it:buffer_iterator, out value):buffer_iterator throws {
    var err:syserr = ENOERR;
    var ret = this.copyout(it, value, err);
    if err then try ioerror(err, "in buffer.copyout");
    return ret;
  }

  /* Write a basic type (integral or floating point value) or `string`
     to a buffer.
     For basic types, this method writes the value by copying to memory -
     so it writes a binary value in native endianness. For strings, this method
     writes a string encoded as the string length (as `int`) followed by
     that number of bytes (as `uint(8)`).

     :arg it: a :record:`buffer_iterator` where reading will start
     :arg value: a basic type or `string`
     :arg error: (optional) capture an error that was encountered instead of
                 halting on error
     :returns: a buffer iterator storing the position immediately after
               the written value.
  */
  proc buffer.copyin( it:buffer_iterator, value: ?T, out error:syserr):buffer_iterator where isNumericType(T) {
    var ret:buffer_iterator;
    ret.home = this.home;
    on this.home {
      //writeln("iterator on way in");
      //debug_print_qbuffer_iter(it._bufit_internal);
      var end = it;
      var tmp = value;
      var sz = numBytes(value.type);
      //writeln("iterator copied to end");
      //debug_print_qbuffer_iter(end._bufit_internal);

      //writeln("sz is ", sz);
      this.advance(end, sz);
      //debug_print_qbuffer_iter(end._bufit_internal);
      error = qbuffer_copyin(this._buf_internal, it._bufit_internal, end._bufit_internal, tmp, sz);
      ret = end;
    }
    return ret;
  }

  pragma "no doc"
  proc buffer.copyin( it:buffer_iterator, value: string, out error:syserr):buffer_iterator {
    var ret:buffer_iterator;
    ret.home = this.home;
    on this.home {
      var start = it;
      var end = it;
      var tmp = value;
      var len = value.length:int;
      // Write string length
      this.advance(end, numBytes(int));
      error = qbuffer_copyin(this._buf_internal,
                             start._bufit_internal, end._bufit_internal,
                             len, numBytes(int));
      ret = end;
      // Write byte array
      if !error {
        this.advance(start, numBytes(int));
        this.advance(end, len);
        error = qbuffer_copyin(this._buf_internal,
                               start._bufit_internal, end._bufit_internal,
                               tmp.c_str():c_void_ptr, len);
        ret = end;
      }
    }
    return ret;
  }

  pragma "no doc"
  proc buffer.copyin( it:buffer_iterator, value):buffer_iterator throws {
    var err:syserr = ENOERR;
    var ret = this.copyin(it, value, err);
    if err then try ioerror(err, "in buffer.copyin");
    return ret;
  }
}
