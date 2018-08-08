 use BitOps;
 use Sort;
 use Random;
 use radixMSBModule;
 use Time;
 config const printStats = true;

 proc main() {
  
   var array:[1..10000] int; 
   fillRandom(array); 
   var t: Timer;   

   for i in array.domain {
     array[i] = abs(array[i]);
   }  
   
	 t.start();
   MSBmodule.radixSortMSB(array);
   t.stop();
   if printStats {
     writeln("Time: ", t.elapsed());
   }
   t.clear();

   //writeln(array);
   writeln("sorted array: ",isSorted(array));

  }
