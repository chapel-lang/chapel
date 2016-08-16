use BlockDist;

config const N = 10;
var space = {0..#N, 0..#N, 0..#N};
var dom = space dmapped Block(space);

/*var topDom = dom._value.dsiPartialDomain(1);*/
/*var leftDom = dom._value.dsiPartialDomain(2);*/

/*var topArr: [topDom] int;*/
/*var leftArr: [leftDom] int;*/

/*forall v in topArr do v = here.id;*/
/*forall v in leftArr do v = here.id;*/

/*writeln(topArr);*/
/*writeln(leftArr);*/

/*writeln(dom._value.__partialTheseLocales(1, (2,7)));*/
/*writeln(dom._value.__partialTheseLocales(2, (8,1)));*/
/*writeln(dom._value.__partialTheseLocales(3, (1,1)));*/

forall i in dom._value.dsiPartialThese(1, (2,7)) do writeln(i);
