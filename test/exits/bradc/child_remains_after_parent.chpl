use Time;

config const numIters = 100000;

def main() {
   begin doSomeWork();
   // while original task exits
   writeln("Original task falling out of main");
}

def doSomeWork() {
   writeln("starting to work!");
   for i in 1..numIters {
     write(".");
     stdout.flush();
   }
   writeln("done working, time to go home");
}
