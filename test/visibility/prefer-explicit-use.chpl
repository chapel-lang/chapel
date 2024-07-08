module M {
  // This test relies on the fact that defaultLowBound is introduced by an
  // auto-usd Chapel module. If it eventually is not, this test should be updated
  // to point to some other `param` or `var` in the auto-used modules.
  module UseInsteadOfAuto {
    param defaultLowBound = "not the auto-used variable";
  }
  public use UseInsteadOfAuto;

  // The particular bug this test checks for is that auto-used modules are
  // preferred from inside methods, so make a record with a method.
  record R {
    proc foo() {
      writeln(defaultLowBound.type : string);
    }
  }

  var r: R;
  r.foo();
}
