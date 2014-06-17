proc foo() { }

proc bar() { }

var done: single bool;

proc timeout(n: uint) {
  use Time;
  begin { sleep(n); writeln("Timeout"); exit(1); }
  begin { done; exit(0); }
}

timeout(30); // exit after 30 seconds or when done is set.

for i in 1..20000 {
  cobegin {
    cobegin { bar(); foo(); }
    cobegin { bar(); foo(); }
    cobegin { bar(); foo(); }
    cobegin { bar(); foo(); }
  }
  if i % 10000 == 0 then
    writeln("iteration ", i, " done.");
}

done = true;
