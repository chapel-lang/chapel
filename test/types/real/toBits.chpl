config param r: real = pi,
       r32: real(32) = r;

param u: uint = r.toBits();
param u32 = r32.toBits();

var rv = r,
    rv32 = r32;

writeln(r32.type:string);

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
  param ui: uint(64) = __primitive("real64 as uint64", this);
  return ui;
}

proc param (real(32)).toBits() param : uint(32) {
  param ui: uint(32) = __primitive("real32 as uint32", this);
  return ui;
}

inline proc (real(?w)).toBits(): uint(w) {
  use CTypes;
  writeln("Converting real(",w,") to uint(",w,")");
  var src = this, dst:uint(w);
  writeln("src is ", src);
  writeln("its type is ", src.type:string);
  
  extern proc memcpy(dst, src, sz: c_size_t);

  memcpy(c_ptrTo(dst), c_ptrTo(src), w/8);
//  c_memcpy(c_ptrTo(dst), c_ptrTo(src), w/8);
  return dst;
}
