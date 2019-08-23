private use List;

config const testIters = 8;

//
// Define a sample type that has an iterator.
//
record container {
  type eltType;
  var lst: list(eltType);

  proc append(in x: eltType) {
    lst.append(x);
  }

  iter these() {
    for x in lst do
      yield x;
  }
}

var ds: container(int);

for i in 1..testIters do
  ds.append(i);

var lst = new list(ds);

for (x, y) in zip(ds, lst) do
  assert(x == y);

