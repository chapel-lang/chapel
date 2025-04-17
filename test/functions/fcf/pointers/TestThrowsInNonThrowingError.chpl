//
// Need the module declaration to put checking into 'relaxed' mode instead
// of the 'fatal' mode that happens for implicitly declared modules.
//
module TestThrowsInNonThrowingError {
  proc test0() {
    proc foo() throws { throw new Error('E!'); }
    record r {
      var p = foo;
      // Should get a compiler error about calling 'p'.
      proc method() { p(); }
    }
    var x = new r();
    x.method();
  }

  proc main() {
    test0();
  }
}
