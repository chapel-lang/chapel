module unitTest {
  use main;

  inline proc f(s, useExpr=false) {
    if useExpr {
      writeMe(s.toByte());
    } else {
      const aStr = s.toByte();
      writeMe(aStr);
    }
  }

  proc byteLocal(type t, useExpr=false) {
    writeln("=== byte local");
    const m0 = allMemoryUsed();
    {
      const s0:t = "s";
      const s1:t = "t";
      f(s0); f(s1);
    }
    checkMemLeaks(m0);
  }

  proc byteRemote(type t, useExpr=false) {
    writeln("=== byte remote");
    const m0 = allMemoryUsed();
    {
      const s0:t = "s";
      const s1:t = "t";
      on Locales[numLocales-1] {
        f(s0); f(s1);
      }
    }
    checkMemLeaks(m0);
  }

  proc doIt(type t) {
    byteLocal(t); byteLocal(t, useExpr=true);
    byteRemote(t); byteRemote(t, useExpr=true);
  }

}
