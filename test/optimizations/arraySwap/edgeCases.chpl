use BlockDist;

proc test(d1, d2) {
  var a1: [d1] int;
  var a2: [d2] int;

  a1 = 10;
  a2 = 20;

  a1 <=> a2;

  writeln(a1);
  writeln(a2);
}


{
  // simple case: everything should be optimized
  var d1 = {1..10} dmapped Block({1..10});
  var d2 = {1..10} dmapped Block({1..10});

  test(d1, d2);
}


{
  // one array is unevenly distributed. block shouldn't optimize
  var d1 = {0..9} dmapped Block({1..10});
  var d2 = {1..10} dmapped Block({1..10});

  test(d1, d2);
}

