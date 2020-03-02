use Time;

// Performs multiple trial runs to obtain the average over; we skip the first
// trial to not only 'warm up the cache' but also give jemalloc the chance to
// warmup as well, allowing it to request more memory for the heap upfront rather
// than on-demand.
config const printTiming : bool = true;
config const numTrials : int = 3;
config const allocationsPerTrial : int = 16 * 1024 * 1024;

proc doUnmanagedAllocation() {
  var timer = new Timer();
  
  
  var times : [1..numTrials] real;
  for trial in 0..numTrials {
    {
      timer.start();
      var arr : [1..allocationsPerTrial] unmanaged object?;
      for i in 1..allocationsPerTrial {
        arr[i] = new unmanaged object();
      }
      for i in 1..allocationsPerTrial {
        delete arr[i];
      }
    }
    timer.stop();
    if trial != 0 then times[trial] = timer.elapsed(TimeUnits.seconds);
    timer.clear();
  }
  
  return times;
}

proc doSharedAllocation() {
  var timer = new Timer();
  
  var times : [1..numTrials] real;
  for trial in 0..numTrials {
    {
      timer.start();
      var arr : [1..allocationsPerTrial] shared object?;
      for i in 1..allocationsPerTrial {
        arr[i] = new shared object();
      }
    }
    timer.stop();
    if trial != 0 then times[trial] = timer.elapsed(TimeUnits.seconds);
    timer.clear();
  }

  return times;
}

proc doOwnedAllocation() {
  var timer = new Timer();
  
  var times : [1..numTrials] real;
  for trial in 0..numTrials {
    {
      timer.start();
      var arr : [1..allocationsPerTrial] owned object?;
      for i in 1..allocationsPerTrial {
        arr[i] = new owned object();
      }
    }
    timer.stop();
    if trial != 0 then times[trial] = timer.elapsed(TimeUnits.seconds);
    timer.clear();
  }

  return times;
}

proc main() {
  if printTiming {
    writeln("Unmanaged-Time:", (+ reduce doUnmanagedAllocation()) / numTrials);
    writeln("Shared-Time:", (+ reduce doSharedAllocation()) / numTrials);
    writeln("Owned-Time:", (+ reduce doOwnedAllocation()) / numTrials);
  } 
}
