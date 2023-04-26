use CTypes;

config const verbose = false;

// TODO: make this test more like 'addrOfArray' after `c_string` deprecation

proc testAddrVsPtr(type t) {
  var x: t = "hello";

  const ptr_x = c_ptrTo(x),
        addr_x = c_addrOf(x);

  if verbose {
    writeln("pointer to x: ", ptr_x: c_void_ptr);
    writeln("address of x: ", addr_x);
  }

  writeln(cmp(ptr_x, addr_x));
}

proc cmp (x, y): bool do return (x: c_void_ptr) == (y: c_void_ptr);

testAddrVsPtr(string);
testAddrVsPtr(bytes);
