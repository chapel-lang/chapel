/*
 * Copyright 2020-2021 Hewlett Packard Enterprise Development LP
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
  private use CPtr;
  private use ByteBufferHelpers;
  private use String.NVStringFactory;

  /*
     ``decodePolicy`` specifies what happens when there is malformed characters
     when decoding a :mod:`Bytes` into a UTF-8 :record:`~String.string`.
       
       - **strict**: default policy; raise error
       - **replace**: replace with UTF-8 replacement character
       - **drop**: silently drop data
       - **escape**: escape invalid data by replacing each byte 0xXX with
                     codepoint 0xDCXX
  */
  enum decodePolicy { strict, replace, drop, escape }

  /*
     ``encodePolicy`` specifies what happens when there is escaped non-UTF8
     bytes when encoding a :record:`~String.string` into a
     :mod:`Bytes`.
       
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
    if _local == false && x.locale_id != chpl_nodeID then
      halt("Cannot call .c_str() on a remote " + t:string);

    var buff: bufferType = x.buff;
    var asCString = __primitive("cast", c_string, buff);
    return asCString;
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
    use SysBasic;
    pragma "fn synchronization free"
    extern proc qio_encode_char_buf(dst: c_void_ptr, chr: int(32)): syserr;
    pragma "fn synchronization free"
    extern proc qio_nbytes_char(chr: int(32)): c_int;

    if length == 0 then return "";

    // allocate buffer the same size as this buffer assuming that the string
    // is in fact perfectly decodable. In the worst case, the user wants the
    // replacement policy and we grow the buffer couple of times.
    // The alternative is to allocate more space from the beginning.
    var (newBuff, allocSize) = bufferAlloc(length+1);
    var hasEscapes = false;
    var numCodepoints = 0;

    var expectedSize = allocSize;

    var thisIdx = 0;
    var decodedIdx = 0;
    while thisIdx < length {
      const (decodeRet, cp, nBytes) = decodeHelp(buff, length, 
                                                 thisIdx, 
                                                 allowEsc=false);
      var buffToDecode = buff + thisIdx;

      numCodepoints += 1;

      if decodeRet != 0 {  //decoder returns error
        if policy == decodePolicy.strict {
          bufferFree(newBuff);
          throw new owned DecodeError();
        }
        else {
          // if nBytes is 1, then we must have read a single byte and found
          // that it was invalid, if nBytes is >1 then we must have read
          // multiple bytes where the last one broke the sequence. But it can
          // be a valid byte itself. So we rewind by 1 in that case
          // we use nInvalidBytes to store how many bytes we are ignoring or
          // replacing
          const nInvalidBytes = if nBytes==1 then nBytes else nBytes-1;
          thisIdx += nInvalidBytes;

          if policy == decodePolicy.replace {
            param replChar: int(32) = 0xfffd;

            // Replacement can cause the string to be larger than initially
            // expected. The Unicode replacement character has codepoint
            // 0xfffd. It is encoded in `encodedReplChar` and its encoded
            // length is `nBytesRepl`, which is 3 bytes in UTF8. If it is used
            // in place of a single byte, we may overflow
            expectedSize += 3-nInvalidBytes;
            (newBuff, allocSize) = bufferEnsureSize(newBuff, allocSize,
                                                    expectedSize);

            qio_encode_char_buf(newBuff+decodedIdx, replChar);

            decodedIdx += 3;  // replacement character is 3 bytes in UTF8
          }
          else if policy == decodePolicy.escape {
              
            hasEscapes = true;

            // encoded escape sequence is 3 bytes. And this is per invalid byte
            expectedSize += 2*nInvalidBytes;
            (newBuff, allocSize) = bufferEnsureSize(newBuff, allocSize,
                                                     expectedSize);
            for i in 0..#nInvalidBytes {
              qio_encode_char_buf(newBuff+decodedIdx,
                                  0xdc00+buff[thisIdx-nInvalidBytes+i]);
              decodedIdx += 3;
            }

            // adjust number of codepoints: for each invalid byte in this
            // sequence we add one codepoint. However, we already incremented
            // by one in the beginning of the loop:
            numCodepoints += nInvalidBytes-1;

          }
          // if policy == decodePolicy.ignore, we don't do anything and skip over
          // the invalid sequence
        }
      }
      else {  // we got valid characters
        // do a naive copy
        bufferMemcpyLocal(dst=newBuff, src=buffToDecode, len=nBytes,
                          dst_off=decodedIdx);
        thisIdx += nBytes;
        decodedIdx += nBytes;
      }
    }

    newBuff[decodedIdx] = 0;
    var ret = chpl_createStringWithOwnedBufferNV(x=newBuff,
                                                 length=decodedIdx,
                                                 size=allocSize,
                                                 numCodepoints=numCodepoints);
    ret.hasEscapes = hasEscapes;
    return ret;
  }

  /*
    This function decodeHelp is used to create a wrapper for 
    qio_decode_char_buf* and qio_decode_char_buf_esc and return 
    the value of syserr , cp and nBytes.
      
      :arg buff: Buffer to decode 
      
      :arg buffLen: Size of buffer
      
      :arg offset: Starting index of read buffer,
      
      :arg allowEsc:  Choice between "qio_decode_char_buf" 
                      and "qio_decode_char_buf_esc" that allows 
                      escaped sequences in the string
    
    :returns: Tuple of decodeRet, chr and nBytes
              decodeRet : error code : syserr
              chr : corresponds to codepoint 
              nBytes : number of bytes of corresponding UTF-8 encoding
   */
  proc decodeHelp(buff:c_ptr(uint(8)), buffLen:int, 
                  offset:int, allowEsc: bool ) {
    use SysBasic;
    pragma "fn synchronization free"
    extern proc qio_decode_char_buf(ref chr:int(32), 
                                    ref nBytes:c_int,
                                    buf:c_string,
                                    buflen:ssize_t): syserr;
    pragma "fn synchronization free"
    extern proc qio_decode_char_buf_esc(ref chr:int(32),
                                        ref nBytes:c_int,
                                        buf:c_string,
                                        buffLen:ssize_t): syserr;
    // esc chooses between qio_decode_char_buf_esc and
    // qio_decode_char_buf as a single wrapper function 
    var chr: int(32);
    var nBytes: c_int;
    var start = offset:c_int;
    var multibytes = (buff + start): c_string;
    var maxbytes = (buffLen - start): ssize_t;
    var decodeRet: syserr;
    if(allowEsc) then
      decodeRet = qio_decode_char_buf_esc(chr, nBytes, 
                                          multibytes,
                                          maxbytes);
    else
      decodeRet = qio_decode_char_buf(chr, nBytes,
                                      multibytes, 
                                      maxbytes);

    return (decodeRet, chr, nBytes);
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
        if t == string then x.cachedNumCodepoints = other.cachedNumCodepoints;
      }
      else {
        // if other is local just adjust my buff and _size
        x.buff = other.buff;
        x.buffSize = other.buffSize;
        if t == string then x.cachedNumCodepoints = other.cachedNumCodepoints;
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

  inline proc initWithOwnedBuffer(ref x: ?t, other: bufferType, length:int, size:int) {
    assertArgType(t, "initWithOwnedBuffer");

    x.isOwned = true;

    // here, we don't need to do anything special if length==0, the buffer may
    // be allocated but empty
    x.buff = other;
    x.buffSize = size;
    x.buffLen = length;
  }

  inline proc initWithNewBuffer(ref x: ?t, other: t) {
    assertArgType(t, "initWithNewBuffer");

    const otherRemote = other.locale_id != chpl_nodeID;
    const otherLen = other.numBytes;
    x.isOwned = true;
    if t == string then x.hasEscapes = other.hasEscapes;
    if t == string then x.cachedNumCodepoints = other.cachedNumCodepoints;

    if otherLen > 0 {
      x.buffLen = otherLen;
      if !_local && otherRemote {
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

  // Checks to see if r is inside the bounds of this and returns a finite
  // range that can be used to iterate over a section of the string
  //
  // This function handles ranges of codepointIndex, byteIndex or numeric types.
  // codepointIndex only makes sense for string, whereas the others can be used
  // with both bytes and string
  //
  // If codepointIndex range was given, converts that to byte index range in the
  // process.
  proc getView(const ref x: ?t, r: range(?)) {
    assertArgType(t, "getView");
    if t == bytes && r.idxType == codepointIndex {
      compilerError("codepointIndex ranges cannot be used with bytes in getView");
    }

    proc simpleCaseHelper() {
      // cast the argument r to `int` to make sure that we are not dealing with
      // byteIndex
      const intR = r:range(int, r.boundedType, r.stridable);
      if boundsChecking {
        if !x.byteIndices.boundsCheck(intR) {
          halt("range ", r, " out of bounds for " + t:string + " with length ",
               x.numBytes);
        }
      }
      if r.idxType == byteIndex {
        return (intR[x.byteIndices], -1);  // -1; I can't know numCodepoints
      }
      else {
        const retRange = intR[x.byteIndices];
        return (retRange, retRange.size); // it maybe ascii string or bytes
      }
    }

    if t == bytes || r.idxType == byteIndex {
      return simpleCaseHelper();
    }
    else if t == string && x.isASCII() {
      return simpleCaseHelper();
    }
    else {  // string with codepoint indexing
      if r.stridable {
        // Slicing by stridable codepoint ranges is unsupported because it
        // creates an irregular sequence of bytes.  We could add support in the
        // future by refactoring the callers of _getView() to add a slow path,
        // or by storing an array of indices marking the beginning of each
        // codepoint alongside the string.
        compilerError("string slicing doesn't support stridable codepoint ranges");
      }

      if r == x.indices then return (x.byteIndices, x.cachedNumCodepoints);

      // cast the argument r to `int` to make sure that we are not dealing with
      // codepointIdx
      const intR = r:range(int, r.boundedType, r.stridable);
      if boundsChecking {
        if !x.indices.boundsCheck(intR) {
          halt("range ", r, " out of bounds for string with length ", x.size);
        }
      }

      // find the byte range of the given codepoint range
      var cpCount = 0;
      const cpIdxLow = if intR.hasLowBound() && intR.alignedLow:int >= 0
                          then intR.alignedLow:int
                          else 0;
      const cpIdxHigh = if intR.hasHighBound()
                           then intR.alignedHigh:int
                           else x.buffLen-1;

      var byteLow = x.buffLen;  // empty range if bounds outside string
      var byteHigh = x.buffLen - 1;

      if cpIdxHigh >= 0 {
        for (i, nBytes) in x._indexLen() {
          if cpCount == cpIdxLow {
            byteLow = i:int;
            if !r.hasHighBound() then
              break;
          }
          if cpCount == cpIdxHigh {
            byteHigh = i:int + nBytes-1;
            break;
          }
          cpCount += 1;
        }
      }
      return (byteLow..byteHigh, cpIdxHigh-cpIdxLow+1);
    }
  }

  // TODO: I wasn't very good about caching variables locally in this one.
  proc getSlice(const ref x: ?t, r: range(?)) {
    assertArgType(t, "getSlice");

    if x.isEmpty() {
      var ret: t;
      return ret;
    }

    const (r2, numChars) = getView(x, r);
    if r2.size <= 0 {
      var ret: t;
      return ret;
    }

    var buff: bufferType;
    var buffSize: int;

    // In case r1 is strided, we cannot just use r2.size for the copy
    // length. For to be able to cover strided copies, we copy the range
    // from low to high then do a strided operation to put the data in the
    // buffer in the correct order.
    const copyLen = r2.high-r2.low+1;
    var (copyBuf, copySize) = bufferCopy(buf=x.buff, off=r2.low,
                                        len=copyLen, loc=x.locale_id);
    if r2.stride == 1 {
      buff = copyBuf;
      buffSize = copySize;
    }
    else {
      // the range is strided
      var (newBuff, allocSize) = bufferAlloc(r2.size+1);
      for (r2_i, i) in zip(r2, 0..) {
        newBuff[i] = copyBuf[r2_i-r2.low];
      }
      buff = newBuff;
      buffSize = allocSize;
      bufferFree(copyBuf);
    }
    const buffLen = r2.size;
    buff[buffLen] = 0;

    if t == string {
      var numCodepoints = numChars;
      if numCodepoints == -1 {
        numCodepoints = countNumCodepoints(buff, buffLen);
      }
      return chpl_createStringWithOwnedBufferNV(x=buff, length=buffLen,
          size=buffSize, numCodepoints=numCodepoints);
    }
    else {
      return createBytesWithOwnedBuffer(x=buff, length=buffLen, size=buffSize);
    }
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

  proc doSplitHelp(const ref localx: ?t, const ref localSep: t,
                   const maxsplit: int = -1, const ignoreEmpty: bool = false,
                   ref start: getIndexType(t), const splitCount: int): t {

    type _idxt = getIndexType(t);

    // really should be <, but we need to avoid returns and extra yields so
    // the iterator gets inlined
    var splitAll: bool = maxsplit <= 0;

    var chunk: t;
    var end: _idxt = -1;
    var done = false;

    if (maxsplit == 0) {
      chunk = localx;
      done = true;
    } else {
      if (splitAll || splitCount < maxsplit) then
        end = localx.find(localSep, start..);

      if(end == -1) {
        // Separator not found
        chunk = localx[start..];
        done = true;
      } else {
        chunk = localx[start..end-1];
      }
    }

    if done {
      start = localx.numBytes+1;
    }
    else {
      start = end+localSep.numBytes;
    }
    return chunk;
  }

  pragma "not order independent yielding loops"
  iter doSplit(const ref x: ?t, sep: t, maxsplit: int = -1,
               ignoreEmpty: bool = false): t {
    assertArgType(t, "doSplit");

    type _idxt = getIndexType(t);

    if !(maxsplit == 0 && ignoreEmpty && x.isEmpty()) {
      const localx: t = x.localize();
      const localSep: t = sep.localize();

      var splitCount: int = 0;

      // this is passed as a ref to the helper
      var start: _idxt = 0;

      while start <= localx.numBytes  {
        const chunk = doSplitHelp(localx, localSep, maxsplit, ignoreEmpty, start, splitCount);
        if !(ignoreEmpty && chunk.isEmpty()) {
          yield chunk;
          splitCount += 1;
        }
      }
    }
  }

  proc doSplitWSNoEncHelp(const ref localx: ?t, maxsplit: int = -1,
                          ref i: int, const splitCount: int,
                          const noSplits: bool, const limitSplits: bool,
                          const iEnd: byteIndex): t {
    var done : bool = false;
    var yieldChunk : bool = false;
    var chunk : t;

    var inChunk : bool = false;
    var chunkStart : idxType;

    // emit whole string, unless all whitespace
    // TODO Engin: Why is noSplit check inside the loop?
    while i < localx.size {
      var c = localx.byte(i);
      if noSplits {
        done = true;
        if !localx.isSpace() then {
          chunk = localx;
          yieldChunk = true;
        }
      } else {
        var cSpace = byte_isWhitespace(c);
        // first char of a chunk
        if !(inChunk || cSpace) {
          chunkStart = i;
          inChunk = true;
          if i > iEnd {
            chunk = localx[chunkStart..];
            yieldChunk = true;
            done = true;
          }
        } else if inChunk {
          // first char out of a chunk
          if cSpace {
            // last split under limit
            if limitSplits && splitCount >= maxsplit {
              chunk = localx[chunkStart..];
              yieldChunk = true;
              done = true;
            // no limit
            } else {
              chunk = localx[chunkStart..i-1];
              yieldChunk = true;
              inChunk = false;
            }
          // out of chars
          } else if i > iEnd {
            chunk = localx[chunkStart..];
            yieldChunk = true;
            done = true;
          }
        }
      }

      if done {
        i = localx.size;
      }
      if yieldChunk {
        return chunk;
      }
      else {
        i += 1;
      }
    }

    return "";
  }

  // split iterator over whitespace
  pragma "not order independent yielding loops"
  iter doSplitWSNoEnc(const ref x: ?t, maxsplit: int = -1): t {
    assertArgType(t, "doSplitWSNoEnc");

    if !x.isEmpty() {
      const localx: t = x.localize();
      var splitCount: int = 0;

      // this is passed to the helper as a ref
      var i = 0;

      while i < localx.numBytes {
        const chunk = doSplitWSNoEncHelp(localx, maxsplit, i, splitCount,
                                         noSplits=(maxsplit==0),
                                         limitSplits=(maxsplit>0),
                                         iEnd=(localx.buffLen-2):byteIndex);
        if !chunk.isEmpty() {
          yield chunk;
          splitCount += 1;
        }
      }
    }
  }

  // Helper function that uses a param bool to toggle between count and find
  //TODO: this could be a much better string search
  //      (Boyer-Moore-Horspool|any thing other than brute force)
  proc doSearchNoEnc(const ref x: ?t, needle: t, region: range(?),
                     param count: bool, param fromLeft: bool = true) {
    assertArgType(t, "doSearch");

    // needle.buffLen is <= than x.buffLen, so go to the home locale
    var ret: int = -1;
    on __primitive("chpl_on_locale_num",
                   chpl_buildLocaleID(x.locale_id, c_sublocid_any)) {
      // any value >= 0 means we have a solution
      // used because we cant break out of an on-clause early
      var localRet: int = -2;
      const nLen = needle.buffLen;
      const (view, _) = getView(x, region);
      const xLen = view.size;

      // Edge cases
      if count {
        if nLen == 0 { // Empty needle
          localRet = view.size;
        }
      } else { // find
        if nLen == 0 { // Empty needle
          if fromLeft {
            localRet = -1;
          } else {
            localRet = if xLen == 0
              then -1
              else xLen;
          }
        }
      }

      if nLen > xLen {
        localRet = -1;
      }

      if localRet == -2 {
        localRet = -1;
        const localNeedle = needle.localize();
        const needleLen = localNeedle.buffLen;

        // i *is not* an index into anything, it is the order of the element
        // of view we are searching from.
        const numPossible = xLen - nLen + 1;
        const searchSpace = if fromLeft
            then 0..#(numPossible)
            else 0..#(numPossible) by -1;
        for i in searchSpace {
          // j *is* the index into the localNeedle's buffer
          for j in 0..#nLen {
            const idx = view.orderToIndex(i+j); // 0s based idx
            if x.buff[idx] != localNeedle.buff[j] then break;

            if j == nLen-1 {
              if count {
                localRet += 1;
              } else { // find
                localRet = view.orderToIndex(i);
              }
            }
          }
          if !count && localRet != -1 then break;
        }
      }
      if count then localRet += 1;
      ret = localRet;
    }
    return ret;
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
      return "":t;
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

      var numCodepoints = 0;

      if joinedSize == 0 then
        return "":t;

      var (newBuff, allocSize) = bufferAlloc(joinedSize+1);

      var first = true;
      var offset = 0;
      for s in S {
        const sLen = s.buffLen;

        // copy x's contents
        if first {
          first = false;
        } else if x.buffLen != 0 {
          bufferMemcpyLocal(dst=newBuff, src=x.buff, len=x.buffLen,
                            dst_off=offset);
          offset += x.buffLen;
          if t == string then numCodepoints += x.numCodepoints;
        }

        // copy s's contents
        if sLen != 0 {
          bufferMemcpy(dst=newBuff, dst_off=offset,
                       src_loc=s.locale_id, src=s.buff,len=sLen);
          offset += sLen;
          if t == string then numCodepoints += s.numCodepoints;
        }
      }
      newBuff[joinedSize] = 0;
      if t == string {
        return chpl_createStringWithOwnedBufferNV(x=newBuff,
                                                  length=joinedSize,
                                                  size=allocSize,
                                                  numCodepoints=numCodepoints);
      }
      else {
        return createBytesWithOwnedBuffer(x=newBuff,
                                          length=joinedSize,
                                          size=allocSize);
      }
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

  proc doDedent(const ref x: ?t, columns=0, ignoreFirst=true): t {
      const low = if ignoreFirst then 1 else 0;
      const newline = '\n':t;
      var lines = x.split(newline);
      var ret = '': t;

      if columns <= 0 {
        // Find common leading whitespace across all non-empty lines
        const margin = computeMargin(lines[low..]);

        // Remove margins from all lines if it's not empty
        if margin.size > 0 {
          for line in lines[low..] {
            // Compute offset
            var offset = 0;
            if !isDedentWhitespaceOnly(line) {
              offset = margin.size;
            } else {
              // Remove margin as long as it matches for empty lines
              for i in 0..<min(margin.size, line.size) {
                if line[i] != margin[i] then break;
                offset += 1;
              }
            }
            // Remove margin from line
            line = line[offset..];
          }
        }
      } else {
        // Remove up to `columns` number of spaces from each line
        for line in lines[low..] {
          // Note: We only consider spaces (not tabs) for columns > 0
          const indent = line.size - line.strip(' ':t, trailing=false).size;
          const offset = min(indent, columns);
          line = line[offset..];
        }
      }

      ret = ('\n':t).join(lines);
      return ret;
    }

    /* Compute margin of common leading white space across lines in a string.
       Spaces and tabs are respected.
     */
    private proc computeMargin(lines: [] ?t): t{
      var margin = '': t;

      for line in lines {

        // Skip empty lines
        if isDedentWhitespaceOnly(line) {
          continue;
        }

        // Determine leading whitespace (spaces and tabs) in line
        var curMargin = '':t;
        const space = ' ':t;
        const tab = '\t':t;
        for char in line.items() {
          if char != space && char != tab then break;
          else curMargin += char:t;
        }

        if curMargin == '':t {
          // An unindented non-empty line means no margin exists, return early
          margin = '';
          break;
        } else if margin == '':t {
          // Initialize margin
          margin = curMargin;
        } else if curMargin.startsWith(margin) {
          // Current indent is deeper than margin, continue
          continue;
        } else if margin.startsWith(curMargin) {
          // Current indent is shallower than margin, update margin
          margin = curMargin;
        } else {
          // Find largest common whitespace between current and previous margin
          for i in margin.indices {
            if margin[i] != curMargin[i] {
              margin = margin[..<i];
              break;
            }
          }
        }
      }
      return margin;
    }

    /* Return true if string only contains spaces and tabs */
    private proc isDedentWhitespaceOnly(s: ?t): bool {
      const space = ' ':t;
      const tab = '\t':t;
      for char in s.items() {
        if char != space && char != tab then
          return false;
      }
      return true;
    }



  proc doAppend(ref lhs: ?t, const ref rhs: t) {
    assertArgType(t, "doAppend");

    // if rhs is empty, nothing to do
    if rhs.buffLen == 0 then return;

    on __primitive("chpl_on_locale_num",
                   chpl_buildLocaleID(lhs.locale_id, c_sublocid_any)) {
      if !safeAdd(lhs.buffLen,rhs.buffLen) then 
        halt("Buffer overflow allocating string copy data");
      const newLength = lhs.buffLen + rhs.buffLen;
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
      bufferMemcpy(dst=lhs.buff, src_loc=rhs.locale_id, rhs.buff, rhs.buffLen,
                   dst_off=lhs.buffLen);
      lhs.buffLen = newLength;
      lhs.buff[newLength] = 0;
      if t == string then lhs.cachedNumCodepoints += rhs.cachedNumCodepoints;
    }
  }

  proc reinitWithNewBuffer(ref lhs: ?t, buff: bufferType, buffLen: int,
                           buffSize: int, numCodepoints: int = 0) {
      if lhs.isEmpty() && buff == nil then return;

      // If the lhs.buff is longer than buff, then reuse the buffer if we are
      // allowed to (lhs.isOwned == true)
      if buffLen != 0 {
        if !lhs.isOwned || buffLen+1 > lhs.buffSize {
          // If the new string is too big for our current buffer or we dont
          // own our current buffer then we need a new one.
          if lhs.isOwned && !lhs.isEmpty() then
            bufferFree(lhs.buff);
          // TODO: should I just allocate 'size' bytes?
          const (buff, allocSize) = bufferAlloc(buffLen+1);
          lhs.buff = buff;
          lhs.buffSize = allocSize;
          // We just allocated a buffer, make sure to free it later
          lhs.isOwned = true;
        }
        bufferMemmoveLocal(lhs.buff, buff, buffLen);
        lhs.buff[buffLen] = 0;
      } else {
        // If buffLen is 0, 'buf' may still have been allocated. Regardless, we
        // need to free the old buffer if 'lhs' is isOwned.
        if lhs.isOwned && !lhs.isEmpty() then bufferFree(lhs.buff);
        lhs.buffSize = 0;

        // If we need to copy, we can just set 'buff' to nil. Otherwise the
        // implication is that the string takes ownership of the given buffer,
        // so we need to store it and free it later.
        lhs.buff = nil;
      }

      lhs.buffLen = buffLen;
      if t==string then lhs.cachedNumCodepoints = numCodepoints;
  }

  proc reinitWithOwnedBuffer(ref lhs: ?t, buff: bufferType, buffLen: int,
                             buffSize: int, numCodepoints: int = 0) {

      if lhs.isEmpty() && buff == nil then return;

      // If the lhs.buff is longer than buff, then reuse the buffer if we are
      // allowed to (lhs.isOwned == true)
      if buffLen != 0 {
        if lhs.isOwned && !lhs.isEmpty() then
          bufferFree(lhs.buff);
        lhs.buff = buff;
        lhs.buffSize = buffSize;
      } else {
        // If buffLen is 0, 'buf' may still have been allocated. Regardless, we
        // need to free the old buffer if 'lhs' is isOwned.
        if lhs.isOwned && !lhs.isEmpty() then bufferFree(lhs.buff);
        lhs.buff = buff;
        lhs.buffSize = 0;
      }

      lhs.isOwned = true;
      lhs.buffLen = buffLen;
      if t==string then lhs.cachedNumCodepoints = numCodepoints;
  }

  proc doAssign(ref lhs: ?t, rhs: t) {
    assertArgType(t, "doAssign");

    inline proc helpMe(ref lhs: t, rhs: t) {
      if _local || rhs.locale_id == chpl_nodeID {
        if t == string {
          reinitWithNewBuffer(lhs, rhs.buff, rhs.buffLen, rhs.buffSize,
                              rhs.numCodepoints);
        }
        else {
          reinitWithNewBuffer(lhs, rhs.buff, rhs.buffLen, rhs.buffSize);
        }
      } else {
        const len = rhs.buffLen;
        var remote_buf:bufferType = nil;
        if len != 0 then
          remote_buf = bufferCopyRemote(rhs.locale_id, rhs.buff, len);
        if t==string {
          reinitWithOwnedBuffer(lhs, remote_buf, len, len+1,
                                rhs.cachedNumCodepoints);
        }
        else {
          reinitWithOwnedBuffer(lhs, remote_buf, len, len+1);
        }
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

    if !safeMul(sLen, n) then 
      halt("Buffer overflow allocating string copy data");

    const buffLen = sLen * n;
    var (buff, allocSize) = bufferAlloc(buffLen+1);

    bufferMemcpy(dst=buff, src_loc=x.locale_id, src=x.buff, len=x.buffLen);
    var offset = sLen;
    for i in 1..(n-1) {
      bufferMemcpyLocal(dst=buff, src=buff, len=x.buffLen,
                        dst_off=offset);
      offset += sLen;
    }
    buff[buffLen] = 0;

    if t==string {
      return chpl_createStringWithOwnedBufferNV(buff, buffLen, allocSize,
                                                x.cachedNumCodepoints*n);
    }
    else {
      return createBytesWithOwnedBuffer(buff, buffLen, allocSize);
    }
  }

  proc doConcat(s0: ?t, s1: t): t {
    assertArgType(t, "doConcat");

    // cache lengths locally
    const s0len = s0.buffLen;
    if s0len == 0 then return s1:t;
    const s1len = s1.buffLen;
    if s1len == 0 then return s0;

    const buffLen = s0len + s1len;
    var (buff, buffSize) = bufferAlloc(buffLen+1);

    bufferMemcpy(dst=buff, src_loc=s0.locale_id, src=s0.buff, len=s0len);
    bufferMemcpy(dst=buff, src_loc=s1.locale_id, src=s1.buff, len=s1len,
                 dst_off=s0len);
    buff[buffLen] = 0;

    var ret: t;
    if t == string {
      ret.cachedNumCodepoints = s0.cachedNumCodepoints + s1.cachedNumCodepoints;
    }
    initWithOwnedBuffer(ret, buff, buffLen, buffSize);
    return ret;
  }

  proc doStripNoEnc(const ref x: ?t, chars: t, leading:bool, trailing:bool) : t {
    if x.isEmpty() then return if t==string then "" else b"";

    if chars.isEmpty() then return x;

    const localX: t = x.localize();
    const localChars: t = chars.localize();

    var start: idxType = 0;
    var end: idxType = localX.buffLen-1;

    if leading {
      label outer for (i, xChar) in zip(x.indices, localX.bytes()) {
        for removeChar in localChars.bytes() {
          if xChar == removeChar {
            start = i + 1;
            continue outer;
          }
        }
        break;
      }
    }

    if trailing {
      // Because we are working with codepoints whose starting byte index
      // is not initially known, it is faster to work forward, assuming we
      // are already past the end of the string, and then update the end
      // point as we are proven wrong.
      end = -1;
      label outer for (i, xChar) in zip(x.indices, localX.bytes()) {
        for removeChar in localChars.bytes() {
          if xChar == removeChar {
            continue outer;
          }
        }
        // This was not a character to be removed, so update tentative end.
        end = i;
      }
    }

    return localX[start..end];
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

  private proc incrementCodepoints(ref lhs: string, rhs: string) {
    if(lhs.cachedNumCodepoints == -1 || rhs.cachedNumCodepoints == -1) {
      lhs.cachedNumCodepoints = -1;
    } else {
      lhs.cachedNumCodepoints = lhs.cachedNumCodepoints + rhs.cachedNumCodepoints;
    }
  }

  proc countNumCodepoints(x: string) {
    var ret: int;
    on __primitive("chpl_on_locale_num",
                   chpl_buildLocaleID(x.locale_id, c_sublocid_any)) {
      ret = countNumCodepoints(x.buff, x.buffLen);
    }
    return ret;
  }

  proc countNumCodepoints(buff: bufferType, buffLen: int) {
    var n = 0;
    var i = 0;
    while i < buffLen {
      i += 1;
      while i < buffLen && !isInitialByte(buff[i]) do
        i += 1;
      n += 1;
    }
    return n;
  }

  /*
    Returns true if the argument is a valid initial byte of a UTF-8
    encoded multibyte character.
  */
  pragma "no doc"
  inline proc isInitialByte(b: uint(8)) : bool {
    return (b & 0xc0) != 0x80;
  }

  /* 
   Returns the byte index of the beginning of the first codepoint starting from
   (and including) i
   */
  proc _findStartOfNextCodepointFromByte(x: string, i: byteIndex) {
    var ret = i:int;
    if ret > 0 {
      while ret < x.buffLen && !isInitialByte(x.buff[ret]) {
        ret += 1;
      }
    }
    return ret;
  }

  // cast helpers
  proc _cleanupForNumericCast(ref x: ?t) {
    assertArgType(t, "_cleanupForNumericCast");

    param underscore = "_".toByte();

    var hasUnderscores = false;
    for bIdx in 1..<x.numBytes {
      if x.byte[bIdx] == underscore then {
        hasUnderscores = true;
        break;
      }
    }

    if hasUnderscores {
      x = x.strip();
      // don't remove anything and let it fail later on
      if _isSingleWord(x) {
        var len = x.size;
        if len >= 2 {
          // Don't remove a leading underscore in the string number,
          // but remove the rest.
          if len > 2 && x.byte(0) == underscore {
            x = x.item(0) + x[1..].replace("_":t, "":t);
          } else {
            x = x.replace("_":t, "":t);
          }
        }
      }
    }
  }

  private proc _isSingleWord(const ref x: ?t) {
    assertArgType(t, "_isSingleWord");
    // here we assume that the string is all ASCII, if not, we'll get an error
    // from the actual conversion function, anyways
    for b in x.bytes() {
      if byte_isWhitespace(b) then return false;
    }
    return true;
  }

  // character-wise operation helpers

  require "ctype.h";

  inline proc byte_isAscii(c: byteType): bool {
    pragma "fn synchronization free"
    extern proc isascii(c: c_int): c_int;
    return isascii(c: c_int) != 0;
  }

  inline proc byte_isWhitespace(c: byteType): bool {
    pragma "fn synchronization free"
    extern proc isspace(c: c_int): c_int;
    return isspace(c: c_int) != 0;
  }

  inline proc byte_isPrintable(c: byteType): bool {
    pragma "fn synchronization free"
    extern proc isprint(c: c_int): c_int;
    return isprint(c: c_int) != 0;
  }

  inline proc byte_isAlpha(c: byteType): bool {
    pragma "fn synchronization free"
    extern proc isalpha(c: c_int): c_int;
    return isalpha(c: c_int) != 0;
  }

  inline proc byte_isUpper(c: byteType): bool {
    pragma "fn synchronization free"
    extern proc isupper(c: c_int): c_int;
    return isupper(c: c_int) != 0;
  }

  inline proc byte_isLower(c: byteType): bool {
    pragma "fn synchronization free"
    extern proc islower(c: c_int): c_int;
    return islower(c: c_int) != 0;
  }

  inline proc byte_isDigit(c: byteType): bool {
    pragma "fn synchronization free"
    extern proc isdigit(c: c_int): c_int;
    return isdigit(c: c_int) != 0;
  }

  inline proc byte_isAlnum(c: byteType): bool {
    pragma "fn synchronization free"
    extern proc isalnum(c: c_int): c_int;
    return isalnum(c: c_int) != 0;
  }

  inline proc byte_toUpper(c: byteType): byteType {
    pragma "fn synchronization free"
    extern proc toupper(c: c_int): c_int;
    return toupper(c: c_int):byteType;
  }

  inline proc byte_toLower(c: byteType): byteType {
    pragma "fn synchronization free"
    extern proc tolower(c: c_int): c_int;
    return tolower(c: c_int):byteType;
  }
}
