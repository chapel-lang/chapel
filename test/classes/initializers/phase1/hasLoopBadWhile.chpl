// Simplified version of hasLoopBad.chpl, to ensure we detect basic field
// updates (because hasLoopBad.chpl is failing to act properly both due to 
// loops and due to the presence of if statements)
class InLoop {
  var highestNum: int = -7;

  proc init(arr: [1..10] int) { // enforcing one's based indexing ...
    var i = arr.domain.low;
    while (i < arr.domain.high){
      highestNum = arr[i]; // uh oh!
      i += 1;
    }
  }
}

proc main() {
  var arr = [3, -5, 2, 7, 1, 2, 5, 8, 3, 0];

  var c: unmanaged InLoop = new unmanaged InLoop(arr);
  writeln(c);
  delete c;
}
