use Time;

var b: bool;
var b1, b2, b3: single bool;

begin with (ref b) {b1 = true; b = b3;}

begin with (ref b) {b2 = b1; b = b3;}

if b2 {
  var s: bool;
  var s1, s2: single bool;
  cobegin with (ref s) {
    {s1 = true; s = s2;}
    s2 = s1;
  }
  writeln(here.totalThreads(), " threads have been created up till now.");
  sleep(5);
  writeln(here.idleThreads(), " of these is/are idle.");
  write(here.runningTasks(), " tasks are running, but ");
  writeln(here.blockedTasks(), " are blocked at the moment.");
}

b3 = true;
