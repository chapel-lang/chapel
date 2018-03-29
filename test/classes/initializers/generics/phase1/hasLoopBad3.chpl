// Alternate version of hasLoopBad2.chpl, where the loop occurs after all
// of the fields have been initialized.
//
// This used to be invalid behavior, but we eventually allowed reads/writes to
// initialized fields in phase 1.

class InLoop {
  var highestNum;

  proc init(arr: [1..10] int) { // enforcing one's based indexing ...
    highestNum = 4;
    for i in arr.domain {
      highestNum = arr[i];
    }
  }
}

proc main() {
  var arr = [3, -5, 2, 7, 1, 2, 5, 8, 3, 0];

  var c: InLoop(int) = new InLoop(arr);
  writeln(c);
  delete c;
}
