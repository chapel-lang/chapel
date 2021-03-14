// This test is intended to exercise if/else and select statements, to evaluate
// their comparative speeds.
use Random, Math, Time;

// The following three configuration variables are set in perfcompopts to avoid
// redundancy
config const numElems = 1000;
config const numIters = 10;
config const verbose = false;

config const fillBy = filler.balanced;

enum filler {
  balanced,
  reversed,
  skewedLow,
  skewedMid,
  skewedHigh,
  random
}

var t1, t2: Timer;

// Get a random number
var arr: [1..#numElems] real;
var checkSum1, checkSum2: real;

fillArr(arr);

// use that in select/if statement to do something

t1.start();
for i in 1..#numIters {
  for val in arr {
    useIfElse(val); // if/else
  }
}
t1.stop();
t2.start();
for i in 1..#numIters {
  for val in arr {
    useSelect(val); // select
  }
}
t2.stop();
if (checkSum1 == checkSum2) then
  writeln("Verification successful");
else
  writeln("Verification failed");

if (verbose) {
  writeln("If/else branching took ", t1.elapsed(TimeUnits.milliseconds)/1000,
          " seconds");
  writeln("Select branching took ", t2.elapsed(TimeUnits.milliseconds)/1000,
          " seconds");
}


// Decides how to fill the array
proc fillArr(arr) {
  select fillBy {
    when filler.balanced {
      // Balanced
      for i in arr.domain {
        arr(i) = i % 10;
      }
    }
    when filler.reversed {
      // Balanced, reversed
      for i in arr.domain {
        arr(i) = 9 - (i % 10);
      }
    }
    when filler.skewedLow {
      // Skewed -> 0
      for i in arr.domain {
        arr(i) = ((i % 15) % 10);
      }
    }
    when filler.skewedMid {
      // Skewed -> 5 <-
      for i in arr.domain {
        arr(i) = (((i % 15) % 10) + 3) % 10;
      }
    }
    when filler.random {
      // Random numbers
      fillRandom(arr);
      arr *= 10;
      arr = floor(arr);
    }
    otherwise {
      // Skewed -> 9
      for i in arr.domain {
        arr(i) = 9 - ((i % 15) % 10);
      }
    }
  }
}


// The following two functions should calculate exactly the same value
// since they take the same arguments in the same order and their only
// difference is how they determine the value of x.
proc useIfElse(x) {
  if (x == 0) {
    checkSum1 -= 1;
  } else if (x == 1) {
    checkSum1 += 1;
  } else if (x == 2) {
    checkSum1 -= 5;
  } else if (x == 3) {
    checkSum1 -= x;
  } else if (x == 4) {
    checkSum1 += x;
  } else if (x == 5) {
    checkSum1 += 2;
  } else if (x == 6) {
    checkSum1 -= 3;
  } else if (x == 7) {
    checkSum1 += 5;
  } else if (x == 8) {
    checkSum1 -= x;
  } else {
    checkSum1 += 7;
  }
}

proc useSelect(x) {
  select x {
    when 0 {
      checkSum2 -= 1;
    }
    when 1 {
      checkSum2 += 1;
    }
    when 2 {
      checkSum2 -= 5;
    }
    when 3 {
      checkSum2 -= x;
    }
    when 4 {
      checkSum2 += x;
    }
    when 5 {
      checkSum2 += 2;
    }
    when 6 {
      checkSum2 -= 3;
    }
    when 7 {
      checkSum2 += 5;
    }
    when 8 {
      checkSum2 -= x;
    }
    otherwise {
      checkSum2 += 7;
    }
  }
}
