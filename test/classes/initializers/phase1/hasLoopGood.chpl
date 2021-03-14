class InLoop {
  var highestNum: int;

  proc init(arr: [0..9] int) { // enforcing zero-based indexing ...
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

  var c: borrowed InLoop = new borrowed InLoop(arr);
  writeln(c);
}
