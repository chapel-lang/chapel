use DataFrames;

var validBits = [true, false, true, false, true];

var columnOne: unmanaged Series = new unmanaged TypedSeries(["a", "b", "c", "d", "e"], validBits);
var columnTwo: unmanaged Series = new unmanaged TypedSeries([1, 2, 3, 4, 5], validBits);
var columnThree: unmanaged Series = new unmanaged TypedSeries([10.0, 20.0, 30.0, 40.0, 50.0]);

var columns = ["columnOne" => columnOne, "columnTwo" => columnTwo, "columnThree" => columnThree];
var idx = new unmanaged TypedIndex(["rowOne", "rowTwo", "rowThree", "rowFour", "rowFive"]);

var dataFrame = new unmanaged DataFrame(columns, idx);
var noIndex = new unmanaged DataFrame(columns);
writeln(dataFrame);
writeln();
writeln(dataFrame["columnThree"]);
writeln();
writeln(noIndex);
