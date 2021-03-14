proc f(args...?n) {
  var whole = 1..10;
  var xxx = args;
  xxx(0) = 1..5;
  writeln(whole((...args)));
  proc noop(args) {} noop(args);
}

proc g(args...?n) {
  var whole = 1..10;
  var xxx = args;
  xxx(0) = 1..5;
  writeln(whole((...args)));
}

f(1..10);
g(1..10);
