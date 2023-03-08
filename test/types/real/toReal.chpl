config param u: uint = 42,
       u32: uint(32) = u;

param r: real = u.toReal();
param r32 = u32.toReal();

var uv = u,
    uv32 = u32;

var rv = uv.toReal(),
    rv32 = uv32.toReal();

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
  writeln(r, ": ", r.type:string, " == ", u, ":", u.type:string, ".toReal()");
}

proc param (uint(64)).toReal() param : real(64) {
  param r: real(64) = __primitive("uint64 as real64", this);
  return r;
}

proc param (uint(32)).toReal() param : real(32) {
  param r: real(32) = __primitive("uint32 as real32", this);
  return r;
}

inline proc (uint(?w)).toReal(): real(w) where w == 32 || w == 64 {
  use CTypes;
  var src = this, dst:real(w);

  c_memcpy(c_ptrTo(dst), c_ptrTo(src), w/8);
  return dst;
}
