
module IO {
  use Sys;
  use Buffers;
  use Error;
 
  enum iokind {
    /* don't change these without updating qio_style.h QIO_NATIVE, etc
       a default of 0 is always reasonable, but you can avoid some
       branches to get faster I/O by setting this to native, big, or little.
       In that case, the style is only consulted for text or string I/O.
       */
    dynamic = 0, // look in iostyle
    native = 1,
    big = 2, /* aka "network" */
    little = 3
  }
  param dynamic = iokind.dynamic;
  param native = iokind.native;
  param big = iokind.big;
  param little = iokind.little;

  enum style {
    string1bLen = -1,
    string2bLen = -2,
    string4bLen = -4,
    string8bLen = -8,
    stringVarbLen = -10,
    stringNullTerm = -0x0100
  }
  proc stringStyleTerminated(terminator:uint(8)) {
    return -(0x0100:int(64) + terminator);
  }
  proc stringStyleNullTerminated() {
    return stringStyleTerminated(0);
  }
  proc stringStyleExactLen(len:int(64)) {
    return len;
  }
  proc stringStyleWithVariableLength() {
    return -10;
  }
  proc stringStyleWithLength(lengthBytes:int) {
    var x = style.stringVarbLen;
    select lengthBytes {
      when 1 do x = -1;
      when 2 do x = -2;
      when 4 do x = -4;
      when 8 do x = -8;
      otherwise halt("Unhandled string length prefix size");
    }
    return x;
  }

  _extern const QIO_FDFLAG_UNK:c_int;
  _extern const QIO_FDFLAG_READABLE:c_int;
  _extern const QIO_FDFLAG_WRITEABLE:c_int;
  _extern const QIO_FDFLAG_SEEKABLE:c_int;

  _extern const QIO_CH_UNBUFFERED:c_int;
  _extern const QIO_CH_BUFFERED:c_int;

  _extern const QIO_METHOD_DEFAULT:c_int;
  _extern const QIO_METHOD_READWRITE:c_int;
  _extern const QIO_METHOD_PREADPWRITE:c_int;
  _extern const QIO_METHOD_FREADFWRITE:c_int;
  _extern const QIO_METHOD_MMAP:c_int;
  _extern const QIO_METHODNUM:c_int;
  _extern const QIO_METHODMASK:c_int;
  _extern const QIO_HINT_RANDOM:c_int;
  _extern const QIO_HINT_SEQUENTIAL:c_int;
  _extern const QIO_HINT_LATENCY:c_int;
  _extern const QIO_HINT_BANDWIDTH:c_int;
  _extern const QIO_HINT_CACHED:c_int;
  _extern const QIO_HINT_DIRECT:c_int;
  _extern const QIO_HINT_NOREUSE:c_int;

  _extern type qio_file_ptr_t;
  _extern const QIO_FILE_PTR_NULL:qio_file_ptr_t;

  _extern type qio_channel_ptr_t;
  _extern const QIO_CHANNEL_PTR_NULL:qio_channel_ptr_t;

  _extern type style_char_t = uint(8);

  //_extern record qio_style_t {
  _extern record iostyle {
    var binary:uint(8);
    // binary style choices
    var byteorder:iokind;
    // string binary style:
    // -1 -- 1 byte of length before
    // -2 -- 2 bytes of length before
    // -4 -- 4 bytes of length before
    // -8 -- 8 bytes of length before
    // -10 -- variable byte length before (hi-bit 1 means more, little endian)
    // -0x01XX -- read until terminator XX is read
    //  + -- nonzero positive -- read exactly this length.
    var str_style:int(64);
    // text style choices
    var min_width:uint(32);
    var max_width:uint(32);
    var string_start:style_char_t;
    var string_end:style_char_t;

    /* if 0: (none) string is as-is
       if 1: (basic) only escape string_end and \ with \
       if 2: (Chapel) escape string_end \ ' " \n with \
             and nonprinting characters c = 0xXY with \xXY
       if 3: (JSON) escape string_end " and \ with \,
             and nonprinting characters c = \uABCD
     */
    var string_format:uint(8);
    // numeric scanning/printing choices
    var base:uint(8);
    var point_char:style_char_t;
    var exponent_char:style_char_t;
    var other_exponent_char:style_char_t;
    var positive_char:style_char_t;
    var negative_char:style_char_t;
    var prefix_base:uint(8);
    // numeric printing choices
    var pad_char:style_char_t;
    var showplus:uint(8);
    var uppercase:uint(8);
    var leftjustify:uint(8);
    var showpoint:uint(8);
    var precision:uint(32);
    var significant_digits:uint(32);
    var realtype:uint(8);

    var imag_style:uint(8);
    var spaces_after_sep:uint(8);
    var array_start_char:style_char_t;
    var array_end_char:style_char_t;
    var array_delim_char1:style_char_t;
    var array_delim_char2:style_char_t;
    var array_delim_char3:style_char_t;

    var array_include_index:uint(8);
    var array_include_domain:uint(8);

    var tuple_start_char:style_char_t;
    var tuple_end_char:style_char_t;
    var tuple_delim_char:style_char_t;

    var record_start_char:style_char_t;
    var record_end_char:style_char_t;
    var record_delim_char:style_char_t;
    var record_after_field_name_char:style_char_t;
    var record_print_field_names:uint(8);
    var record_print_name:uint(8);

    var class_start_char:style_char_t;
    var class_end_char:style_char_t;
    var class_delim_char:style_char_t;
    var class_after_field_name_char:style_char_t;
    var class_print_field_names:uint(8);
    var class_print_name:uint(8);
  }

  _extern const QIO_STYLE_SIZE:size_t;

  _extern proc qio_style_init_default(inout s: iostyle);


  /*
  _extern proc qio_style_copy(dst:qio_style_ptr_t, src:qio_style_ptr_t);
  _extern proc qio_style_dup(src:qio_style_ptr_t):qio_style_ptr_t;
  _extern proc qio_style_free(style:qio_style_ptr_t);
  _extern proc bulk_get_style(src_locale:int, dst_addr:qio_style_ptr_t, src_addr:qio_style_ptr_t);
  */

  /* Extern functions */
  _extern proc qio_file_retain(f:qio_file_ptr_t);
  _extern proc qio_file_release(f:qio_file_ptr_t);

  _extern proc qio_file_init(inout file_out:qio_file_ptr_t, fp:_file, fd:fd_t, iohints:c_int, inout style:iostyle):err_t;
  _extern proc qio_file_open_access(inout file_out:qio_file_ptr_t, path:string, access:string, iohints:c_int, inout style:iostyle):err_t;
  _extern proc qio_file_open_tmp(inout file_out:qio_file_ptr_t, iohints:c_int, inout style:iostyle):err_t;

  /* Close a file (asserts ref count==1)
     This is not usually necessary to call, but a program will
     halt if it isn't called.
   */
  _extern proc qio_file_close(f:qio_file_ptr_t):err_t;

  _extern proc qio_file_lock(f:qio_file_ptr_t):err_t;
  _extern proc qio_file_unlock(f:qio_file_ptr_t);

  /* The general way to make sure data is written without error */
  _extern proc qio_file_sync(f:qio_file_ptr_t):err_t;

  //_extern proc qio_file_style_ptr(f:qio_file_ptr_t):qio_style_ptr_t;
  _extern proc qio_file_get_style(f:qio_file_ptr_t, inout style:iostyle);
  _extern proc qio_file_set_style(f:qio_file_ptr_t, inout style:iostyle);

  /* our wrapper for the read function;
     calls readv to read into our buffer.
     Will read data into buf (overwriting,
      not allocating) and advance the file descriptor's offset.
   */
  //_extern proc qio_readv(fd:fd_t, inout buf:qbuffer_t, start:qbuffer_iter_t, end:qbuffer_iter_t, inout num_read:int(64)):err_t;
  /* our wrapper for the write function;
     calls writev to write our buffer.
     Will write data from buf and not change it.
     Will advance the file descriptor's offset.
   */
  //_extern proc qio_writev(fd:fd_t, inout buf:qbuffer_t, start:qbuffer_iter_t, end:qbuffer_iter_t, inout num_read:int(64)):err_t;

  /* calls preadv where it exists;
     otherwise, will do a series of pread calls
     Will read data into buf (overwriting, not allocating).
     Will not advance the file descriptor's position.
   */
  //_extern proc qio_preadv(fd:fd_t, inout buf:qbuffer_t, start:qbuffer_iter_t, end:qbuffer_iter_t, offset:int(64), inout num_read:int(64)):err_t;
  /* calls pwritev where it exists;
     otherwise, will do a series of pwrite calls
     Will write data from buf and not change it.
     Will not advance the file descriptor's position.
   */
  //_extern proc qio_pwritev(fd:fd_t, inout buf:qbuffer_t, start:qbuffer_iter_t, end:qbuffer_iter_t, offset:int(64), inout num_read:int(64)):err_t;

  _extern proc qio_channel_create(inout ch:qio_channel_ptr_t, file:qio_file_ptr_t, hints:c_int, readable:c_int, writeable:c_int, start:int(64), end:int(64), inout style:iostyle):err_t;

  _extern proc qio_channel_retain(ch:qio_channel_ptr_t);
  _extern proc qio_channel_release(ch:qio_channel_ptr_t);

  _extern proc qio_channel_lock(ch:qio_channel_ptr_t):err_t;
  _extern proc qio_channel_unlock(ch:qio_channel_ptr_t);

  //_extern proc qio_channel_style_ptr(ch:qio_channel_ptr_t):qio_style_ptr_t;
  _extern proc qio_channel_get_style(ch:qio_channel_ptr_t, inout style:iostyle);
  _extern proc qio_channel_set_style(ch:qio_channel_ptr_t, inout style:iostyle);

  _extern proc qio_channel_binary(ch:qio_channel_ptr_t):uint(8);
  _extern proc qio_channel_byteorder(ch:qio_channel_ptr_t):uint(8);
  _extern proc qio_channel_str_style(ch:qio_channel_ptr_t):int(64);

  _extern proc qio_channel_flush(threadsafe:c_int, ch:qio_channel_ptr_t):err_t;

  _extern proc qio_channel_read(threadsafe:c_int, ch:qio_channel_ptr_t, inout ptr, len:ssize_t, inout amt_read:ssize_t):err_t;
  _extern proc qio_channel_read_amt(threadsafe:c_int, ch:qio_channel_ptr_t, inout ptr, len:ssize_t):err_t;

  _extern proc qio_channel_write(threadsafe:c_int, ch:qio_channel_ptr_t, inout ptr, len:ssize_t, inout amt_written:ssize_t):err_t;
  _extern proc qio_channel_write_amt(threadsafe:c_int, ch:qio_channel_ptr_t, inout ptr, len:ssize_t):err_t;

  /*
  _extern proc qio_scanf(threadsafe:c_int, ch:qio_channel_ptr_t, inout nmatched:c_int, fmt:string, inout args...?numargs):err_t;
  _extern proc qio_scanf1(threadsafe:c_int, ch:qio_channel_ptr_t, fmt:string, inout arg):err_t;
  _extern proc qio_printf(threadsafe:c_int, qio_channel_ptr_t, fmt:string, args...?numargs):err_t;
  _extern proc qio_choose_io_type(hints:c_int):c_int;
  */

  _extern proc qio_file_path_for_fd(fd:fd_t, inout path:string):err_t;
  _extern proc qio_file_path_for_fp(fp:_file, inout path:string):err_t;
  _extern proc qio_file_path(f:qio_file_ptr_t, inout path:string):err_t;

  _extern proc qio_channel_read_int(threadsafe:c_int, byteorder:c_int, ch:qio_channel_ptr_t, inout ptr, len:size_t, issigned:c_int):err_t;
  _extern proc qio_channel_write_int(threadsafe:c_int, byteorder:c_int, ch:qio_channel_ptr_t, inout ptr, len:size_t, issigned:c_int):err_t;

  _extern proc qio_channel_read_float(threadsafe:c_int, byteorder:c_int, ch:qio_channel_ptr_t, inout ptr, len:size_t):err_t;
  _extern proc qio_channel_write_float(threadsafe:c_int, byteorder:c_int, ch:qio_channel_ptr_t, inout ptr, len:size_t):err_t;

  _extern proc qio_channel_read_complex(threadsafe:c_int, byteorder:c_int, ch:qio_channel_ptr_t, inout re_ptr, inout im_ptr, len:size_t):err_t;
  _extern proc qio_channel_write_complex(threadsafe:c_int, byteorder:c_int, ch:qio_channel_ptr_t, inout re_ptr, inout im_ptr, len:size_t):err_t;

  _extern proc qio_channel_read_string(threadsafe:c_int, byteorder:c_int, str_style:int(64), ch:qio_channel_ptr_t, inout s:string, inout len:ssize_t):err_t;
  _extern proc qio_channel_write_string(threadsafe:c_int, byteorder:c_int, str_style:int(64), ch:qio_channel_ptr_t, s:string, len:ssize_t):err_t;

  _extern proc qio_channel_scan_int(threadsafe:c_int, ch:qio_channel_ptr_t, inout ptr, len:size_t, issigned:c_int):err_t;
  _extern proc qio_channel_print_int(threadsafe:c_int, ch:qio_channel_ptr_t, inout ptr, len:size_t, issigned:c_int):err_t;

  _extern proc qio_channel_scan_float(threadsafe:c_int, ch:qio_channel_ptr_t, inout ptr, len:size_t):err_t;
  _extern proc qio_channel_print_float(threadsafe:c_int, ch:qio_channel_ptr_t, inout ptr, len:size_t):err_t;

  _extern proc qio_channel_scan_complex(threadsafe:c_int, ch:qio_channel_ptr_t, inout re_ptr, inout im_ptr, len:size_t):err_t;
  _extern proc qio_channel_print_complex(threadsafe:c_int, ch:qio_channel_ptr_t, inout re_ptr, inout im_ptr, len:size_t):err_t;


  _extern proc qio_channel_read_char(threadsafe:c_int, ch:qio_channel_ptr_t, inout char:c_wchar_t):err_t;
  _extern proc qio_channel_write_char(threadsafe:c_int, ch:qio_channel_ptr_t, char:c_wchar_t):err_t;
  _extern proc qio_channel_skip_past_newline(threadsafe:c_int, ch:qio_channel_ptr_t):err_t;
  _extern proc qio_channel_write_newline(threadsafe:c_int, ch:qio_channel_ptr_t):err_t;

  _extern proc qio_channel_scan_string(threadsafe:c_int, ch:qio_channel_ptr_t, inout ptr:string, inout len:ssize_t):err_t;
  _extern proc qio_channel_print_string(threadsafe:c_int, ch:qio_channel_ptr_t, ptr:string, len:ssize_t):err_t;
 
  //type iostyle = qio_style_t;

  proc defaultStyle():iostyle {
    var ret:iostyle;
    qio_style_init_default(ret);
    return ret;
  }

  proc iostyle.iostyle() {
    qio_style_init_default(this);
  }
  proc iostyle.native(str_style:int(64)=stringStyleWithVariableLength()):iostyle {
    var ret = this;
    ret.binary = 1;
    ret.byteorder = iokind.native;
    ret.str_style = str_style;
    return ret;
  }
  proc iostyle.big(str_style:int(64)=stringStyleWithVariableLength()):iostyle {
    var ret = this;
    ret.binary = 1;
    ret.byteorder = iokind.big;
    ret.str_style = str_style;
    return ret;
  }
  proc iostyle.little(str_style:int(64)=stringStyleWithVariableLength()):iostyle  {
    var ret = this;
    ret.binary = 1;
    ret.byteorder = iokind.little;
    ret.str_style = str_style;
    return ret;
  }
  proc iostyle.text(/* args coming later */):iostyle  {
    var ret = this;
    ret.binary = 0;
    return ret;
  }



  /*
    QIO_FDFLAG_UNK,
    QIO_FDFLAG_READABLE,
    QIO_FDFLAG_WRITEABLE,
    QIO_FDFLAG_SEEKABLE
  */

  _extern type qio_fdflag_t = c_int;
  type fdflag_t = qio_fdflag_t;

  /* Access hints describe how a file will be used.
     These can help optimize.
   */
  /*
    QIO_METHOD_DEFAULT,
    QIO_METHOD_READWRITE,
    QIO_METHOD_P_READWRITE,
    QIO_METHOD_MMAP,
    QIO_HINT_RANDOM,
    QIO_HINT_SEQUENTIAL,
    QIO_HINT_LATENCY,
    QIO_HINT_BANDWIDTH,
    QIO_HINT_CACHED,
    QIO_HINT_NOREUSE
  }
  */

  _extern type qio_hint_t = c_int;
  type iohint_t = qio_hint_t;

  record file {
    var home:locale;
    var _file_internal:qio_file_ptr_t;
    var onErr:ErrorHandler;
  }



  // used for giving old warnings anyways...
  enum FileAccessMode { read, write };

  param _oldioerr="This program is using old-style I/O which is no longer supported.\n" +
                  "See doc/README.io.\n" +
                  "You'll probably want something like:\n" +
                  "var f = open(filename).writing()\n" + 
                  "or\n" + 
                  "var f = open(filename).reading()\n";
 
  // This file constructor exists to throw an error for old I/O code.
  proc file.file(filename:string="",
                 mode:FileAccessMode=FileAccessMode.read,
                 path:string=".") {
    compilerError(_oldioerr);
  }
  proc file.open() {
    compilerError(_oldioerr);
  }
  proc file.filename var : string {
    compilerError(_oldioerr + "file.filename is no longer supported");
  }
  proc file.mode var {
    compilerError(_oldioerr + "file.mode is no longer supported");
  }
  proc file.isOpen: bool {
    compilerError(_oldioerr + "file.isOpen is no longer supported");
  }

  // The Writer class exists to throw an error for old I/O code.
  class Writer {
    proc writeIt(s:string) {
      compilerError(_oldioerr + "Writer no longer supported");
    }
    proc lockWrite() {
      compilerError(_oldioerr + "Writer no longer supported");
    }
    proc unlockWrite() {
      compilerError(_oldioerr + "Writer no longer supported");
    }
    proc write(args ...?n) {
      compilerError(_oldioerr + "Writer no longer supported");
    }
    proc writeln(args ...?n) {
      compilerError(_oldioerr + "Writer no longer supported");
    }
    proc writeln() {
      compilerError(_oldioerr + "Writer no longer supported");
    }
  }


  // TODO -- shouldn't have to write this this way!
  proc chpl__initCopy(x: file) {
    on x.home {
      qio_file_retain(x._file_internal);
    }
    return x;
  }

  proc =(ret:file, x:file) {
    // retain -- release
    on x.home {
      qio_file_retain(x._file_internal);
    }

    on ret.home {
      qio_file_release(ret._file_internal);
    }

    // compiler will do this copy.
    ret.home = x.home;
    ret.onErr = x.onErr;
    ret._file_internal = x._file_internal;
    return ret;
  }

  // Open a file from a system file descriptor
  /*proc file.file(fd: fd_t, hints:iohint_t=0, err:ErrorHandler=nil) {
    seterr(err, qio_file_init(_file_internal, fd, hints));
  }
  proc file.file(path:string, access:string, hints:iohint_t=0, err:ErrorHandler=nil) {
    seterr(err, qio_file_open_access(_file_internal, path, access, hints), path);
  }*/

  proc file.file(onErr:ErrorHandler=nil) {
    this.home = here;
    this._file_internal = QIO_FILE_PTR_NULL;
    this.onErr = onErr;
  }

  proc file.~file() {
    on this.home {
      qio_file_release(_file_internal);
      this._file_internal = QIO_FILE_PTR_NULL;
    }
  }

  /*
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

  // File style cannot be modified after the file is created;
  // this prevents race conditions;
  // channel style is protected by channel lock, can be modified.
  proc file._style:iostyle {
    var ret:iostyle;
    on this.home {
      var local_style:iostyle;
      qio_file_get_style(_file_internal, local_style);
      ret = local_style;
    }
    return ret;
  }
  /*
  proc file._set_style(style:iostyle) {
    on this.home {
      var local_style:iostyle = style;
      qio_file_set_style(_file_internal, local_style);
    }
  }*/



  /* Close a file.
     Alternately, file will be closed when it is no longer referred to */
  proc file.close(onErr:ErrorHandler=nil) {
    on this.home {
      seterr(onErr, this.onErr, qio_file_close(_file_internal));
    }
  }

  /* Sync a file to disk. */
  proc file.fsync(onErr:ErrorHandler=nil) {
    on this.home {
      seterr(onErr, this.onErr, qio_file_sync(_file_internal));
    }
  }

  /* Get the path to a file. */
  proc file.getPath(onErr:ErrorHandler=nil) : string {
    var ret:string;
    on this.home {
      var tmp:string;
      seterr(onErr, this.onErr, qio_file_path(_file_internal, tmp));
      ret = tmp;
    }
   return ret; 
  }
  proc file.path : string {
    return this.getPath(nil);
  }


  proc open(path:string, access:string, hints:iohint_t=0, style:iostyle = defaultStyle(), onErr:ErrorHandler=nil) {
    var local_style = style;
    var ret = new file(onErr);
    seterr(onErr, qio_file_open_access(ret._file_internal, path, access, hints, local_style), path);
    return ret;
  }
  proc openfd(fd: fd_t, hints:iohint_t=0, style:iostyle = defaultStyle(), onErr:ErrorHandler=nil) {
    var local_style = style;
    var ret = new file(onErr);
    var e = qio_file_init(ret._file_internal, chpl_cnullfile(), fd, hints, local_style);
    if e != 0 {
      var path:string;
      qio_file_path_for_fd(fd, path);
      seterr(onErr, e, path);
    }
    return ret;
  }
  proc openfp(fp: _file, hints:iohint_t=0, style:iostyle = defaultStyle(), onErr:ErrorHandler=nil):file {
    var local_style = style;
    var ret = new file(onErr);
    var e = qio_file_init(ret._file_internal, fp, -1, hints, local_style);
    if e != 0 {
      var path:string;
      qio_file_path_for_fp(fp, path);
      seterr(onErr, e, path);
    }
    return ret;
  }

  proc opentmp(hints:iohint_t=0, style:iostyle = defaultStyle(), onErr:ErrorHandler=nil) {
    var local_style = style;
    var ret = new file(onErr);
    seterr(onErr, qio_file_open_tmp(ret._file_internal, hints, local_style));
    return ret;
  }


  /* in the future, this will be an interface.
     */
  record channel {
    param writing:bool;
    param kind:iokind;
    var home:locale;
    var _channel_internal:qio_channel_ptr_t = QIO_CHANNEL_PTR_NULL;
    var onErr:ErrorHandler; /* starts out as file.onErr, but can be set to something else */
  }

  // TODO -- shouldn't have to write this this way!
  proc chpl__initCopy(x: channel) {
    on x.home {
      qio_channel_retain(x._channel_internal);
    }
    return x;
  }

  proc =(ret:channel, x:channel) {
    // retain -- release
    on x.home {
      qio_channel_retain(x._channel_internal);
    }

    on ret.home {
      qio_channel_release(ret._channel_internal);
    }

    ret.home = x.home;
    ret._channel_internal = x._channel_internal;
    ret.onErr = x.onErr;
    return ret;
  }

  proc channel.channel(param writing:bool, param kind:iokind, f:file, hints:c_int, start:int(64), end:int(64), style:iostyle, onErr:ErrorHandler) {
    on f.home {
      this.home = here;
      this.onErr = onErr;
      if( this.onErr == nil ) {
        this.onErr = f.onErr;
      }
      var local_style = style;
      seterr(onErr, this.onErr, qio_channel_create(this._channel_internal, f._file_internal, hints, !writing, writing, start, end, local_style));
    }
  }

  proc channel.~channel() {
    on this.home {
      qio_channel_release(_channel_internal);
      this._channel_internal = QIO_CHANNEL_PTR_NULL;
    }
  }

  // Used to represent "\n", but never escaped...
  record ioNewline {
    proc writeThis(inout ch: channel):err_t {
      return qio_channel_write_newline(false, ch._channel_internal);
    }
    proc readThis(inout ch: channel):err_t {
      return qio_channel_skip_past_newline(false, ch._channel_internal);
    }
  }


  proc channel.lock() {
    on this.home {
      seterr(this.onErr, qio_channel_lock(_channel_internal));
    }
  }
  proc channel.unlock() {
    on this.home {
      qio_channel_unlock(_channel_internal);
    }
  }

  // you should have a lock before you use these...
  proc channel._style():iostyle {
    var ret:iostyle;
    on this.home {
      var local_style:iostyle;
      qio_channel_get_style(_channel_internal, local_style);
      ret = local_style;
    }
    return ret;
  }
  proc channel._set_style(style:iostyle) {
    on this.home {
      var local_style:iostyle = style;
      qio_channel_set_style(_channel_internal, local_style);
    }
  }

  /*
  class read_channel : channel {
  }
  class write_channel : channel {
  }
  */

  proc file.reader(param kind=iokind.dynamic, start:int(64) = 0, end:int(64) = max(int(64)), hints:c_int = 0, style:iostyle = this._style, onErr:ErrorHandler = nil): channel(false, kind) {
    var ret:channel(false, kind);
    on this.home {
      ret = new channel(false, kind, this, hints, start, end, style, onErr);
    }
    return ret;
  }
  // for convenience..
  proc file.lines(start:int(64) = 0, end:int(64) = max(int(64)), hints:c_int = 0, style:iostyle = this._style, onErr:ErrorHandler = nil) {
    style.string_format = 0;
    style.string_end = 0x0a; // '\n'

    param kind = iokind.dynamic;
    var ret:ItemReader(string, kind);
    on this.home {
      ret = new ItemReader(string, kind, new channel(false, kind, this, hints, start, end, style, onErr));
    }
    return ret;
  }


  proc file.writer(param kind=iokind.dynamic, start:int(64) = 0, end:int(64) = max(int(64)), hints:c_int = 0, style:iostyle = this._style, onErr:ErrorHandler = nil): channel(true,kind) {
    var ret:channel(true, kind);
    on this.home {
      ret = new channel(true, kind, this, hints, start, end, style, onErr);
    }
    return ret;
  }

  proc _isIoPrimitiveType(type t) param return
    _isPrimitiveType(t) || _isComplexType(t) ||
    _isImagType(t) || _isEnumeratedType(t);
 
  // Read routines for all primitive types.
  proc channel._read_text_internal(out x:?t):err_t where _isIoPrimitiveType(t) {
    if writing then compilerError("read on write-only channel");

    if _isIntegralType(t) || _isBooleanType(t) {
      // handles bool (as unsigned), int types
      return qio_channel_scan_int(false, _channel_internal, x, numBytes(t), _isSignedType(t));
    } else if _isFloatType(t) {
      // handles real, imag
      return qio_channel_scan_float(false, _channel_internal, x, numBytes(t));
    } else if _isComplexType(t)  {
      // handle complex types
      var re:x.re.type;
      var im:x.im.type;
      var err:err_t;
      err = qio_channel_scan_complex(false, _channel_internal, re, im, numBytes(x.re.type));
      x = (re, im):t; // cast tuple to complex to get complex num.
      return err;
    } else if t == string {
      // handle string
      var len:ssize_t;
      return qio_channel_scan_string(false, _channel_internal, x, len);
    } else {
      compilerError("Unknown primitive type in _read_text_internal", typeToString(t));
      halt("Unknown primitive type in _read_text_internal", typeToString(t));
    }
    return EINVAL;
  }

  proc channel._write_text_internal(x:?t):err_t where _isIoPrimitiveType(t) {
    if !writing then compilerError("write on read-only channel");

    if _isIntegralType(t) || _isBooleanType(t) {
      // handles bool (as unsigned), int types
      return qio_channel_print_int(false, _channel_internal, x, numBytes(t), _isSignedType(t));
    } else if _isFloatType(t) {
      // handles real, imag
      return qio_channel_print_float(false, _channel_internal, x, numBytes(t));
    } else if _isComplexType(t)  {
      // handle complex types
      var re = x.re;
      var im = x.im;
      return qio_channel_print_complex(false, _channel_internal, re, im, numBytes(x.re.type));
    } else if t == string {
      // handle string
      return qio_channel_print_string(false, _channel_internal, x, x.length);
    } else {
      compilerError("Unknown primitive type in _read_text_internal", typeToString(t));
    }
    return EINVAL;
  }

  proc channel._read_binary_internal(param byteorder:iokind, out x:?t):err_t where _isIoPrimitiveType(t) {
    if writing then compilerError("read on write-only channel");

    if _isIntegralType(t) || _isBooleanType(t) {
      // handles bool (as unsigned), int types
      return qio_channel_read_int(false, byteorder, _channel_internal, x, numBytes(t), _isSignedType(t));
    } else if _isFloatType(t) {
      // handles real, imag
      return qio_channel_read_float(false, byteorder, _channel_internal, x, numBytes(t));
    } else if _isComplexType(t)  {
      // handle complex types
      var re:x.re.type;
      var im:x.im.type;
      var err:err_t;
      err = qio_channel_read_complex(false, byteorder, _channel_internal, re, im, numBytes(x.re.type));
      x = (re, im):t; // cast tuple to complex to get complex num.
      return err;
    } else if t == string {
      // handle string
      var len:ssize_t;
      return qio_channel_read_string(false, byteorder, qio_channel_str_style(_channel_internal), _channel_internal, x, len);
    } else {
      compilerError("Unknown primitive type in _read_text_internal", typeToString(t));
    }
  }

  proc channel._write_binary_internal(param byteorder:iokind, x:?t):err_t where _isIoPrimitiveType(t) {
    if !writing then compilerError("write on read-only channel");

    if _isIntegralType(t) || _isBooleanType(t) {
      // handles bool (as unsigned), int types
      return qio_channel_write_int(false, byteorder, _channel_internal, x, numBytes(t), _isSignedType(t));
    } else if _isFloatType(t) {
      // handles real, imag
      return qio_channel_write_float(false, byteorder, _channel_internal, x, numBytes(t));
    } else if _isComplexType(t)  {
      // handle complex types
      var re = x.re;
      var im = x.im;
      return qio_channel_write_complex(false, byteorder, _channel_internal, re, im, numBytes(x.re.type));
    } else if t == string {
      return qio_channel_write_string(false, byteorder, qio_channel_str_style(_channel_internal), _channel_internal, x, x.length);
    } else {
      halt("Unknown primitive type");
    }
  }

  // Channel must be locked, must be running on this.home
  proc channel._read_one_internal(param kind:iokind, out x:?t):err_t where _isIoPrimitiveType(t) {
    var e:err_t = EINVAL;
    if kind == iokind.dynamic {
      var binary:uint(8) = qio_channel_binary(_channel_internal);
      var byteorder:uint(8) = qio_channel_byteorder(_channel_internal);
      if binary {
        select byteorder {
          when big    do e = _read_binary_internal(big, x);
          when little do e = _read_binary_internal(little, x);
          otherwise      e = _read_binary_internal(native, x);
        }
      } else {
        e = _read_text_internal(x);
      }
    } else {
      e = _read_binary_internal(kind, x);
    }
    return e;
  }
  proc channel._read_one_internal(param kind:iokind, out x:?t):err_t {
    return x.readThis(this);
  }

  /*
  proc channel.read_one(param kind:iokind, err:ErrorHandler, out x:?t) where _isPrimitiveType(t) {
    var e:err_t;
    on this.home {
      this.lock();
      e = this._read_one_internal(kind, x);
      this.unlock();
    }
    seterr(err, e);
  }*/

  // Channel must be locked, must be running on this.home
  proc channel._write_one_internal(param kind:iokind, x:?t):err_t where _isIoPrimitiveType(t) {
    var e:err_t = EINVAL;
    if kind == iokind.dynamic {
      var binary:uint(8) = qio_channel_binary(_channel_internal);
      var byteorder:uint(8) = qio_channel_byteorder(_channel_internal);
      if binary {
        select byteorder {
          when big    do e = _write_binary_internal(big, x);
          when little do e = _write_binary_internal(little, x);
          otherwise      e = _write_binary_internal(native, x);
        }
      } else {
        e = _write_text_internal(x);
      }
    } else {
      e = _write_binary_internal(kind, x);
    }
    return e;
  }
  proc channel._write_one_internal(param kind:iokind, x:?t):err_t {
    return x.writeThis(this);
  }

  /*
  proc channel.write_one(param kind:iokind, err:error, x:?t) where _isPrimitiveType(t) {
    var e:err_t;
    on this.home {
      this.lock();
      e = this._write_one_internal(kind, x);
      this.unlock();
    }
    seterr(err, e);
  }*/


  /* Returns true if we read all the args,
     false if we encountered EOF (or possibly another error and didn't halt)*/
  proc channel.read(inout args ...?k):bool {
    var e:err_t = 0;
    on this.home {
      this.lock();
      for param i in 1..k {
        if e == 0 {
          e = _read_one_internal(kind, args[i]);
        }
      }
      this.unlock();
    }
    if e == 0 then return true;
    else if e == EEOF then return false;
    else {
      seterr(nil, this.onErr, e); // TODO -- include path and offset
      return false;
    }
  }
  proc channel.read(inout args ...?k,
                    onErr:ErrorHandler):bool {
    var e:err_t = 0;
    on this.home {
      this.lock();
      for param i in 1..k {
        if e == 0 {
          e = _read_one_internal(kind, args[i]);
        }
      }
      this.unlock();
    }
    if e == 0 then return true;
    else if e == EEOF then return false;
    else {
      seterr(onErr, this.onErr, e); // TODO -- include path and offset
      return false;
    }
  }
  proc channel.read(inout args ...?k,
                    style:iostyle,
                    onErr:ErrorHandler=nil):bool {
    var e:err_t = 0;
    on this.home {
      this.lock();
      var save_style = this._style();
      this._set_style(style);
      for param i in 1..k {
        if e == 0 {
          e = _read_one_internal(kind, args[i]);
        }
      }
      this._set_style(save_style);
      this.unlock();
    }
    if e == 0 then return true;
    else if e == EEOF then return false;
    else {
      seterr(onErr, this.onErr, e); // TODO -- include path and offset
      return false;
    }
  }

  proc channel.readline(inout arg:string, onErr:ErrorHandler=nil):bool {
    var e:err_t = 0;
    on this.home {
      this.lock();
      var save_style = this._style();
      var mystyle = defaultStyle().text();
      mystyle.string_format = 0;
      mystyle.string_end = 0x0a; // ascii newline.
      this._set_style(mystyle);
      e = _read_one_internal(kind, arg);
      this._set_style(save_style);
      this.unlock();
    }
    if e == 0 then return true;
    else if e == EEOF then return false;
    else {
      seterr(onErr, this.onErr, e); // TODO -- include path and offset
      return false;
    }
  }

  proc channel.readln(inout args ...?k):bool {
    var nl = new ioNewline();
    return this.read((...args), nl);
  }
  proc channel.readln(inout args ...?k,
                      onErr:ErrorHandler):bool {
    var nl = new ioNewline();
    return this.read((...args), nl, onErr=onErr);
  }
  proc channel.readln(inout args ...?k,
                    style:iostyle,
                    onErr:ErrorHandler=nil):bool {
    var nl = new ioNewline();
    return this.read((...args), nl, style=style, onErr=onErr);
  }
  proc channel.read(type t) {
    var tmp:t;
    var didread = this.read(tmp);
    if !didread then seterr(nil, this.onErr, EEOF); // TODO -- include path and offset
    return tmp;
  }
  proc channel.read(type t, onErr:ErrorHandler) {
    var tmp:t;
    var didread = this.read(tmp);
    if !didread then seterr(onErr, this.onErr, EEOF); // TODO -- include path and offset
    return tmp;
  }
  proc channel.read(type t, style:iostyle, onErr:ErrorHandler=nil) {
    var tmp:t;
    var didread = this.read(tmp);
    if !didread then seterr(onErr, this.onErr, EEOF); // TODO -- include path and offset
    return tmp;
  }


  proc channel.readln(type t) {
    var tmp:t;
    this.readln(tmp);
    return tmp;
  }


  proc channel.write(args ...?k):bool {
    var e:err_t = 0;
    on this.home {
      this.lock();
      for param i in 1..k {
        if e == 0 {
          e = _write_one_internal(kind, args(i));
        }
      }
      this.unlock();
    }
    if e == 0 then return true;
    else {
      seterr(nil, this.onErr, e); // TODO -- include path and offset
      return false;
    }
  }
  proc channel.write(args ...?k,
                     onErr:ErrorHandler):bool {
    var e:err_t = 0;
    on this.home {
      this.lock();
      for param i in 1..k {
        if e == 0 {
          e = _write_one_internal(kind, args(i));
        }
      }
      this.unlock();
    }
    if e == 0 then return true;
    else {
      seterr(onErr, this.onErr, e); // TODO -- include path and offset
      return false;
    }
  }

  proc channel.write(args ...?k,
                     style:iostyle,
                     onErr:ErrorHandler = nil):bool {
    var e:err_t = 0;
    on this.home {
      this.lock();
      var save_style = this._style();
      this._set_style(style);
      for param i in 1..k {
        if e == 0 {
          e = _write_one_internal(dynamic, args(i));
        }
      }
      this._set_style(save_style);
      this.unlock();
    }
    if e == 0 then return true;
    else {
      seterr(onErr, this.onErr, e); // TODO -- include path and offset
      return false;
    }
  }

  proc channel.writeln(args ...?k):bool {
    return this.write((...args), new ioNewline());
  }
  proc channel.writeln(args ...?k,
                       onErr:ErrorHandler):bool {
    return this.write((...args), new ioNewline(), onErr=onErr);
  }
  proc channel.writeln(args ...?k,
                       style:iostyle,
                       onErr:ErrorHandler = nil):bool {
    return this.write((...args), new ioNewline(), style=style, onErr=onErr);
  }

  proc channel.flush(onErr:ErrorHandler = nil) {
    var e:err_t;
    on this.home {
      e = qio_channel_flush(true, _channel_internal);
    }
    seterr(onErr, this.onErr, e); // TODO -- include path and offset
  }

  proc channel.modifyStyle(f:func(iostyle, void))
  {
    on this.home {
      this.lock();
      var style = this._style();
      f(style);
      this._set_style(style);
      this.unlock();
    }
  }


  // TODO: more convenient methods for modifying the style


  /* Move between min_len and max_len bytes
     of data from a read channel to a write channel.
     Blocks on reading up to min_len; blocks on writing
     to the write channel. Returns the number of bytes
     moved and/or an error number. */ 
  /*proc splice(input:read_channel, output:write_channel,
             min_length:int(64), max_length:int(64), onErr:ErrorHandler = nil) {
    halt("unimplemented");
  }*/

  /* Move between min_len and max_len bytes
     of data from a read channel to several write channels;
     the data is written to all of the write channels.
     Blocks on reading up to min_len; blocks on writing to
     all the write channels. Returns the number of bytes
     moved and/or an error number. */
  /*proc tee(input:read_channel, output:[] write_channel, onErr:ErrorHandler = nil) {
    halt("unimplemented");
  }*/

  record ItemReader {
    type ItemType;
    param kind:iokind;
    var ch:channel(false,kind);
    proc read(out arg:ItemType, onErr:ErrorHandler=nil):bool {
      return ch.read( arg, onErr=onErr );
    }
    iter these() {
      while true {
        var x:ItemType;
        var gotany = read(x);
        if ! gotany then break;
        yield x;
      }
    }
  }

  proc channel.itemReader(type ItemType, param kind:iokind=iokind.dynamic) {
    if writing then compilerError(".itemReader on write-only channel");
    return new ItemReader(ItemType, kind, this);
  }

  record ItemWriter {
    type ItemType;
    param kind:iokind;
    var ch:channel(false,kind);
    proc write(arg:ItemType, onErr:ErrorHandler = nil) {
      return ch.write( arg, onErr=onErr );
    }
  }

  proc channel.itemWriter(type ItemType, param kind:iokind=iokind.dynamic) {
    if !writing then compilerError(".itemWriter on read-only channel");
    return new ItemWriter(ItemType, kind, this);
  }

  /* As before, a user may overide the read/write
     methods for a particular type.

     __format__( chan:?t ) t is a read_channel or write_channel
       -- looks like Boost serialization library; ie
       for i in 1..10 {
         chan & value[i];
       }

     */

  // And now, the toplevel items.
  const stdin:channel(false, iokind.dynamic) = openfp(chpl_cstdin()).reader(); 
  const stdout:channel(true, iokind.dynamic) = openfp(chpl_cstdout()).writer(); 
  const stderr:channel(true, iokind.dynamic) = openfp(chpl_cstderr()).writer(); 

  proc write(args ...?n) {
    stdout.write((...args));
    stdout.flush();
  }
  proc writeln(args ...?n) {
    stdout.writeln((...args));
    stdout.flush();
  }
  proc writeln() {
    stdout.writeln();
    stdout.flush();
  }

  proc read(inout args ...?n) {
    stdin.read((...args));
  }
  proc readln(inout args ...?n) {
    stdin.readln((...args));
  }
  proc readln() {
    stdin.readln();
  }
  proc readln(type t) {
    return stdin.readln(t);
  }

  // Read/write tuples of types.
  proc file.readln(type t ...?numTypes) where numTypes > 1 {
    var tupleVal: t;
    for param i in 1..numTypes-1 do
      tupleVal(i) = this.read(t(i));
    tupleVal(numTypes) = this.readln(t(numTypes));
    return tupleVal;
  }

  proc file.read(type t ...?numTypes) where numTypes > 1 {
    var tupleVal: t;
    for param i in 1..numTypes do
      tupleVal(i) = this.read(t(i));
    return tupleVal;
  }

  proc readln(type t ...?numTypes) where numTypes > 1 {
    return stdin.readln((...t));
  }

  proc read(type t ...?numTypes) where numTypes > 1 {
    return stdin.read((...t));
  }
}

