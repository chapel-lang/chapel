/*
 * Copyright 2004-2014 Cray Inc.
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

module BufferInternals {
  use SysBasic;
  use Error;

  extern type qbytes_ptr_t;
  extern type qbuffer_ptr_t;
  extern type qbuffer_iter_t;
  extern const QBYTES_PTR_NULL:qbytes_ptr_t;
  extern const QBUFFER_PTR_NULL:qbuffer_ptr_t;


  extern proc qbytes_retain(qb:qbytes_ptr_t);
  extern proc qbytes_release(qb:qbytes_ptr_t);
  extern proc qbytes_len(qb:qbytes_ptr_t):int(64);
  extern proc qbytes_data(qb:qbytes_ptr_t):c_void_ptr;

  extern proc qbytes_create_iobuf(out ret:qbytes_ptr_t):syserr;
  extern proc qbytes_create_calloc(out ret:qbytes_ptr_t, len:int(64)):syserr;

  extern proc qbuffer_iter_null():qbuffer_iter_t;

  extern proc qbuffer_create(out buf:qbuffer_ptr_t):syserr;
  extern proc qbuffer_retain(buf:qbuffer_ptr_t);
  extern proc qbuffer_release(buf:qbuffer_ptr_t);

  extern proc qbuffer_append(buf:qbuffer_ptr_t, bytes:qbytes_ptr_t, skip_bytes:int(64), len_bytes:int(64)):syserr;
  extern proc qbuffer_append_buffer(buf:qbuffer_ptr_t, src:qbuffer_ptr_t, src_start:qbuffer_iter_t, src_end:qbuffer_iter_t):syserr;
  extern proc qbuffer_prepend(buf:qbuffer_ptr_t, bytes:qbytes_ptr_t, skip_bytes:int(64), len_bytes:int(64)):syserr;
  extern proc qbuffer_flatten(buf:qbuffer_ptr_t, start:qbuffer_iter_t, end:qbuffer_iter_t, out bytes_out):syserr;
  extern proc qbuffer_copyout(buf:qbuffer_ptr_t, start:qbuffer_iter_t, end:qbuffer_iter_t, ref x, size):syserr;
  extern proc qbuffer_copyin(buf:qbuffer_ptr_t, start:qbuffer_iter_t, end:qbuffer_iter_t, ref x, size):syserr;

  extern proc qbuffer_begin(buf:qbuffer_ptr_t):qbuffer_iter_t;
  extern proc qbuffer_end(buf:qbuffer_ptr_t):qbuffer_iter_t;
  extern proc qbuffer_iter_next_part(buf:qbuffer_ptr_t, ref it:qbuffer_iter_t);
  extern proc qbuffer_iter_prev_part(buf:qbuffer_ptr_t, ref it:qbuffer_iter_t);
  extern proc qbuffer_iter_advance(buf:qbuffer_ptr_t, ref it:qbuffer_iter_t, amt:int(64));

  extern proc qbuffer_iter_get(it: qbuffer_iter_t, end:qbuffer_iter_t, 
                                out bytes_out:qbytes_ptr_t,
                                out skip_out:int(64),
                                out len_out:int(64));
  extern proc qbuffer_iter_num_bytes(start:qbuffer_iter_t, end:qbuffer_iter_t):int(64);

  extern proc qbuffer_len(buf:qbuffer_ptr_t):int(64);

  extern proc debug_print_qbuffer_iter(/*const*/ ref it:qbuffer_iter_t);


  extern proc qbuffer_start_offset(buf:qbuffer_ptr_t):int(64);
  extern proc qbuffer_end_offset(buf:qbuffer_ptr_t):int(64);
  extern proc qbuffer_reposition(buf:qbuffer_ptr_t, new_offset_start:int(64));

  extern proc bulk_get_bytes(src_locale:int, src_addr:qbytes_ptr_t):qbytes_ptr_t;

  extern proc bulk_put_buffer(dst_locale:int, dst_addr:c_void_ptr, dst_len:int(64), buf:qbuffer_ptr_t, start:qbuffer_iter_t, end:qbuffer_iter_t):syserr;

}

module Buffers {
  use BufferInternals; // TODO -- non-exporting use

  // Now define the Chapel types using the originals..
  record bytes {
    var home: locale;
    var _bytes_internal:qbytes_ptr_t;
  }

  record buffer_iterator {
    var home: locale;
    var _bufit_internal:qbuffer_iter_t;
  }
  record buffer_range {
    var start:buffer_iterator;
    var end:buffer_iterator;
  }

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
  record buffer {
    var home:locale;
    var _buf_internal:qbuffer_ptr_t;
  }

  // bytes methods.
  proc bytes.bytes() {
    this._bytes_internal = QBYTES_PTR_NULL;
    this.home = here;
  }
  proc bytes.bytes(len:int(64), out error:syserr) {
    error = qbytes_create_calloc(this._bytes_internal, len);
    // The buffer is "retained" internally on creation, but only on success.
    this.home = here;
  }
  proc bytes.bytes(len:int(64)) {
    var error:syserr = ENOERR;
    error = qbytes_create_calloc(this._bytes_internal, len);
    if error then ioerror(error, "in bytes constructor");
    // The buffer is retained internally on construction, but only on success.
    this.home = here;
  }


  proc create_iobuf(out error:syserr):bytes {
    var ret:bytes;
    error = qbytes_create_iobuf(ret._bytes_internal);
    // The buffer is "retained" internally on creation, but only on success.
    ret.home = here;
    return ret;
  }
  proc create_iobuf():bytes {
    var err:syserr = ENOERR; 
    var ret = create_iobuf(err);
    if err then ioerror(err, "in create_iobuf");
    return ret;
  }


  // TODO -- shouldn't have to write this this way!
  pragma "init copy fn"
  proc chpl__initCopy(x: bytes) {
    if x.home == here {
      qbytes_retain(x._bytes_internal);
      return x;
    } else {
      var ret:bytes;
      ret.home = here;
      writeln("Bulk moving bytes");
      // The initial ref count is 1, so no need to call qbytes_retain here.
      ret._bytes_internal = bulk_get_bytes(x.home.id, x._bytes_internal); 
      return ret;
    }
  }

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
      writeln("Bulk moving bytes");
      ret._bytes_internal = bulk_get_bytes(x.home.id, x._bytes_internal); 
      // On return from bulk_get_bytes, the ref count in ret._bytes_internal 
      // should be 1.
      // Note that the error case is not handled (bulk_get_bytes must succeed).
    }
  }

  proc bytes.~bytes() {
    on this.home {
      qbytes_release(this._bytes_internal);
      this._bytes_internal = QBYTES_PTR_NULL;
    }
  }
  proc bytes.len:int(64) {
    var ret:int(64);
    on home {
     ret = qbytes_len(this._bytes_internal);
    }
    return ret;
  }

  // buffer iterator
  proc buffer_iterator.buffer_iterator() {
    this.home = here;
    this._bufit_internal = qbuffer_iter_null();
  }
  /* don't believe this is necessary...
  pragma "init copy fn"
  proc chpl__initCopy(x: buffer_iterator) {
    return x;
  }
  */

  // buffer methods.
  proc buffer.buffer(out error:syserr) {
    this.home = here;
    error = qbuffer_create(this._buf_internal);
  }
  proc buffer.buffer() {
    var error:syserr = ENOERR;
    this.home = here;
    error = qbuffer_create(this._buf_internal);
    if error then ioerror(error, "in buffer constructor");
  }


  // buffer.flatten is special in that it creates a 
  // bytes object wherever it is called (and communicates it 
  // intelligently). It is use in buffer's initCopy.
  proc buffer.flatten(range:buffer_range, out error:syserr):bytes {
    var ret:bytes;
    if this.home == here {
      error = qbuffer_flatten(this._buf_internal, range.start._bufit_internal, range.end._bufit_internal, ret);
    } else {
      var dst_locale = here;
      var dst_len:int(64) = range.len;
      ret = new bytes(dst_len);
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

  // TODO -- shouldn't have to write this this way!
  pragma "init copy fn"
  proc chpl__initCopy(x: buffer) {
    if x.home == here {
      qbuffer_retain(x._buf_internal);
      return x;
    } else {
      var ret:buffer;
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
        if err then ioerror(err, "in buffer init copy");
      }

      ret.append(b);
      return ret;
    }
  }

  proc =(ref ret:buffer, x:buffer) {
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
        if err then ioerror(err, "in buffer assignment");
      }

      ret.append(b);
    }
  }


  proc buffer.~buffer() {
    on this.home {
      qbuffer_release(this._buf_internal);
      this._buf_internal = QBUFFER_PTR_NULL;
    }
  }

  proc buffer.len {
    var ret:int(64);
    on this.home {
      ret = qbuffer_len(this._buf_internal);
    }
    return ret;
  }

  proc buffer.append(b:bytes, skip_bytes:int(64) = 0, len_bytes:int(64) = b.len, out error:syserr) {
    on this.home {
      error = qbuffer_append(this._buf_internal, b._bytes_internal, skip_bytes, len_bytes);
    }
  }
  proc buffer.append(b:bytes, skip_bytes:int(64) = 0, len_bytes:int(64) = b.len) {
    var err:syserr = ENOERR;
    this.append(b, skip_bytes, len_bytes, err);
    if err then ioerror(err, "in buffer.append");
  }


  proc buffer.append(buf:buffer, part:buffer_range = buf.all(), out error:syserr) {
    on this.home {
      error = qbuffer_append_buffer(this._buf_internal, buf._buf_internal, part.start._bufit_internal, part.end._bufit_internal);
    }
  }
  proc buffer.append(buf:buffer, part:buffer_range = buf.all()) {
    var err:syserr = ENOERR;
    this.append(buf, part, err);
    if err then ioerror(err, "in buffer.append");
  }

  proc buffer.prepend(b:bytes, skip_bytes:int(64) = 0, len_bytes:int(64) = b.len, out error:syserr) {
    on this.home {
      qbuffer_prepend(this._buf_internal, b._bytes_internal, skip_bytes, len_bytes);
    }
  }
  proc buffer.prepend(b:bytes, skip_bytes:int(64) = 0, len_bytes:int(64) = b.len) {
    var err:syserr = ENOERR;
    this.prepend(b, skip_bytes, len_bytes, err);
    if err then ioerror(err, "in buffer.prepend");
  }

  proc buffer.start():buffer_iterator {
    var ret:buffer_iterator;
    ret.home = this.home;
    on this.home {
      ret._bufit_internal = qbuffer_begin(this._buf_internal);
    }
    return ret;
  }
  proc buffer.end():buffer_iterator {
    var ret:buffer_iterator;
    ret.home = this.home;
    on this.home {
      ret._bufit_internal = qbuffer_end(this._buf_internal);
    }
    return ret;
  }
  proc buffer.all():buffer_range {
    return new buffer_range(this.start(), this.end());
  }

  proc buffer.next_part(inout it:buffer_iterator) {
    on this.home {
      var x = it;
      qbuffer_iter_next_part(this._buf_internal, x._bufit_internal);
      it = x;
    }
  }
  proc buffer.prev_part(inout it:buffer_iterator) {
    on this.home {
      var x = it;
      qbuffer_iter_prev_part(this._buf_internal, x._bufit_internal);
      it = x;
    }
  }
  proc buffer.advance(inout it:buffer_iterator, amount:int(64)) {
    on this.home {
      var x = it;
      qbuffer_iter_advance(this._buf_internal, x._bufit_internal, amount);
      it = x;
    }
  }

  proc buffer_iterator.debug_print()
  {
    on this.home {
      debug_print_qbuffer_iter(this._bufit_internal);
    }
  }

  /*
  proc buffer.clone(range:buffer_range, err:ErrorHandler = nil):buffer {
    on this.home {
      var ret:buffer;
      seterr(qbuffer_clone(this._buf_internal, range.start, range.end, ret._buf_internal));
      return ret;
    }
  }*/

  // methods to read/write basic types.
  proc buffer.copyout(it:buffer_iterator, out value, out error:syserr):buffer_iterator {
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
  proc buffer.copyout(it:buffer_iterator, out value):buffer_iterator {
    var err:syserr = ENOERR;
    var ret = this.copyout(it, value, err);
    if err then ioerror(err, "in buffer.copyout");
    return ret;
  }

  proc buffer.copyin( it:buffer_iterator, value, out error:syserr):buffer_iterator {
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
  proc buffer.copyin( it:buffer_iterator, value):buffer_iterator {
    var err:syserr = ENOERR;
    var ret = this.copyin(it, value, err);
    if err then ioerror(err, "in buffer.copyin");
    return ret;
  }
}

