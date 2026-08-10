proc foo(type t, param fiveInts: bool): [] {
  if (fiveInts) {
    var x: [1..5] int;
    return x;
  } else {
    var x: [1..3, 1..4] t;
    return x;
  }
}
proc bar(type t, param fiveInts: bool): ([], [], ([], [])) {
  if (fiveInts) {
    var x: [1..5] int;
    return (x, x, (x, x));
  } else {
    var x: [1..3, 1..4] t;
    return (x, x, (x, x));
  }
}

var a = foo(real, true);
var b = foo(real, false);
writeln(a);
writeln(b);
var c = bar(real, true);
var d = bar(real, false);
writeln(c);
writeln(d);
