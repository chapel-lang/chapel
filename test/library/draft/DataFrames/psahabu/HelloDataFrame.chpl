use DataFrames;

var validBits = [true, false, true, false, true];

var columnOne: owned Series? = new owned TypedSeries(["a", "b", "c", "d", "e"], validBits);
var columnTwo: owned Series? = new owned TypedSeries([1, 2, 3, 4, 5], validBits);
var columnThree: owned Series? = new owned TypedSeries([10.0, 20.0, 30.0, 40.0, 50.0]);

var columns = ["columnOne" => columnOne, "columnTwo" => columnTwo, "columnThree" => columnThree];
var idx = new shared TypedIndex(["rowOne", "rowTwo", "rowThree", "rowFour", "rowFive"]);

var dataFrame = new owned DataFrame(columns, idx);
var noIndex = new owned DataFrame(columns);
writeln(dataFrame);
writeln();
writeln(dataFrame["columnThree"]);
writeln();
writeln(noIndex);
