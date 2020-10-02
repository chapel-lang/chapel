module CommPrimitives {
  use CPtr;

  inline proc getAddr(const ref p): c_ptr(p.type) {
    // TODO can this use c_ptrTo?
    return __primitive("_wide_get_addr", p): c_ptr(p.type);
  }

  inline proc GET(addr, node, rAddr, size) {
    __primitive("chpl_comm_get", addr, node, rAddr, size);
  }

  inline proc PUT(addr, node, rAddr, size) {
    __primitive("chpl_comm_put", addr, node, rAddr, size);
  }
}
