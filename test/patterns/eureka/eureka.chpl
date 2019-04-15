use BlockDist, DSIUtil, Time;

config const printTiming = false;

// This is the problem size.
config const n = 1_000_000;

// This is the array of values to search in.
const valsD: domain(1) dmapped Block(boundingBox={0..#n}) = {0..#n};
var vals: [valsD] int;
[i in valsD] { vals[i] = i; }

// This is the value to search for.
config const findVal = n - 1;

// If we do find findVal, this is where we'll record its index.
var eurekaIdx: atomic int = -1;

// Search the array, in parallel across the locales.
const startTime = getCurrentTime();
var endCount: atomic int = numLocales;
coforall loc in Locales {
  on loc {
    begin {
      searchOnLocale(vals, findVal, eurekaIdx);
      endCount.sub(1, memory_order_release);
    }
  }
}
endCount.waitFor(0);
const execTime = getCurrentTime() - startTime;

// Did we find the value?  And maybe, how long did the search take?
const foundIdx = eurekaIdx.read();
if foundIdx < 0 {
  writeln('NOT found');
} else {
  writeln('found, index ', foundIdx);
}
if printTiming {
  writeln('time: ', execTime, 's');
}


proc searchOnLocale(A, findVal, eurekaIdx) {
  // This subdomain covers the part of the array stored on our locale.
  const localDom = A.localSubdomain();

  // Compute the number of chunks to search in parallel on this locale.
  // This is the lesser of the number of indices and and number of CPUs.
  const numChunks = min(localDom.dim(1).length,
                        if dataParTasksPerLocale == 0
                        then here.maxTaskPar
                        else dataParTasksPerLocale);

  // Search this locale's part of the array.
  var endCount: atomic int = numChunks;
  coforall chunk in 0..#numChunks {
    begin {
      searchOnCpu(A[localDom], chunk, numChunks, findVal, eurekaIdx);
      endCount.sub(1, memory_order_release);
    }
  }
  endCount.waitFor(0);
}


proc searchOnCpu(A, chunk, numChunks, findVal, eurekaIdx) {
  // Compute this task's part of the indices.
  const indices = A.domain.dim(1);
  proc intCeilXDivByY(x, y) return 1 + (x - 1)/y;
  const lo = indices.low
             + (if chunk == 0
                then 0
                else intCeilXDivByY(indices.length * chunk, numChunks));
  const hi = if chunk == numChunks - 1
             then indices.high
             else (indices.low
                   + intCeilXDivByY(indices.length * (chunk + 1), numChunks)
                   - 1);

  // Search this locale's part of the array, in serial.
  for i in lo..hi {
    // If we found what we're looking for, save its location and we're done.
    if A[i] == findVal {
      eurekaIdx.compareExchange(-1, i);
      break;
    }
    // Now and then, check if someone else found it.  If so, quit early.
    if i % 1024 == 0 && eurekaIdx.read() >= 0 {
      break;
    }
  }
}
