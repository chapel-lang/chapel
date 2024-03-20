record R {
  var x: int;
  proc ref postinit() {
    x = 7;
    writeln("postinit: ", x);
  }
}

const cr: R;
writeln(cr);
