use Time, MemDiagnostics, BlockDist, ChplConfig;

// compute a target problem size if one is not specified; assume homogeneity
config const memFraction = 0;
const totMem = here.physicalMemory(unit = MemUnits.Bytes);
const defaultN = if memFraction == 0
                   then 30
                   else numLocales * ((totMem / numBytes(int)) / memFraction);

config const n = defaultN,
             printTiming = false,
             printArray = true;

var D = if CHPL_COMM=='none' then {1..n}
                             else {1..n} dmapped Block({1..n});

var A: [D] int = 1;

var t: stopwatch;

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

if printArray {
  writeln("distributed:");
  forall b in B do
    b = here.id;
  writeln(B);
}

