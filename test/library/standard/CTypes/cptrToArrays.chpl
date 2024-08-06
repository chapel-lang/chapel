use CTypes;

var Arr1D: [1..10] int;
var Arr2D: [1..10, 1..10] int;

proc test(ref x) {
  // note that this test has noexec
  writeln(c_ptrTo(x));
  writeln(c_ptrToConst(x));
  writeln(c_addrOf(x));
  writeln(c_addrOfConst(x));
}

// we want to make sure that all of the following resolves
test(Arr1D);
test(Arr1D[1..10]);
test(Arr2D[1, ..]);
test(Arr2D[.., 1]);
