// This should give a compiler warning if param warnMaximalRange is true.

const r = 0:uint .. max(uint);

// Should execute zero times.
for i in r do
  writeln(i);

writeln("DONE");


