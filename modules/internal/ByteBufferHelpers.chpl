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

module ByteBufferHelpers {
  private use ChapelStandard;
  private use SysCTypes;

  pragma "no doc"
  type byteType = uint(8);
  pragma "no doc"
  type bufferType = c_ptr(uint(8));
  pragma "no doc"
  type locIdType = chpl_nodeID.type;

  // Growth factor to use when extending the buffer for appends
  config param chpl_stringGrowthFactor = 1.5;

  //
  // Externs and constants used to implement strings
  //
  pragma "fn synchronization free"
  private extern proc chpl_memhook_md_num(): chpl_mem_descInt_t;

  // Calls to chpl_here_alloc increment the memory descriptor by
  // `chpl_memhook_md_num`. For internal runtime descriptors like the ones
  // below, this would result in selecting the incorrect descriptor string.
  //
  // Instead, decrement the CHPL_RT_MD* descriptor and use the result when
  // calling chpl_here_alloc.
  private proc offset_STR_COPY_DATA {
    extern const CHPL_RT_MD_STR_COPY_DATA: chpl_mem_descInt_t;
    return CHPL_RT_MD_STR_COPY_DATA - chpl_memhook_md_num();
  }
  private proc offset_STR_COPY_REMOTE {
    extern const CHPL_RT_MD_STR_COPY_REMOTE: chpl_mem_descInt_t;
    return CHPL_RT_MD_STR_COPY_REMOTE - chpl_memhook_md_num();
  }

  inline proc chpl_string_comm_get(dest: bufferType, src_loc_id: int(64),
                                   src_addr: bufferType, len: integral) {
    __primitive("chpl_comm_get", dest, src_loc_id, src_addr, len.safeCast(size_t));
  }

  inline proc bufferAlloc(requestedSize) {
    const allocSize = max(chpl_here_good_alloc_size(requestedSize),
                          chpl_stringMinAllocSize);
    var buf = chpl_here_alloc(allocSize,
                              offset_STR_COPY_DATA): bufferType;
    return (buf, allocSize);
  }

  proc bufferAllocExact(requestedSize) {
    var buf = chpl_here_alloc(requestedSize,
                              offset_STR_COPY_DATA): bufferType;
    return buf;
  }

  proc bufferRealloc(buf, requestedSize) {
    const allocSize = max(chpl_here_good_alloc_size(requestedSize+1),
                          chpl_stringMinAllocSize);
    var newBuff = chpl_here_realloc(buf, allocSize,
                                offset_STR_COPY_DATA): bufferType;
    return (newBuff, allocSize);
  }

  proc bufferEnsureSize(buf, currentSize, requestedSize) {
    if currentSize < requestedSize then
      return bufferRealloc(buf, requestedSize);
    else
      return (buf, currentSize);
  }

  proc bufferCopyRemote(src_loc_id: int(64), src_addr: bufferType,
                        len: int): bufferType {
      const dest = chpl_here_alloc(len+1, offset_STR_COPY_REMOTE): bufferType;
      chpl_string_comm_get(dest, src_loc_id, src_addr, len);
      dest[len] = 0;
      return dest;
  }

  inline proc bufferCopyLocal(src_addr: bufferType, len: int) {
      const (dst, allocSize) = bufferAlloc(len+1);
      bufferMemcpyLocal(dst=dst, src=src_addr, len=len);
      return (dst, allocSize);
  }

  inline proc bufferFree(buf) {
    chpl_here_free(buf);
  }

  inline proc bufferCopy(buf: bufferType, off: int, len: int, loc: locIdType) {
    if !_local && loc != chpl_nodeID {
      var newBuf = bufferCopyRemote(loc, buf+off, len);
      return (newBuf, len);
    }
    else {
      return bufferCopyLocal(buf+off, len);
    }
  }

  //dst must be local
  inline proc bufferMemcpy(dst, src_loc, src, len, dst_off=0, src_off=0) {
    if !_local && src_loc != chpl_nodeID {
      chpl_string_comm_get(dst+dst_off, src_loc, src+src_off, len);
    }
    else {
      c_memcpy(dst+dst_off, src+src_off, len);
    }
  }

  inline proc bufferMemcpyLocal(dst, src, len, dst_off=0, src_off=0) {
    c_memcpy(dst:bufferType+dst_off, src:bufferType+src_off, len);
  }

  inline proc bufferMemmoveLocal(dst, src, len, dst_off=0, src_off=0) {
    c_memmove(dst+dst_off, src+src_off, len);
  }

  inline proc bufferGetByte(buf, off, loc) {
    if !_local && loc != chpl_nodeID {
      const newBuf = bufferCopyRemote(src_loc_id=loc, src_addr=buf+off, len=1);
      return newBuf[0];
    }
    else {
      return buf[off];
    }
  }

  inline proc bufferEquals(buf1, off1, loc1, buf2, off2, loc2, len) {
    return _strcmp(buf1=buf1+off1,len1=len,loc1=loc1,
                   buf2=buf2+off2,len2=len,loc2=loc1) == 0;
  }

  inline proc bufferEqualsLocal(buf1, off1, buf2, off2, len) {
    return _strcmp_local(buf1=buf1+off1,len1=len,
                         buf2=buf2+off2,len2=len) == 0;
  }

  private inline proc _strcmp_local(buf1, len1, buf2, len2) : int {
    // Assumes a and b are on same locale and not empty.
    const size = min(len1, len2);
    const result =  c_memcmp(buf1, buf2, size);

    if (result == 0) {
      // Handle cases where one string is the beginning of the other
      if (size < len1) then return 1;
      if (size < len2) then return -1;
    }
    return result;
  }

  inline proc _strcmp(buf1, len1, loc1, buf2, len2, loc2) {
    if loc1 == chpl_nodeID && loc2 == chpl_nodeID {
      // it's local
      return _strcmp_local(buf1, len1, buf2, len2);
    } 
    else if loc1 != chpl_nodeID && loc2 == chpl_nodeID {
      var locBuf1 = bufferCopyRemote(loc1, buf1, len1);
      return _strcmp_local(locBuf1, len1, buf2, len2);
    }
    else if loc1 == chpl_nodeID && loc2 != chpl_nodeID {
      var locBuf2 = bufferCopyRemote(loc2, buf2, len2);
      return _strcmp_local(buf1, len1, locBuf2, len2);
    }
    else {
      var locBuf1 = bufferCopyRemote(loc1, buf1, len1);
      var locBuf2 = bufferCopyRemote(loc2, buf2, len2);
      return _strcmp_local(locBuf1, len1, locBuf2, len2);
    }
  }
}
