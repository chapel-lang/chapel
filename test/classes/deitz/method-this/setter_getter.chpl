class C {
  var x: int;
  proc x ref {
    writeln("setter invoked");
    return x;
  }
  proc x {
    writeln("getter invoked");
    return x;
  }

}

var c = new C();
c.x = 2;
writeln(c.x);
delete c;
