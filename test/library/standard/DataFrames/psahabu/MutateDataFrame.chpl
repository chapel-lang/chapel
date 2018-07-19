use DataFrames;

var df = new unmanaged DataFrame();

var validBits = [true, false, true, false, true];
var columnOne: unmanaged Series = new unmanaged TypedSeries(["a", "b", "c", "d", "e"], validBits);
var columnTwo: unmanaged Series = new unmanaged TypedSeries([1, 2, 3, 4, 5], validBits);
var columnThree: unmanaged Series = new unmanaged TypedSeries([10.0, 20.0, 30.0, 40.0, 50.0]);

writeln(columnOne);
writeln();
writeln(columnTwo);
writeln();
writeln(columnThree);
writeln();

df.insert("columnOne", columnOne);
df.insert("columnTwo", columnTwo);
df.insert("columnThree", columnThree);

var idx = new unmanaged TypedIndex(["rowOne", "rowTwo", "rowThree", "rowFour", "rowFive"]);
df.reindex(idx);

writeln(df);
