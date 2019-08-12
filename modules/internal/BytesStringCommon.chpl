module BytesStringCommon {
  use Bytes;
  private use ByteBufferHelpers;

  private proc isBytesOrStringType(type t) param: bool {
    return t==_bytes || t==string;
  }

  proc get_c_str(const ref x: ?t): c_string where isBytesOrStringType(t) {
    inline proc _cast(type t:c_string, b:bufferType) {
      return __primitive("cast", t, b);
    }

    if _local == false && x.locale_id != chpl_nodeID then
      halt("Cannot call .c_str() on a remote bytes");

    return x.buff:c_string;
  }

  proc get_slice(const ref x: ?t, r: range(?)) where isBytesOrStringType(t) {
      var ret: t;
      if x.isEmpty() then return ret;

      const r2 = x._getView(r);
      if r2.size <= 0 {
        // TODO: I can't just return "" (ret var gets freed for some reason)
        ret = "";
      } else {
        // In case r1 is strided, we cannot just use r2.size for the copy
        // length. For to be able to cover strided copies, we copy the range
        // from low to high then do a strided operation to put the data in the
        // buffer in the correct order.
        const copyLen = r2.high-r2.low+1;
        var (copyBuf, copySize) = copyChunk(buf=x.buff, off=r2.low-1,
                                            len=copyLen, loc=x.locale_id);
        if r2.stride == 1 {
          // TODO Engin: I'd like to call init or something that constructs a byte
          // object instead of doing the these
          ret.buff = copyBuf;
          ret._size = copySize;
        }
        else {
          // the range is strided
          var (newBuf, allocSize) = allocBuffer(r2.size+1);
          for (r2_i, i) in zip(r2, 0..) {
            newBuf[i] = copyBuf[r2_i-r2.low];
          }
          ret.buff = newBuf;
          ret._size = allocSize;
          freeBuffer(copyBuf);
        }
        ret.len = r2.size;
        ret.buff[ret.len] = 0;
      }
      return ret;
  }

  proc getIndexType(type t) type {
    if t==_bytes then return Bytes.idxType;
    else if t==string then return String.byteIndex;
    else compilerError("This function should only be used by bytes or string");
  }

  proc do_replace(const ref x: ?t, needle: t, replacement: t,
                  count: int = -1): t where isBytesOrStringType(t) {
    type _idxt = getIndexType(t);
    var result: t = x;
    var found: int = 0;
    var startIdx: _idxt = 1;
    const localNeedle: t = needle.localize();
    const localReplacement: t = replacement.localize();

    while (count < 0) || (found < count) {
      const idx = result.find(localNeedle, startIdx..);
      if !idx then break;

      found += 1;
      result = result[..idx-1] + localReplacement +
               result[(idx + localNeedle.numBytes)..];
      var tmp_res = result[..idx-1] + localReplacement +
               result[(idx + localNeedle.numBytes)..];

      startIdx = idx + localReplacement.numBytes;
    }
    return result;
  }

  iter do_split(const ref x: ?t, sep: t, maxsplit: int = -1,
                ignoreEmpty: bool = false): t {
    type _idxt = getIndexType(t);

      if !(maxsplit == 0 && ignoreEmpty && x.isEmpty()) {
        const localThis: t = x.localize();
        const localSep: t = sep.localize();

        // really should be <, but we need to avoid returns and extra yields so
        // the iterator gets inlined
        var splitAll: bool = maxsplit <= 0;
        var splitCount: int = 0;

        var start: _idxt = 1;
        var done: bool = false;
        while !done  {
          var chunk: t;
          var end: _idxt;

          if (maxsplit == 0) {
            chunk = localThis;
            done = true;
          } else {
            if (splitAll || splitCount < maxsplit) then
              end = localThis.find(localSep, start..);

            if(end == 0) {
              // Separator not found
              chunk = localThis[start..];
              done = true;
            } else {
              chunk = localThis[start..end-1];
            }
          }

          if !(ignoreEmpty && chunk.isEmpty()) {
            // Putting the yield inside the if prevents us from being inlined
            // in the zippered case, but I don't think there is any way to avoid
            // that easily
            yield chunk;
            splitCount += 1;
          }
          start = end+localSep.length;
        }
      }
  }

  // TODO: could use a multi-pattern search or some variant when there are
  // multiple needles. Probably wouldn't be worth the overhead for small
  // needles though
  pragma "no doc"
  inline proc _startsEndsWith(const ref x: ?t, needles,
                              param fromLeft: bool) : bool 
                              where isBytesOrStringType(t) && 
                                    isHomogeneousTuple(needles) &&
                                    needles[1].type==t {

      var ret: bool = false;
      on __primitive("chpl_on_locale_num",
                     chpl_buildLocaleID(x.locale_id, c_sublocid_any)) {
        for needle in needles {
          const needleLen = needle.len;
          if needleLen == 0 {
            ret = true;
            break;
          }
          if needleLen > x.len then continue;

          const localNeedle = needle.localize();

          if fromLeft {
            ret = bufferEqualsLocal(buf1=x.buff, off1=0,
                                    buf2=localNeedle.buff, off2=0,
                                    len=needleLen);
          } else {
            var offset = x.len-needleLen;
            ret = bufferEqualsLocal(buf1=x.buff, off1=offset,
                                    buf2=localNeedle.buff, off2=0,
                                    len=needleLen);

          }
          if ret == true then break;
        }
      }
      return ret;
  }

  proc do_join_iterator(const ref x: ?t, ir: _iteratorRecord): t
        where isBytesOrStringType(t) {
    var s: t;
    var first: bool = true;
    for i in ir {
      if first then
        first = false;
      else
        s += x;
      s += i;
    }
    return s;
  }

  proc do_join(const ref x: ?t, const ref S): t
               where isBytesOrStringType(t) && (isTuple(S) || isArray(S)) {
    if S.size == 0 {
      return '';
    } else if S.size == 1 {
      // TODO: ensures copy, clean up when no longer needed
      var ret: t;
      if (isArray(S)) {
        ret = S[S.domain.first];
      } else {
        ret = S[1];
      }
      return ret;
    } else {
      var joinedSize: int = x.len * (S.size - 1);
      for s in S do joinedSize += s.numBytes;

      if joinedSize == 0 then
        return '';

      var joined: t;
      joined.len = joinedSize;

      var (newBuf, allocSize) = allocBuffer(joined.len+1);
      joined._size = allocSize;
      joined.buff = newBuf;

      var first = true;
      var offset = 0;
      for s in S {
        const sLen = s.len;

        // copy x's contents
        if first {
          first = false;
        } else if x.len != 0 {
          bufferMemcpyLocal(dst=joined.buff, src=x.buff, len=x.len,
                            dst_off=offset);
          offset += x.len;
        }

        // copy s's contents
        if sLen != 0 {
          bufferMemcpy(dst=joined.buff, dst_off=offset,
                       src_loc=s.locale_id, src=s.buff,len=sLen);
          offset += sLen;
        }
      }
      joined.buff[joined.len] = 0;
      return joined;
    }
  }

  // TODO: I could make this and other routines that use find faster by
  // making a version of search helper that only takes in local strings and
  // localizing in the calling function
  proc do_partition(const ref x: ?t, sep: t): 3*t where isBytesOrStringType(t) {
    const idx = x.find(sep);
    if idx != 0 {
      return (x[..idx-1], sep, x[idx+sep.numBytes..]);
    } else {
      return (x, "":t, "":t);
    }
  }

  proc do_append(ref lhs: ?t, const ref rhs: t) where isBytesOrStringType(t) {
    // if rhs is empty, nothing to do
    if rhs.len == 0 then return;

    on __primitive("chpl_on_locale_num",
                   chpl_buildLocaleID(lhs.locale_id, c_sublocid_any)) {
      const rhsLen = rhs.len;
      const newLength = lhs.len+rhsLen; //TODO: check for overflow
      //resize the buffer if needed
      if lhs._size <= newLength {
        const requestedSize = max(newLength+1,
                                  lhs.len*chpl_stringGrowthFactor):int;
        if lhs.isowned {
          var (newBuff, allocSize) = reallocBuffer(lhs.buff, requestedSize);
          lhs.buff = newBuff;
          lhs._size = allocSize;
        } else {
          var (newBuff, allocSize) = allocBuffer(requestedSize);
          bufferMemcpyLocal(dst=newBuff, src=lhs.buff, lhs.len);
          lhs.buff = newBuff;
          lhs._size = allocSize;
          lhs.isowned = true;
        }
      }
      // copy the data from rhs
      bufferMemcpy(dst=lhs.buff, src_loc=rhs.locale_id, rhs.buff, rhsLen,
                   dst_off=lhs.len);
      lhs.len = newLength;
      lhs.buff[newLength] = 0;
    }
  }

  proc do_assign(ref lhs: ?t, rhs: t) where isBytesOrStringType(t) {
    inline proc helpMe(ref lhs: t, rhs: t) {
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

  proc do_assign(ref lhs: ?t, rhs_c: c_string)
                 where isBytesOrStringType(t) {
    // Make this some sort of local check once we have local types/vars
    if !_local && (lhs.locale_id != chpl_nodeID) then
      halt("Cannot assign a c_string to a remote string.");

    const len = rhs_c.length;
    const buff:bufferType = rhs_c:bufferType;
    lhs.reinitString(buff, len, len+1, needToCopy=true);
  }

  proc do_multiply(const ref x: ?t, n: integral) where isBytesOrStringType(t) {
    if n <= 0 then return new t("");

    const sLen = x.length;
    if sLen == 0 then return new t("");

    // TODO Engin: Implement a factory function for this case
    var ret: t;
    ret.len = sLen * n; // TODO: check for overflow
    var (buff, allocSize) = allocBuffer(ret.len+1);
    ret.buff = buff;
    ret._size = allocSize;
    ret.isowned = true;

    bufferMemcpy(dst=ret.buff, src_loc=x.locale_id, src=x.buff, len=x.len);

    var offset = sLen;
    for i in 1..(n-1) {
      bufferMemcpyLocal(dst=ret.buff, src=ret.buff, len=x.len,
                        dst_off=offset);
      offset += sLen;
    }
    ret.buff[ret.len] = 0;
    return ret;
  }

  proc do_concat(s0: ?t, s1: t): t {
    // cache lengths locally
    const s0len = s0.len;
    if s0len == 0 then return s1:t;
    const s1len = s1.len;
    if s1len == 0 then return s0;

    // TODO Engin: Implement a factory function for this case
    var ret: t;
    ret.len = s0len + s1len;
    var (buff, allocSize) = allocBuffer(ret.len+1);
    ret.buff = buff;
    ret._size = allocSize;
    ret.isowned = true;

    bufferMemcpy(dst=ret.buff, src_loc=s0.locale_id, src=s0.buff, len=s0len);
    bufferMemcpy(dst=ret.buff, src_loc=s1.locale_id, src=s1.buff, len=s1len,
                 dst_off=s0len);
    ret.buff[ret.len] = 0;
    return ret;
  }

  proc do_eq(a: ?t1, b: ?t2) where isBytesOrStringType(t1) && 
                                   isBytesOrStringType(t2) {
    // At the moment, this commented out section will not work correctly. If a
    // and b are on the same locale, we will go to that locale, but an autoCopy
    // will localize a and b, before they are placed into the on bundle,
    // causing us to access garbage data inside the doEq routine. Always
    // localize for now.
    //
    /* if a.locale_id == b.locale_id {
      var ret: bool = false;
      on __primitive("chpl_on_locale_num",
                     chpl_buildLocaleID(a.locale_id, c_sublocid_any)) {
        ret = doEq(a, b);
      }
      return ret;
    } else { */
    return _strcmp(a.buff, a.len, a.locale_id, b.buff, b.len, b.locale_id) == 0;
  }
}
