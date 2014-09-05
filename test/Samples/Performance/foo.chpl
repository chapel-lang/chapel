config const printStats = true;

// Pretend there's a real computation here and that we time it
// and compute the amount of memory that it requires...

const execTime = 194.3;
const memInGB = 24;
const validated = true;

if printStats {
  writeln("Time: ", execTime, " seconds");
  writeln("Memory: ", memInGB, " GB");
}
write("Validation: ");
if validated then
  writeln("SUCCESS");
 else
  writeln("FAILURE");
