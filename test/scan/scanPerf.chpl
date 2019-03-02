use Time, Memory;

// compute a target problem size if one is not specified
config const memFraction = 0;
const totalMem = here.physicalMemory(unit = MemUnits.Bytes);
const defaultN = if memFraction == 0 then 10
                                     else (totalMem / numBytes(int)) / memFraction;

config const n = defaultN,
             printTiming = false,
             printArray = true;

var A: [1..n] int = 1;

var t: Timer;

// time scan
t.start();
var B = + scan A;
t.stop();

if printTiming then
  writeln("Scan of ", n, " elements took ", t.elapsed(), " seconds");

if printArray then
  writeln(B);

// make sure result was correct
var exp = (n/2 * (1 + n)) + ((n%2) * (n/2 + 1));
var tot = + reduce B;
if (tot != exp) then
  writeln("Verification failed: ", tot, " != ", exp);
else
   writeln("Verification passed!");
