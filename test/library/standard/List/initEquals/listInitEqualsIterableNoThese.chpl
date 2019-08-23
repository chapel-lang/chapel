private use Lists;

//
// Define a new type that implements the "these" iterator.
//
record fooList {
  type eltType;
  var lst: list(eltType);

  proc append(in x: eltType) {
    lst.append(x);
  }
}

//
// Initialize a list from an arbitrary iterable type.
//
{
  var foo: fooList(int);
  
  for x in 1..8 do
    foo.append(x);

  var lst: list(int) = foo;

  for (x, y) in zip(foo, lst) do
    assert(x == y);
}
