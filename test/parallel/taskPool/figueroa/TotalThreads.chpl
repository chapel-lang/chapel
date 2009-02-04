use Time;

var b: bool;
var b1, b2, b3: single bool;

begin {b1 = true; b = b3;}

begin {b2 = b1; b = b3;}

if b2 {
  var s: bool;
  var s1, s2: single bool;
  cobegin {
    {s1 = true; s = s2;}
    s2 = s1;
  }
  writeln(here.totalThreads(), " threads have been created up till now.");
  sleep(5);
  writeln(here.runningTasks(), " of these are running.");
  write("Including ", here.idleThreads(), " that is idle, ");
  writeln(here.blockedTasks(), " are blocked at the moment.");
}

b3 = true;
