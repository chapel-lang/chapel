class foo {
  var i : int;

  proc f() { return 10 + i; }
}

proc f(x : borrowed foo) {
  return 20 + x.i;
}

var a = new unmanaged foo();

a.i = 1;

writeln(f(a));
writeln(a.f());

delete a;

