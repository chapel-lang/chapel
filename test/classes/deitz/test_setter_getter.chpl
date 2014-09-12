class C {
  var x: int;
  proc x ref {
    if setter then
      writeln("setter invoked");
    else
      writeln("getter invoked");
    return x;
  }
}

var c = new C();
c.x = 2;
writeln(c.x);
delete c;
