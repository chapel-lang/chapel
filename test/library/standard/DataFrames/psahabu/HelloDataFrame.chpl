use DataFrames;

// TODO: if columns are homogenous
var columnOne: Series = new TypedSeries(["a", "b", "c", "d", "e"]);
var columnTwo: Series = new TypedSeries([1, 2, 3, 4, 5]);
var columnTree: Series = new TypedSeries([10.0, 20.0, 30.0, 40.0, 50.0]);

var columns = ["columnOne" => columnOne, "columnTwo" => columnTwo, "columnTree" => columnTree];
var idx = new TypedIndex(["rowOne", "rowTwo", "rowTree", "rowFour", "rowFive"]);

var dataFrame = new DataFrame(columns, idx);
writeln(dataFrame);
