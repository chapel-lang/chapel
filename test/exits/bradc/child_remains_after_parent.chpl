use Time, IO;

config const numIters = 100000;
var s$: sync bool;

proc main() {
   begin doSomeWork();
   // while original task exits
   writeln("Original task falling out of main");
   s$.writeEF(true);
}

proc doSomeWork() {
   s$.readFE();
   for i in 1..numIters {
     write("");
     stdout.flush();
   }
   writeln("done working");
}
