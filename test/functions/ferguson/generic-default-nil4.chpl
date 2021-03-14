class C { var x:int; }
  
proc foo(x: borrowed C? = nil) {
  writeln(x);
}

foo();
