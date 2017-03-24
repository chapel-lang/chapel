proc foo() { }

proc bar() { }

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
