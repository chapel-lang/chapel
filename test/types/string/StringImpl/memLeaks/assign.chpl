// Memory from local variables is not necessarily reclaimed until the end of
// the contain routine.  This test was modified to reflect that.

module unitTest {
  use main;

  proc assign(type t) {
    writeln("=== assignment");
    const m0 = allMemoryUsed();
    proc assign_help()
    {
      var s0: t;
      s0 = "s0";
      if doCorrectnessTest then writeln(s0);
      var s1: t;
      s1 = s0;
      if doCorrectnessTest then writeln(s1);
    }
    assign_help();
    checkMemLeaks(m0);
  }

  proc assignShorter(type t) {
    writeln("=== assignment with shorter string");
    const m0 = allMemoryUsed();
    proc assignShorter_help()
    {
      var s0: t;
      s0 = "s0";
      if doCorrectnessTest then writeln(s0);
      var s1: t;
      s1 = "s1long";
      if doCorrectnessTest then writeln(s1);
      s1 = s0;
      if doCorrectnessTest then writeln(s1);
    }
    assignShorter_help();
    checkMemLeaks(m0);
  }

  proc assignLonger(type t) {
    writeln("=== assignment with longer string");
    const m0 = allMemoryUsed();
    proc assignLonger_help()
    {
      var s0: t;
      s0 = "s0long";
      if doCorrectnessTest then writeln(s0);
      var s1: t;
      s1 = "s1";
      if doCorrectnessTest then writeln(s1);
      s1 = s0;
      if doCorrectnessTest then writeln(s1);
    }
    assignLonger_help();
    checkMemLeaks(m0);
  }

  proc assignEmpty(type t) {
    writeln("=== assignment to empty string");
    const m0 = allMemoryUsed();
    proc assignEmpty_help()
    {
      var s0: t;
      s0 = "s0";
      if doCorrectnessTest then writeln(s0);
      var s1: t;
      s0 = s1;
      if doCorrectnessTest then writeln(s0);
    }
    assignEmpty_help();
    checkMemLeaks(m0);
  }

  proc remote_assign(type t) {
    writeln("=== remote assignment");
    const m0 = allMemoryUsed();
    proc remote_assign_help(type t)
    {
      var s0, s3: t;
      s0 = "s0";
      if doCorrectnessTest then writeln((s0.locale.id, s0));
      on Locales[numLocales-1] {
        var s1, s2: t;
        s2 = "s2";
        if doCorrectnessTest then writeln((s2.locale.id, s2));
        // local-remote
        s1 = s0;
        if doCorrectnessTest then writeln((s1.locale.id, s1));
        on Locales[0] {
          s3 = "sr";
          if doCorrectnessTest then writeln((s3.locale.id, s3));
          // remote-local
          s0 = s3;
          if doCorrectnessTest then writeln((s0.locale.id, s0));
          // remote-remote
          s1 = s2;
          if doCorrectnessTest then writeln((s1.locale.id, s1));
        }
        // empty string
        var s4: t;
        // local empty-remote
        s4 = s0;
        if doCorrectnessTest then writeln((s4.locale.id, s4));
        var s5, s6, s7: t;
        // remote-local empty
        s0 = s5;
        if doCorrectnessTest then writeln((s0.locale.id, s0));
        on Locales[0] {
          // local empty-remote empty
          s0 = s5;
          if doCorrectnessTest then writeln((s0.locale.id, s0));
          // remote empty-local
          s6 = s3;
          if doCorrectnessTest then writeln((s6.locale.id, s6));
          // local-remote empty
          s6 = s7;
          if doCorrectnessTest then writeln((s6.locale.id, s6));
        }
      }
    }
    remote_assign_help(t);
    checkMemLeaks(m0);
  }

  proc doIt(type t) {
    assign(t);
    assignShorter(t);
    assignLonger(t);
    assignEmpty(t);
    remote_assign(t);
  }

}
