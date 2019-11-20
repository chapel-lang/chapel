module unitTest {
  use main;

  proc substringLocal(type t, useExpr=false) {
    writeln("=== local substring");

    proc substringHelp(i) {
      const m0 = allMemoryUsed();
      {
        const s: t = "substring";
        if useExpr {
          writeMe(s[i]);
        } else {
          const ss = s[i];
          writeMe(ss);
        }
      }
      checkMemLeaks(m0);
    }

    var idx = 2;
    substringHelp(idx);

    var slice = ..idx;
    substringHelp(slice);

    var slice2 = idx..#(idx+1);
    substringHelp(slice2);

    var slice3 = idx..;
    substringHelp(slice3);
  }

  proc substringRemote(type t, useExpr=false) {
    writeln("=== remote substring");

    proc substringHelp(i) {
      const m0 = allMemoryUsed();
      {
        const s0: t = "substring";
        on Locales[numLocales-1] {
          if useExpr {
            writeMe(s0[i]);
          } else {
            const ss = s0[i];
            writeMe(ss);
          }
          const s1: t = "substring";
          on Locales[0] {
            if useExpr {
              writeMe(s1[i]);
            } else {
              const ss = s1[i];
              writeMe(ss);
            }
          }
        }
      }
      checkMemLeaks(m0);
    }

    var idx = 2;
    substringHelp(idx);

    var slice = ..idx;
    substringHelp(slice);

    var slice2 = idx..#(idx+1);
    substringHelp(slice2);

    var slice3 = idx..;
    substringHelp(slice3);
  }

  proc doIt(type t) {
    substringLocal(t); substringLocal(t, true);
    substringRemote(t); substringRemote(t, true);
  }

}
