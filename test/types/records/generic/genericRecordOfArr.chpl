
record R {
  type t;

  proc foo(in x: t) {
    writeln("In foo, got ", x);
  }
}

var myR: R([0..1] int);

myR.foo([4, 5]);
