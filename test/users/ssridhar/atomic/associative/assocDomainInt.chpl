config const n = 10;

var D: domain(int);

var A: [D] int;

config const trackStmStats: bool = true;

if (trackStmStats) then startStmStats();

coforall i in 1..n {
  atomic D.add(i);
}

coforall i in 1..n {
  atomic D.remove(i);
}

if (trackStmStats) then stopStmStats();

writeln("Domain (after remove) = ", D);
