def make_fun() : func(func(int)) {
//def make_fun() {
  return lambda() { return (lambda() { return 3; }); };
}

var g = make_fun();
var h = g();
writeln(h());
