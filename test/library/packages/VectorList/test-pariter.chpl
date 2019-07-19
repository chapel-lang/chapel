// Parallel iteration for VectorList(int).

use VectorLists;

config param dr = false;
config const s: int = 31;

// test a collection of sz elements
proc test(sz: int) {
  if dr {
    var arr: [0..-1] int;
    help(arr, sz); // generate reference output

  } else {
    const VL = new owned VectorList(int,1); // "1" for more thorough testing
    help(VL.borrow(), sz);
  }
}

proc help(lst, sz) {
  writeln("======== size ", sz, "  ", isArray(lst));
  const datarange = 1..#sz;

  writeln("building");
  for i1 in datarange do
    lst.push_back(i1);

  var bud: [0..#sz] int = datarange;

  writeln("parallel leading");
  forall (d,b) in zip(lst,bud) do
    assert(b==d);
  writeln("done");

  writeln("parallel following");
  forall (b,d) in zip(bud,lst) do
    assert(b==d);
  writeln("done");

 if !dr {
  writeln("converting");
  const DR = lst.toArray();
  assert(DR == datarange);
  writeln("done");
 }
}

writeln();
for siz in 0..s do test(siz);
test(1234);
