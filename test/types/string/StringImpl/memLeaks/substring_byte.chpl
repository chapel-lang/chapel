module unitTest {
  use main;

  proc substringLocalByte(type t, useExpr=false) {
    writeln("=== local substring byte");

    proc substringHelp(i) {
      const m0 = allMemoryUsed();
      {
        const s: t = "substring";
        if useExpr {
          writeMe(try! s[i]);
        } else {
          const ss = try! s[i];
          writeMe(ss);
        }
      }
      checkMemLeaks(m0);
    }

    var idx = 2:byteIndex;
    substringHelp(idx);

    var slice = ..idx;
    substringHelp(slice);

    var slice2 = idx..#((idx+1):int);
    substringHelp(slice2);

    var slice3 = idx..;
    substringHelp(slice3);
  }

  proc substringRemoteByte(type t, useExpr=false) {
    writeln("=== remote substring byte");

    proc substringHelp(i) {
      const m0 = allMemoryUsed();
      {
        const s0: t = "substring";
        on Locales[numLocales-1] {
          if useExpr {
            writeMe(try! s0[i]);
          } else {
            const ss = try! s0[i];
            writeMe(ss);
          }
          const s1: t = "substring";
          on Locales[0] {
            if useExpr {
              writeMe(try! s1[i]);
            } else {
              const ss = try! s1[i];
              writeMe(ss);
            }
          }
        }
      }
      checkMemLeaks(m0);
    }

    var idx = 2:byteIndex;
    substringHelp(idx);

    var slice = ..idx;
    substringHelp(slice);

    var slice2 = idx..#((idx+1):int);
    substringHelp(slice2);

    var slice3 = idx..;
    substringHelp(slice3);
  }

  proc doIt(type t) {
    substringLocalByte(t); substringLocalByte(t, true);
    substringRemoteByte(t); substringRemoteByte(t, true);
  }

}
