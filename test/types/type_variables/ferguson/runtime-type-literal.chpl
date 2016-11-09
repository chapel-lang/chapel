proc run() {
  type arrType = [{1}] real;

  var a:arrType;

  // now a.domain should be {1}
  writeln(a.domain);
  writeln(a);
}

run();
