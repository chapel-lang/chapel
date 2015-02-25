module unitTest {
  use main;

  // create some tasks before running the tests
  proc dummy(doIt=false) {
    coforall loc in Locales do on loc do begin if doIt then writeln(here.id);
  }

  proc beginLocal(type t) {
    writeln("=== local begin");
    const m0 = allMemoryUsed();
    proc beginLocal_help()
    {
      var s0: t;
      s0 = "s0";
      sync {
        begin if doCorrectnessTest then writeln(s0);
      }
      var s1: t;
      sync {
        begin if doCorrectnessTest then writeln(s1);
      }
    }
    beginLocal_help();
    checkMemLeaks(m0);
  }

  proc beginRemote(type t) {
    writeln("=== remote begin");
    const m0 = allMemoryUsed();
    proc beginRemote_help(type t)
    {
      var s0: t;
      s0 = "s0";
      sync {
        on Locales[numLocales-1] do begin if doCorrectnessTest then writeln(s0);
        begin on Locales[numLocales-1] do if doCorrectnessTest then writeln(s0);
      }
      var s1: t;
      sync {
        on Locales[numLocales-1] do begin if doCorrectnessTest then writeln(s1);
        begin on Locales[numLocales-1] do if doCorrectnessTest then writeln(s1);
      }
    }
    beginRemote_help(t);
    checkMemLeaks(m0);
  }

  proc doIt(type t) {
    dummy();
    beginLocal(t);
    beginRemote(t);
    writeln("DONE"); // sentinel to check for end of test output
  }

}
