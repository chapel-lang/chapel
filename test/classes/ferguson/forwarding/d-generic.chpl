record Wrapper {
  forwarding var instance; // e.g. some class
  proc foo(x:int) { writeln("in Wrapper.foo int (", x, ")"); }
}

class C {
  var field:int;
  proc foo(x) { writeln("in C.foo(", x, ")"); }
  proc foo(x) where x.type == real { writeln("in C.foo where real (", x, ")"); }
  proc foo(x:uint) { writeln("in C.foo uint (", x, ")"); }
}

var r = new Wrapper(new C());

writeln("int");
r.foo(1:int);
writeln("real");
r.foo(2:real);
writeln("uint");
r.foo(3:uint);
writeln("complex");
r.foo(4:complex);
