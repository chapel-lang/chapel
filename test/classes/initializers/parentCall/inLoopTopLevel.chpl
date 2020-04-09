class Looper {
  var highestNum: int;

  proc init(arr: [1..10] int) { // enforcing one's based indexing ...
    var highest: int;
    for i in arr.domain {
      if (i == 1) {
        highest = arr[i];
      } else {
        if (arr[i] > highest) {
          highest = arr[i];
        }
      }
      super.init(); // Uh oh!
    }
    highestNum = highest;
  }
}

proc main() {
  var arr = [3, -5, 2, 7, 1, 2, 5, 8, 3, 0];

  var c: shared Looper = new shared Looper(arr);
  writeln(c);
}
