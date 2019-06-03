use Time;

config const numTrials : int = 10;
config const allocationsPerTrial : int = 32 * 1024 * 1024;

proc doUnmanagedAllocation() {
  var timer = new Timer();
  
  
  var times : [1..numTrials] real;
  for trial in 0..numTrials {
    {
      timer.start();
      var arr : [1..allocationsPerTrial] unmanaged object;
      for i in 1..allocationsPerTrial {
        arr[i] = new unmanaged object();
      }
      for i in 1..allocationsPerTrial {
        delete arr[i];
      }
    }
    timer.stop();
    if trial != 0 then times[trial] = timer.elapsed(TimeUnits.milliseconds);
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
      var arr : [1..allocationsPerTrial] shared object;
      for i in 1..allocationsPerTrial {
        arr[i] = new shared object();
      }
    }
    timer.stop();
    if trial != 0 then times[trial] = timer.elapsed(TimeUnits.milliseconds);
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
      var arr : [1..allocationsPerTrial] owned object;
      for i in 1..allocationsPerTrial {
        arr[i] = new owned object();
      }
    }
    timer.stop();
    if trial != 0 then times[trial] = timer.elapsed(TimeUnits.milliseconds);
    timer.clear();
  }

  return times;
}

proc main() {
  writeln("Unmanaged-Time:", (+ reduce doUnmanagedAllocation()) / numTrials);
  writeln("Shared-Time:", (+ reduce doSharedAllocation()) / numTrials);
  writeln("Owned-Time:", (+ reduce doOwnedAllocation()) / numTrials);
}
