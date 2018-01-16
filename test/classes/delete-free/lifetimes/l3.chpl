record A {
  var x:int;
  var y:real;
}

pragma "safe"
proc identity(const ref x) const ref {
  return x;
}

pragma "safe"
proc bad_two_args(x:int, y:real) const ref {
  return identity(x);
}

pragma "safe"
proc bad_record_arg(in arg:A) const ref {
  return arg.x;
}

pragma "safe"
proc return_record_arg(arg:A) const ref {
  return arg.x;
}

pragma "safe"
proc bad_return_return_record_arg() const ref {
  var aa = new A(1, 2.0);
  return return_record_arg(aa);
}

pragma "safe"
proc test() {
  var oneI = 1;
  var twoR = 2.0;
  const ref a = bad_two_args(1, 2.0);
  var aa = new A(1, 2.0);
  const ref b = bad_record_arg(aa);
  const ref c = bad_return_return_record_arg();
}

test();
