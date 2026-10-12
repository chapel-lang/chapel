use Print;

record R {
  type T;
  var x : T;
}

proc main() {
  var x = new R(int, 5);
  var y = new R(real, 42.0);
  println(x.x);
  println(y.x);

  type T = R(int);
  var z : T;
  println(z.x);
}
