class InLoop {
  var highestNum: int;

  proc init(arr: [1..10] int) { // enforcing one's based indexing ...
    var highest: int;
    // Demonstrates that loops in Phase 1 behave properly
    for i in arr.domain {
      if (i == 1) {
        highest = arr[i];
      } else {
        if (arr[i] > highest) {
          highest = arr[i];
        }
      }
    }
    highestNum = highest;
  }
}

proc main() {
  var arr = [3, -5, 2, 7, 1, 2, 5, 8, 3, 0];

  var c: InLoop = new InLoop(arr);
  writeln(c);
  delete c;
}
