class C { var x; }
  
proc foo(x: borrowed C = nil) {
  writeln(x);
}

foo();
