use Sort;

config param resolveDeps = false;

// This is a simple comparator for types with '<'
// It is identical to the Sort module for normal sorting
// To make sure we get all the deprecation warnings, we need to use a unique comparator type for each call

record myCmp1{proc compare(x,y) do return if x<y then -1 else if y<x then 1 else 0;}
record myCmp2{proc compare(x,y) do return if x<y then -1 else if y<x then 1 else 0;}
record myCmp3{proc compare(x,y) do return if x<y then -1 else if y<x then 1 else 0;}
record myCmp4{proc compare(x,y) do return if x<y then -1 else if y<x then 1 else 0;}
record myCmp5{proc compare(x,y) do return if x<y then -1 else if y<x then 1 else 0;}
record myCmp6{proc compare(x,y) do return if x<y then -1 else if y<x then 1 else 0;}
record myCmp7{proc compare(x,y) do return if x<y then -1 else if y<x then 1 else 0;}
record myCmp8{proc compare(x,y) do return if x<y then -1 else if y<x then 1 else 0;}
record myCmp9{proc compare(x,y) do return if x<y then -1 else if y<x then 1 else 0;}

record myCmpResolveDeps: keyPartComparator {
  proc compare(x,y) do return if x<y then -1 else if y<x then 1 else 0;
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

