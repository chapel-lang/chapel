// See also:
//  test/functions/deitz/test_resolve_best_shadowed2.chpl
//  https://github.com/chapel-lang/chapel/commit/bde150a476e

module M1 {
  use M3;
  use M4;
  proc main {
    h(1);
  }
}
module M3 {
    public use M5;
    proc h(r:real) { writeln("h(real"); }
          // does not shadow other h based on call to h since
          // there is another path (through M4) to the other h
}

module M4 {
    public use M5;
}

module M5 {
    proc h(i:int) { writeln("h(int)"); }
}
