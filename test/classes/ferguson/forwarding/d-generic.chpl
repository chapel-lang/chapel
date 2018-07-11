record Wrapper {
  forwarding var instance; // e.g. some class
  proc foo(x:int) { writeln("in Wrapper.foo int (", x, ")"); }
  proc deinit() {
    delete instance;
  }
}

class C {
  var field:int;
  proc foo(x) { writeln("in C.foo(", x, ")"); }
  proc foo(x) where x.type == real(32) { writeln("in C.foo where real(32) (", x, ")"); }
  proc foo(x:uint) { writeln("in C.foo uint (", x, ")"); }
}

var r = new Wrapper(new unmanaged C());

writeln("int");
r.foo(1:int);
writeln("real(32)");
r.foo(2:real(32));
writeln("uint");
r.foo(3:uint);
writeln("complex");
r.foo(4:complex);
