module unitTest {
  use main;

  proc findLocal(type t) {
    writeln("=== find local");
    const m0 = allMemoryUsed();
    proc findLocal_help(type t)
    {
      const s: t = "She sells seashells by the seashore.";

      writeln(s.find(""));
      var s0: t;
      writeln(s.find(s0));

      writeln(s.find("conch"));
      s0 = "conch";
      writeln(s.find(s0));

      writeln(s.find("She"));
      s0 = "She";
      writeln(s.find(s0));

      writeln(s.find("sells"));
      s0 = "sells";
      writeln(s.find(s0));

      writeln(s.find("seashells"));
      s0 = "seashells";
      writeln(s.find(s0));

      writeln(s.find("by"));
      s0 = "by";
      writeln(s.find(s0));

      writeln(s.find("the"));
      s0 = "the";
      writeln(s.find(s0));

      writeln(s.find("seashore"));
      s0 = "seashore";
      writeln(s.find(s0));
    }
    findLocal_help(t);
    checkMemLeaks(m0);
  }

  proc findRemote(type t) {
    writeln("=== find remote");
    const m0 = allMemoryUsed();
    proc findRemote_help(type t)
    {
      // const s: t = "humuhumunukunukuāpuaʻa";
      const s: t = "humuhumunukunukuapuaa";
      var s0: t = "umunuk";

      on Locales[numLocales-1] {
        writeln(s.find("umunuk"));
        writeln(s.find(s0));
        var s1: t;
        writeln(s.find(s1));
        s1 = "puaa";
        writeln(s.find(s1));
      }
    }
    findRemote_help(t);
    checkMemLeaks(m0);
  }

  proc doIt(type t) {
    findLocal(t);
    findRemote(t);
  }

}
