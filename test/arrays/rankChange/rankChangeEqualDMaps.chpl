use BlockDist;

proc test(a, b) {
  writeln(a.domain.dist == b.domain.dist);
}

var space = {0..9, 0..9};
var offsetSpace = {10..19, 10..19};

var dom = space dmapped Block(space);
var offsetDom = space dmapped Block(offsetSpace);

var arr: [dom] int;
var offsetArr: [offsetDom] int;

ref arrRC1 = arr[1, ..];
ref arrRC2 = arr[1, ..];

// these all should be equal
test(arr[1, ..], arr[1, ..]);
test(arr[1, ..], arr[2, ..]);
test(arr[.., 9], arr[.., 9]);
test(arr[.., 8], arr[.., 9]);

// even funky ones should be equal, because all we care is the distribution
test(arr[1, ..], arr[.., 8]);

// these all should be equal
test(offsetArr[1, ..], offsetArr[1, ..]);
test(offsetArr[1, ..], offsetArr[2, ..]);
test(offsetArr[.., 9], offsetArr[.., 9]);
test(offsetArr[.., 8], offsetArr[.., 9]);

// even funky ones should be equal, because all we care is the distribution
test(offsetArr[1, ..], offsetArr[.., 8]);

// these all should be not equal
test(offsetArr[1, ..], arr[1, ..]);
test(offsetArr[1, ..], arr[2, ..]);
test(offsetArr[.., 9], arr[.., 9]);
test(offsetArr[.., 8], arr[.., 9]);

// even funky ones should be equal, because all we care is the distribution
test(offsetArr[1, ..], arr[.., 8]);


