module unitTest {
  use main;

  proc substringLocalCodepoint(type t, useExpr=false) {
    writeln("=== local substring codepoint");

    proc substringHelp(i) {
      const m0 = allMemoryUsed();
      {
        const s: t = "sûḃstríng";
        if useExpr {
          writeMe(s[i]);
        } else {
          const ss = s[i];
          writeMe(ss);
        }
      }
      checkMemLeaks(m0);
    }

    var idx = 2:codepointIndex;
    substringHelp(idx);

    var slice = ..idx;
    substringHelp(slice);

    var slice2 = idx..#((idx+1):int);
    substringHelp(slice2);

    var slice3 = idx..;
    substringHelp(slice3);
  }

  proc substringRemoteCodepoint(type t, useExpr=false) {
    writeln("=== remote substring codepoint");

    proc substringHelp(i) {
      const m0 = allMemoryUsed();
      {
        const s0: t = "sûḃstríng";
        on Locales[numLocales-1] {
          if useExpr {
            writeMe(s0[i]);
          } else {
            const ss = s0[i];
            writeMe(ss);
          }
          const s1: t = "sûḃstríng";
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

    var idx = 2:codepointIndex;
    substringHelp(idx);

    var slice = ..idx;
    substringHelp(slice);

    var slice2 = idx..#((idx+1):int);
    substringHelp(slice2);

    var slice3 = idx..;
    substringHelp(slice3);
  }

  proc doIt(type t) {
    substringLocalCodepoint(t); substringLocalCodepoint(t, true);
    substringRemoteCodepoint(t); substringRemoteCodepoint(t, true);
  }

}
