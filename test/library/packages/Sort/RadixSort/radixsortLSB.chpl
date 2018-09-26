 use BitOps;
 use Random;
 use RadixLSB;
 use Time;

 //config const size = 10000;

 proc main() {
   
   var array:[1..13] int = [ 233, 10, 9, 182, 667, 549, 55, 48, 675, 1100, 32, 13457, 12];
   writeln(array);

   var sortedArray = radixSort(array);
   writeln("sorted array: ",sortedArray);

   var t: Timer;
	 t.start();

   RadixLSB.radixSort(array);

   t.stop();
 
   //writeln(t.elapsed());
 
   t.clear();

   //writeln(array);
  }