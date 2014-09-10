class foo {
  var a : int;
}

proc bar(x : foo) ref {
  return x;
}

var f : foo = new foo(a = 12);

writeln(f);

bar(f) = new foo(a = 13);

writeln(f);

