module unitTest {
  use main;

  proc initcopy(type t) {
    writeln("=== initcopy");
    const m0 = allMemoryUsed();
    {
      var s: t;
      const s0 = s;
      if doCorrectnessTest then writeln(s0);
      s = "s0";
      const s1 = s;
      if doCorrectnessTest then writeln(s1);
    }
    checkMemLeaks(m0);
  }

  proc remote_initcopy(type t) {
    writeln("=== remote initcopy");
    const m0 = allMemoryUsed();
    {
      var s: t;
      on Locales[numLocales-1] {
        const s0 = s;
        if doCorrectnessTest then writeln(s0);
        s = "s0":t;
        const s1 = s;
        if doCorrectnessTest then writeln(s1);
      }
    }
    checkMemLeaks(m0);
  }

  proc doIt(type t) {
    initcopy(t);
    remote_initcopy(t);
  }

}
