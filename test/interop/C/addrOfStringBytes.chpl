use CTypes;

config const verbose = false;

proc testAddrVsPtr(type t) {
  var x: t = "hello";

  const ptr_x = c_ptrTo(x),
        addr_x = c_addrOf(x),
        ptr_buf0 = c_ptrTo(x.buff[0]),
        addr_buf0 = c_addrOf(x.buff[0]);

  const ptr_x_const = c_ptrToConst(x),
        addr_x_const = c_addrOfConst(x),
        ptr_buf0_const = c_ptrToConst(x.buff[0]),
        addr_buf0_const = c_addrOfConst(x.buff[0]);

  if verbose {
    writeln("pointer to x: ", ptr_x: c_void_ptr);
    writeln("address of x: ", addr_x);
    writeln("pointer to x.buff[0]: ", ptr_buf0);
    writeln("address of x.buff[0]: ", addr_buf0);

    writeln("pointer to const x: ", ptr_x_const: c_void_ptr);
    writeln("address of const x: ", addr_x_const);
    writeln("pointer to const x.buff[0]: ", ptr_buf0_const);
    writeln("address of const x.buff[0]: ", addr_buf0_const);
  }

  writeln(!cmp(ptr_x, addr_x));
  writeln(cmp(ptr_x, ptr_buf0));
  writeln(cmp(ptr_x, addr_buf0));

  writeln(!cmp(ptr_x_const, addr_x_const));
  writeln(cmp(ptr_x_const, ptr_buf0_const));
  writeln(cmp(ptr_x_const, addr_buf0_const));
}

proc cmp (x, y): bool do return (x: c_void_ptr) == (y: c_void_ptr);

testAddrVsPtr(string);
testAddrVsPtr(bytes);
