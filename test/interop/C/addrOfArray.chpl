use CTypes;

var a = [1, 2, 3, 4, 5];

config const verbose = false;

const ptr_a = c_ptrTo(a),
      addr_a = c_addrOf(a),
      ptr_a0 = c_ptrTo(a[0]),
      addr_a0 = c_addrOf(a[0]);

if verbose {
  writeln("pointer to a: ", ptr_a);
  writeln("address of a: ", addr_a);
  writeln("pointer to a[0]: ", ptr_a0);
  writeln("address of a[0]: ", addr_a0);
}

proc cmp (x, y): bool do return (x: c_void_ptr) == (y: c_void_ptr);

writeln(cmp(ptr_a, ptr_a0));
writeln(cmp(ptr_a, addr_a0));
writeln(cmp(ptr_a, addr_a));
