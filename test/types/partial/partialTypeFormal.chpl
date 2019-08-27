
config param error : bool = false;

record R {
  type T;
  param n : int;
}

proc foo(type T, x : T(int)) {
  writeln(x.type:string, ": ", x);
}

proc main() {
  var x = new R(int, 5);
  foo(R, x);

  if error {
    var y = new R(real, 5);
    foo(R, y);
  }
}
