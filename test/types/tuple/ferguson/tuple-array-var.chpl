proc t1( tup )
{
  tup(1)[1] = 2;
  writeln(tup(2)[1]);
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

  var t = (A,A);

  reset(A);
  t5( (...t) );

  reset(A);
  t4( (...t) );

  reset(A);
  t3( (...t) );

  reset(A);
  t2( t );

  reset(A);
  t1( t );
}

proc reset(A)
{
  A = [1,2,3,4];
}

run();

