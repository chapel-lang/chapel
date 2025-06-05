on here.gpus[0] {
  writeln("I shall not halt");
  @assertOnGpu
  foreach i in 1..10 {
    // halt();
  }
  writeln("I shall halt");
  @assertOnGpu
  foreach i in 1..10 {
    halt();
  }
}
