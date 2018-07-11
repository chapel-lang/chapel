use DataFrames;

var noTrue = new TypedSeries([false, false, false]);
var oneTrue = new TypedSeries([false, true, false]);
var allTrue = new TypedSeries([true, true, true]);

writeln("noTrue:");
writeln(noTrue);
writeln();
writeln("oneTrue:");
writeln(oneTrue);
writeln();
writeln("allTrue:");
writeln(allTrue);

writeln();
writeln("noTrue.and(): " + noTrue.and());
writeln();
writeln("oneTrue.and(): " + oneTrue.and());
writeln();
writeln("allTrue.and(): " + allTrue.and());

writeln();
writeln("noTrue.or(): " + noTrue.or());
writeln();
writeln("oneTrue.or(): " + oneTrue.or());
writeln();
writeln("allTrue.or(): " + allTrue.or());
