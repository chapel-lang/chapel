extern record rec {
  var x: int;
}

export proc foo(in arg: rec) {
  writeln(arg.x);
}
