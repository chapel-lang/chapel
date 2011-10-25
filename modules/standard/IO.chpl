use SysBasic;
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

  extern const QIO_FDFLAG_UNK:c_int;
  extern const QIO_FDFLAG_READABLE:c_int;
  extern const QIO_FDFLAG_WRITEABLE:c_int;
  extern const QIO_FDFLAG_SEEKABLE:c_int;

  extern const QIO_CH_ALWAYS_UNBUFFERED:c_int;
  extern const QIO_CH_ALWAYS_BUFFERED:c_int;
  extern const QIO_CH_BUFFERED:c_int;

  extern const QIO_METHOD_DEFAULT:c_int;
  extern const QIO_METHOD_READWRITE:c_int;
  extern const QIO_METHOD_PREADPWRITE:c_int;
  extern const QIO_METHOD_FREADFWRITE:c_int;
  extern const QIO_METHOD_MMAP:c_int;
  extern const QIO_METHODNUM:c_int;
  extern const QIO_METHODMASK:c_int;
  extern const QIO_HINT_RANDOM:c_int;
  extern const QIO_HINT_SEQUENTIAL:c_int;
  extern const QIO_HINT_LATENCY:c_int;
  extern const QIO_HINT_BANDWIDTH:c_int;
  extern const QIO_HINT_CACHED:c_int;
  extern const QIO_HINT_DIRECT:c_int;
  extern const QIO_HINT_NOREUSE:c_int;

  extern type qio_file_ptr_t;
  extern const QIO_FILE_PTR_NULL:qio_file_ptr_t;

  extern type qio_channel_ptr_t;
  extern const QIO_CHANNEL_PTR_NULL:qio_channel_ptr_t;

  // also the type for a buffer for qio_file_open_mem.
  extern type qbuffer_ptr_t;
  extern const QBUFFER_PTR_NULL:qbuffer_ptr_t;

  extern type style_char_t = c_char_t;

  extern const QIO_STRING_FORMAT_WORD:uint(8);
  extern const QIO_STRING_FORMAT_BASIC:uint(8);
  extern const QIO_STRING_FORMAT_CHPL:uint(8);
  extern const QIO_STRING_FORMAT_JSON:uint(8);
  extern const QIO_STRING_FORMAT_TOEND:uint(8);

  //extern record qio_style_t {
  extern record iostyle {
    var binary:uint(8);
    // binary style choices
    var byteorder:uint(8);
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

    /* QIO_STRING_FORMAT_WORD  string is as-is; reading reads until whitespace.
       QIO_STRING_FORMAT_BASIC only escape string_end and \ with \
       QIO_STRING_FORMAT_CHPL  escape string_end \ ' " \n with \
                               and nonprinting characters c = 0xXY with \xXY
       QIO_STRING_FORMAT_JSON  escape string_end " and \ with \,
                               and nonprinting characters c = \uABCD
       QIO_STRING_FORMAT_TOEND string is as-is; reading reads until string_end
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
    var precision:int(32);
    var significant_digits:int(32);
    var realtype:uint(8);

    var complex_style:uint(8);

    /*
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
    */
  }

  extern const QIO_STYLE_SIZE:size_t;

  extern proc qio_style_init_default(inout s: iostyle);


  /*
  extern proc qio_style_copy(dst:qio_style_ptr_t, src:qio_style_ptr_t);
  extern proc qio_style_dup(src:qio_style_ptr_t):qio_style_ptr_t;
  extern proc qio_style_free(style:qio_style_ptr_t);
  extern proc bulk_get_style(src_locale:int, dst_addr:qio_style_ptr_t, src_addr:qio_style_ptr_t);
  */

  /* Extern functions */
  extern proc qio_file_retain(f:qio_file_ptr_t);
  extern proc qio_file_release(f:qio_file_ptr_t);

  extern proc qio_file_init(inout file_out:qio_file_ptr_t, fp:_file, fd:fd_t, iohints:c_int, inout style:iostyle, usefilestar:c_int):err_t;
  extern proc qio_file_open_access(inout file_out:qio_file_ptr_t, path:string, access:string, iohints:c_int, inout style:iostyle):err_t;
  extern proc qio_file_open_tmp(inout file_out:qio_file_ptr_t, iohints:c_int, inout style:iostyle):err_t;
  extern proc qio_file_open_mem(inout file_out:qio_file_ptr_t, buf:qbuffer_ptr_t, inout style:iostyle):err_t;

  /* Close a file (asserts ref count==1)
     This is not usually necessary to call, but a program will
     halt if it isn't called.
   */
  extern proc qio_file_close(f:qio_file_ptr_t):err_t;

  extern proc qio_file_lock(f:qio_file_ptr_t):err_t;
  extern proc qio_file_unlock(f:qio_file_ptr_t);

  /* The general way to make sure data is written without error */
  extern proc qio_file_sync(f:qio_file_ptr_t):err_t;

  //extern proc qio_file_style_ptr(f:qio_file_ptr_t):qio_style_ptr_t;
  extern proc qio_file_get_style(f:qio_file_ptr_t, inout style:iostyle);
  extern proc qio_file_set_style(f:qio_file_ptr_t, inout style:iostyle);

  /* our wrapper for the read function;
     calls readv to read into our buffer.
     Will read data into buf (overwriting,
      not allocating) and advance the file descriptor's offset.
   */
  //extern proc qio_readv(fd:fd_t, inout buf:qbuffer_t, start:qbuffer_iter_t, end:qbuffer_iter_t, inout num_read:int(64)):err_t;
  /* our wrapper for the write function;
     calls writev to write our buffer.
     Will write data from buf and not change it.
     Will advance the file descriptor's offset.
   */
  //extern proc qio_writev(fd:fd_t, inout buf:qbuffer_t, start:qbuffer_iter_t, end:qbuffer_iter_t, inout num_read:int(64)):err_t;

  /* calls preadv where it exists;
     otherwise, will do a series of pread calls
     Will read data into buf (overwriting, not allocating).
     Will not advance the file descriptor's position.
   */
  //extern proc qio_preadv(fd:fd_t, inout buf:qbuffer_t, start:qbuffer_iter_t, end:qbuffer_iter_t, offset:int(64), inout num_read:int(64)):err_t;
  /* calls pwritev where it exists;
     otherwise, will do a series of pwrite calls
     Will write data from buf and not change it.
     Will not advance the file descriptor's position.
   */
  //extern proc qio_pwritev(fd:fd_t, inout buf:qbuffer_t, start:qbuffer_iter_t, end:qbuffer_iter_t, offset:int(64), inout num_read:int(64)):err_t;

  extern proc qio_channel_create(inout ch:qio_channel_ptr_t, file:qio_file_ptr_t, hints:c_int, readable:c_int, writeable:c_int, start:int(64), end:int(64), inout style:iostyle):err_t;

  extern proc qio_channel_path_offset(threadsafe:c_int, ch:qio_channel_ptr_t, inout path:string, inout offset:int(64)):err_t;

  extern proc qio_channel_retain(ch:qio_channel_ptr_t);
  extern proc qio_channel_release(ch:qio_channel_ptr_t);

  extern proc qio_channel_lock(ch:qio_channel_ptr_t):err_t;
  extern proc qio_channel_unlock(ch:qio_channel_ptr_t);

  //extern proc qio_channel_style_ptr(ch:qio_channel_ptr_t):qio_style_ptr_t;
  extern proc qio_channel_get_style(ch:qio_channel_ptr_t, inout style:iostyle);
  extern proc qio_channel_set_style(ch:qio_channel_ptr_t, inout style:iostyle);

  extern proc qio_channel_binary(ch:qio_channel_ptr_t):uint(8);
  extern proc qio_channel_byteorder(ch:qio_channel_ptr_t):uint(8);
  extern proc qio_channel_str_style(ch:qio_channel_ptr_t):int(64);

  extern proc qio_channel_flush(threadsafe:c_int, ch:qio_channel_ptr_t):err_t;
  extern proc qio_channel_close(threadsafe:c_int, ch:qio_channel_ptr_t):err_t;

  extern proc qio_channel_read(threadsafe:c_int, ch:qio_channel_ptr_t, inout ptr, len:ssize_t, inout amt_read:ssize_t):err_t;
  extern proc qio_channel_read_amt(threadsafe:c_int, ch:qio_channel_ptr_t, inout ptr, len:ssize_t):err_t;
  extern proc qio_channel_read_byte(threadsafe:c_int, ch:qio_channel_ptr_t):int(32);

  extern proc qio_channel_write(threadsafe:c_int, ch:qio_channel_ptr_t, inout ptr, len:ssize_t, inout amt_written:ssize_t):err_t;
  extern proc qio_channel_write_amt(threadsafe:c_int, ch:qio_channel_ptr_t, inout ptr, len:ssize_t):err_t;
  extern proc qio_channel_write_byte(threadsafe:c_int, ch:qio_channel_ptr_t, byte:uint(8)):err_t;

  extern proc qio_channel_mark(threadsafe:c_int, ch:qio_channel_ptr_t):err_t;
  extern proc qio_channel_revert_unlocked(ch:qio_channel_ptr_t);
  extern proc qio_channel_commit_unlocked(ch:qio_channel_ptr_t);

  /*
  extern proc qio_scanf(threadsafe:c_int, ch:qio_channel_ptr_t, inout nmatched:c_int, fmt:string, inout args...?numargs):err_t;
  extern proc qio_scanf1(threadsafe:c_int, ch:qio_channel_ptr_t, fmt:string, inout arg):err_t;
  extern proc qio_printf(threadsafe:c_int, qio_channel_ptr_t, fmt:string, args...?numargs):err_t;
  extern proc qio_choose_io_type(hints:c_int):c_int;
  */

  extern proc qio_file_path_for_fd(fd:fd_t, inout path:string):err_t;
  extern proc qio_file_path_for_fp(fp:_file, inout path:string):err_t;
  extern proc qio_file_path(f:qio_file_ptr_t, inout path:string):err_t;

  extern proc qio_channel_read_int(threadsafe:c_int, byteorder:c_int, ch:qio_channel_ptr_t, inout ptr, len:size_t, issigned:c_int):err_t;
  extern proc qio_channel_write_int(threadsafe:c_int, byteorder:c_int, ch:qio_channel_ptr_t, inout ptr, len:size_t, issigned:c_int):err_t;

  extern proc qio_channel_read_float(threadsafe:c_int, byteorder:c_int, ch:qio_channel_ptr_t, inout ptr, len:size_t):err_t;
  extern proc qio_channel_write_float(threadsafe:c_int, byteorder:c_int, ch:qio_channel_ptr_t, inout ptr, len:size_t):err_t;

  extern proc qio_channel_read_complex(threadsafe:c_int, byteorder:c_int, ch:qio_channel_ptr_t, inout re_ptr, inout im_ptr, len:size_t):err_t;
  extern proc qio_channel_write_complex(threadsafe:c_int, byteorder:c_int, ch:qio_channel_ptr_t, inout re_ptr, inout im_ptr, len:size_t):err_t;

  extern proc qio_channel_read_string(threadsafe:c_int, byteorder:c_int, str_style:int(64), ch:qio_channel_ptr_t, inout s:string, inout len:ssize_t, maxlen:ssize_t):err_t;
  extern proc qio_channel_write_string(threadsafe:c_int, byteorder:c_int, str_style:int(64), ch:qio_channel_ptr_t, s:string, len:ssize_t):err_t;

  extern proc qio_channel_scan_int(threadsafe:c_int, ch:qio_channel_ptr_t, inout ptr, len:size_t, issigned:c_int):err_t;
  extern proc qio_channel_print_int(threadsafe:c_int, ch:qio_channel_ptr_t, inout ptr, len:size_t, issigned:c_int):err_t;

  extern proc qio_channel_scan_float(threadsafe:c_int, ch:qio_channel_ptr_t, inout ptr, len:size_t):err_t;
  extern proc qio_channel_print_float(threadsafe:c_int, ch:qio_channel_ptr_t, inout ptr, len:size_t):err_t;

  extern proc qio_channel_scan_complex(threadsafe:c_int, ch:qio_channel_ptr_t, inout re_ptr, inout im_ptr, len:size_t):err_t;
  extern proc qio_channel_print_complex(threadsafe:c_int, ch:qio_channel_ptr_t, inout re_ptr, inout im_ptr, len:size_t):err_t;


  extern proc qio_channel_read_char(threadsafe:c_int, ch:qio_channel_ptr_t, inout char:int(32)):err_t;
  extern proc qio_channel_write_char(threadsafe:c_int, ch:qio_channel_ptr_t, char:int(32)):err_t;
  extern proc qio_channel_skip_past_newline(threadsafe:c_int, ch:qio_channel_ptr_t):err_t;
  extern proc qio_channel_write_newline(threadsafe:c_int, ch:qio_channel_ptr_t):err_t;

  extern proc qio_channel_scan_string(threadsafe:c_int, ch:qio_channel_ptr_t, inout ptr:string, inout len:ssize_t, maxlen:ssize_t):err_t;
  extern proc qio_channel_print_string(threadsafe:c_int, ch:qio_channel_ptr_t, ptr:string, len:ssize_t):err_t;
 
  extern proc qio_channel_scan_literal(threadsafe:c_int, ch:qio_channel_ptr_t, match:string, len:ssize_t, skipws:c_int):err_t;
  extern proc qio_channel_print_literal(threadsafe:c_int, ch:qio_channel_ptr_t, match:string, len:ssize_t):err_t;
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
    ret.byteorder = iokind.native:uint(8);
    ret.str_style = str_style;
    return ret;
  }
  proc iostyle.big(str_style:int(64)=stringStyleWithVariableLength()):iostyle {
    var ret = this;
    ret.binary = 1;
    ret.byteorder = iokind.big:uint(8);
    ret.str_style = str_style;
    return ret;
  }
  proc iostyle.little(str_style:int(64)=stringStyleWithVariableLength()):iostyle  {
    var ret = this;
    ret.binary = 1;
    ret.byteorder = iokind.little:uint(8);
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

  extern type fdflag_t = c_int;
//  type fdflag_t = qio_fdflag_t; commented out because of too many statics

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

  extern type iohint_t = c_int;
  //type iohint_t = qio_hint_t;

  inline
  proc _current_locale():int {
    var tmp:int;
    return __primitive("_get_locale", tmp);
  }

  record file {
    var home_uid:int = _current_locale();
    var _file_internal:qio_file_ptr_t = QIO_FILE_PTR_NULL;
  }



  // used for giving old warnings anyways...
  enum FileAccessMode { read, write };

  param _oldioerr="This program is using old-style I/O which is no longer supported.\n" +
                  "See doc/README.io.\n" +
                  "You'll probably want something like:\n" +
                  "var f = open(filename, \"w\").writer()\n" + 
                  "or\n" + 
                  "var f = open(filename, \"r\").reader()\n";
 
  // This file constructor exists to throw an error for old I/O code.
  proc file.file(filename:string="",
                 mode:FileAccessMode=FileAccessMode.read,
                 path:string=".") {
    compilerError(_oldioerr);
  }
  proc file.open() {
    compilerError(_oldioerr);
  }
  proc file.filename : string {
    compilerError(_oldioerr + "file.filename is no longer supported");
  }
  proc file.mode {
    compilerError(_oldioerr + "file.mode is no longer supported");
  }
  proc file.isOpen: bool {
    compilerError(_oldioerr + "file.isOpen is no longer supported");
  }

  // TODO -- shouldn't have to write this this way!
  proc chpl__initCopy(x: file) {
    on __primitive("chpl_on_locale_num", x.home_uid) {
      qio_file_retain(x._file_internal);
    }
    return x;
  }

  proc =(ret:file, x:file) {
    // retain -- release
    on __primitive("chpl_on_locale_num", x.home_uid) {
      qio_file_retain(x._file_internal);
    }

    on __primitive("chpl_on_locale_num", ret.home_uid) {
      qio_file_release(ret._file_internal);
    }

    // compiler will do this copy.
    ret.home_uid = x.home_uid;
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
  proc file.check() {
    if(1 == is_c_nil(_file_internal)) {
      halt("Operation attempted on an invalid file");
    }
  }

  /*
  proc file.file() {
    var tmp:int;
    this.home_uid = _current_locale();
    this._file_internal = QIO_FILE_PTR_NULL;
  }
  */

  proc file.~file() {
    on __primitive("chpl_on_locale_num", this.home_uid) {
      qio_file_release(_file_internal);
      this._file_internal = QIO_FILE_PTR_NULL;
    }
  }

  /*
  proc file.lock() {
    on __primitive("chpl_on_locale_num", this.home_uid) {
      seterr(nil, qio_file_lock(_file_internal));
    }
  }
  proc file.unlock() {
    on __primitive("chpl_on_locale_num", this.home_uid) {
      qio_file_unlock(_file_internal);
    }
  }
  */

  // File style cannot be modified after the file is created;
  // this prevents race conditions;
  // channel style is protected by channel lock, can be modified.
  proc file._style:iostyle {
    check();

    var ret:iostyle;
    on __primitive("chpl_on_locale_num", this.home_uid) {
      var local_style:iostyle;
      qio_file_get_style(_file_internal, local_style);
      ret = local_style;
    }
    return ret;
  }
  /*
  proc file._set_style(style:iostyle) {
    on __primitive("chpl_on_locale_num", this.home_uid) {
      var local_style:iostyle = style;
      qio_file_set_style(_file_internal, local_style);
    }
  }*/

  /* Close a file.
     Alternately, file will be closed when it is no longer referred to */
  proc file.close(out error:err_t) {
    check();
    on __primitive("chpl_on_locale_num", this.home_uid) {
      error = qio_file_close(_file_internal);
    }
  }

  proc file.close() {
    var err:err_t = ENOERR;
    this.close(err);
    if err then _ioerror(err, "in file.close", this.tryGetPath());
  }

  /* Sync a file to disk. */
  proc file.fsync(out error:err_t) {
    check();
    on __primitive("chpl_on_locale_num", this.home_uid) {
      error = qio_file_sync(_file_internal);
    }
  }
  proc file.fsync() {
    var err:err_t = ENOERR;
    this.fsync();
    if err then _ioerror(err, "in file.fsync", this.tryGetPath());
  }


  /* Get the path to a file. */
  proc file.getPath(out error:err_t) : string {
    check();
    var ret:string;
    on __primitive("chpl_on_locale_num", this.home_uid) {
      var tmp:string;
      error = qio_file_path(_file_internal, tmp);
      ret = tmp;
    }
   return ret; 
  }

  proc file.tryGetPath() : string {
    var err:err_t = ENOERR;
    var ret:string;
    ret = this.getPath(err);
    if err then return "unknown";
    else return ret;
  }

  proc file.path : string {
    var err:err_t = ENOERR;
    var ret:string;
    ret = this.getPath(err);
    if err then _ioerror(err, "in file.path");
  }

  proc open(path:string, access:string, out error:err_t, hints:iohint_t=0, style:iostyle = defaultStyle()):file {
    var local_style = style;
    var ret:file;
    ret.home_uid = _current_locale();
    error = qio_file_open_access(ret._file_internal, path, access, hints, local_style);
    return ret;
  }
  proc open(path:string, access:string, hints:iohint_t=0, style:iostyle = defaultStyle()):file {
    var err:err_t = ENOERR;
    var ret = open(path, access, err, hints, style);
    if err then _ioerror(err, "in open", path);
    return ret;
  }
  proc openfd(fd: fd_t, out error:err_t, hints:iohint_t=0, style:iostyle = defaultStyle()):file {
    var local_style = style;
    var ret:file;
    ret.home_uid = _current_locale();
    error = qio_file_init(ret._file_internal, chpl_cnullfile(), fd, hints, local_style, 0);
    return ret;
  }
  proc openfd(fd: fd_t, hints:iohint_t=0, style:iostyle = defaultStyle()):file {
    var err:err_t = ENOERR;
    var ret = openfd(fd, err, hints, style);
    if err {
      var path:string;
      var e2:err_t = ENOERR;
      e2 = qio_file_path_for_fd(fd, path);
      if e2 then path = "unknown";
      _ioerror(err, "in openfd", path);
    }
    return ret;
  }
  proc openfp(fp: _file, out error:err_t, hints:iohint_t=0, style:iostyle = defaultStyle()):file {
    var local_style = style;
    var ret:file;
    ret.home_uid = _current_locale();
    error = qio_file_init(ret._file_internal, fp, -1, hints, local_style, 1);
    return ret;
  }
  proc openfp(fp: _file, hints:iohint_t=0, style:iostyle = defaultStyle()):file {
    var err:err_t = ENOERR;
    var ret = openfp(fp, err, hints, style);
    if err {
      var path:string;
      var e2:err_t = ENOERR;
      e2 = qio_file_path_for_fp(fp, path);
      if e2 then path = "unknown";
      _ioerror(err, "in openfp", path);
    }
    return ret;
  }

  proc opentmp(out error:err_t, hints:iohint_t=0, style:iostyle = defaultStyle()):file {
    var local_style = style;
    var ret:file;
    ret.home_uid = _current_locale();
    error = qio_file_open_tmp(ret._file_internal, hints, local_style);
    return ret;
  }
  proc opentmp(hints:iohint_t=0, style:iostyle = defaultStyle()):file {
    var err:err_t = ENOERR;
    var ret = opentmp(err, hints, style);
    if err then _ioerror(err, "in opentmp");
    return ret;
  }

  proc openmem(out error:err_t, style:iostyle = defaultStyle()) {
    var local_style = style;
    var ret:file;
    ret.home_uid = _current_locale();
    error = qio_file_open_mem(ret._file_internal, QBUFFER_PTR_NULL, local_style);
    return ret;
  }
  proc openmem(hints:iohint_t=0, style:iostyle = defaultStyle()):file {
    var err:err_t = ENOERR;
    var ret = openmem(err, hints, style);
    if err then _ioerror(err, "in openmem");
    return ret;
  }



  /* in the future, this will be an interface.
     */
  record channel {
    param writing:bool;
    param kind:iokind;
    param locking:bool;
    var home_uid:int;
    var _channel_internal:qio_channel_ptr_t = QIO_CHANNEL_PTR_NULL;
  }

  // TODO -- shouldn't have to write this this way!
  proc chpl__initCopy(x: channel) {
    on __primitive("chpl_on_locale_num", x.home_uid) {
      qio_channel_retain(x._channel_internal);
    }
    return x;
  }

  proc =(ret:channel, x:channel) {
    // retain -- release
    on __primitive("chpl_on_locale_num", x.home_uid) {
      qio_channel_retain(x._channel_internal);
    }

    on __primitive("chpl_on_locale_num", ret.home_uid) {
      qio_channel_release(ret._channel_internal);
    }

    ret.home_uid = x.home_uid;
    ret._channel_internal = x._channel_internal;
    return ret;
  }

  proc channel.channel(param writing:bool, param kind:iokind, param locking:bool, f:file, out error:err_t, hints:c_int, start:int(64), end:int(64), style:iostyle) {
    on __primitive("chpl_on_locale_num", f.home_uid) {
      this.home_uid = f.home_uid;
      var local_style = style;
      error = qio_channel_create(this._channel_internal, f._file_internal, hints, !writing, writing, start, end, local_style);
    }
  }

  proc channel.~channel() {
    on __primitive("chpl_on_locale_num", this.home_uid) {
      qio_channel_release(_channel_internal);
      this._channel_internal = QIO_CHANNEL_PTR_NULL;
    }
  }

  // Used to represent a Unicode character
  record ioChar {
    var ch:int(32);
    proc writeThis(f: Writer) {
      halt("ioChar.writeThis must be written in Writer subclasses");
    } 
  }
  proc Reader.readType(inout x:ioChar):bool {
    halt("ioChar.readType must be implemented in Reader subclasses.");
    return false;
  }

  // Used to represent "\n", but never escaped...
  record ioNewline {
    proc writeThis(f: Writer) {
      // Normally this is handled explicitly in read/write.
      f.write("\n");
    }
  }
  proc Reader.readType(inout x:ioNewline):bool {
    halt("ioNewline.readType must be implemented in Reader subclasses.");
    return false;
  }
  pragma "inline" proc _cast(type t, x: ioNewline) where t == string {
    return "\n";
  }

  // Used to represent a constant string we want to read or write...
  record ioLiteral {
    var val: string;
    var ignoreWhiteSpace: bool = true;
    proc writeThis(f: Writer) {
      // Normally this is handled explicitly in read/write.
      f.write(val);
    }
  }
  proc Reader.readType(inout x:ioLiteral):bool {
    halt("readType(ioLiteral) must be implemented in Reader subclasses.");
    return false;
  }

  pragma "inline" proc _cast(type t, x: ioLiteral) where t == string {
    return x.val;
  }

  proc channel._ch_ioerror(syserr:err_t, msg:string) {
    var path:string = "unknown";
    var offset:int(64) = -1;
    on __primitive("chpl_on_locale_num", this.home_uid) {
      var tmp_path:string;
      var tmp_offset:int(64);
      var err:err_t = ENOERR;
      err = qio_channel_path_offset(locking, _channel_internal, tmp_path, tmp_offset);
      if !err {
        path = tmp_path;
        offset = tmp_offset;
      }
    }
    _ioerror(syserr, msg, path, offset);
  }

  inline
  proc channel.lock(out error:err_t) {
    error = ENOERR;
    if locking {
      on __primitive("chpl_on_locale_num", this.home_uid) {
        error = qio_channel_lock(_channel_internal);
      }
    }
  }
  inline
  proc channel.lock() {
    var err:err_t = ENOERR;
    this.lock(err);
    if err then this._ch_ioerror(err, "in lock");
  }

  inline
  proc channel.unlock() {
    if locking {
      on __primitive("chpl_on_locale_num", this.home_uid) {
        qio_channel_unlock(_channel_internal);
      }
    }
  }

  // you should have a lock before you use these...
  inline
  proc channel._mark():err_t {
    return qio_channel_mark(false, _channel_internal);
  }
  inline
  proc channel._revert() {
    qio_channel_revert(false, _channel_internal);
  }
  inline
  proc channel._commit() {
    qio_channel_commit(false, _channel_internal);
  }
  proc channel._style():iostyle {
    var ret:iostyle;
    on __primitive("chpl_on_locale_num", this.home_uid) {
      var local_style:iostyle;
      qio_channel_get_style(_channel_internal, local_style);
      ret = local_style;
    }
    return ret;
  }
  proc channel._set_style(style:iostyle) {
    on __primitive("chpl_on_locale_num", this.home_uid) {
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

  proc file.reader(out error:err_t, param kind=iokind.dynamic, param locking=true, start:int(64) = 0, end:int(64) = max(int(64)), hints:c_int = 0, style:iostyle = this._style): channel(false, kind, locking) {
    check();

    var ret:channel(false, kind, locking);
    on __primitive("chpl_on_locale_num", this.home_uid) {
      ret = new channel(false, kind, locking, this, error, hints, start, end, style);
    }
    return ret;
  }
  proc file.reader(param kind=iokind.dynamic, param locking=true, start:int(64) = 0, end:int(64) = max(int(64)), hints:c_int = 0, style:iostyle = this._style): channel(false, kind, locking) {
    var err:err_t = ENOERR;
    var ret = this.reader(err, kind, locking, start, end, hints, style);
    if err then _ioerror(err, "in file.reader", this.tryGetPath());
    return ret;
  }
  // for convenience..
  proc file.lines(out error:err_t, param locking:bool = true, start:int(64) = 0, end:int(64) = max(int(64)), hints:c_int = 0, style:iostyle = this._style) {
    check();

    style.string_format = QIO_STRING_FORMAT_TOEND;
    style.string_end = 0x0a; // '\n'

    param kind = iokind.dynamic;
    var ret:ItemReader(string, kind, locking);
    on __primitive("chpl_on_locale_num", this.home_uid) {
      var ch = new channel(false, kind, locking, this, error, hints, start, end, style);
      ret = new ItemReader(string, kind, locking, ch);
    }
    return ret;
  }
  proc file.lines(param locking:bool = true, start:int(64) = 0, end:int(64) = max(int(64)), hints:c_int = 0, style:iostyle = this._style) {
    var err:err_t = ENOERR;
    var ret = this.lines(err, locking, start, end, hints, style);
    if err then _ioerror(err, "in file.lines", this.tryGetPath());
    return ret;
  }


  proc file.writer(out error:err_t, param kind:iokind, param locking:bool, start:int(64), end:int(64), hints:c_int, style:iostyle): channel(true,kind,locking) {
    check();

    var ret:channel(true, kind, locking);
    on __primitive("chpl_on_locale_num", this.home_uid) {
      ret = new channel(true, kind, locking, this, error, hints, start, end, style);
    }
    return ret;
  }
  proc file.writer(param kind=iokind.dynamic, param locking=true, start:int(64) = 0, end:int(64) = max(int(64)), hints:c_int = 0, style:iostyle = this._style): channel(true,kind,locking) 
  {
    var err:err_t = ENOERR;
    var ret = this.writer(err, kind, locking, start, end, hints, style);

    if err then _ioerror(err, "in file.writer", this.tryGetPath());
    return ret;
  }

  proc _isIoPrimitiveType(type t) param return
    _isPrimitiveType(t) || _isComplexType(t) ||
    _isImagType(t) || _isEnumeratedType(t);

   proc _isIoPrimitiveTypeOrNewline(type t) param return
    _isIoPrimitiveType(t) || t == ioNewline || t == ioLiteral || t == ioChar;

      //var trues = ("true", "1", "yes");
      //var falses = ("false", "0", "no");
  var _trues = tuple("true");
  var _falses = tuple("false");
  var _i = "i";

  // Read routines for all primitive types.
  proc _read_text_internal(_channel_internal:qio_channel_ptr_t, out x:?t):err_t where _isIoPrimitiveType(t) {
    if _isBooleanType(t) {
      var num = _trues.size;
      var err:err_t = ENOERR;
      var got:bool;

      err = EFORMAT;

      for i in 1..num {
        err = qio_channel_scan_literal(false, _channel_internal, _trues(i), _trues(i).length, 1);
        if !err {
          got = true;
          break;
        } else if err == EEOF {
          break;
        }
        err = qio_channel_scan_literal(false, _channel_internal, _falses(i), _falses(i).length, 1);
        if !err {
          got = false;
          break;
        } else if err == EEOF {
          break;
        }
      }

      if !err then x = got;
      return err;
    } else if _isIntegralType(t) {
      // handles int types
      return qio_channel_scan_int(false, _channel_internal, x, numBytes(t), _isSignedType(t));
    } else if _isRealType(t) {
      // handles real
      return qio_channel_scan_float(false, _channel_internal, x, numBytes(t));
    } else if _isImagType(t) {
      var err = qio_channel_mark(false, _channel_internal);
      if err then return err;

      err = qio_channel_scan_float(false, _channel_internal, x, numBytes(t));
      if !err {
        err = qio_channel_scan_literal(false, _channel_internal, _i, 1, false);
      }
      if !err {
        qio_channel_commit_unlocked(_channel_internal);
      } else {
        qio_channel_revert_unlocked(_channel_internal);
      }
      return err;
    } else if _isComplexType(t)  {
      // handle complex types
      var re:x.re.type;
      var im:x.im.type;
      var err:err_t = ENOERR;
      err = qio_channel_scan_complex(false, _channel_internal, re, im, numBytes(x.re.type));
      x = (re, im):t; // cast tuple to complex to get complex num.
      return err;
    } else if t == string {
      // handle string
      var len:ssize_t;
      return qio_channel_scan_string(false, _channel_internal, x, len, -1);
    } else if _isEnumeratedType(t) {
      var err:err_t = ENOERR;
      for i in chpl_enumerate(t) {
        var str:string = i:string;
        var slen:ssize_t = str.length;
        err = qio_channel_scan_literal(false, _channel_internal, str, slen, 1);
        if !err {
          x = i;
          break;
        } else if err != EFORMAT then break;
      }
      return err;
    } else {
      compilerError("Unknown primitive type in _read_text_internal ", typeToString(t));
    }
    return EINVAL;
  }

  proc _write_text_internal(_channel_internal:qio_channel_ptr_t, x:?t):err_t where _isIoPrimitiveType(t) {
    if _isBooleanType(t) {
      if x {
        return qio_channel_print_string(false, _channel_internal, _trues(1), _trues(1).length);
      } else {
        return qio_channel_print_string(false, _channel_internal, _falses(1), _falses(1).length);
      }
    } else if _isIntegralType(t) {
      // handles int types
      return qio_channel_print_int(false, _channel_internal, x, numBytes(t), _isSignedType(t));
 
    } else if _isRealType(t) {
      // handles real
      return qio_channel_print_float(false, _channel_internal, x, numBytes(t));
    } else if _isImagType(t) {
      var err = qio_channel_mark(false, _channel_internal);
      if err then return err;

      err = qio_channel_print_float(false, _channel_internal, x, numBytes(t));
      if !err {
        err = qio_channel_print_literal(false, _channel_internal, _i, 1);
      }
      if !err {
        qio_channel_commit_unlocked(_channel_internal);
      } else {
        qio_channel_revert_unlocked(_channel_internal);
      }
      return err;
    } else if _isComplexType(t)  {
      // handle complex types
      var re = x.re;
      var im = x.im;
      return qio_channel_print_complex(false, _channel_internal, re, im, numBytes(x.re.type));
    } else if t == string {
      // handle string
      return qio_channel_print_string(false, _channel_internal, x, x.length);
    } else if _isEnumeratedType(t) {
      var s = x:string;
      return qio_channel_print_string(false, _channel_internal, s, s.length);
    } else {
      compilerError("Unknown primitive type in _write_text_internal ", typeToString(t));
    }
    return EINVAL;
  }

  inline
  proc _read_binary_internal(_channel_internal:qio_channel_ptr_t, param byteorder:iokind, out x:?t):err_t where _isIoPrimitiveType(t) {
    if _isBooleanType(t) {
      var got:int(32);
      got = qio_channel_read_byte(false, _channel_internal);
      if got >= 0 {
        x = (got != 0);
        return ENOERR;
      } else {
        return (-got):err_t;
      }
    } else if _isIntegralType(t) {
      if numBytes(t) == 1 {
        var got:int(32);
        got = qio_channel_read_byte(false, _channel_internal);
        if got >= 0 {
          x = (got:uint(8)):t;
          return ENOERR;
        } else {
          return (-got):err_t;
        }
      } else {
        // handles int types
        return qio_channel_read_int(false, byteorder, _channel_internal, x, numBytes(t), _isSignedType(t));
      }
    } else if _isFloatType(t) {
      // handles real, imag
      return qio_channel_read_float(false, byteorder, _channel_internal, x, numBytes(t));
    } else if _isComplexType(t)  {
      // handle complex types
      var re:x.re.type;
      var im:x.im.type;
      var err:err_t = ENOERR;
      err = qio_channel_read_complex(false, byteorder, _channel_internal, re, im, numBytes(x.re.type));
      x = (re, im):t; // cast tuple to complex to get complex num.
      return err;
    } else if t == string {
      // handle string
      var len:ssize_t;
      return qio_channel_read_string(false, byteorder, qio_channel_str_style(_channel_internal), _channel_internal, x, len, -1);
    } else if _isEnumeratedType(t) {
      var i:enum_mintype(t);
      var err:err_t = ENOERR;
      err = qio_channel_read_int(false, byteorder, _channel_internal, i, numBytes(i.type), _isSignedType(i.type));
      x = i:t;
      return err;
    } else {
      compilerError("Unknown primitive type in _read_binary_internal ", typeToString(t));
    }
  }

  inline
  proc _write_binary_internal(_channel_internal:qio_channel_ptr_t, param byteorder:iokind, x:?t):err_t where _isIoPrimitiveType(t) {
    if _isBooleanType(t) {
      var zero_one:uint(8) = if x then 1:uint(8) else 0:uint(8);
      return qio_channel_write_byte(false, _channel_internal, zero_one);
    } else if _isIntegralType(t) {
      if numBytes(t) == 1 {
        return qio_channel_write_byte(false, _channel_internal, x:uint(8));
      } else {
        // handles int types
        return qio_channel_write_int(false, byteorder, _channel_internal, x, numBytes(t), _isSignedType(t));
      }
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
    } else if _isEnumeratedType(t) {
      var i:enum_mintype(t) = x:enum_mintype(t);
      return qio_channel_write_int(false, byteorder, _channel_internal, i, numBytes(i.type), _isSignedType(i.type));
    } else {
      halt("Unknown primitive type in write_binary_internal " + typeToString(t));
    }
  }

  // Channel must be locked, must be running on this.home
  // x is inout because it might contain a literal string.
  inline
  proc _read_one_internal(_channel_internal:qio_channel_ptr_t, param kind:iokind, inout x:?t):err_t where _isIoPrimitiveTypeOrNewline(t) {
    var e:err_t = EINVAL;
    if t == ioNewline {
      return qio_channel_skip_past_newline(false, _channel_internal);
    } else if t == ioChar {
      return qio_channel_read_char(false, _channel_internal, x.ch);
    } else if t == ioLiteral {
      //writeln("in scan literal ", x.val);
      return qio_channel_scan_literal(false, _channel_internal, x.val, x.val.length, x.ignoreWhiteSpace);
      //e = qio_channel_scan_literal(false, _channel_internal, x.val, x.val.length, x.ignoreWhiteSpace);
      //writeln("Scanning literal ", x.val,  " yeilded error ", e);
      //return e;
    } else if kind == iokind.dynamic {
      var binary:uint(8) = qio_channel_binary(_channel_internal);
      var byteorder:uint(8) = qio_channel_byteorder(_channel_internal);
      if binary {
        select byteorder {
          when big    do e = _read_binary_internal(_channel_internal, big, x);
          when little do e = _read_binary_internal(_channel_internal, little, x);
          otherwise      e = _read_binary_internal(_channel_internal, native, x);
        }
      } else {
        e = _read_text_internal(_channel_internal, x);
      }
    } else {
      e = _read_binary_internal(_channel_internal, kind, x);
    }
    return e;
  }

  // Channel must be locked, must be running on this.home
  inline
  proc _write_one_internal(_channel_internal:qio_channel_ptr_t, param kind:iokind, x:?t):err_t where _isIoPrimitiveTypeOrNewline(t) {
    var e:err_t = EINVAL;
    if t == ioNewline {
      return qio_channel_write_newline(false, _channel_internal);
    } else if t == ioChar {
      return qio_channel_write_char(false, _channel_internal, x.ch);
    } else if t == ioLiteral {
      return qio_channel_print_literal(false, _channel_internal, x.val, x.val.length);
    } else if kind == iokind.dynamic {
      var binary:uint(8) = qio_channel_binary(_channel_internal);
      var byteorder:uint(8) = qio_channel_byteorder(_channel_internal);
      if binary {
        select byteorder {
          when big    do e = _write_binary_internal(_channel_internal, big, x);
          when little do e = _write_binary_internal(_channel_internal, little, x);
          otherwise      e = _write_binary_internal(_channel_internal, native, x);
        }
      } else {
        e = _write_text_internal(_channel_internal, x);
      }
    } else {
      e = _write_binary_internal(_channel_internal, kind, x);
    }
    return e;
  }
  
  inline
  proc _read_one_internal(_channel_internal:qio_channel_ptr_t, param kind:iokind, inout x:?t):err_t {
    var reader = new ChannelReader(_channel_internal=_channel_internal);
    var err:err_t = ENOERR;
    reader.read(x);
    err = reader.err;
    delete reader;
    return err;
  }

  inline
  proc _write_one_internal(_channel_internal:qio_channel_ptr_t, param kind:iokind, x:?t):err_t {
    var writer = new ChannelWriter(_channel_internal=_channel_internal);
    var err:err_t = ENOERR;
    writer.write(x);
    err = writer.err;
    delete writer;
    return err;
  }

  /* Returns true if we read all the args,
     false if we encountered EOF (or possibly another error and didn't halt)*/
  inline
  proc channel.read(inout args ...?k,
                    out error:err_t):bool {
    if writing then compilerError("read on write-only channel");
    error = ENOERR;
    on __primitive("chpl_on_locale_num", this.home_uid) {
      this.lock();
      for param i in 1..k {
        if !error {
          error = _read_one_internal(_channel_internal, kind, args[i]);
        }
      }
      this.unlock();
    }
    return !error;
  }
  var _arg_to_proto_names = ("a", "b", "c", "d", "e", "f");
  proc _args_to_proto(args ...?k,
                      preArg:string) {
    var err_args:string = "";
    for param i in 1..k {
      var name:string;
      if i <= _arg_to_proto_names.size then name = _arg_to_proto_names(i);
      else name = "x" + i:string;
      err_args += preArg + name + ":" + typeToString(args(i).type);
      if i != k then err_args += ", ";
    }
    return err_args;
  }

  inline
  proc channel.read(inout args ...?k):bool {
    var e:err_t = ENOERR;
    this.read((...args), error=e);
    if !e then return true;
    else if e == EEOF then return false;
    else {
      this._ch_ioerror(e, "in channel.read(" +
                          _args_to_proto((...args), preArg="inout ") +
                          ")");
      return false;
    }
  }
  proc channel.read(inout args ...?k,
                    style:iostyle,
                    out error:err_t):bool {
    if writing then compilerError("read on write-only channel");
    error = ENOERR;
    on __primitive("chpl_on_locale_num", this.home_uid) {
      this.lock();
      var save_style = this._style();
      this._set_style(style);
      for param i in 1..k {
        if !error {
          error = _read_one_internal(_channel_internal, kind, args[i]);
        }
      }
      this._set_style(save_style);
      this.unlock();
    }
    return !error;
  }
  proc channel.read(inout args ...?k,
                    style:iostyle):bool {
    var e:err_t = ENOERR;
    this.read((...args), style=iostyle, error=e);
    if !e then return true;
    else if e == EEOF then return false;
    else {
      this._ch_ioerror(e, "in channel.read(" +
                          _args_to_proto((...args), preArg="inout ") +
                          "style:iostyle)");
      return false;
    }
  }

  proc channel.readline(inout arg:string, out error:err_t):bool {
    if writing then compilerError("read on write-only channel");
    error = ENOERR;
    on __primitive("chpl_on_locale_num", this.home_uid) {
      this.lock();
      var save_style = this._style();
      var mystyle = save_style.text();
      mystyle.string_format = QIO_STRING_FORMAT_TOEND;
      mystyle.string_end = 0x0a; // ascii newline.
      this._set_style(mystyle);
      error = _read_one_internal(_channel_internal, iokind.dynamic, arg);
      this._set_style(save_style);
      this.unlock();
    }
    return !error;
  }
  proc channel.readline(inout arg:string):bool {
    var e:err_t = ENOERR;
    this.readline(arg, error=e);
    if !e then return true;
    else if e == EEOF then return false;
    else {
      this._ch_ioerror(e, "in channel.readline(inout arg:string)");
      return false;
    }
  }

  proc channel.readln(out error:err_t):bool {
    var nl = new ioNewline();
    return this.read(nl, error=error);
  }
  proc channel.readln():bool {
    var nl = new ioNewline();
    return this.read(nl);
  }


  proc channel.readln(inout args ...?k):bool {
    var nl = new ioNewline();
    return this.read((...args), nl);
  }
  proc channel.readln(inout args ...?k,
                      out error:err_t):bool {
    var nl = new ioNewline();
    return this.read((...args), nl, error=error);
  }
  proc channel.readln(inout args ...?k,
                      style:iostyle,
                      out error:err_t):bool {
    var nl = new ioNewline();
    return this.read((...args), nl, style=style, error=error);
  }
  proc channel.readln(inout args ...?k,
                      style:iostyle):bool {
    var nl = new ioNewline();
    return this.read((...args), nl, style=style);
  }

  proc channel.read(type t) {
    var tmp:t;
    var e:err_t = ENOERR;
    this.read(tmp, error=e);
    if e then this._ch_ioerror(e, "in channel.read(type)");
    return tmp;
  }
  proc channel.readln(type t) {
    var tmp:t;
    var e:err_t = ENOERR;
    this.readln(tmp, error=e);
    if e then this._ch_ioerror(e, "in channel.readln(type)");
    return tmp;
  }
  // Read/write tuples of types.
  proc channel.readln(type t ...?numTypes) where numTypes > 1 {
    var tupleVal: t;
    for param i in 1..numTypes-1 do
      tupleVal(i) = this.read(t(i));
    tupleVal(numTypes) = this.readln(t(numTypes));
    return tupleVal;
  }
  proc channel.read(type t ...?numTypes) where numTypes > 1 {
    var tupleVal: t;
    for param i in 1..numTypes do
      tupleVal(i) = this.read(t(i));
    return tupleVal;
  }

  /*
  proc _debugWriteTypes(args ...?k) {
    for param i in 1..k {
      _debugWrite(typeToString(args(i).type));
      _debugWrite(" ");
    }
    _debugWriteln();
  }*/

  inline
  proc channel.write(args ...?k, out error:err_t):bool {
    if !writing then compilerError("write on read-only channel");
    error = ENOERR;
    on __primitive("chpl_on_locale_num", this.home_uid) {
      this.lock();
      for param i in 1..k {
        if !error {
          error = _write_one_internal(_channel_internal, kind, args(i));
        }
      }
      this.unlock();
    }
    return !error;
  }

  inline
  proc channel.write(args ...?k):bool {
    var e:err_t = ENOERR;
    this.write((...args), error=e);
    if !e then return true;
    else {
      this._ch_ioerror(e, "in channel.write(" +
                          _args_to_proto((...args), preArg="") +
                          ")");
      return false;
    }
  }

  proc channel.write(args ...?k,
                     style:iostyle,
                     out error:err_t):bool {
    if !writing then compilerError("write on read-only channel");
    error = ENOERR;
    on __primitive("chpl_on_locale_num", this.home_uid) {
      this.lock();
      var save_style = this._style();
      this._set_style(style);
      for param i in 1..k {
        if !error {
          error = _write_one_internal(dynamic, args(i));
        }
      }
      this._set_style(save_style);
      this.unlock();
    }
    return !error;
  }
  proc channel.write(args ...?k,
                     style:iostyle):bool {
    var e:err_t = ENOERR;
    this.write((...args), style=style, error=e);
    if !e then return true;
    else {
      this._ch_ioerror(e, "in channel.write(" +
                          _args_to_proto((...args), preArg="") +
                          "style:iostyle)");
      return false;
    }
  }

  proc channel.writeln(out error:err_t):bool {
    return this.write(new ioNewline(), error=error);
  }
  proc channel.writeln():bool {
    return this.write(new ioNewline());
  }
  proc channel.writeln(args ...?k, out error:err_t):bool {
    return this.write((...args), new ioNewline(), error=error);
  }
  proc channel.writeln(args ...?k):bool {
    return this.write((...args), new ioNewline());
  }
  proc channel.writeln(args ...?k,
                       style:iostyle):bool {
    return this.write((...args), new ioNewline(), style=style);
  }
  proc channel.writeln(args ...?k,
                       style:iostyle,
                       out error:err_t):bool {
    return this.write((...args), new ioNewline(), style=style, error=error);
  }


  proc channel.flush(out error:err_t) {
    error = ENOERR;
    on __primitive("chpl_on_locale_num", this.home_uid) {
      error = qio_channel_flush(locking, _channel_internal);
    }
  }
  proc channel.flush() {
    var e:err_t = ENOERR;
    this.flush(error=e);
    if e then this._ch_ioerror(e, "in channel.flush");
  }

  proc channel.close(out error:err_t) {
    error = ENOERR;
    on __primitive("chpl_on_locale_num", this.home_uid) {
      error = qio_channel_close(locking, _channel_internal);
    }
  }

  proc channel.close() {
    var e:err_t = ENOERR;
    this.close(error=e);
    if e then this._ch_ioerror(e, "in channel.close");
  }


  proc channel.modifyStyle(f:func(iostyle, void))
  {
    on __primitive("chpl_on_locale_num", this.home_uid) {
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
    param locking:bool;
    var ch:channel(false,kind,locking);
    proc read(out arg:ItemType, out error:err_t):bool {
      return ch.read(arg, error=error);
    }
    proc read(out arg:ItemType):bool {
      return ch.read(arg);
    }

    iter these() {
      while true {
        var x:ItemType;
        var gotany = ch.read(x);
        if ! gotany then break;
        yield x;
      }
    }
    /*
    iter these(out error:err_t) {
      while true {
        var x:ItemType;
        var gotany = ch.read(x, error=error);
        if ! gotany then break;
        yield x;
      }
    }*/
  }

  proc channel.itemReader(type ItemType, param kind:iokind=iokind.dynamic) {
    if writing then compilerError(".itemReader on write-only channel");
    return new ItemReader(ItemType, kind, locking, this);
  }

  record ItemWriter {
    type ItemType;
    param kind:iokind;
    param locking:bool;
    var ch:channel(false,kind);
    proc write(arg:ItemType, out error:err_t):bool {
      return ch.write(arg, error=error);
    }
    proc write(arg:ItemType):bool {
      return ch.write(arg);
    }
  }

  proc channel.itemWriter(type ItemType, param kind:iokind=iokind.dynamic) {
    if !writing then compilerError(".itemWriter on read-only channel");
    return new ItemWriter(ItemType, kind, locking, this);
  }

  // And now, the toplevel items.

  const stdin:channel(false, iokind.dynamic, true) = openfd(0).reader(); 
  const stdout:channel(true, iokind.dynamic, true) = openfp(chpl_cstdout()).writer(); 
  const stderr:channel(true, iokind.dynamic, true) = openfp(chpl_cstderr()).writer(); 

  proc write(args ...?n) {
    stdout.write((...args));
  }
  proc writeln(args ...?n) {
    stdout.writeln((...args));
  }
  proc writeln() {
    stdout.writeln();
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
  proc read(type t) {
    return stdin.read(t);
  }
  proc readln(type t) {
    return stdin.readln(t);
  }
  proc readln(type t ...?numTypes) where numTypes > 1 {
    return stdin.readln((...t));
  }
  proc read(type t ...?numTypes) where numTypes > 1 {
    return stdin.read((...t));
  }

  // readType and writeThis methods for the basic types
  // these just need to call writer.writePrimitive or reader.readPrimitive
  proc Reader.readType(inout x:numeric):bool {
    return this.readPrimitive(x);
    /*var m = this.readPrimitive(x.type);
    x = m.it;
    return m.hasit;*/
  }
  proc Reader.readType(inout x:enumerated):bool {
    return this.readPrimitive(x);
    /*var m = this.readPrimitive(x.type);
    x = m.it;
    return m.hasit;*/
  }
  proc Reader.readType(inout x:bool):bool {
     return this.readPrimitive(x);
    /*var m = this.readPrimitive(x.type);
    x = m.it;
    return m.hasit;*/
  }
  proc Reader.readType(inout x:string):bool {
    return this.readPrimitive(x);
    /*var m = this.readPrimitive(x.type);
    x = m.it;
    return m.hasit;*/
  }

  proc numeric.writeThis(r: Writer) { r.writePrimitive(this); }
  proc enumerated.writeThis(r: Writer) { r.writePrimitive(this); }
  proc bool.writeThis(r: Writer) { r.writePrimitive(this); }
  proc string.writeThis(r: Writer) { r.writePrimitive(this); }


  class ChannelWriter : Writer {
    var _channel_internal:qio_channel_ptr_t = QIO_CHANNEL_PTR_NULL;
    var err:err_t = ENOERR;
    proc binary:bool {
      var ret:uint(8) = qio_channel_binary(_channel_internal);
      return ret != 0;
    }
    proc error():err_t {
      return err;
    }
    proc setError(e:err_t) {
      err = e;
    }
    proc clearError() {
      err = 0;
    }
    proc writePrimitive(x) {
      if !err {
        err = _write_one_internal(_channel_internal, iokind.dynamic, x);
      }
    }
    proc writeThis(w:Writer) {
      // MPF - I don't understand why I had to add this,
      // but without it test/modules/diten/returnClassDiffModule5.chpl fails.
      compilerError("writeThis on ChannelWriter called");
    }
  }
  class ChannelReader : Reader {
    var _channel_internal:qio_channel_ptr_t = QIO_CHANNEL_PTR_NULL;
    var err:err_t = ENOERR;
    proc binary:bool {
      var ret:uint(8) = qio_channel_binary(_channel_internal);
      return ret != 0;
    }
    proc error():err_t {
      return err;
    }
    proc setError(e:err_t) {
      err = e;
    }
    proc clearError() {
      err = ENOERR;
    }
    proc readPrimitive(inout x:?t):bool where _isIoPrimitiveTypeOrNewline(t) {
      if !err {
        //err = _read_one_internal(_channel_internal, iokind.dynamic, ret.it);
        err = _read_one_internal(_channel_internal, iokind.dynamic, x);
        if err == EEOF {
          clearError();
          return false;
          //ret.hasit = false;
        } else {
          return true;
          //ret.hasit = true;
        }
      } else {
        //ret.hasit = false;
        return false;
      }
      //return ret;
    }
  }


