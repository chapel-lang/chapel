use Print;

record R {
  var x: int;
}

proc R.foo(arg: int) {
  println(arg*2);
}

proc R.implicit() {
  foo(7);
}

proc main() {
  var r : R;
  r.foo(42);
  r.implicit();
}
