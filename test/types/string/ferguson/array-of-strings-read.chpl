use Time;

// test array of string reading performance

config const numTrials = 1;
config const n = 100000;
config const expect = 279922;
config const timing = false;

var A:[1..n] string;

for i in 1..n {
  var x = 123456789123 * i + i*i*i + i*i + i;
  A[i] = x:string + x:string;
}

var nines1: int;
var t1: Timer;

t1.start();
nines1 = 0;
for i in 1..numTrials {
  for j in 1..n {
    nines1 += A[j].count("9");
  }
}
t1.stop();

proc count9s(s: string): int {
  return s.count("9");
}

var nines2: int;
var t2: Timer;
t2.start();
nines2 = 0;
for i in 1..numTrials {
  for j in 1..n {
    nines2 += count9s(A[j]);
  }
}
t2.stop();

proc count9sConstRef(const ref s: string): int {
  return s.count("9");
}

var nines3: int;
var t3: Timer;
t3.start();
nines3 = 0;
for i in 1..numTrials {
  for j in 1..n {
    nines3 += count9sConstRef(A[j]);
  }
}
t3.stop();

proc count9sRef(ref s: string): int {
  return s.count("9");
}

var nines4: int;
var t4: Timer;
t4.start();
nines4 = 0;
for i in 1..numTrials {
  for j in 1..n {
    nines4 += count9sRef(A[j]);
  }
}
t4.stop();

if (nines1/numTrials == expect &&
    nines2/numTrials == expect &&
    nines3/numTrials == expect &&
    nines4/numTrials == expect ) {
  writeln("Success");
} else {
  writeln("Did not get expected output ", expect, " but got:");
  writeln(nines1);
  writeln(nines2);
  writeln(nines3);
  writeln(nines4);
}

if timing {
  writeln("numTrials=", numTrials, " n=", n);
  writeln("time in seconds for array access by argument intent:");
  writeln ("1 this      ", t1.elapsed());
  writeln ("2 default   ", t2.elapsed());
  writeln ("3 const ref ", t3.elapsed());
  writeln ("4 ref       ", t4.elapsed());
}
