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

test(arr[1, ..], arr[1, ..]);  // true
test(arr[1, ..], arr[2, ..]);  // false
test(arr[.., 9], arr[.., 9]);  // true
test(arr[.., 8], arr[.., 9]);  // false

test(arr[1, ..], arr[.., 8]);  // false

test(offsetArr[1, ..], offsetArr[1, ..]);  // true
test(offsetArr[1, ..], offsetArr[2, ..]);  // false
test(offsetArr[.., 9], offsetArr[.., 9]);  // true
test(offsetArr[.., 8], offsetArr[.., 9]);  // false

test(offsetArr[1, ..], offsetArr[.., 8]);  // false

test(offsetArr[1, ..], arr[1, ..]);   // false
test(offsetArr[1, ..], arr[2, ..]);   // false
test(offsetArr[.., 9], arr[.., 9]);   // false
test(offsetArr[.., 8], arr[.., 9]);   // false

test(offsetArr[1, ..], arr[.., 8]);  // false


