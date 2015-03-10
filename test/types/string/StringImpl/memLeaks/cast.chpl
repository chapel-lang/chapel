module unitTest {
  use main;

  proc castAll(type t, useExpr=false) {
    inline proc f(x) {
      if useExpr {
        writeMe(x:t);
      } else {
        const s = x:t;
        writeMe(s);
      }
    }

    proc fint(param size=8) {
      writeln("=== cast from int(", size, ")");
      const m0 = allMemoryUsed();
      {
        const x = 17:int(size);
        f(x);
      }
      checkMemLeaks(m0);
    }
    for param i in 1..4 do fint(4<<i);

    proc fuint(param size=8) {
      writeln("=== cast from uint(", size, ")");
      const m0 = allMemoryUsed();
      {
        const x = 23:uint(size);
        f(x);
      }
      checkMemLeaks(m0);
    }
    for param i in 1..4 do fuint(4<<i);

    proc freal(param size=32) {
      writeln("=== cast from real(", size, ")");
      const m0 = allMemoryUsed();
      {
        const x = 3.14:real(size);
        f(x);
      }
      checkMemLeaks(m0);
    }
    freal(32);
    freal(64);

    proc fimag(param size=32) {
      writeln("=== cast from imag(", size, ")");
      const m0 = allMemoryUsed();
      {
        const x = 3.14i:imag(size);
        f(x);
      }
      checkMemLeaks(m0);
    }
    fimag(32);
    fimag(64);

    proc fcomplex(param size=64) {
      writeln("=== cast from complex(", size, ")");
      const m0 = allMemoryUsed();
      {
        const x = (3.0+0.14i):complex(size);
        f(x);
      }
      checkMemLeaks(m0);
    }
    fcomplex(64);
    fcomplex(128);

    proc fenum() {
      writeln("=== cast from enum E");
      const m0 = allMemoryUsed();
      {
        const x = E.three;
        f(x);
      }
      checkMemLeaks(m0);
    }
    fenum();

    proc fbool() {
      writeln("=== cast from bool");
      const m0 = allMemoryUsed();
      {
        const x = true;
        f(x);
      }
      checkMemLeaks(m0);
    }
    fbool();
  }

  proc cast_from_c_string(type t, useExpr=false) {
    writeln("=== cast from c_string");
    const m0 = allMemoryUsed();
    {
      const x: c_string = "cs";
      if useExpr {
        writeMe(x:t);
      } else {
        const s = x:t;
        writeMe(s);
      }
    }
    checkMemLeaks(m0);
  }

  proc cast_to_c_string(type t, useExpr=false) {
    writeln("=== cast to c_string");
    const m0 = allMemoryUsed();
    {
      const x: t = "cs";
      if useExpr {
        writeMe(x.c_str());
      } else {
        const s = x.c_str();
        writeMe(s);
      }
    }
    checkMemLeaks(m0);
  }

  proc doIt(type t) {
    castAll(t); castAll(t, true);
    cast_from_c_string(t); cast_from_c_string(t, true);
    cast_to_c_string(t); cast_to_c_string(t, true);
  }

}
