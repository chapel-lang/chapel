proc t1( tup )
{
  tup(0)[1] = 2;
  writeln(tup(1)[1]);
}

proc t2( tup )
{
  t1(tup);
}

proc t3( args ... )
{
  t2(args);
}

proc t4( args ... )
{
  t3( (...args) );
}

proc t5( a, b )
{
  t4(a, b);
}

proc run()
{
  var A = [1,2,3,4];

  reset(A);
  t5(A, A);

  reset(A);
  t4(A, A);

  reset(A);
  t3(A, A);

  reset(A);
  t2( (A, A) );

  reset(A);
  t1( (A, A) );
}

proc reset(A)
{
  A = [1,2,3,4];
}

run();

