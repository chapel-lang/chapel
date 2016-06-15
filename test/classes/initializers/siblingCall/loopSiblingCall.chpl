class Looper {
  var val: int;

  proc init(arr: [1..10] int) { // enforcing one's based indexing ...
    var highest: int;
    for i in arr.domain {
      if (i == 1) {
        highest = arr[i];
      } else {
        if (arr[i] > highest) {
          highest = arr[i];
        }
        this.init(highest); // Uh oh!
      }
    }
  }

  proc init(best: int) {
    val = best;
    super.init();
  }
}

proc main() {
  var arr = [3, -5, 2, 7, 1, 2, 5, 8, 3, 0];

  var c: Looper = new Looper(arr);
  writeln(c);
  delete c;
}
