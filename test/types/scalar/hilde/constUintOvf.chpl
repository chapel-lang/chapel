param maxU2 : uint = 2**31;
param maxU1 : uint = maxU2 - 1 + maxU2;
param maxU11 : uint = maxU1 + 1;

writeln(typeToString(maxU11.type));
writeln(maxU11);          // silent compile-time rollover!
var m:uint = maxU1;
for i in 0..2 {
  writeln(m);     // silent run-time rollover.
  m += 1;
}

