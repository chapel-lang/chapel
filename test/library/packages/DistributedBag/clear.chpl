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

writeln("before clear: ", bag);
bag.clear();
writeln("after clear:  ", bag);
