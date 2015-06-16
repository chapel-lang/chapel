/* feature request: minloc/maxloc reduction over array without zipper

   A minloc reduction currently requires zipping the array with its 
   domain, ie. minloc reduce zip(A, A.domain) instead of just
   minloc reduce A, which would return a tuple with the value and
   the index/indices at the minimum.

   This request is in addition to using a zipper, which has its own
   uses.
*/

var dom : domain(rank=2) = { 1..10, 1..10 };
var a : [dom] int;

for (x,y) in dom do a(x,y) = abs(150 - 3 * (10 * x + y));

var (minval, minpos) = minloc reduce a;
writeln("min ", minval, " at ", minpos);

/*
var (minval, minpos) = minloc reduce zip(a, dom);
*/
