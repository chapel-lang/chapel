// Alternate version of hasLoopBad2.chpl, where the incorrect loop actions
// occur after we've already seen the initialization of all fields.
class InLoop {
  var highestNum: int;

  proc init(arr: [1..10] int) { // enforcing one's based indexing ...
    highestNum = 4;
    for i in arr.domain {
      highestNum = arr[i]; // uh oh!
    }
  }
}

proc main() {
  var arr = [3, -5, 2, 7, 1, 2, 5, 8, 3, 0];

  var c: InLoop = new InLoop(arr);
  writeln(c);
  delete c;
}
