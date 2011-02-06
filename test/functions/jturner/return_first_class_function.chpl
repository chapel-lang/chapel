use Functions;

proc make_fun() : func(func(int)) {
  return lambda() { return (lambda() { return 3; }); };
}

var g = make_fun();
var h = g();
writeln(h());
