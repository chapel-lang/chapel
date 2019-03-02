 use BitOps;
 use Sort;
 use Random;
 use RadixLSB;
 use Time;

 config const printStats = true;
 config const size = 10000;

 proc main() {
   
   var array:[1..size] int; 
   fillRandom(array);
   
   for i in array.domain {
     array[i] = abs(array[i]);
   }

   var t: Timer;
   t.start();

   RadixLSB.radixSort(array);

   t.stop();
 
   if printStats {
     writeln("Time: ", t.elapsed());
   }

   t.clear();

   //writeln(array);
   writeln("sorted array: ",isSorted(array));

  }
