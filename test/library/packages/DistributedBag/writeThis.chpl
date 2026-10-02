/*
  This test checks the 'distBag.writeThis()' method.
*/

use DistributedBag;
config const tasks = 4;

var bag = new distBag(int);

// Insert multiple values concurrently.
forall taskId in 0..#tasks do
  bag.add(taskId, taskId);

writeln("single-locale: ", bag);

// Clear the bag.
bag.clear();

writeln("empty:         ", bag);

// Insert multiple values concurrently from different locales.
coforall locId in 0..#numLocales do on Locales[locId] {
  forall taskId in 0..#tasks do
    bag.add(taskId + locId * tasks, taskId);
}

writeln("multi-locale:  ", bag);
