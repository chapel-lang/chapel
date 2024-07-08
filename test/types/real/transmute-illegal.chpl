param r = 3.14,
      r32 = 3.14: real(32);

var rv = 3.14,
    rv32 = 3.14: real(32);

param u = 42,
      u32 = 42: uint(32);

var uv = 42,
    uv32 = 42: uint(32);

// These should all be errors
writeln(r.transmute(uint(32)));
writeln(r32.transmute(uint(64)));
writeln(rv.transmute(uint(32)));
writeln(rv32.transmute(uint(64)));
writeln(u.transmute(real(32)));
writeln(u32.transmute(real(64)));
writeln(uv.transmute(real(32)));
writeln(uv32.transmute(real(64)));
