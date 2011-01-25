use Time;

config const numIters = 100000;

proc main() {
   begin doSomeWork();
   // while original task exits
   writeln("Original task falling out of main");
}

proc doSomeWork() {
   writeln("starting to work!");
   for i in 1..numIters {
     write(".");
     stdout.flush();
   }
   writeln("done working, time to go home");
}
