record man : contextManager {
  var x = 0;
  proc ref enterContext() ref: int { return x; }
  proc exitContext(in err: owned Error?) {
    if err then halt();
  }
}

proc test1() {
  var myManager = new man();
  writeln(myManager);
  manage myManager as myResource do
    myResource += 1;
  writeln(myManager);
}
test1();

proc test2() {
  var myManager = new man();
  writeln(myManager);
  manage myManager as ref myResource do
    myResource += 1;
  writeln(myManager);
}
test2();
 
