use Memory;

var u = memoryUsed();

def foo() {
  var s: seq(int) = _seq(1, 2, 3, 4, 5);
}

foo();

var uu = memoryUsed();

writeln(uu-u);
