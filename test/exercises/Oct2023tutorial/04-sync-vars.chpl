// Using a sync to lock around a critical section.
var mystr = "hello";

var lock: sync bool;

begin {
  lock.writeEF(true);
  critical();
  lock.readFE();
}

// Other tasks would have to do this same pattern to respect lock.
begin {
  lock.writeEF(true);
  notCritical();
  lock.readFE();
}

proc critical() {
  writeln("Do something important that I don't want others to interfere with.");
  mystr = "critical";
}

proc notCritical() {
  writeln("Do something that might interfere with another thread.");
  mystr = "not critical";
}

////////////////////////////////////////////////
// Using sync to implement futures.
var future: sync real;

begin future.writeEF( compute() );
var res = computeSomethingElse();
writeln("useComputedResults(future.readFE(), res) = ",
        useComputedResults(future.readFE(), res));

proc compute() do return 1.0;
proc computeSomethingElse() do return 2.0;
proc useComputedResults(x, y) do return x + y;

