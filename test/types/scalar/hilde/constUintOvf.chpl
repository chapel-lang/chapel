param maxU1 = max(uint) + 1;
writeln(maxU1);	  // silent compile-time rollover!
for i in 0..2 do
  writeln(max(uint) + i);  // silent run-time rollover.

