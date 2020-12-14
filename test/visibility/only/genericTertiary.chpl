// Tests the behavior when listing a generic type in the only list of a used
// module that defines a tertiary method on the generic type.
module definesType {
  record GenericRec {
    param p: int;
  }
}

module definesTertiary {
  use definesType;

  proc GenericRec.tertiaryMethod() {
     writeln("In tertiary method");
  }
}

module User {
  use definesType;
  use definesTertiary only GenericRec;

  proc main() {
    var x = new GenericRec(2);
    x.tertiaryMethod(); // Should work
  }
}
