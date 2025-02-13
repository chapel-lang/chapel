/*
  This test checks the 'distBag.remove()' method, and more specifically the
  `enum VictimPolicy` allowing to choose the victim selection policy for work stealing.
*/

use DistributedBag;

config const n = 1000;

{
  // Create a distBag
  var bag = new distBag(int);

  // Insert `n` elements in segment 0
  bag.addBulk(1..n, 0);

  // Manually adjusts the pointers to place all these elements in the shared portion.
  bag.bag!.segments[0].split.add(n);
  bag.bag!.segments[0].o_split += n;
  bag.bag!.segments[0].nElts_shared.add(n);

  // Remove these elements from tasks where taskId != 0, ensuring that each removal
  // triggers a work stealing operation according to the selected victim policy.
  coforall taskId in 1..<here.maxTaskPar {
    while (bag.getSize() > 0) {
      var elt = bag.remove(taskId, DistributedBag.VictimPolicy.RAND);
    }
  }
}

{
  // Create a distBag
  var bag = new distBag(int);

  // Insert `n` elements in segment 0
  bag.addBulk(1..n, 0);

  // Manually adjusts the pointers to place all these elements in the shared portion.
  bag.bag!.segments[0].split.add(n);
  bag.bag!.segments[0].o_split += n;
  bag.bag!.segments[0].nElts_shared.add(n);

  // Remove these elements from tasks where taskId != 0, ensuring that each removal
  // triggers a work stealing operation according to the selected victim policy.
  coforall taskId in 1..<here.maxTaskPar {
    while (bag.getSize() > 0) {
      var elt = bag.remove(taskId, DistributedBag.VictimPolicy.RING);
    }
  }
}

writeln("SUCCESS");
