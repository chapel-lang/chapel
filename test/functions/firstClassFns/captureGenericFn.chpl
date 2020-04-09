proc f1(x) {
  return x;
}

class R {
  type t;
}

proc f2(x:borrowed R) {
  return x;
}

proc f3(x:?t) {
  return x;
}

proc f4(A, B) {
  // Compiler should not attempt to resolve this function due to the generic
  // args, and so we should not encounter an error for this expression.
  return A + B;
}

var func1 = f1;
var func2 = f2;
var func3 = f3;
var func4 = f4;

writeln(func1(42));
writeln(func2(42));
writeln(func3(42));
writeln(func4(40, 2));
