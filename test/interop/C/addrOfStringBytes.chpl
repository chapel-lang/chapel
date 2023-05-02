use CTypes;

config const verbose = false;

proc testAddrVsPtr(type t) {
  var x: t = "hello";

  const ptr_x = c_ptrTo(x),
        addr_x = c_addrOf(x),
        ptr_buf0 = c_ptrTo(x.buff[0]),
        addr_buf0 = c_addrOf(x.buff[0]);

  if verbose {
    writeln("pointer to x: ", ptr_x: c_void_ptr);
    writeln("address of x: ", addr_x);
    writeln("pointer to x.buff[0]: ", ptr_buf0);
    writeln("address of x.buff[0]: ", addr_buf0);
  }

  writeln(cmp(ptr_x, addr_x));
  writeln(cmp(ptr_x, ptr_buf0));
  writeln(cmp(ptr_x, addr_buf0));
}

proc cmp (x, y): bool do return (x: c_void_ptr) == (y: c_void_ptr);

testAddrVsPtr(string);
testAddrVsPtr(bytes);
