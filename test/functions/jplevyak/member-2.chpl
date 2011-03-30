class foo {
  var i : int;
  proc f() { return 10 + i; } 
}

proc f(x : foo) {
  return 20 + x.i;
}

var a = new foo();
a.i = 1;

writeln(f(a));
writeln(a.f()); 
