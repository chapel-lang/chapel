-- This is a test to make sure that local type default
-- values are evaluated each time the type is encountered

def foo(defval: int) {
  typedef myfooint: int;
  var x: myfooint;
  writeln("x is: ", x);
}

def main() {
  foo(2);
  foo(4);
}
