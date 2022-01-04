config const n = 77;

var t: [1..n-1] sync bool;
var q: [2..n] sync bool;
var c: sync int = n;
var s: single bool;

coforall i in 1..n {
  var myc:int = c.readFE()-1;
  if myc==0 then s.writeEF(true); else c.writeEF(myc);
  // wait for everyone to get here
  s.readFF();

  if i!=n then
  t[i].readFE();
  writeln(here.runningTasks());
  if i!=1 then
    t[i-1].writeEF(true);
}

