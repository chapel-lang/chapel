use Time;

var b: bool;
var b1, b2, b3: single bool;

begin with (ref b) {b1.writeEF(true); b = b3.readFF();}

begin with (ref b) {b2.writeEF(b1.readFF()); b = b3.readFF();}

if b2.readFF() {
  var s: bool;
  var s1, s2: single bool;
  cobegin with (ref s) {
    {s1.writeEF(true); s = s2.readFF();}
    s2.writeEF(s1.readFF());
  }
  writeln(here.totalThreads(), " threads have been created up till now.");
  sleep(5);
  writeln(here.idleThreads(), " of these is/are idle.");
  write(here.runningTasks(), " tasks are running, but ");
  writeln(here.blockedTasks(), " are blocked at the moment.");
}

b3.writeEF(true);
