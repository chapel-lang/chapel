 use BitOps;
 use Sort;
 use Random;
 use flagsortMSB;
 use Time;

 config const printStats = false;
 config const size = 1000;
  

 proc main() {
   
   var array:[1..size] int; 
   fillRandom(array);
   
   for i in array.domain {
     array[i] = abs(array[i]);
   }

   var t: Timer;
   t.start();
   radixSortMSB(array);
   t.stop();
 
   if printStats {
     writeln("Time: ", t.elapsed());
   }

   t.clear();

   writeln("sorted array: ",isSorted(array));

  }
