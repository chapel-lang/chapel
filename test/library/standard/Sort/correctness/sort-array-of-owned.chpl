use Sort;

config const n = 100;
config param useKeyComparator = true;

class Value {
  var x: int;
}


record keyValueComparator {
  proc key(v: Value?) {
    if v == nil then
      return 0;
    else
      return v!.x;
  }
}
record compareValueComparator: relativeComparator {
  proc compare(a: Value?, b: Value?) {
    var aNum = 0;
    if a != nil then
      aNum = a!.x;
    var bNum = 0;
    if b != nil then
      bNum = b!.x;
    return aNum - bNum;
  }
}
proc ValueComparator type do
  return if useKeyComparator then keyValueComparator else compareValueComparator;

// TEST 1: SORTING OWNED ELEMENTS
{
  writeln("Testing sorting owned");
  var A: [1..n] owned Value?;
  var comparator = new ValueComparator();
  for i in 1..n {
    A[i] = new owned Value(n-i);
  }
  sort(A, comparator=comparator);
  if n <= 20 then writeln(A);
  for i in 1..n {
    if A[i] == nil then halt("nil was introduced");
  }
  assert(isSorted(A, comparator));
}

// TEST 2: SORTING RECORD CONTAINING OWNED ELEMENTS

record Wrapper {
  var elt: owned Value?;
}
proc Wrapper.init=(ref other: Wrapper) {
  this.elt = other.elt;
}
operator Wrapper.=(ref lhs: Wrapper, ref rhs: Wrapper) {
  lhs.elt = rhs.elt;
}

record keyWrapperComparator {
  proc key(v: Wrapper) {
    if v.elt == nil then
      return 0;
    else
      return v.elt!.x;
  }
}
record compareWrapperComparator: relativeComparator {
  proc compare(a: Wrapper, b: Wrapper) {
    var aNum = 0;
    if a.elt != nil then
      aNum = a.elt!.x;
    var bNum = 0;
    if b.elt != nil then
      bNum = b.elt!.x;
    return aNum - bNum;
  }

}
proc WrapperComparator type do
  return if useKeyComparator then keyWrapperComparator else compareWrapperComparator;

{
  writeln("Testing sorting record containing owned");
  var A: [1..n] Wrapper;
  var comparator = new WrapperComparator();
  for i in 1..n {
    A[i] = new Wrapper(new owned Value(n-i));
  }
  sort(A, comparator=comparator);
  if n <= 20 then writeln(A);
  for i in 1..n {
    if A[i].elt == nil then halt("nil was introduced");
  }
  assert(isSorted(A, comparator));
}

// TEST 3: SORTING RECORD CONTAINING ARRAY OF OWNED ELEMENTS

record ArrayWrapper {
  var elts: [1..1] owned Value?;
  proc init() {
    init this;
  }
  proc init(in arg: owned Value) {
    init this;
    elts[1] = arg;
  }
}
record keyArrayWrapperComparator {
  proc key(v: ArrayWrapper) {
    if v.elts[1] == nil then
      return 0;
    else
      return v.elts[1]!.x;
  }
}
record compareArrayWrapperComparator: relativeComparator {
  proc compare(a: ArrayWrapper, b: ArrayWrapper) {
    var aNum = 0;
    if a.elts[1] != nil then
      aNum = a.elts[1]!.x;
    var bNum = 0;
    if b.elts[1] != nil then
      bNum = b.elts[1]!.x;
    return aNum - bNum;
  }
}
proc ArrayWrapperComparator type do
  return if useKeyComparator then keyArrayWrapperComparator
                             else compareArrayWrapperComparator;

{
  writeln("Testing sorting record containing array of owned");
  var A: [1..n] ArrayWrapper;
  var comparator = new ArrayWrapperComparator();
  for i in 1..n {
    A[i] = new ArrayWrapper(new owned Value(n-i));
  }
  sort(A, comparator=comparator);
  if n <= 20 then writeln(A);
  for i in 1..n {
    if A[i].elts[1] == nil then halt("nil was introduced");
  }
  assert(isSorted(A, comparator));
}

// TEST 4: SORTING ARRAY CONTAINING ARRAY OF OWNED ELEMENTS

record keyArrayComparator {
  proc key(v: [] owned Value?) {
    if v[1] == nil then
      return 0;
    else
      return v[1]!.x;
  }
}
record compareArrayComparator: relativeComparator {
  proc compare(a: [] owned Value?, b: [] owned Value?) {
    var aNum = 0;
    if a[1] != nil then
      aNum = a[1]!.x;
    var bNum = 0;
    if b[1] != nil then
      bNum = b[1]!.x;
    return aNum - bNum;
  }
}
proc ArrayComparator type do
  return if useKeyComparator then keyArrayComparator else compareArrayComparator;

{
  writeln("Testing sorting array containing array of owned");
  var A: [1..n] [1..1] owned Value?;
  var comparator = new ArrayComparator();
  for i in 1..n {
    A[i][1] = new owned Value(n-i);
  }
  sort(A, comparator=comparator);
  if n <= 20 then writeln(A);
  for i in 1..n {
    if A[i][1] == nil then halt("nil was introduced");
  }
  assert(isSorted(A, comparator));
}
