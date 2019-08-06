
module ByteBufferHelpers {
  use Bytes;
  pragma "no doc"
  type bufferType = c_ptr(uint(8));

  private proc isBytesOrStringType(type t) param: bool {
    return t==_bytes || t==string;
  }

  private proc copyRemoteBuffer(src_loc_id: int(64), src_addr: bufferType,
                                len: int): bufferType {
      const dest = chpl_here_alloc(len+1, offset_STR_COPY_REMOTE): bufferType;
      chpl_string_comm_get(dest, src_loc_id, src_addr, len);
      dest[len] = 0;
      return dest;
  }

  private proc localizeBufAndLen(

  private inline proc _strcmp_local(buf1, len1, buf2, len2) : int {
    // Assumes a and b are on same locale and not empty.
    const size = min(a.len, b.len);
    const result =  c_memcmp(a.buff, b.buff, size);

    if (result == 0) {
      // Handle cases where one string is the beginning of the other
      if (size < a.len) then return 1;
      if (size < b.len) then return -1;
    }
    return result;
  }

  inline proc _strcmp(a, b): int
      where isBytesOrStringType(a.type) && isBytesOrStringType(b.type) {
    if a.locale_id == chpl_nodeID && b.locale_id == chpl_nodeID {
      // it's local
      /*return _strcmp_local(a, b);*/
      return _strcmp_local(a.buff, a.len, b.buff, b.len);
    } else {
      var localA: _bytes = a.localize();
      var localB: _bytes = b.localize();
      /*return _strcmp_local(localA, localB);*/
      return _strcmp_local(localA.buff, localA.len,
                           localB.buff, localB.len);
    }
  }

  // Concatenation with other types is done by casting to bytes
  inline proc concatHelp(s: ?t, x)
      where isBytesOrStringType(t) && x.type != t {
    var cs = x:t;
    const ret = s + cs;
    return ret;
  }

  inline proc concatHelp(x, s: ?t)
      where isBytesOrStringType(t) && x.type != t {
    var cs = x:t;
    const ret = cs + s;
    return ret;
  }

  /*
     Copies the bytes `rhs` into the bytes `lhs`.
  */
  proc =(ref lhs: ?t, rhs: t) where isBytesOrStringType(t) {
    inline proc helpMe(ref lhs: ?t, rhs: t) 
          where isBytesOrStringType(t) {
      if _local || rhs.locale_id == chpl_nodeID {
        lhs.reinitString(rhs.buff, rhs.len, rhs._size, needToCopy=true);
      } else {
        const len = rhs.len; // cache the remote copy of len
        var remote_buf:bufferType = nil;
        if len != 0 then
          remote_buf = copyRemoteBuffer(rhs.locale_id, rhs.buff, len);
        lhs.reinitString(remote_buf, len, len+1, needToCopy=false);
      }
    }

    if _local || lhs.locale_id == chpl_nodeID then {
      helpMe(lhs, rhs);
    }
    else {
      on __primitive("chpl_on_locale_num",
                     chpl_buildLocaleID(lhs.locale_id, c_sublocid_any)) {
        helpMe(lhs, rhs);
      }
    }
  }

  /*
     Copies the c_string `rhs_c` into the bytes `lhs`.

     Halts if `lhs` is a remote bytes.
  */
  proc =(ref lhs: ?t, rhs_c: c_string) where isBytesOrStringType(t) {
    // Make this some sort of local check once we have local types/vars
    if !_local && (lhs.locale_id != chpl_nodeID) then
      halt("Cannot assign a c_string to a remote string.");

    const len = rhs_c.length;
    const buff:bufferType = rhs_c:bufferType;
    lhs.reinitString(buff, len, len+1, needToCopy=true);
  }
}
