
var g = [1,2,3,4];

var tmp = (g,g);

proc t1( tup: tmp.type )
{
  g[0] = 2;
  writeln(tup(1)[0]);
}

proc t2( tup: tmp.type )
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

proc t5( a:g.type, b:g.type )
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
  g[0] = 1;
}

run();

