// a small reproducer for #24591
// can compile with or without --minimal-modules

record RR {
  param p1 = 5;
  param p2 = 6;
}

proc f1(arg1: RR)       { }
proc f2(arg3: RR(?))    { }
proc f3(arg2: RR(p1=?)) { }

//f1(new RR(5,7)); // error
f2(new RR(5,7));   // ok
f3(new RR(5,7));   // should be an error
