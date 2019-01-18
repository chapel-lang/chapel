// Simplified version of hasLoopBad.chpl, to ensure we detect basic field
// updates (because hasLoopBad.chpl is failing to act properly both due to 
// loops and due to the presence of if statements)
class InLoop {
  var highestNum: int;

  proc init(arr: [1..10] int) { // enforcing one's based indexing ...
    for i in arr.domain {
      highestNum = arr[i]; // uh oh!
    }
  }
}

proc main() {
  var arr = [3, -5, 2, 7, 1, 2, 5, 8, 3, 0];

  var c: shared InLoop = new shared InLoop(arr);
  writeln(c);
}
