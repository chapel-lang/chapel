var A = [1,2,3,4];

proc t1( const tup )
{
  A[1] = 2;
  writeln(tup(1)[1]);
}

proc t2( const tup )
{
  t1(tup);
}

proc t3( const args ... )
{
  t2(args);
}

proc t4( const args ... )
{
  t3( (...args) );
}

proc t5( const a, const b )
{
  t4(a, b);
}

proc run()
{

  reset();
  t5(A, A);

  reset();
  t4(A, A);

  reset();
  t3(A, A);

  reset();
  t2( (A, A) );

  reset();
  t1( (A, A) );
}

proc reset()
{
  A = [1,2,3,4];
}

run();

