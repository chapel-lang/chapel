/*
  This test checks the 'DistBag_DFS.writeThis()' method.
*/

use DistributedBag_DFS;

var bag = new DistBag_DFS(int);

// Insert multiple values concurrently.
forall taskId in 0..#here.maxTaskPar do
  bag.add(taskId, taskId);

writeln("single-locale: ", bag);

// Clear the bag.
bag.clear();

writeln("empty:         ", bag);

// Insert multiple values concurrently from different locales.
coforall locId in 0..#numLocales do on Locales[locId] {
  forall taskId in 0..#here.maxTaskPar do
    bag.add(taskId + locId * here.maxTaskPar, taskId);
}

writeln("multi-locale:  ", bag);
