
record R {
  proc foo {
    return 1;
  }
}

var a: [1..10] R;

writeln(a.foo());
