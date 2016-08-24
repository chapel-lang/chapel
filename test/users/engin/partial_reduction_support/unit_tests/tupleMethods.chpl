use utilities;

var t = (10,20,30,40,50);

writeln(t);
writeln(t.strip(4));
writeln(t.merge(6,60));


// test with ranges
var tRange = (10..100, 20..200, 30..300);
writeln(tRange);
writeln(tRange.strip(2));
writeln(tRange.merge(3,60..600));

