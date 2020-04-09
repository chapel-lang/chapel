// Alternate version of hasLoopBad2.chpl, where the incorrect loop actions
// occur after we've already seen the initialization of all fields.
class InLoop {
  var highestNum: int;

  proc init(arr: [0..9] int) { // enforcing zero-based indexing ...
    highestNum = 4;
    for i in arr.domain {
      highestNum = arr[i]; // uh oh!
    }
  }
}

proc main() {
  var arr = [3, -5, 2, 7, 1, 2, 5, 8, 3, 0];

  var c: borrowed InLoop = new borrowed InLoop(arr);
  writeln(c);
}
