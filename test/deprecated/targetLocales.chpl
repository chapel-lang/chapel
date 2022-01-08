// TODO: .targetLocales should be available on DefaultDist as well
// so we should be able to do this without involving BlockDist.

use BlockDist;

const D = newBlockDom({1..3, 1..4});
var A: [D] int;

writeln(A.targetLocales());
writeln(D.targetLocales());
writeln(D.dist.targetLocales());

// ensure we do not issue duplicate warnings
// when using the result of targetLocales()

proc ident(arg) return arg;

ident(A.targetLocales());
writeln(ident(A.targetLocales()));

var tl1 = A.targetLocales();
writeln(tl1);
var tl2 = ident(tl1);
writeln(tl2);
