use CTypes;

record r {
  var x: int;
}

var r1 = new r(1);

const ptr_r = c_ptrTo(r1),
      addr_r = c_addrOf(r1);

const ptr_r_const = c_ptrToConst(r1),
      addr_r_const = c_addrOfConst(r1);

writeln(cmp(ptr_r, addr_r));
writeln(cmp(ptr_r_const, addr_r_const));

proc cmp (x, y): bool do return (x: c_void_ptr) == (y: c_void_ptr);
