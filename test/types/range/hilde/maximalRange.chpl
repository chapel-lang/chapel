// This should give a compiler warning if param warnMaximalRange is true.

const r = 0:uint .. max(uint);

// Should execute zero times.
for i in r {
  writeln(i);
  break; // but just incase it didn't, we don't want to print out all numbers
}

writeln("DONE");


