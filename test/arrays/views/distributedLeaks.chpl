use BlockDist;


config const n = 10;
const space = {1..n, 1..n};
const dom: domain(2) dmapped Block(space) = space;
var arr: [dom] int;

var viewRankChange = arr[1, 3..4];
writeln(viewRankChange.size);

var viewSlice = arr[1..3, 4..5];
writeln(viewSlice.size);


const reindexSpace = {0..#n, 0..#n};
var viewReindex = arr.reindex(space);
writeln(viewReindex.size);
