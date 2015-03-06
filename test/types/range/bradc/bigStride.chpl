config const n: uint(64) = max(uint(64));

const r = 0..2*(n/3) by n/3;

writeln("r is: ", r);

for i in r do
  writeln(i);

writeln("r.type = ", typeToString(r.type));
writeln("r.str.type = ", typeToString(r.stride.type));
