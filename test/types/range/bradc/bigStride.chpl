config const n: uint(64) = max(uint(64));

const r = 0..2*(n/3) by n/3;

writeln("r is: ", r);

for i in r do
  writeln(i);

writeln("r.type = ", r.type:string);
writeln("r.str.type = ", r.stride.type:string);
