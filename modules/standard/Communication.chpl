module Communication {
  private use CTypes;

  inline proc get(dst: c_ptr, src: c_ptr, locId: int, numBytes: int) {
    __primitive("chpl_comm_get", dst, locId, src, numBytes);
  }

  inline proc put(dst: c_ptr, src: c_ptr, locId: int, numBytes: int) {
    __primitive("chpl_comm_put", src, locId, dst, numBytes);
  }
}
