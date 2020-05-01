extern proc chpl_cache_print();
extern proc printf(fmt: c_string, vals...?numvals): int;

config var n: int = 8;

var P: [LocaleSpace] atomic int;
//var D: domain(1) dmapped new dmap(new Block(boundingBox={1..n})) = {1..n};
//var A: [D] int;

//startCommDiagnostics();
//coforall loc in Locales do on loc {
for loc in Locales do on loc {
  var id = loc.id;
  var v = 100+loc.id;
  var got = 0;
  P(id).write(v);
  got = P(id).read();
  //printf("on %i wrote %i\n", id:int(32), v:int(32));
  //printf("on %i read %i\n", id:int(32), got:int(32));
  assert( got == v );
}
//stopCommDiagnostics();

if numLocales > 0 {
  //writeln("locale 0 about to read");
  var x = P(1).read();
  //writeln("locale 0 read from locale 1: ", x);
  assert( x == 101 );
  on Locales[1] {
    //writeln("locale 1 about to read");
    var y = P(1).read();
    //writeln("locale 1 read from locale 1: ", y);
    assert( y == 101 );
  }
  //writeln("locale 0 about to read");
  x = P(1).read();
  //writeln("locale 0 read from locale 1: ", x);
  assert( x == 101 );
}

