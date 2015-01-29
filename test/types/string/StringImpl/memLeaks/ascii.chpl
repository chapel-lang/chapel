module unitTest {
  use main;

  inline proc f(s, useExpr=false) {
    if useExpr {
      writeMe(ascii(s));
    } else {
      const as = ascii(s);
      writeMe(as);
    }
  }

  proc asciiLocal(type t, useExpr=false) {
    writeln("=== ascii local");
    const m0 = allMemoryUsed();
    proc asciiLocal_help()
    {
      const s0:t = "s";
      const s1:t = "";
      f(s0); f(s1);
    }
    asciiLocal_help();
    checkMemLeaks(m0);
  }

  proc asciiRemote(type t, useExpr=false) {
    writeln("=== ascii remote");
    const m0 = allMemoryUsed();
    proc asciiRemote_help()
    {
      const s0:t = "s";
      const s1:t = "";
      on Locales[numLocales-1] {
        f(s0); f(s1);
      }
    }
    asciiRemote_help();
    checkMemLeaks(m0);
  }

  proc doIt(type t) {
    asciiLocal(t); asciiLocal(t, useExpr=true);
    asciiRemote(t); asciiRemote(t, useExpr=true);
  }

}
