record bar {
  type t;
  var x : t;
}

var b : bar(int);

writeln(b);

function foo(b : bar(int)) {
  writeln(b);
}

foo(b);
