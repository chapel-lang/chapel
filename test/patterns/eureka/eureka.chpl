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
  // Compute the number of tasks on this locale; generally, this is nCPUs.
  const nTasks = if dataParTasksPerLocale==0 then here.maxTaskPar
                                             else dataParTasksPerLocale;

  // Split our locale's indices into nTasks chunks, nearly equal in size.
  // Note that if !A.hasSingleLocalSubdomain(), this would need to be
  // more fancy.
  const localDom = A.localSubdomain();
  const (numChunks, _) = _computeChunkStuff(maxTasks = nTasks,
                                            ignoreRunning = true,
                                            minSize = 1, localDom.dims());

  // Search this locale's part of the array, in parallel across the CPUs.
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
  const (lo,hi) = _computeBlock(indices.length,
                                numChunks, chunk,
                                indices._high,
                                indices._low,
                                indices._low);

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
