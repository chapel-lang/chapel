use BitOps;
 use Sort;
 use MSBRadixSort;
 use Random;
 use Time;
 use CTypes;
 use IO, ChplFormat;

 config const printStats = false;
 config const size = 10000;

 var chplout = stdout.withSerializer(chplSerializer);

 record uintCriterion8: keyPartComparator {
   inline
   proc keyPart(x, start:int):(keyPartStatus,uint(8)) {
     if start >= 8 then return (keyPartStatus.pre, 0:uint(8));
     var key:uint = (x:uint) >> (64 - 8*(start+1));
     //writef("in keyPart8(%016xu, %u)\n", x, start);
     return (keyPartStatus.returned, (key & 0xff):uint(8));
   }
 }
 record uintCriterion16: keyPartComparator {
   inline
   proc keyPart(x, start:int):(keyPartStatus,uint(16)) {
     if start >= 4 then return (keyPartStatus.pre, 0:uint(8));
     var key:uint = (x:uint) >> (64 - 16*(start+1));
     //writef("in keyPart16(%016xu, %u)\n", x, start);
     return (keyPartStatus.returned, (key & 0xffff):uint(16));
   }
 }
 record uintCriterion32: keyPartComparator {
   inline
   proc keyPart(x, start:int):(keyPartStatus, uint(32)) {
     if start >= 2 then return (keyPartStatus.pre, 0:uint(32));
     var key:uint = (x:uint) >> (64 - 32*(start+1));
     //writef("in keyPart32(%016xu, %u)\n", x, start);
     return (keyPartStatus.returned, (key & 0xffffffff):uint(32));
   }
 }
 record uintCriterion64: keyPartComparator {
   inline
   proc keyPart(x, start:int):(keyPartStatus, uint(64)) {
     if start >= 1 then return (keyPartStatus.pre, 0:uint(64));
     var key:uint = x:uint;
     //writef("in keyPart64(%016xu, %u)\n", x, start);
     return (keyPartStatus.returned, key);
   }
 }
 record intCriterion: keyPartComparator {
   inline
   proc keyPart(x, start:int):(keyPartStatus, int) {
     if start >= 1 then return (keyPartStatus.pre, 0:int);
     //writef("in intCriterion64(%016xu, %u)\n", x, start);
     return (keyPartStatus.returned, x);
   }
 }
 record intTupleCriterion: keyPartComparator {
   inline
   proc keyPart(x:2*int, start:int):(keyPartStatus, int) {
     if start >= 2 then
       return (keyPartStatus.pre, 0);

     //writef("in intTupleCriterion(%016xu %016xu %u)\n", x(1), x(2), start);
     return (keyPartStatus.returned, x(start));
   }
 }
 record stringCriterion: keyPartComparator {
   inline
   proc keyPart(x:string, start:int):(keyPartStatus, uint(8)) {
     var ptr = x.c_str():c_ptr(uint(8));
     var len = x.numBytes;
     var section = if start < len then keyPartStatus.returned else keyPartStatus.pre;
     var part =    if start < len then ptr[start]             else 0:uint(8);
     return (section, part);
   }
 }

 proc testSorted(A, comparator) {
   assert(isSorted(A, comparator));
   assert(isSorted(A));
 }
 proc testReverseSorted(A, comparator) {
   assert(isSorted(A, new reverseComparator(comparator)));
   assert(isSorted(A, new reverseComparator()));
 }


 proc testSomeSorts(input, comparator) {
   var start = input.domain.low;
   var end = input.domain.high;

   var settingsTuple =
     (new MSBRadixSortSettings(CHECK_SORTS=true), // default settings
      new MSBRadixSortSettings(DISTRIBUTE_BUFFER=1,
                               sortSwitch=1,
                               minForTask=1,
                               CHECK_SORTS=true,
                               alwaysSerial=true),
      new MSBRadixSortSettings(DISTRIBUTE_BUFFER=10,
                               sortSwitch=1,
                               minForTask=1,
                               CHECK_SORTS=true,
                               alwaysSerial=false));

   writeln(comparator.type:string);
   chplout.writef("Sorting    %?\n", input);

   var A = input;
   ShellSort.shellSort(A, comparator);
   chplout.writef("shellSort  %?\n", A);
   testSorted(A, comparator);

   var Ar = input;
   ShellSort.shellSort(Ar, new reverseComparator(comparator));
   chplout.writef("shellSort- %?\n", Ar);
   testReverseSorted(Ar, comparator);

   for param i in 0..settingsTuple.size-1 {
     var s = settingsTuple(i);
     var B = input;
     MSBRadixSort.msbRadixSort(B, start, end, comparator,
                               0, max(int), s);
     if i == 1 then
      chplout.writef("radixSort  %?\n", B);
     testSorted(B, comparator);

     var Br = input;
     MSBRadixSort.msbRadixSort(Br, start, end,
                               new reverseComparator(comparator),
                               0, max(int), s);
     if i == 1 then
      chplout.writef("radixSort- %?\n", Br);
     testReverseSorted(Br, comparator);
   }
 }

 proc testSortsPositive(input) {
   testSomeSorts(input, new intCriterion());
   testSomeSorts(input, new uintCriterion8());
   testSomeSorts(input, new uintCriterion16());
   testSomeSorts(input, new uintCriterion32());
   testSomeSorts(input, new uintCriterion64());
 }

proc testSortsUnsigned(input) {
   testSomeSorts(input, new uintCriterion8());
   testSomeSorts(input, new uintCriterion16());
   testSomeSorts(input, new uintCriterion32());
   testSomeSorts(input, new uintCriterion64());
 }


 proc testSortsSigned(input) {
   testSomeSorts(input, new intCriterion());
 }

 proc testIntTupleSorts(input) {
   testSomeSorts(input, new intTupleCriterion());
 }

 proc testStringSorts(input) {
   testSomeSorts(input, new stringCriterion());
 }


 proc main() {

   assert(chpl_compare(9,3, new uintCriterion8()) > 0);
   assert(chpl_compare(2,3, new uintCriterion8()) < 0);
   assert(chpl_compare(0,0, new uintCriterion8()) == 0);
   assert(chpl_compare(9,3, new reverseComparator(new uintCriterion8())) < 0);
   assert(chpl_compare(2,3, new reverseComparator(new uintCriterion8())) > 0);
   assert(chpl_compare(0,0, new reverseComparator(new uintCriterion8())) == 0);

   testSortsPositive([2,1]);

   testSortsPositive([9,3,8,100,43,2]);
   testSortsPositive([0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
   testSortsPositive([1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1]);

   {
     var A:[1..64] uint;
     for i in 0..63 {
       A[64-i] = 1:uint << i;
     }
     testSortsUnsigned(A);
   }

   testSortsSigned([-1, 57, 23, -7]);
   testSortsSigned([0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
   testSortsSigned([1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1]);
   testSortsSigned([-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1]);
   testIntTupleSorts([ (4,8), (-7, -20),
                       (0,0), (0,0), (-3, 5), (-3, -5), (4, -8) ]);


   testStringSorts([ "hi", "hii", "hiii", "a", "b", "boo", "", "x", "zoo" ]);

   var array:[1..size] int;
   fillRandom(array);

   for i in array.domain {
     array[i] = abs(array[i]);
   }

   var t: stopwatch;
   t.start();

   MSBRadixSort.msbRadixSort(array, 1, size, new intCriterion(),
                             0, max(int), new MSBRadixSortSettings());

   t.stop();

   if printStats {
     writeln("Time: ", t.elapsed());
   }

   t.clear();

   //writeln(array);
   writeln("sorted array: ",isSorted(array));

  }
