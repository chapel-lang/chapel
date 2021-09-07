record man1 {
  var x = 0;
  proc enterThis() const ref: int { return x; }
  proc leaveThis(in err: owned Error?) {
    if err then halt();
  }
}

record man2 {
  var x = 0;
  proc enterThis(): int { return x; }
  proc enterThis() const ref: int { return x; }
  proc leaveThis(in err: owned Error?) {
    if err then halt();
  }
}

// Specified storage behavior as a baseline...
proc test1() {
  var myManager = new man1();
  writeln(myManager);
  manage myManager as const ref myResource do
    myResource += 1;
}
test1();

proc test2() {
  var myManager = new man1();
  writeln(myManager);
  manage myManager as myResource do
    myResource += 1;
}
test2();

// TODO: Should this prefer VALUE since the resource is being mutated?
proc test3() {
  var myManager = new man2();
  writeln(myManager);
  manage myManager as myResource do
    myResource += 1;
}
test3();

