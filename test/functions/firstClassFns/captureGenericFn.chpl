proc f1(x) {
  return x;
}

class R {
  type t;
}

proc f2(x:R) {
  return x;
}

proc f3(x:?t) {
  return x;
}

var func1 = f1;
var func2 = f2;
var func3 = f3;

writeln(func1(42));
writeln(func2(42));
writeln(func3(42));
