def f(args...?n) {
  var whole = 1..10;
  var xxx = args;
  xxx(1) = 1..5;
  writeln(whole((...args)));
  def noop(args) {} noop(args);
}

def g(args...?n) {
  var whole = 1..10;
  var xxx = args;
  xxx(1) = 1..5;
  writeln(whole((...args)));
}

f(1..10);
g(1..10);
