class InLoop {
  var highestNum: int;

  proc init(arr: [1..10] int) { // enforcing one's based indexing ...
    for i in arr.domain {
      if (i == 1) {
        highestNum = arr[i]; // uh oh!
      } else {
        if (arr[i] > highestNum) {
          highestNum = arr[i]; // uh oh!
        }
      }
    }
    // The correct way to do this is to store the result in a temporary
    // variable and update highestNum once.  Allowing initialization of a field
    // in a loop could result in out of order initialization, or multiple
    // initialization, which would interfere with our ability to implicitly
    // initialize omitted fields.
  }
}

proc main() {
  var arr = [3, -5, 2, 7, 1, 2, 5, 8, 3, 0];

  var c: owned InLoop = new owned InLoop(arr);
  writeln(c);
}
