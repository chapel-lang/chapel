use Dataframes;

var I = new TypedIndex(["A", "B", "C", "D", "E"]);

var oneDigit = new TypedSeries([1, 2, 3, 4, 5], I);
writeln("initial:");
writeln(oneDigit);
writeln();

writeln("less than 3:");
var ltThree = oneDigit < 3;
writeln(ltThree);
ltThree = 3 > oneDigit;
writeln(ltThree);
writeln();

writeln("greater than 3:");
var gtThree = oneDigit > 3;
writeln(gtThree);
gtThree = 3 < oneDigit;
writeln(gtThree);
writeln();

writeln("equal to 3:");
var eqThree = oneDigit == 3;
writeln(eqThree);
eqThree = 3 == oneDigit;
writeln(eqThree);
writeln();

writeln("less than or equal to 3:");
var lteqThree = oneDigit <= 3;
writeln(lteqThree);
lteqThree = 3 >= oneDigit;
writeln(lteqThree);
writeln();

writeln("greater than or equal to 3:");
var gteqThree = oneDigit >= 3;
writeln(gteqThree);
gteqThree = 3 <= oneDigit;
writeln(gteqThree);
