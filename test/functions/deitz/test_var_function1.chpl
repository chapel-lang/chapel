class foo {
  var a : integer;
}

function bar(x : foo) var {
  return x;
}

var f : foo = foo(a = 12);

writeln(f);

bar(f) = foo(a = 13);

writeln(f);

