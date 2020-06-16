
config const pow = 14;

proc main() {
  var D : domain(string);
  var A : [D] int;

  D += "foo";
  D += "bar";

  A["foo"] = 4;
  A["bar"] = 5;

  for i in 3..pow {
    const cap = 2**i;

    test(cap);

    D.requestCapacity(cap);
    verify(A, cap);
  }
}

proc test(cap : int) {
  var D : domain(string);
  var A : [D] int;

  D += "foo";
  D += "bar";

  A["foo"] = 4;
  A["bar"] = 5;

  D.requestCapacity(cap);
  verify(A, cap);
}

proc verify(A : [], cap : int) {

  const minSize = 2 * (cap + 1);
  const correct = A.domain._value.table.tableSize > cap &&
                  A.size == 2 &&
                  A["foo"] == 4 &&
                  A["bar"] == 5;

  if !correct then halt("FAILURE for capacity ", cap);
}
