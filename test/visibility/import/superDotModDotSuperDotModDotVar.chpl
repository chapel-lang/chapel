module M {
  module Mod {
    var x: bool;
  }
  module SubMod {
     var x: int;
  }
  module User {
     // Should this work or fail?  Currently fails
    import super.Mod.super.SubMod.x;
    proc main() {
      writeln(x);
    }
  }
}
