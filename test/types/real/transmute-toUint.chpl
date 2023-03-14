config param r: real = pi,
       r32: real(32) = r;

param u: uint = r.transmute(uint);
param u32 = r32.transmute(uint(32));

var rv = r,
    rv32 = r32;

var uv = rv.transmute(uint),
    uv32 = rv32.transmute(uint(32));

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
  writeln(u, ": ", u.type:string, " == ", r, ":", r.type:string, ".transmute(uint(?))");
}
