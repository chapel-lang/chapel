module Communication {
  private use CTypes;

  inline proc get(dest: c_void_ptr, src: c_void_ptr, srcLocID: int, numBytes: integral) {
    __primitive("chpl_comm_get", dest, srcLocID, src, numBytes);
  }

  inline proc put(dest: c_ptr, src: c_ptr, destLocID: int, numBytes: integral) {
    __primitive("chpl_comm_put", src, destLocID, dest, numBytes);
  }
}
