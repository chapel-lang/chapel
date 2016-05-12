use Time;

config const n = 1000000;
config const timing = true;

// passing
var pass = "passing a string";
var acc = 0;
var tPassing: Timer;
if timing then tPassing.start();
for i in 1..n {
  acc += receive(pass);
}
if timing then tPassing.stop();

// returning
var tReturning: Timer;
if timing then tReturning.start();
for i in 1..n {
  var s = send(i);
}
if timing then tReturning.stop();

if timing {
  writeln("passing: ", tPassing.elapsed());
  writeln("returning: ", tReturning.elapsed());
}
if acc == n * pass.len then
  writeln("SUCCESS");

// procs
proc receive(test: string) {
  return test.len;
}

proc send(l: int) {
  var m = l % 3;
  if m == 0 {
    return "returning";
  } else if m == 1 {
    return "a";
  } else {
    return "string";
  }
}
