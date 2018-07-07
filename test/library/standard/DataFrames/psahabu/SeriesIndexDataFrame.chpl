use DataFrames;

var validBits = [true, false, true, false, true];

var columnOne: Series = new TypedSeries(["a", "b", "c", "d", "e"], validBits);
var columnTwo: Series = new TypedSeries([1, 2, 3, 4, 5], validBits);
var columnThree: Series = new TypedSeries([10.0, 20.0, 30.0, 40.0, 50.0]);

var columns = ["columnOne" => columnOne, "columnTwo" => columnTwo, "columnThree" => columnThree];
var idx = new TypedIndex(["rowOne", "rowTwo", "rowThree", "rowFour", "rowFive"]);

var dataFrame = new DataFrame(columns, idx);
var noIndex = new DataFrame(columns);
var dataFrameSet = dataFrame.set_index(real, "columnThree");
var noIndexSet = noIndex.set_index(real, "columnThree");

writeln(dataFrame);
writeln();
writeln(dataFrameSet);
writeln();
writeln(noIndex);
writeln();
writeln(noIndexSet);

delete columns, idx;
delete dataFrame.idx, dataFrame.columns, dataFrame;
delete noIndex.idx, noIndex.columns, noIndex;
delete dataFrameSet.idx, dataFrameSet.columns, dataFrameSet;
delete noIndexSet.idx, noIndexSet.columns, noIndexSet;
