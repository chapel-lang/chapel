record R {
  var x: int;
  proc postinit() {
    x = 7;
    writeln("postinit: ", x);
  }
}

const cr: R;
writeln(cr);
