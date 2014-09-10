class foo {
  var a : int;
}

proc bar(x : foo) ref {
  return x;
}

var f : foo = new foo(a = 12);

writeln(f);

var i : int;

writeln(i);

i = bar(f).a;

writeln(i);

bar(f).a = 2;

writeln(f);
