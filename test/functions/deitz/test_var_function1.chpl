class foo {
  var a : int;
}

proc bar(x : unmanaged foo) ref {
  return x;
}

var f : unmanaged foo = new unmanaged foo(a = 12);

writeln(f);

bar(f) = new unmanaged foo(a = 13);

writeln(f);

