use Sort;

config param resolveDeps = false;

// To make sure we get all the deprecation warnings, we need to use a unique comparator type for each call

record myCmp1 { proc key(elt) do return elt; }
record myCmp2 { proc key(elt) do return elt; }
record myCmp3 { proc key(elt) do return elt; }
record myCmp4 { proc key(elt) do return elt; }
record myCmp5 { proc key(elt) do return elt; }
record myCmp6 { proc key(elt) do return elt; }
record myCmp7 { proc key(elt) do return elt; }
record myCmp8 { proc key(elt) do return elt; }
record myCmp9 { proc key(elt) do return elt; }

record myCmpResolveDeps: keyComparator {
  proc key(elt) do return elt;
}

proc getComparator(param i: int) type {
  if resolveDeps then return myCmpResolveDeps;
  else select i {
    when 1 do return myCmp1;
    when 2 do return myCmp2;
    when 3 do return myCmp3;
    when 4 do return myCmp4;
    when 5 do return myCmp5;
    when 6 do return myCmp6;
    when 7 do return myCmp7;
    when 8 do return myCmp8;
    when 9 do return myCmp9;
  }
  compilerError("");
}

use Random;
var arr: [1..1000] int;

// there should be 9 deprecations here
fillRandom(arr);
writeln("isSorted before ", isSorted(arr, comparator=new (getComparator(1))()));
sort(arr, comparator=new (getComparator(2))());
writeln("isSorted after ", isSorted(arr, comparator=new (getComparator(3))()));

fillRandom(arr);
writeln("isSorted before ", isSorted(arr, comparator=new ReverseComparator(new (getComparator(4))())));
sort(arr, comparator=new ReverseComparator(new (getComparator(5))()));
writeln("isSorted after ", isSorted(arr, comparator=new ReverseComparator(new (getComparator(6))())));

fillRandom(arr);
writeln("isSorted before ", isSorted(arr, comparator=new (getComparator(7))()));
var temp = sorted(arr, comparator=new (getComparator(8))());
writeln("isSorted after ", isSorted(temp, comparator=new (getComparator(9))()));

