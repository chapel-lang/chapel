module unitTest {
  use main;

  proc coforallLocal(type t) {
    writeln("=== coforall");
    const m0 = allMemoryUsed();
    proc coforallLocal_help(type t)
    {
      var s0: t;
      s0 = "s0";
      coforall loc in Locales do if doCorrectnessTest then writeln(s0);
    }
    coforallLocal_help(t);
    checkMemLeaks(m0);
  }

  proc coforallOn(type t) {
    writeln("=== coforall on");
    const m0 = allMemoryUsed();
    proc coforallOn_help(type t)
    {
      var s0: t;
      s0 = "s0";
      coforall loc in Locales do on loc do if doCorrectnessTest then writeln(s0);
    }
    coforallOn_help(t);
    stdout.flush();
    checkMemLeaks(m0);
  }

  proc doIt(type t) {
    coforallLocal(t);
    coforallOn(t);
  }

}
