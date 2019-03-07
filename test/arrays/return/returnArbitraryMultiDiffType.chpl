proc foo(type t, param fiveInts: bool): [] {
  if (fiveInts) {
    var x: [1..5] int;
    return x;
  } else {
    var x: [1..3] t;
    return x;
  }
}

var a = foo(real, true);
var b = foo(real, false);
writeln(a);
writeln(b);
