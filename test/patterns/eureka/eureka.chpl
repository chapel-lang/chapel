// This is a pattern for doing parallel eureka, basically:
//   coforall across per-locale array chunks {
//     coforall across per-CPU array chunks {
//       search the appropriate chunk; broadcast 'Eureka!' if we find it
//     }
//   }
use BlockDist, Time;

// Print timing info?
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
const foundD: domain(1) dmapped Block(boundingBox={0..#numLocales})
              = {0..#numLocales};
var found: [foundD] atomic int;
[f in found] f.write(-1); // alas, can't (yet) write this as an initial value

// Search the array, in parallel across the locales.
const startTime = getCurrentTime();
coforall loc in Locales {
  on loc {
    searchOnLocale(vals, findVal, found);
  }
}
const execTime = getCurrentTime() - startTime;

// Did we find the value?  Report.
if found[0].read() < 0 {
  writeln('NOT found');
} else {
  writeln('found, index ', found[0].read());
}
if printTiming {
  writeln('time: ', execTime, 's');
}


// Do a parallel search over this locale's portion of the array.
proc searchOnLocale(A, findVal, found) {
  // Figure out the part of the array stored on this locale.
  const (hereLo, hereHi) = computeChunk(vals.domain.dim(1), here.id,
                                        numLocales);
  const hereDim = vals.domain.dim(1)(hereLo..hereHi);

  // Split the work coarsely enough that every CPU has some.
  const numChunks = min(hereDim.length,
                        if dataParTasksPerLocale == 0 then
                          here.maxTaskPar
                        else
                          dataParTasksPerLocale);
  coforall chunk in 0..#numChunks {
    searchOnCpu(A, hereDim, chunk, numChunks, findVal, found);
  }
}


// Do a serial search over this task's portion of the array.
proc searchOnCpu(A, dim, chunk, numChunks, findVal, found) {
  const (lo, hi) = computeChunk(dim, chunk, numChunks);
  for i in lo..hi {
    // If we find the value, tell everyone and quit looking.
    if A[i] == findVal {
      if found[here.id].compareExchange(-1, i, memory_order_release) {
        // The cmpxchg on our own found[] element here is a superfluous no-op.
        [f in found] { f.compareExchange(-1, i, memory_order_release); }
      }
      break;
    }

    // Check now and then; if someone else found it, quit early.
    if i % (2**17) == 0 && found[here.id].read() >= 0 {
      break;
    }
  }
}


// These are helper routines for chunking index ranges.
inline proc computeChunk(dim, chunk, numChunks) {
  const lo = if chunk == 0 then
               dim.low
             else
               dim.low + intCeilXDivByY(dim.length * chunk, numChunks);
  const hi = if chunk == numChunks - 1 then
               dim.high
             else
               dim.low + intCeilXDivByY(dim.length * (chunk+1), numChunks) - 1;
  return (lo, hi);
}

inline proc intCeilXDivByY(x, y) {
  return 1 + (x - 1) / y;
}
