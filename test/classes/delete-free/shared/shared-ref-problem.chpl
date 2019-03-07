

class State {
  var done: int;
}

proc foo(old: shared State): int {
  return old.done;
}

var x = new shared State(42);
var y = foo(x);
writeln(y);
