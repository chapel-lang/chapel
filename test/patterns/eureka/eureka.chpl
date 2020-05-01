// This is a pattern for doing parallel eureka, basically:
//   coforall across per-locale array chunks {
//     coforall across per-CPU array chunks {
//       search the appropriate chunk; broadcast 'Eureka!' if we find it
//     }
//   }
use BlockDist, RangeChunk, Time;

// Print timing info?
config const printTiming = false;

// This is the problem size.
config const n = 1_000_000;

// This is the value to search for.
config const findVal = n - 1;

// Every this-many iterations we check to see if findVal has been found.
// For best performance this must be a compile-time power of 2.
config param eurekaInterval = 2**17;

// This is the array of values to search in.
const valsD: domain(1) dmapped Block(boundingBox={0..#n}) = {0..#n};
var vals: [valsD] int;
[i in valsD] { vals[i] = i; }

// If we do find findVal, this is where we'll record its index.
const foundD: domain(1) dmapped Block(boundingBox={0..#numLocales})
              = {0..#numLocales};
var found: [foundD] atomic int;
[f in found] f.write(-1); // alas, can't (yet) write this as an initial value

var T: Timer;
T.start();

// Do a parallel search across the locales.
coforall loc in Locales {
  on loc {
    // Split the on-locale work coarsely enough that every task/CPU has some.
    const rangeOnLoc = chunk(vals.domain.dim(0), numLocales, here.id);
    const numChunks = min(rangeOnLoc.size,
                          if dataParTasksPerLocale == 0
                          then here.maxTaskPar
                          else dataParTasksPerLocale);

    // Do a parallel search across the CPUs on this locale.
    coforall rangeOnCpu in chunks(rangeOnLoc, numChunks) {
      // Do a serial search over this task's (CPU's) portion of the array.
      for i in rangeOnCpu {
        // If we find the value, tell everyone and quit looking.
        if vals[i] == findVal {
          if found[here.id].compareAndSwap(-1, i, memoryOrder.release) {
            // The cmpxchg on found[here.id] here is a superfluous no-op.
            [f in found] { f.compareAndSwap(-1, i, memoryOrder.release); }
          }
          break;
        }

        // Check now and then; if someone else found it, quit early.
        if i % eurekaInterval == 0 && found[here.id].read() >= 0 {
          break;
        }
      } // for i in rangeOnCpu
    } // coforall across CPUs
  } // coforall across Locales
}

T.stop();

// Did we find the value?  Report.
if found[0].read() < 0 {
  writeln('NOT found');
} else {
  writeln('found, index ', found[0].read());
}

if printTiming {
  writeln('time: ', T.elapsed(), 's');
}
