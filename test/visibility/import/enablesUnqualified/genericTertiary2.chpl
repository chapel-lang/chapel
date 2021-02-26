// Tests the behavior when listing a generic type in the import list when
// importing a module that defines a tertiary method on the generic type.

// This version lists the type in an explicit list for the import path (using
// {})
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
  import definesTertiary.{GenericRec};

  proc main() {
    var x = new GenericRec(2);
    x.tertiaryMethod(); // Should work
  }
}
