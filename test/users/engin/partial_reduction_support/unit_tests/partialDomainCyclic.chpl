use utilities;
use dsiMethods;
use templates;

use CyclicDist;

config const N = 10;

const Space = {10..#N, 20..#N, 30..#N};
const Dom = Space dmapped Cyclic(Space.low);

const pDom1 = Dom._value.dsiPartialDomain(0);
const pDom2 = Dom._value.dsiPartialDomain(1);
const pDom3 = Dom._value.dsiPartialDomain(2);

writeln(pDom1);
writeln(pDom2);
writeln(pDom3);
writeln();

var testArr1: [pDom1] int;
var testArr2: [pDom2] int;
var testArr3: [pDom3] int;

forall v in testArr1 do v = v.locale.id;
forall v in testArr2 do v = v.locale.id;
forall v in testArr3 do v = v.locale.id;

writeln(testArr1);
writeln();
writeln(testArr2);
writeln();
writeln(testArr3);
