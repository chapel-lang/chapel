
const r = 0:uint(8) .. max(uint(8));

// Should halt during execution
for i in r {
  writeln(i);
  break; // but just incase it didn't, we don't want to print out all numbers
}

writeln("DONE");


