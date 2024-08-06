module Outer {
  config param Value1: uint = 10;
  config param Size1: uint = 64;

  record MyUint1 {
    var value: uint = Value1;
    var data: uint(Size1); // Okay
  }

  // From within a module.
  module M2 {
    config param Value2: uint = 10;
    config param Size2: uint = 64;
    record MyUint2 {
      var value: uint = Value2;
      var data: uint(Size2); // error: use of 'Size2' before encountering its definition, type unknown
    }

    // From within a submodule
    module SM {
      import super.super.M2;

      record MyUint3 {
        var value: uint = M2.Value2;
        var data: uint(M2.Size2); // error: use of 'Size2' before encountering its definition, type unknown
      }
    }
  }

  // Type alias
  module M3 {
    param Size = 64;
    type Type = uint(Size); // internal error: COD-TYP-0050
  }

  module M4 {
    import Outer;
    type Type = uint(Outer.Size1); // Okay
  }

  module M5 {
    proc main() {
      use Outer, super.M2, super.M3, super.M4;
      use SM;

      var a1: MyUint1;
      writeln(a1);
      writeln(a1.data.type : string);
      var a2: MyUint2;
      writeln(a2);
      writeln(a2.data.type : string);
      var a3: MyUint3;
      writeln(a3);
      writeln(a3.data.type : string);
      writeln(M4.Type : string);
    }
  }
}
