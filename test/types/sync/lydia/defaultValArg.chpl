proc foo (x$: sync int = 3) {
  var val = x$;
  writeln(val);
}

foo();
