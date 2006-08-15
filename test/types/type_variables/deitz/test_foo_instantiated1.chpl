record bar {
  type t;
  var x : t;
}

var b : bar(int);

writeln(b);

def foo(b : bar(int)) {
  writeln(b);
}

foo(b);
