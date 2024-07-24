/*
 Test to make sure we do/don't warn about the changed behavior (depending on
 if using chpl or chpldoc) in these primitives from v1.31 to v1.32.
 Should remove when we stop warning about the change.
*/

inline proc GET(addr, node, rAddr, size) {
  __primitive("chpl_comm_get", addr, node, rAddr, size);
}

inline proc PUT(addr, node, rAddr, size) {
  __primitive("chpl_comm_put", addr, node, rAddr, size);
}

/*
 Test for depreacting chpl_comm_array_* primitives
*/

inline proc GETARR(addr, node, rAddr, size) {
  __primitive("chpl_comm_array_get", addr, node, rAddr, size);
}


inline proc PUTARR(addr, node, rAddr, size) {
  __primitive("chpl_comm_array_put", addr, node, rAddr, size);
}
