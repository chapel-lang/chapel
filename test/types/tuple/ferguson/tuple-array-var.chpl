proc t1( ref tup )
{
  tup(0)[0] = 2;
  writeln(tup(1)[0]);
}

proc t2( ref tup )
{
  t1(tup);
}

proc t3( ref args ... )
{
  t2(args);
}

proc t4( ref args ... )
{
  t3( (...args) );
}

proc t5( ref a, ref b )
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

proc reset(ref A)
{
  A = [1,2,3,4];
}

run();

