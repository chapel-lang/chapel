proc foo (x$: sync int = 3) {
  var val = x$.readFE();
  writeln(val);
}

foo();
