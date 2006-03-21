record bar {
  type t;
  var x : t;
}

var b : bar(int);

writeln(b);

fun foo(b : bar(int)) {
  writeln(b);
}

foo(b);
