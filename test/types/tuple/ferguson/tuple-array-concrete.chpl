
var g = [1,2,3,4];

var tmp = (g,g);

proc t1( const tup: tmp.type )
{
  g[1] = 2;
  writeln(tup(1)[1]);
}

proc t2( const tup: tmp.type )
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

proc t5( const a:g.type, const b:g.type )
{
  t4(a, b);
}

proc run()
{

  reset();
  t5( g, g );

  reset();
  t4( g, g );

  reset();
  t3( g, g );

  reset();
  t2( (g, g) );

  reset();
  t1( (g, g) );
}

proc reset()
{
  g[1] = 1;
}

run();

