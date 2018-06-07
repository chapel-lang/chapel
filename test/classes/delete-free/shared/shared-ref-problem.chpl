use SharedObject;

class State {
  var done: int;
}

proc foo(old: Shared(State)): int {
  return old.done;
}

var x = new Shared(new State(42));
var y = foo(x);
writeln(y);
