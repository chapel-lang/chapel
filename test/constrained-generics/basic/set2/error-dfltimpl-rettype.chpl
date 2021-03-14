// This test ensures that the compiler issues an error
// when the return type of a default implementation is inferred.
// Exception: no error if the return type is 'void'.

interface IFC {
  proc dfltVoid() { // ok: void return type does not need explicit declaration
    writeln("in dfltVoid");
  }
  proc dfltInt() {  // error: 'int' return type must be declared explicilty
    return 5;
  }
}
