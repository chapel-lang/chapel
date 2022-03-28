record man {
  var x = 0;
  proc enterThis(): int { return x; }
  proc enterThis() const ref: int { return x; }
  proc leaveThis(in err: owned Error?) {
    if err then halt();
  }
}

// Emit an error because there are two overloads of 'enterThis()'.
proc test1() {
  var myManager = new man();
  writeln(myManager);
  manage myManager as myResource do
    myResource += 1;
}
test1();

// Ensure that definition hints do not appear again.
proc test2() {
  manage new man() as myResource do writeln(myResource);
}
test2();

