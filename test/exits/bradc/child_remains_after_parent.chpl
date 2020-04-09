use Time, IO;

config const numIters = 100000;
var s$: sync bool;

proc main() {
   begin doSomeWork();
   // while original task exits
   writeln("Original task falling out of main");
   s$ = true;
}

proc doSomeWork() {
   s$;
   for i in 1..numIters {
     write("");
     stdout.flush();
   }
   writeln("done working");
}
