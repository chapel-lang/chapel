
module ByteBufferHelpers {
  use Bytes;
  pragma "no doc"
  type bufferType = c_ptr(uint(8));

  // Following is copy-paste from string
  //
  // Externs and constants used to implement strings
  //
  private        param chpl_string_min_alloc_size: int = 16;
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

  private inline proc chpl_string_comm_get(dest: bufferType, src_loc_id: int(64),
                                           src_addr: bufferType, len: integral) {
    __primitive("chpl_comm_get", dest, src_loc_id, src_addr, len.safeCast(size_t));
  }

  private proc isBytesOrStringType(type t) param: bool {
    return t==_bytes || t==string;
  }

  private proc allocBuffer(requestedSize) {
    const allocSize = max(chpl_here_good_alloc_size(requestedSize+1),
                          chpl_string_min_alloc_size);
    var buf = chpl_here_alloc(allocSize,
                              offset_STR_COPY_DATA): bufferType;
    return (buf, allocSize);
  }

  private proc copyRemoteBuffer(src_loc_id: int(64), src_addr: bufferType,
                                len: int): bufferType {
      const dest = chpl_here_alloc(len+1, offset_STR_COPY_REMOTE): bufferType;
      chpl_string_comm_get(dest, src_loc_id, src_addr, len);
      dest[len] = 0;
      return dest;
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
      var locBuf1 = copyRemoteBuffer(loc1, buf1, len1);
      return _strcmp_local(locBuf1, len1, buf2, len2);
    }
    else if loc1 == chpl_nodeID && loc2 != chpl_nodeID {
      var locBuf2 = copyRemoteBuffer(loc2, buf2, len2);
      return _strcmp_local(buf1, len1, locBuf2, len2);
    }
    else {
      var locBuf1 = copyRemoteBuffer(loc1, buf1, len1);
      var locBuf2 = copyRemoteBuffer(loc2, buf2, len2);
      return _strcmp_local(locBuf1, len1, locBuf2, len2);
    }
  }

  proc copyChunk(buf, off, len, loc) {
    if !_local && loc != chpl_nodeID {
      var newBuf = copyRemoteBuffer(loc, buf+off, len);
      return (newBuf, len);
    }
    else {
      var (newBuf,size) = allocBuffer(len);
      c_memcpy(newBuf, buf+off, len);
      return (newBuf, size);
    }
  }

  proc getByteFromBuf(buf, off, loc) {
    if !_local && loc != chpl_nodeID {
      const newBuf = copyRemoteBuffer(loc, buf+off, 1);
      return newBuf[0];
    }
    else {
      return buf[off];
    }
  }
}
