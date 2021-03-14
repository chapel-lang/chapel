
class C {
  var x: int;
}

type tq = borrowed C?;
var bq:borrowed C? = nil;

// how is bq : tq ! intepreted?
//   bq : (tq!)   ->  throwing cast to non-nilable type
//   (bq : tq) !  ->  no-op cast and then halt if nil
try {
  writeln(bq:tq!);  // expecting compilation failure (can't apply ! to type)
} catch e {
  writeln("case 2 error as expected");
}
