module unitTest {
  use main;

  proc concatAll(type t, useExpr=false) {
    inline proc fLocal(x) {
      const s: t = "s";
      if useExpr {
        writeMe(s+x:string);
        writeMe(x:string+s);
      } else {
        const sx = s+x:string;
        writeMe(sx);
        const xs = x:string+s;
        writeMe(xs);
      }
    }

    inline proc fRemote(x) {
      const s: t = "s";
      on Locales[numLocales-1] {
        if useExpr {
          writeMe(s+x:string);
          writeMe(x:string+s);
        } else {
          const sx = s+x:string;
          writeMe(sx);
          const xs = x:string+s;
          writeMe(xs);
        }
      }
    }

    // Without this string-specific overload, the above version of the
    // function generates an error: "follower iterators accepting a
    // non-POD argument by in-intent are not implemented" when strings
    // are cast to string.

    inline proc fRemote(x:string) {
      const s: t = "s";
      on Locales[numLocales-1] {
        if useExpr {
          writeMe(s+x);
          writeMe(x+s);
        } else {
          const sx = s+x;
          writeMe(sx);
          const xs = x+s;
          writeMe(xs);
        }
      }
    }

    proc fempty() {
      writeln("=== concat with empty");
      const m0 = allMemoryUsed();
      {
        const x: t;
        fLocal(x); fRemote(x);;
      }
      checkMemLeaks(m0);
    }
    fempty();

    proc fint(param size=8) {
      writeln("=== concat with int(", size, ")");
      const m0 = allMemoryUsed();
      {
        const x = 17:int(size);
        fLocal(x); fRemote(x);;
      }
      checkMemLeaks(m0);
    }
    for param i in 1..4 do fint(4<<i);

    proc fuint(param size=8) {
      writeln("=== concat with uint(", size, ")");
      const m0 = allMemoryUsed();
      {
        const x = 23:uint(size);
        fLocal(x); fRemote(x);;
      }
      checkMemLeaks(m0);
    }
    for param i in 1..4 do fuint(4<<i);

    proc freal(param size=32) {
      writeln("=== concat with real(", size, ")");
      const m0 = allMemoryUsed();
      {
        const x = 3.14:real(size);
        fLocal(x); fRemote(x);;
      }
      checkMemLeaks(m0);
    }
    freal(32);
    freal(64);

    proc fimag(param size=32) {
      writeln("=== concat with imag(", size, ")");
      const m0 = allMemoryUsed();
      {
        const x = 3.14i:imag(size);
        fLocal(x); fRemote(x);;
      }
      checkMemLeaks(m0);
    }
    fimag(32);
    fimag(64);

    proc fcomplex(param size=64) {
      writeln("=== concat with complex(", size, ")");
      const m0 = allMemoryUsed();
      {
        const x = (3.0+0.14i):complex(size);
        fLocal(x); fRemote(x);;
      }
      checkMemLeaks(m0);
    }
    fcomplex(64);
    fcomplex(128);

    proc fenum() {
      writeln("=== concat with enum E");
      const m0 = allMemoryUsed();
      {
        const x = E.three;
        fLocal(x); fRemote(x);;
      }
      checkMemLeaks(m0);
    }
    fenum();

    proc fbool() {
      writeln("=== concat with bool");
      const m0 = allMemoryUsed();
      {
        const x = true;
        fLocal(x); fRemote(x);;
      }
      checkMemLeaks(m0);
    }
    fbool();
  }

  proc concat0(type t, useExpr=false) {
    writeln("=== concat: string + string");
    const m0 = allMemoryUsed();
    {
      var s0, s1: t;
      s0 = "s";
      s1 = "0";
      if useExpr {
        writeMe(s0+s1);
      } else {
        const s0s1 = s0+s1;
        writeMe(s0s1);
      }
    }
    checkMemLeaks(m0);
  }

  proc concat1(type t, useExpr=false) {
    writeln("=== concat: remote + remote");
    const m0 = allMemoryUsed();
    {
      var s0, s1: t;
      s0 = "s";
      s1 = "0";
      on Locales[numLocales-1] {
        if useExpr {
          writeMe(s0+s1);
        } else {
          const s0s1 = s0+s1;
          writeMe(s0s1);
        }
      }
    }
    checkMemLeaks(m0);
  }

  proc concat2(type t, useExpr=false) {
    writeln("=== concat: remote + local");
    const m0 = allMemoryUsed();
    {
      var s0: t;
      s0 = "s";
      on Locales[numLocales-1] {
        const s1: t = "r";
        if useExpr {
          writeMe(s0+s1);
        } else {
          const s0s1 = s0+s1;
          writeMe(s0s1);
        }
      }
    }
    checkMemLeaks(m0);
  }

  proc concat_c_string0(type t, useExpr=false) {
    writeln("=== concat: string + c_string");
    const m0 = allMemoryUsed();
    {
      const s: t = "s";
      const cs: c_string = "0";
      try! {
        if useExpr {
          writeMe(s+createStringWithNewBuffer(cs));
        } else {
          const scs = s+createStringWithNewBuffer(cs);
          writeMe(scs);
        }
      }
    }
    checkMemLeaks(m0);
  }

  proc concat_c_string1(type t, useExpr=false) {
    writeln("=== concat: c_string + string");
    const m0 = allMemoryUsed();
    {
      const cs: c_string = "s";
      const s: t = "0";
      try! {
        if useExpr {
          writeMe(createStringWithNewBuffer(cs)+s);
        } else {
          const css = createStringWithNewBuffer(cs)+s;
          writeMe(css);
        }
      }
    }
    checkMemLeaks(m0);
  }

  proc concat_c_string2(type t, useExpr=false) {
    writeln("=== concat: remote string + c_string");
    const m0 = allMemoryUsed();
    {
      const s: t = "s";
      on Locales[numLocales-1] {
        const cs: c_string = "r";
        try! {
          if useExpr {
            writeMe(s+createStringWithNewBuffer(cs));
          } else {
            const scs = s+createStringWithNewBuffer(cs);
            writeMe(scs);
          }
        }
      }
    }
    checkMemLeaks(m0);
  }

  proc concat_c_string3(type t, useExpr=false) {
    writeln("=== concat: c_string + remote string");
    const m0 = allMemoryUsed();
    {
      const s: t = "0";
      on Locales[numLocales-1] {
        const cs: c_string = "s";
        try! {
          if useExpr {
            writeMe(createStringWithNewBuffer(cs)+s);
          } else {
            const css = createStringWithNewBuffer(cs)+s;
            writeMe(css);
          }
        }
      }
    }
    checkMemLeaks(m0);
  }

  proc doIt(type t) {
    concatAll(t); concatAll(t, true);

    concat0(t); concat0(t, true);
    concat1(t); concat1(t, true);
    concat2(t); concat2(t, true);

    concat_c_string0(t); concat_c_string0(t, true);
    concat_c_string1(t); concat_c_string1(t, true);
    concat_c_string2(t); concat_c_string2(t, true);
    concat_c_string3(t); concat_c_string3(t, true);
  }

}
