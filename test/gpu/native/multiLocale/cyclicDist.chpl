// Do some computation on gpus across multiple locales, collect results
// together in some cycically distributed array
use CyclicDist;
const Dom = {0..<numLocales} dmapped Cyclic(startIdx=0);
var A : [Dom] int;

coforall loc in Locales do on loc {
  on here.gpus[0] {
    var B : [0..10] int;
    foreach i in 0..10 {
      B[i] = here.id * 100 + i;
    }
    A[here.id] = B[0];
  }
}

writeln(A);
