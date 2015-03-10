module unitTest {
  use main;

  proc substringLocal(type t, useExpr=false) {
    writeln("=== local substring");

    proc substringHelp(i) {
      const m0 = allMemoryUsed();
      proc substringHelp_help(i)
      {
        const s: t = "substring";
        if useExpr {
          writeMe(s[i]);
        } else {
          const ss = s[i];
          writeMe(ss);
        }
      }
      substringHelp_help(i);
      checkMemLeaks(m0);
    }

    var idx = -3;
    substringHelp(idx);
    var slice = idx..#abs(idx);
    substringHelp(slice);

    idx = 3;
    substringHelp(idx);
    slice = idx..#idx;
    substringHelp(slice);

    idx = 300;
    substringHelp(idx);
    slice = idx..#idx;
    substringHelp(slice);
  }

  proc substringRemote(type t, useExpr=false) {
    writeln("=== remote substring");

    proc substringHelp(i) {
      const m0 = allMemoryUsed();
      proc substringHelp_help(i)
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
      substringHelp_help(i);
      checkMemLeaks(m0);
    }

    var idx = -3;
    substringHelp(idx);
    var slice = idx..#abs(idx);
    substringHelp(slice);

    idx = 3;
    substringHelp(idx);
    slice = idx..#idx;
    substringHelp(slice);

    idx = 300;
    substringHelp(idx);
    slice = idx..#idx;
    substringHelp(slice);
  }

  proc doIt(type t) {
    substringLocal(t); substringLocal(t, true);
    substringRemote(t); substringRemote(t, true);
  }

}
