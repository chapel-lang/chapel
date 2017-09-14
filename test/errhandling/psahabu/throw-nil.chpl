proc throwsNil() throws {
  writeln("throwing nil");
  throw nil;
  writeln("fail: did not throw NilThrownError");
}

try! throwsNil();
