// This test is here because it used to crash the compiler.

iter ITER() { yield 555; }

iter ITER(param tag) {
  forall loc in Locales do
    on loc do
      yield 666;
}

proc main {
  forall III in ITER() {
    writeln();
  }
}
