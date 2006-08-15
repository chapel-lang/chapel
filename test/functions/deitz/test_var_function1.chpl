class foo {
  var a : int;
}

def bar(x : foo) var {
  return x;
}

var f : foo = foo(a = 12);

writeln(f);

bar(f) = foo(a = 13);

writeln(f);

