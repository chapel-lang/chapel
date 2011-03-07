proc foo(i: int) param {
  var ii: int;
  while ii < i {
    ii += 1;
  }
  return 2;
}

writeln(foo(3));
