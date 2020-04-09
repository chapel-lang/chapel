// This is a test to make sure that local type default
// values are evaluated each time the type is encountered

proc foo(defval: int) {
  type myfooint = int;
  var x: myfooint;
  writeln("x is: ", x);
}

proc main() {
  foo(2);
  foo(4);
}
