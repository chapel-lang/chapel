config param r: real = pi,
       r32: real(32) = r;

param u: uint = r.toBits();
param u32 = r32.toBits();

var rv = r,
    rv32 = r32;

var uv = rv.toBits(),
    uv32 = rv32.toBits();

print(u,r);
print(u32,r32);
print(uv,rv);
print(uv32,rv32);

proc print(param u, param r) {
  write('param ');
  var uv = u, rv = r;
  print(uv, rv);
}

proc print(u, r) {
  writeln(u, ": ", u.type:string, " == ", r, ":", r.type:string, ".toBits()");
}

proc param (real(64)).toBits() param : uint {
  param ui: uint(64) = __primitive("real64 as uint64", pi);
  return ui;
}

proc param (real(32)).toBits() param : uint(32) {
  param ui: uint(32) = __primitive("real32 as uint32", pi);
  return ui;
}

inline proc (real(?w)).toBits(): uint(w) {
  use CTypes;
  var src = this, dst:uint(w);

  c_memcpy(c_ptrTo(dst), c_ptrTo(src), c_sizeof(src.type));
  return dst;
}

// convert the IEEE 754 encoding inside a uint(w) to a real(w)

inline proc type (real(?w)).asReal(x: uint(?k)): real(w) {
  use CTypes;
  var src = x, dst:real(w);

  compilerAssert(w == k);
  c_memcpy(c_ptrTo(dst), c_ptrTo(src), c_sizeof(src.type));
  return dst;
}
