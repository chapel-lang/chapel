class C {
  var x_field: int;
  proc x ref {
    writeln("setter invoked");
    return x_field;
  }
  proc x {
    writeln("getter invoked");
    return x_field;
  }

}

var c = new unmanaged C();
c.x = 2;
writeln(c.x);
delete c;
