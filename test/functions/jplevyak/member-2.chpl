class foo {
  var i : integer;
 function f() { return 10 + i; } 
}

function f(x : foo) {
  return 20 + x.i;
}

var a = foo();
a.i = 1;

writeln(f(a));
writeln(a.f()); 
