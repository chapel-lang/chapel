
use Sort;

record myRec {
  var x: int;
  var creationIndex: int;
}
record myRecComparator: keyComparator {
  proc key(rec: myRec): int do
    return rec.x;
}
proc factory(x: int): myRec {
  @functionStatic
  ref next = 0;
  next += 1;
  return new myRec(x, next);
}

record myRecComparatorByCreationIndex: keyComparator {
  proc key(rec: myRec): int do
    return rec.creationIndex;
}
proc inCreationOrder(arr) {
  var inOrder = true;
  forall (a, aprev) in zip(arr[arr.domain.low+1..],
                           arr[arr.domain.low..arr.domain.high-1])
    with (&& reduce inOrder) {
    if aprev.x == a.x then
      inOrder reduce= aprev.creationIndex < a.creationIndex;
  }
  return inOrder;
}

config const n = 1000;
config const duplicates = 10;
var arr: [0..#(n*duplicates)] myRec;
for i in arr.domain by duplicates {
  for j in 0..<duplicates {
    arr[i+j] = factory(arr.domain.high - i);
  }
}

config const printArray = false;
proc print(arr, label_="") {
  if printArray {
    if label_ != "" then
      writeln(label_);
    for a in arr do
      writeln(a);
  }
}

var recCmp = new myRecComparator();

writeln("orig array:");
writeln("isSorted: ", isSorted(arr, comparator=recCmp));
writeln("inCreationOrder: ", inCreationOrder(arr));
print(arr);
writeln();

writeln("unstable sort:");
var arr1 = arr;
sort(arr1, stable=false, comparator=recCmp);
writeln("isSorted: ", isSorted(arr1, comparator=recCmp));
writeln("inCreationOrder: ", inCreationOrder(arr1));
print(arr1);
writeln();

writeln("stable sort:");
var arr2 = arr;
sort(arr2, stable=true, comparator=recCmp);
writeln("isSorted: ", isSorted(arr2, comparator=recCmp));
writeln("inCreationOrder: ", inCreationOrder(arr2));
print(arr2);
