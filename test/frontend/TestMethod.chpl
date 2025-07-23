use Print;

record R {
  var x: int;
}

proc R.foo(arg: int) {
  println(arg*2);
}

proc main() {
  var r : R;
  r.foo(42);
}
