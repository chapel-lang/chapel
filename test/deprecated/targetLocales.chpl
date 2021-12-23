// TODO: .targetLocales should be available on DefaultDist as well
// so we should be able to do this without involving BlockDist.

use BlockDist;

const D = newBlockDom({1..3, 1..4});
var A: [D] int;

writeln(A.targetLocales());
writeln(D.targetLocales());
writeln(D.dist.targetLocales());
