config const n = 77;

var t: [1..n-1] sync bool;
var q: [2..n] sync bool;
var c: sync int = n;
var s: single bool;

coforall i in 1..n {
  var myc:int = c-1;
  if myc==0 then s=true; else c=myc;
  // wait for everyone to get here
  s;

  if i!=n then
  t[i];
  writeln(here.runningTasks());
  if i!=1 then
    t[i-1] = true;
}

