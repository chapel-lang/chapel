proc myfunc() {
  var x = 3;
  var f = lambda() { writeln("f: x=", x, "  assigning 4"); x = 4; };
  var g = lambda() { writeln("g: x=", x, "  assigning 5"); x = 5; };
  return (f,g);
}

var (a,b) = myfunc();

a();
b();
a();
b();
