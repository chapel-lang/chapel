use utilities;
use dsiMethods;
use templates;

use utilities;

var t = (10,20,30,40,50);

writeln(t);
writeln(t.withoutIdx(4));
writeln(t.withIdx(6,60));


// test with ranges
var tRange = (10..100, 20..200, 30..300);
writeln(tRange);
writeln(tRange.withoutIdx(2));
writeln(tRange.withIdx(3,60..600));

