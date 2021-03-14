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
  reset();
  t5(g, g);

  reset();
  t4(g, g);

  reset();
  t3(g, g);

  reset();
  t2( (g, g) );

  reset();
  t1( (g, g) );
}

proc reset()
{
  g.x = 1;
}

run();

