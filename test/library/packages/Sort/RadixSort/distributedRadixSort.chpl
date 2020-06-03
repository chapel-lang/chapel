use Random;
use BlockDist;
use Time;
use Sort;
use CommDiagnostics;

config param debug = false;
config const timing = true;
config const comms = true;

proc simpletestcore(input:[]) {
  var seed = SeedGenerator.oddCurrentTime;

  var localCopy = input;
  shuffle(localCopy, seed);

  const blockDom = newBlockDom(input.domain);
  var A: [blockDom] uint = localCopy;

  if debug {
    writef("input %xt\n", input);
  }

  TwoArrayRadixSort.twoArrayRadixSort(A, defaultComparator);

  if debug {
    writef("output %xt\n", A);
  }

  forall i in input.domain {
    if input[i] != A[i] {
      writeln("Sorting error in simpletest seed=", seed,
              " element ", i, " in incorrect order");
      writef("input was %xt\n", input);
      halt("failure");
    }
  }
}
proc simpletest(input:[]) {
  var size = input.domain.size;
  simpletestcore(input.reindex(1..#size));
  simpletestcore(input.reindex(0..#size));
  simpletestcore(input.reindex(2..#size));
  simpletestcore(input.reindex(-1..#size));
}

simpletest([0:uint]);
simpletest([0:uint, 0:uint, 0:uint, 0:uint]);
simpletest([0x0:uint,
            0xff00000000000000:uint,
            0xff00000000888888:uint,
            0xffffffffffffffff:uint]);
simpletest([0x01:uint,
            0x02:uint,
            0x03:uint,
            0x04:uint,
            0x05:uint,
            0x06:uint]);
simpletest([0x01:uint,
            0x01:uint,
            0x03:uint,
            0x03:uint,
            0x05:uint,
            0x05:uint]);


proc randomtest(n:int) {
  const blockDom = newBlockDom({0..#n});
  var A: [blockDom] uint;
  var seed = SeedGenerator.oddCurrentTime;
  fillRandom(A, seed);
  var localCopy:[0..#n] uint = A;

  writeln("Sorting ", n, " elements");

  if comms {
    resetCommDiagnostics();
    startCommDiagnostics();
    //startVerboseComm();
  }

  var timer:Timer;
  timer.start();
  TwoArrayRadixSort.twoArrayRadixSort(A, defaultComparator);
  timer.stop();

  if comms {
    //stopVerboseComm();
    stopCommDiagnostics();
  }

  if timing {
    if comms then writeln(getCommDiagnostics());
    writeln("Sorted ", n, " elements in ", timer.elapsed(), " seconds",
            " (", 8.0*n/timer.elapsed()/1024.0/1024.0, " MiB/s");
  }

  writeln("Verifying");
  Sort.sort(localCopy);
  assert(Sort.isSorted(localCopy));
  forall i in 0..#n {
    if localCopy[i] != A[i] {
      writeln("Sorting error in randomtest seed=", seed, " n=", n,
              " element ", i, " in incorrect order");
      halt("failure");
    }
  }
  writeln("Done Verifying");
}

proc main() {
  randomtest(10);
  randomtest(100);
  randomtest(1000);
  randomtest(10000);
  randomtest(100000);
  //randomtest(1000000);
  //randomtest(10000000);
  //randomtest(100000000);
}
