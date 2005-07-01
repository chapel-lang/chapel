record bar {
  type t;
  var x : t;
}

var b : bar(integer);

writeln(b);

function foo(b : bar(integer)) {
  writeln(b);
}

foo(b);
