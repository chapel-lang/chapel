// Test building, indexing, iterating for VectorList(int).

use VectorLists;

config const dr = false;
config const s: int = 15;

// test a collection of sz elements
proc test(sz: int) {
  if dr {
    var arr: [0..-1] int;
    help(arr, sz); // generate reference output

  } else {
    const VL = new owned VectorList(int);
    help(VL, sz);
  }
}

proc help(lst, sz) {
  writeln("======== size ", sz);

  writeln("building");
  for i1 in 11..#sz do
    lst.push_back(i1);

  writeln("random indexing");
  for i2 in 0..#sz by -1 do
    write(lst[i2], " ");
  writeln(".");

  writeln("iteration");
  for elm3 in lst do
    write(elm3, " ");
  writeln(".");

  writeln();
}

writeln();
for siz in 0..s do test(siz);
