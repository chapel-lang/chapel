param maxU2 : uint(32) = 2**31;
param maxU1 : uint(32) = maxU2 - 1 + maxU2;
param maxU11 : uint(32) = maxU1 + 1;

writeln(maxU11.type:string);
writeln(maxU11);          // silent compile-time rollover!
var m:uint = maxU1;
for i in 0..2 {
  writeln(m);     // silent run-time rollover.
  m += 1;
}

