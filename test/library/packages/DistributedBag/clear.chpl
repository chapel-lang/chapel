/*
  This test checks the 'distBag.clear()' method.
*/

use DistributedBag;

var bag = new distBag(int);

// Insert multiple values concurrently from different locales.
coforall locId in 0..#numLocales do on Locales[locId] {
  coforall taskId in 0..#here.maxTaskPar {
    bag.add(taskId + locId * here.maxTaskPar, taskId);
  }
}

assert(bag.getSize() == numLocales * here.maxTaskPar);
bag.clear();
assert(bag.getSize() == 0);

writeln("SUCCESS");
