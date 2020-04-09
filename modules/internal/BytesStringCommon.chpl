/*
 * Copyright 2020 Hewlett Packard Enterprise Development LP
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

module BytesStringCommon {
  private use ChapelStandard;
  private use SysCTypes;
  private use Bytes;
  private use ByteBufferHelpers;

  /*
     ``decodePolicy`` specifies what happens when there is malformed characters
     when decoding a :record:`bytes` into a UTF-8 :record:`~String.string`.
       
       - **strict**: default policy; raise error
       - **replace**: replace with UTF-8 replacement character
       - **drop**: silently drop data
       - **escape**: escape invalid data by replacing each byte 0xXX with
                     codepoint 0xDCXX
       - **ignore**: silently drop data (Deprecated)
  */
  enum decodePolicy { strict, replace, drop, escape, ignore }

  /*
     ``encodePolicy`` specifies what happens when there is escaped non-UTF8
     bytes when encoding a :record:`~String.string` into a
     :record:`~Bytes.bytes`.
       
       - **pass**: default policy; copy directly
       - **unescape**: recover the original data from the escaped data
  */
  enum encodePolicy { unescape, pass };


  pragma "no doc"
  config param showStringBytesInitDeprWarnings = true;

  pragma "no doc"
  param surrogateEscape = 0xdc:byteType;

  private proc isBytesOrStringType(type t) param: bool {
    return t==bytes || t==string;
  }

  private proc assertArgType(type t, param methodName: string) param {
    if !isBytesOrStringType(t) {
      compilerError("BytesStringCommon."+methodName+
                    " can only be called with bytes or string argument(s)");
    }
  }

  // 2019/8/22 Engin: This proc needs to be inlined to avoid an Intel compiler
  // issue (#448 chapel-private)
  inline proc getCStr(const ref x: ?t): c_string {
    assertArgType(t, "getCStr");
    inline proc _cast(type t:c_string, b:bufferType) {
      return __primitive("cast", t, b);
    }

    if _local == false && x.locale_id != chpl_nodeID then
      halt("Cannot call .c_str() on a remote " + t:string);

    return x.buff:c_string;
  }

  /*
   This function is called by `bytes.decode` and string factory functions that
   take a C array as the buffer.

   It iterates over the buffer, trying to decode codepoints out of it. If there
   is an illegal sequence that doesn't correspond to any valid codepoint, the
   behavior is determined by the `policy` argument. See the `decodePolicy`
   documentation above for the meaning of different policies.
  */
  proc decodeByteBuffer(buff: bufferType, length: int, policy: decodePolicy)
      throws {

    pragma "fn synchronization free"
    extern proc qio_decode_char_buf(ref chr:int(32), ref nbytes:c_int,
                                    buf:c_string, buflen:ssize_t): syserr;
    pragma "fn synchronization free"
    extern proc qio_encode_char_buf(dst: c_void_ptr, chr: int(32)): syserr;
    pragma "fn synchronization free"
    extern proc qio_nbytes_char(chr: int(32)): c_int;

    // allocate buffer the same size as this buffer assuming that the string
    // is in fact perfectly decodable. In the worst case, the user wants the
    // replacement policy and we grow the buffer couple of times.
    // The alternative is to allocate more space from the beginning.
    var ret: string;
    var (newBuff, allocSize) = bufferAlloc(length+1);
    ret.buff = newBuff;
    ret.buffSize = allocSize;
    ret.isOwned = true;
    ret.hasEscapes = false;

    var expectedSize = ret.buffSize;

    var thisIdx = 0;
    var decodedIdx = 0;
    while thisIdx < length {
      var cp: int(32);
      var nbytes: c_int;
      var bufToDecode = (buff + thisIdx): c_string;
      var maxbytes = (length - thisIdx): ssize_t;
      const decodeRet = qio_decode_char_buf(cp, nbytes,
                                            bufToDecode, maxbytes);

      if decodeRet != 0 {  //decoder returns error
        if policy == decodePolicy.strict {
          throw new owned DecodeError();
        }
        else {
          // if nbytes is 1, then we must have read a single byte and found
          // that it was invalid, if nbytes is >1 then we must have read
          // multiple bytes where the last one broke the sequence. But it can
          // be a valid byte itself. So we rewind by 1 in that case
          // we use nInvalidBytes to store how many bytes we are ignoring or
          // replacing
          const nInvalidBytes = if nbytes==1 then nbytes else nbytes-1;
          thisIdx += nInvalidBytes;

          if policy == decodePolicy.replace {
            param replChar: int(32) = 0xfffd;

            // Replacement can cause the string to be larger than initially
            // expected. The Unicode replacement character has codepoint
            // 0xfffd. It is encoded in `encodedReplChar` and its encoded
            // length is `nbytesRepl`, which is 3 bytes in UTF8. If it is used
            // in place of a single byte, we may overflow
            expectedSize += 3-nInvalidBytes;
            (ret.buff, ret.buffSize) = bufferEnsureSize(ret.buff, ret.buffSize,
                                                     expectedSize);

            qio_encode_char_buf(ret.buff+decodedIdx, replChar);

            decodedIdx += 3;  // replacement character is 3 bytes in UTF8
          }
          else if policy == decodePolicy.escape {

            ret.hasEscapes = true;
            // encoded escape sequence is 3 bytes. And this is per invalid byte
            expectedSize += 2*nInvalidBytes;
            (ret.buff, ret.buffSize) = bufferEnsureSize(ret.buff, ret.buffSize,
                                                     expectedSize);
            for i in 0..#nInvalidBytes {
              qio_encode_char_buf(ret.buff+decodedIdx,
                                  0xdc00+buff[thisIdx-nInvalidBytes+i]);
              decodedIdx += 3;
            }
          }
          // if policy == decodePolicy.ignore, we don't do anything and skip over
          // the invalid sequence
        }
      }
      else {  // we got valid characters
        // do a naive copy
        bufferMemcpyLocal(dst=ret.buff, src=bufToDecode, len=nbytes,
                          dst_off=decodedIdx);
        thisIdx += nbytes;
        decodedIdx += nbytes;
      }
    }

    ret.buffLen = decodedIdx;
    ret.buff[ret.buffLen] = 0;
    return ret;
  }

  proc initWithBorrowedBuffer(ref x: ?t, other: t) {
    assertArgType(t, "initWithBorrowedBuffer");

    x.isOwned = false;
    if t == string then x.hasEscapes = other.hasEscapes;

    const otherRemote = other.locale_id != chpl_nodeID;
    const otherLen = other.numBytes;

    if otherLen > 0 {
      x.buffLen = otherLen;
      if otherRemote {
        // if other is remote, copy and own the buffer no matter what
        x.isOwned = true;
        x.buff = bufferCopyRemote(other.locale_id, other.buff, otherLen);
        x.buffLen = otherLen+1;
      }
      else {
        // if other is local just adjust my buff and _size
        x.buff = other.buff;
        x.buffSize = other.buffSize;
      }
    }
  }

  proc initWithBorrowedBuffer(ref x: ?t, other: bufferType, length:int, size:int) {
    assertArgType(t, "initWithBorrowedBuffer");

    x.isOwned = false;

    // here, we don't need to do anything special if length==0, the buffer may
    // be allocated but empty
    x.buff = other;
    x.buffSize = size;
    x.buffLen = length;
  }

  proc initWithOwnedBuffer(ref x: ?t, other: bufferType, length:int, size:int) {
    assertArgType(t, "initWithOwnedBuffer");

    x.isOwned = true;

    // here, we don't need to do anything special if length==0, the buffer may
    // be allocated but empty
    x.buff = other;
    x.buffSize = size;
    x.buffLen = length;
  }

  proc initWithNewBuffer(ref x: ?t, other: t) {
    assertArgType(t, "initWithNewBuffer");

    const otherRemote = other.locale_id != chpl_nodeID;
    const otherLen = other.numBytes;
    x.isOwned = true;
    if t == string then x.hasEscapes = other.hasEscapes;

    if otherLen > 0 {
      x.buffLen = otherLen;
      if otherRemote {
        // if s is remote, copy and own the buffer
        x.buff = bufferCopyRemote(other.locale_id, other.buff, otherLen);
        x.buffSize = otherLen+1;
      }
      else {
        // if s is local create a copy of its buffer and own it
        const (buff, allocSize) = bufferCopyLocal(other.buff, otherLen);
        x.buff = buff;
        x.buff[x.buffLen] = 0;
        x.buffSize = allocSize;
      }
    }
  }

  proc initWithNewBuffer(ref x: ?t, other: bufferType, length:int, size:int) {
    assertArgType(t, "initWithNewBuffer");

    const otherLen = length;
    x.isOwned = true;

    if otherLen > 0 {
      // create a copy of s's buffer and own it
      const (buff, allocSize) = bufferCopyLocal(other:bufferType, otherLen);
      x.buff = buff;
      x.buffLen = otherLen;
      x.buff[x.buffLen] = 0;
      x.buffSize = allocSize;
    }
  }

  // TODO: I wasn't very good about caching variables locally in this one.
  proc getSlice(const ref x: ?t, r: range(?)) {
    assertArgType(t, "getSlice");

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
      var (copyBuf, copySize) = bufferCopy(buf=x.buff, off=r2.low,
                                          len=copyLen, loc=x.locale_id);
      if r2.stride == 1 {
        // TODO Engin: I'd like to call init or something that constructs a
        // new bytes/string object instead of doing the these all the time
        ret.buff = copyBuf;
        ret.buffSize = copySize;
      }
      else {
        // the range is strided
        var (newBuff, allocSize) = bufferAlloc(r2.size+1);
        for (r2_i, i) in zip(r2, 0..) {
          newBuff[i] = copyBuf[r2_i-r2.low];
        }
        ret.buff = newBuff;
        ret.buffSize = allocSize;
        bufferFree(copyBuf);
      }
      ret.buffLen = r2.size;
      ret.buff[ret.buffLen] = 0;
    }
    return ret;
  }

  proc getIndexType(type t) type {
    if t==bytes then return Bytes.idxType;
    else if t==string then return String.byteIndex;
    else compilerError("This function should only be used by bytes or string");
  }

  // TODO: not ideal - count and single allocation probably faster
  //                 - can special case on replacement|needle.size (0, 1)
  proc doReplace(const ref x: ?t, needle: t, replacement: t,
                  count: int = -1): t {
    assertArgType(t, "doReplace");

    type _idxt = getIndexType(t);
    var result: t = x;
    var found: int = 0;
    var startIdx: _idxt = 0;
    const localNeedle: t = needle.localize();
    const localReplacement: t = replacement.localize();

    while (count < 0) || (found < count) {
      const idx = result.find(localNeedle, startIdx..);
      if idx == -1 then break;

      found += 1;
      result = result[..idx-1] + localReplacement +
               result[(idx + localNeedle.numBytes)..];

      startIdx = idx + localReplacement.numBytes;
    }
    return result;
  }

  iter doSplit(const ref x: ?t, sep: t, maxsplit: int = -1,
                ignoreEmpty: bool = false): t {
    assertArgType(t, "doSplit");

    type _idxt = getIndexType(t);

    if !(maxsplit == 0 && ignoreEmpty && x.isEmpty()) {
      const localThis: t = x.localize();
      const localSep: t = sep.localize();

      // really should be <, but we need to avoid returns and extra yields so
      // the iterator gets inlined
      var splitAll: bool = maxsplit <= 0;
      var splitCount: int = 0;

      var start: _idxt = 0;
      var done: bool = false;
      while !done  {
        var chunk: t;
        var end: _idxt = -1;

        if (maxsplit == 0) {
          chunk = localThis;
          done = true;
        } else {
          if (splitAll || splitCount < maxsplit) then
            end = localThis.find(localSep, start..);

          if(end == -1) {
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
        start = end+localSep.numBytes;
      }
    }
  }

  // TODO: could use a multi-pattern search or some variant when there are
  // multiple needles. Probably wouldn't be worth the overhead for small
  // needles though
  pragma "no doc"
  inline proc startsEndsWith(const ref x: ?t, needles,
                             param fromLeft: bool) : bool 
                             where isHomogeneousTuple(needles) &&
                                   needles[0].type==t {
    assertArgType(t, "startsEndsWith");

    var ret: bool = false;
    on __primitive("chpl_on_locale_num",
                   chpl_buildLocaleID(x.locale_id, c_sublocid_any)) {
      for needle in needles {
        const needleLen = needle.buffLen;
        if needleLen == 0 {
          ret = true;
          break;
        }
        if needleLen > x.buffLen then continue;

        const localNeedle = needle.localize();

        if fromLeft {
          ret = bufferEqualsLocal(buf1=x.buff, off1=0,
                                  buf2=localNeedle.buff, off2=0,
                                  len=needleLen);
        } else {
          var offset = x.buffLen-needleLen;
          ret = bufferEqualsLocal(buf1=x.buff, off1=offset,
                                  buf2=localNeedle.buff, off2=0,
                                  len=needleLen);

        }
        if ret == true then break;
      }
    }
    return ret;
  }

  proc doJoinIterator(const ref x: ?t, ir: _iteratorRecord): t {
    assertArgType(t, "doJoinIterator");

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

  proc doJoin(const ref x: ?t, const ref S): t
               where (isTuple(S) || isArray(S)) {
    assertArgType(t, "doJoin");

    if S.size == 0 {
      return '';
    } else if S.size == 1 {
      // TODO: ensures copy, clean up when no longer needed
      var ret: t;
      if (isArray(S)) {
        ret = S[S.domain.first];
      } else {
        ret = S[0];
      }
      return ret;
    } else {
      var joinedSize: int = x.buffLen * (S.size - 1);
      for s in S do joinedSize += s.numBytes;

      if joinedSize == 0 then
        return '';

      var joined: t;
      joined.buffLen = joinedSize;

      var (newBuff, allocSize) = bufferAlloc(joined.buffLen+1);
      joined.buffSize = allocSize;
      joined.buff = newBuff;

      var first = true;
      var offset = 0;
      for s in S {
        const sLen = s.buffLen;

        // copy x's contents
        if first {
          first = false;
        } else if x.buffLen != 0 {
          bufferMemcpyLocal(dst=joined.buff, src=x.buff, len=x.buffLen,
                            dst_off=offset);
          offset += x.buffLen;
        }

        // copy s's contents
        if sLen != 0 {
          bufferMemcpy(dst=joined.buff, dst_off=offset,
                       src_loc=s.locale_id, src=s.buff,len=sLen);
          offset += sLen;
        }
      }
      joined.buff[joined.buffLen] = 0;
      return joined;
    }
  }

  // TODO: I could make this and other routines that use find faster by
  // making a version of search helper that only takes in local strings and
  // localizing in the calling function
  proc doPartition(const ref x: ?t, sep: t): 3*t {
    assertArgType(t, "doPartition");

    const idx = x.find(sep);
    if idx != -1 {
      return (x[..idx-1], sep, x[idx+sep.numBytes..]);
    } else {
      return (x, "":t, "":t);
    }
  }

  proc doAppend(ref lhs: ?t, const ref rhs: t) {
    assertArgType(t, "doAppend");

    // if rhs is empty, nothing to do
    if rhs.buffLen == 0 then return;

    on __primitive("chpl_on_locale_num",
                   chpl_buildLocaleID(lhs.locale_id, c_sublocid_any)) {
      const rhsLen = rhs.buffLen;
      const newLength = lhs.buffLen+rhsLen; //TODO: check for overflow
      //resize the buffer if needed
      if lhs.buffSize <= newLength {
        const requestedSize = max(newLength+1,
                                  (lhs.buffLen*chpl_stringGrowthFactor):int);
        if lhs.isOwned {
          var (newBuff, allocSize) = bufferRealloc(lhs.buff, requestedSize);
          lhs.buff = newBuff;
          lhs.buffSize = allocSize;
        } else {
          var (newBuff, allocSize) = bufferAlloc(requestedSize);
          bufferMemcpyLocal(dst=newBuff, src=lhs.buff, lhs.buffLen);
          lhs.buff = newBuff;
          lhs.buffSize = allocSize;
          lhs.isOwned = true;
        }
      }
      // copy the data from rhs
      bufferMemcpy(dst=lhs.buff, src_loc=rhs.locale_id, rhs.buff, rhsLen,
                   dst_off=lhs.buffLen);
      lhs.buffLen = newLength;
      lhs.buff[newLength] = 0;
    }
  }

  proc doAssign(ref lhs: ?t, rhs: t) {
    assertArgType(t, "doAssign");

    inline proc helpMe(ref lhs: t, rhs: t) {
      if _local || rhs.locale_id == chpl_nodeID {
        lhs.reinitString(rhs.buff, rhs.buffLen, rhs.buffSize, needToCopy=true);
      } else {
        const len = rhs.buffLen; // cache the remote copy of len
        var remote_buf:bufferType = nil;
        if len != 0 then
          remote_buf = bufferCopyRemote(rhs.locale_id, rhs.buff, len);
        lhs.reinitString(remote_buf, len, len+1, needToCopy=false,
                                                 ownBuffer=true);
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

  proc doAssign(ref lhs: ?t, rhs_c: c_string) {
    assertArgType(t, "doAssign");

    // Make this some sort of local check once we have local types/vars
    if !_local && (lhs.locale_id != chpl_nodeID) then
      halt("Cannot assign a c_string to a remote string.");

    const len = rhs_c.size;
    const buff:bufferType = rhs_c:bufferType;
    lhs.reinitString(buff, len, len+1, needToCopy=true);
  }

  proc doMultiply(const ref x: ?t, n: integral) {
    assertArgType(t, "doMultiply");

    const sLen = x.numBytes;
    if isBytesType(t) {
      if n <= 0 then return b"";
      if sLen == 0 then return b"";
    }
    else if isStringType(t) {
      if n <= 0 then return "";
      if sLen == 0 then return "";
    }
    else {
      compilerError("Unexpected type");
    }

    // TODO Engin: Implement a factory function for this case
    var ret: t;
    ret.buffLen = sLen * n; // TODO: check for overflow
    var (buff, allocSize) = bufferAlloc(ret.buffLen+1);
    ret.buff = buff;
    ret.buffSize = allocSize;
    ret.isOwned = true;

    bufferMemcpy(dst=ret.buff, src_loc=x.locale_id, src=x.buff, len=x.buffLen);

    var offset = sLen;
    for i in 1..(n-1) {
      bufferMemcpyLocal(dst=ret.buff, src=ret.buff, len=x.buffLen,
                        dst_off=offset);
      offset += sLen;
    }
    ret.buff[ret.buffLen] = 0;
    return ret;
  }

  proc doConcat(s0: ?t, s1: t): t {
    // cache lengths locally
    const s0len = s0.buffLen;
    if s0len == 0 then return s1:t;
    const s1len = s1.buffLen;
    if s1len == 0 then return s0;

    // TODO Engin: Implement a factory function for this case
    var ret: t;
    ret.buffLen = s0len + s1len;
    var (buff, allocSize) = bufferAlloc(ret.buffLen+1);
    ret.buff = buff;
    ret.buffSize = allocSize;
    ret.isOwned = true;

    bufferMemcpy(dst=ret.buff, src_loc=s0.locale_id, src=s0.buff, len=s0len);
    bufferMemcpy(dst=ret.buff, src_loc=s1.locale_id, src=s1.buff, len=s1len,
                 dst_off=s0len);
    ret.buff[ret.buffLen] = 0;
    return ret;
  }

  inline proc doEq(a: ?t1, b: ?t2) {
    assertArgType(t1, "doEq");
    assertArgType(t2, "doEq");

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
    return _strcmp(a.buff, a.buffLen, a.locale_id, b.buff, b.buffLen, b.locale_id) == 0;
  }

  inline proc doLessThan(a: ?t1, b: ?t2) {
    assertArgType(t1, "doEq");
    assertArgType(t2, "doEq");

    return _strcmp(a.buff, a.buffLen, a.locale_id, b.buff, b.buffLen, b.locale_id) < 0;
  }

  inline proc doGreaterThan(a: ?t1, b: ?t2) {
    assertArgType(t1, "doEq");
    assertArgType(t2, "doEq");

    return _strcmp(a.buff, a.buffLen, a.locale_id, b.buff, b.buffLen, b.locale_id) > 0;
  }

  inline proc doLessThanOrEq(a: ?t1, b: ?t2) {
    assertArgType(t1, "doEq");
    assertArgType(t2, "doEq");

    return _strcmp(a.buff, a.buffLen, a.locale_id, b.buff, b.buffLen, b.locale_id) <= 0;
  }

  inline proc doGreaterThanOrEq(a: ?t1, b: ?t2) {
    assertArgType(t1, "doEq");
    assertArgType(t2, "doEq");

    return _strcmp(a.buff, a.buffLen, a.locale_id, b.buff, b.buffLen, b.locale_id) >= 0;
  }

  inline proc getHash(x: ?t) {
    assertArgType(t, "getHash");

    var hash: int(64);
    on __primitive("chpl_on_locale_num",
                   chpl_buildLocaleID(x.locale_id, c_sublocid_any)) {
      // Use djb2 (Dan Bernstein in comp.lang.c), XOR version
      var locHash: int(64) = 5381;
      for c in 0..#(x.numBytes) {
        locHash = ((locHash << 5) + locHash) ^ x.buff[c];
      }
      hash = locHash;
    }
    return hash:uint;
  }
}
