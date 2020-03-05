var n = 512;

var all = 1..n;
var odds = all by 2;
var large = all by 64;

writeln(all.size);    //512
writeln(odds.size);     //256
writeln(large.size);    //8
