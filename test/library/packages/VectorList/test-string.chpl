// Test building, indexing, iterating for VectorList(string).

use VectorLists;

config param dr = false;
config const s: int = 31;
writeln();

if dr {
  var arr: [0..-1] string;
  help(arr, s); // generate reference output

} else {
  const VL = new owned VectorList(string);
  help(VL, s);
}

proc help(lst, sz) {
  writeln("======== size ", sz, "  ", isArray(lst));

  writeln("building");
  for i1 in 11..#sz do
    lst.push_back("e"+i1:string);

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
