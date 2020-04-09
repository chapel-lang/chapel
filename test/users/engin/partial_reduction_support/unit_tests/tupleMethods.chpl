use utilities;
use dsiMethods;
use templates;

use utilities;

var t = (10,20,30,40,50);

writeln(t);
writeln(t.withoutIdx(3));
writeln(t.withIdx(5,60));


// test with ranges
var tRange = (10..100, 20..200, 30..300);
writeln(tRange);
writeln(tRange.withoutIdx(1));
writeln(tRange.withIdx(2,60..600));

