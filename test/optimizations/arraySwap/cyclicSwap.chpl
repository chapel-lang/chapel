use CyclicDist;

proc test(d1, d2) {
  var a1: [d1] int;
  var a2: [d2] int;

  a1 = 10;
  a2 = 20;

  writeln("'Forward' swap");
  a1 <=> a2;
  writeln(a1);
  writeln(a2);
  writeln();

  writeln("'Backward' swap");
  a2 <=> a1;
  writeln(a1);
  writeln(a2);
  writeln();
}

writeln("*** Cyclic ***");
{
  writeln("simple case: everything should be optimized");
  var d1 = {1..10} dmapped new cyclicDist(1);
  var d2 = {1..10} dmapped new cyclicDist(1);

  test(d1, d2);
  
  writeln();
}

{
  writeln("shapes match and domains are aligned with their distribions but are different. We decided not to optimized this for now");
  var d1 = {1..10} dmapped new cyclicDist(1);
  var d2 = {0..9} dmapped new cyclicDist(0);

  test(d1, d2);
  
  writeln();
}


{
  writeln("arrays are not distributed the same across locales; shouldn't optimize");
  var d1 = {0..9} dmapped new cyclicDist(1);
  var d2 = {1..10} dmapped new cyclicDist(1);

  test(d1, d2);

  writeln();
}

{
  writeln("both are offset from startIdx, but they align with each other");
  var d1 = {0..9} dmapped new cyclicDist(1);
  var d2 = {0..9} dmapped new cyclicDist(1);

  test(d1, d2);

  writeln();
}

{
  // this case can technically be optimized but it isn't today, mostly because
  // abundance of caution
  writeln("Alignment is circumstantial / non-obvious / specific to two locales");
  var d1 = {0..9} dmapped new cyclicDist(1);
  var d2 = {2..11} dmapped new cyclicDist(1);

  test(d1, d2);

  writeln();
}

{
  writeln("both are strided and aligned");
  var d1 = {0..9 by 2} dmapped new cyclicDist(1);
  var d2 = {0..9 by 2} dmapped new cyclicDist(1);

  test(d1, d2);

  writeln();
}

{
  writeln("both are strided, have same number of elements but not aligned, shouldn't optimize");
  var d1 = {0..9 by 2} dmapped new cyclicDist(1);
  var d2 = {1..10 by 2} dmapped new cyclicDist(1);

  test(d1, d2);

  writeln();
}

{
  // this case can technically be optimized but it isn't today, mostly because
  // abundance of caution
  writeln("both are strided and misaligned");
  var d1 = {0..9 by 2 align 0} dmapped new cyclicDist(0);
  var d2 = {0..9 by 2 align 1} dmapped new cyclicDist(1);

  test(d1, d2);

  writeln();
}
writeln();

writeln("*** Cross type ***");
{
  writeln("Cross type swaps shouldn't be optimized");
  var d1 = {1..10} dmapped new cyclicDist(1);
  var d2 = {1..10};

  test(d1, d2);
  
  writeln();
}


{
  writeln("Cross type swaps shouldn't be optimized");
  var d1 = {0..9} dmapped new cyclicDist(1);
  var d2 = {1..10};

  test(d1, d2);

  writeln();
}

{
  writeln("Cross type swaps shouldn't be optimized");
  var d1 = {0..9} dmapped new cyclicDist(1);
  var d2 = {0..9};

  test(d1, d2);

  writeln();
}

{
  writeln("Cross type swaps shouldn't be optimized");
  var d1 = {0..9 by 2} dmapped new cyclicDist(1);
  var d2 = {0..4};

  test(d1, d2);

  writeln();
}

{
  writeln("Cross type swaps shouldn't be optimized");
  var d1 = {0..9 by 2} dmapped new cyclicDist(1);
  var d2 = {0..9 by 2};

  test(d1, d2);

  writeln();
}

{
  writeln("both are strided, have same number of elements but not aligned, shouldn't optimize");
  var d1 = {0..9 by 2} dmapped new cyclicDist(1);
  var d2 = {1..10 by 2};

  test(d1, d2);

  writeln();
}

{
  // this case can technically be optimized but it isn't today, mostly because
  // abundance of caution
  writeln("both are strided and misaligned");
  var d1 = {0..9 by 2 align 1} dmapped new cyclicDist(1);
  var d2 = {0..9 by 2 align 1};

  test(d1, d2);

  writeln();
}
writeln();
