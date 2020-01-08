use Sort;

config const n = 100;
config param useKeyComparator = true; 

class Value {
  var x: int;
}


record ValueComparator {
  proc key(v: Value?) where useKeyComparator {
    if v == nil then
      return 0;
    else
      return v!.x;
  }
  proc compare(a: Value?, b: Value?) where !useKeyComparator {
    var aNum = 0;
    if a != nil then
      aNum = a!.x;
    var bNum = 0;
    if b != nil then
      bNum = b!.x;
    return aNum - bNum;
  }
}


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
proc =(ref lhs: Wrapper, ref rhs: Wrapper) {
  lhs.elt = rhs.elt;
}

record WrapperComparator {
  proc key(v: Wrapper) where useKeyComparator {
    if v.elt == nil then
      return 0;
    else
      return v.elt!.x;
  }
  proc compare(a: Wrapper, b: Wrapper) where !useKeyComparator {
    var aNum = 0;
    if a.elt != nil then
      aNum = a.elt!.x;
    var bNum = 0;
    if b.elt != nil then
      bNum = b.elt!.x;
    return aNum - bNum;
  }

}

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
    this.complete();
  }
  proc init(in arg: owned Value) {
    this.complete();
    elts[1] = arg;
  }
}
record ArrayWrapperComparator {
  proc key(v: ArrayWrapper) where useKeyComparator {
    if v.elts[1] == nil then
      return 0;
    else
      return v.elts[1]!.x;
  }
  proc compare(a: ArrayWrapper, b: ArrayWrapper) where !useKeyComparator {
    var aNum = 0;
    if a.elts[1] != nil then
      aNum = a.elts[1]!.x;
    var bNum = 0;
    if b.elts[1] != nil then
      bNum = b.elts[1]!.x;
    return aNum - bNum;
  }
}

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

record ArrayComparator {
  proc key(v: [] owned Value?) where useKeyComparator {
    if v[1] == nil then
      return 0;
    else
      return v[1]!.x;
  }
  proc compare(a: [] owned Value?, b: [] owned Value?) where !useKeyComparator {
    var aNum = 0;
    if a[1] != nil then
      aNum = a[1]!.x;
    var bNum = 0;
    if b[1] != nil then
      bNum = b[1]!.x;
    return aNum - bNum;
  }
}

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
