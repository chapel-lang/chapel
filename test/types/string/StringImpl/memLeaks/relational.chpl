module unitTest {
  use main;

  inline proc f(s0, s1, useExpr=false) {
    if useExpr {
      writeMe(s0==s1);
      writeMe(s0!=s1);
      writeMe(s0<s1);
      writeMe(s0<=s1);
      writeMe(s0>s1);
      writeMe(s0>=s1);
    } else {
      var b = s0==s1;
      writeMe(b);
      b = s0!=s1;
      writeMe(b);
      b = s0<s1;
      writeMe(b);
      b = s0<=s1;
      writeMe(b);
      b = s0>s1;
      writeMe(b);
      b = s0>=s1;
      writeMe(b);
    }
  }

  proc relLocal(type t, useExpr=false) {
    writeln("=== relational operators (local)");
    const m0 = allMemoryUsed();
    proc relLocal_hgelp(type t, useExpr=false)
    {
      const s0:t = "chicken";
      const s1:t = "egg";
      f(s0, s1, useExpr);
      const s2:t = s0;
      f(s2, s0, useExpr);
      const s3:t = "";
      f(s3, s0, useExpr);
      f(s0, s3, useExpr);
    }
    relLocal_hgelp(t, useExpr);
    checkMemLeaks(m0);
  }

  proc relRemote(type t, useExpr=false) {
    writeln("=== relational operators (remote)");
    const m0 = allMemoryUsed();
    proc relRemote_help(type t, useExpr=false)
    {
      const s0:t = "chicken";
      on Locales[numLocales-1] {
        const s1:t = "egg";
        f(s0, s1, useExpr);
        const s2:t = s0;
        f(s2, s0, useExpr);
        on Locales[0] {
          f(s0, s1, useExpr);
          f(s2, s0, useExpr);
        }
        const s3:t ="";
        f(s3, s0, useExpr);
        f(s0, s3, useExpr);
        on Locales[0] {
          f(s3, s0, useExpr);
          f(s0, s3, useExpr);
        }
      }
    }
    relRemote_help(t, useExpr);
    checkMemLeaks(m0);
  }

  proc doIt(type t) {
    relLocal(t); relLocal(t, true);
    relRemote(t); relRemote(t, true);
  }

}
