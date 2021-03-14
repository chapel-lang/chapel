module LongName {
  var x: int;
}
module L {
  var y: bool;
}
module User {
  use LongName as L;
  // L is not ambiguous in this scenario, as the rename from the previous use is
  // closer than the module definition
  use L;

  proc main() {
    writeln(L.x);
    writeln(x);
    // writeln(y); // This will fail, as we have not imported its module
  }
}
