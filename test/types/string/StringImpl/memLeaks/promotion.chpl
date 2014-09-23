module unitTest {
  use main;

  proc promotion(type t) {
    inline proc f() {
      var s0: t;
      var S0: [3..17] t;
      S0 = s0;
      if doCorrectnessTest then writeln(S0);
      s0 = "s0";
      S0 = s0;
      if doCorrectnessTest then writeln(S0);
    }

    writeln("=== promotion");
    f(); // call once to allocate I/O buffers and task data
    const m0 = allMemoryUsed();
    {
      f(); // call again to measure
    }
    checkMemLeaks(m0);
  }

  proc doIt(type t) {
    promotion(t);
  }

}
