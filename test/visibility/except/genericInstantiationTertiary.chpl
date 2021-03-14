// Tests the behavior when listing a generic type in the except list of a used
// module that defines a tertiary method on an instantiation of the generic
// type.
module definesType {
  record GenericRec {
    param p: int;
  }
}

module definesTertiary {
  use definesType;

  proc (GenericRec(2)).tertiaryMethod() {
     writeln("In tertiary method");
  }
}

module User {
  use definesType;
  use definesTertiary except GenericRec;

  proc main() {
    var x = new GenericRec(2);
    x.tertiaryMethod(); // Should not work
  }
}
