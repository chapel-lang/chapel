config var usercfg = true;

// This should be an illegal where clause. Argument z is not a param.
proc foo(x, y, z) where z {
  writeln(x, " ", y);
}

// This should be an illegal where clause. Argument z is not a param.
proc foo(x, y, z) where !z {
  writeln("Not z, ", x, " ", y);
}

foo("hello", "hello", usercfg);

