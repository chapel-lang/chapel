proc makeit(type t) {
  var x: t;
  return x;
}

proc main() {
  var A:[1..3] int;
  var x = makeit( (A.type, A.type) );
  writeln(x(0));
  writeln(x(1));
}
