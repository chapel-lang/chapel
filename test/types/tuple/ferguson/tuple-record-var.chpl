record R {
  var x:int;
}

var g:R;

proc t1( tup )
{
  g.x = 2;
  writeln(tup(1).x);
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
  var t = (g, g);

  reset();
  t5( (...t) );

  reset();
  t4( (...t) );

  reset();
  t3( (...t) );

  reset();
  t2( t );

  reset();
  t1( t );
}

proc reset()
{
  g.x = 1;
}

run();

