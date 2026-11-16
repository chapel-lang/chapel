config const trials = 10_000;

proc main() {
  on Locales[numLocales-1] {
    for trial in 1..trials {
      var x: atomic int;
      on Locales[0] do
        x.add(1);
      const actual = x.read();
      assert(actual == 1, "trial ", trial, ": expected 1, got ", actual);
    }
    writeln("success");
  }
}
