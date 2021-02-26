extern proc chpl_cache_print();
extern proc printf(fmt: c_string, vals...?numvals): int;

use CPtr;
use BlockDist;

config var n: int = 8;


const D = LocaleSpace dmapped Block(boundingBox=LocaleSpace);
var P: [D] atomic int;
//var D: domain(1) dmapped new dmap(new Block(boundingBox={1..n})) = {1..n};
//var A: [D] int;

//startCommDiagnostics();
//coforall loc in Locales do on loc {
for loc in Locales do on loc {
  var id = loc.id;
  var v = 100+loc.id;
  var got = 0;
  var lo = P(id).locale.id;
  var ptr = c_ptrTo(P(id));
  //printf("on %i stored on %i at %p\n", id:int(32), lo:int(32), ptr);
  P(id).write(v);
  got = P(id).read();
  //printf("on %i wrote %i\n", id:int(32), v:int(32));
  //printf("on %i read %i\n", id:int(32), got:int(32));
  assert( got == v );
  //chpl_cache_print();
}
//stopCommDiagnostics();

if numLocales > 0 {
  //printf("locale 0 about to read %i\n", 1:int(32));
  //chpl_cache_print();
  var x = P(1).read();
  //printf("locale 0 read from locale 1: %i\n", x:int(32));
  //chpl_cache_print();
  assert( x == 101 );

  on Locales[1] {
    //printf("locale 1 about to read %i\n", 1:int(32));
    var y = P(1).read();
    //printf("locale 1 read from locale 1: %i\n", y:int(32));
    assert( y == 101 );
  }

  //chpl_cache_print();
  //printf("locale 0 about to read %i\n", 1:int(32));
  x = P(1).read();
  //printf("locale 0 read from locale 1: %i\n", x:int(32));
  //chpl_cache_print();
  assert( x == 101 );
}

