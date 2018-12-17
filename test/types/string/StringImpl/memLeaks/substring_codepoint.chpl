module unitTest {
  use main;

  proc substringLocalCodePoint(type t, useExpr=false) {
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

    proc substringHelpCPI(i) {
      const m0 = allMemoryUsed();
      {
        const s: t = "sûḃstríng";
        if useExpr {
          writeMe(codePointToString(s[i]));
        } else {
          const ss = codePointToString(s[i]);
          writeMe(ss);
        }
      }
      checkMemLeaks(m0);
    }

    var idx = 3:codePointIndex;
    substringHelpCPI(idx);

    var slice = ..idx;
    substringHelp(slice);

    var slice2 = idx..#(idx:int);
    substringHelp(slice2);

    var slice3 = idx..;
    substringHelp(slice3);
  }

  proc substringRemoteCodePoint(type t, useExpr=false) {
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

    proc substringHelpCPI(i) {
      const m0 = allMemoryUsed();
      {
        const s0: t = "sûḃstríng";
        on Locales[numLocales-1] {
          if useExpr {
            writeMe(codePointToString(s0[i]));
          } else {
            const ss = codePointToString(s0[i]);
            writeMe(ss);
          }
          const s1: t = "sûḃstríng";
          on Locales[0] {
            if useExpr {
              writeMe(codePointToString(s1[i]));
            } else {
              const ss = codePointToString(s1[i]);
              writeMe(ss);
            }
          }
        }
      }
      checkMemLeaks(m0);
    }

    var idx = 3:codePointIndex;
    substringHelpCPI(idx);

    var slice = ..idx;
    substringHelp(slice);

    var slice2 = idx..#(idx:int);
    substringHelp(slice2);

    var slice3 = idx..;
    substringHelp(slice3);
  }

  proc doIt(type t) {
    substringLocalCodePoint(t); substringLocalCodePoint(t, true);
    substringRemoteCodePoint(t); substringRemoteCodePoint(t, true);
  }

}
