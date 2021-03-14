config const noisy = false;
config type myType = string;

enum E { a, e, i, o, u };
record R {
  var x: myType;
}
class C {
  var x: myType;
}

proc f(lid, rid) {
  on Locales[lid] {
    var x: myType;
    if noisy then
      writeln(x.locale.id);
    else
      assert(x.locale.id==lid);
    on Locales[rid] {
      if noisy then
        writeln(x.locale.id);
      else
        assert(x.locale.id==lid);
      on x do if noisy then writeln(here.id); else assert(here.id==lid);
    }

    var tx = (x, x);
    if noisy {
      writeln(tx.locale.id);
      writeln(tx(0).locale.id);
      writeln(tx(1).locale.id);
    } else {
      assert(tx.locale.id==lid);
      assert(tx(0).locale.id==lid);
      assert(tx(1).locale.id==lid);
    }
    on Locales[rid] {
      if noisy {
        writeln(tx.locale.id);
        writeln(tx(0).locale.id);
        writeln(tx(1).locale.id);
      } else {
        assert(tx.locale.id==lid);
        assert(tx(0).locale.id==lid);
        assert(tx(1).locale.id==lid);
      }
      on tx do if noisy then writeln(here.id); else assert(here.id==lid);
      on tx(0) do if noisy then writeln(here.id); else assert(here.id==lid);
      on tx(1) do if noisy then writeln(here.id); else assert(here.id==lid);
    }

    var rx: R;
    if noisy {
      writeln(rx.locale.id);
      writeln(rx.x.locale.id);
    } else {
      assert(rx.locale.id==lid);
      assert(rx.x.locale.id==lid);
    }
    on Locales[rid] {
      if noisy {
        writeln(rx.locale.id);
        writeln(rx.x.locale.id);
      } else {
        assert(rx.locale.id==lid);
        assert(rx.x.locale.id==lid);
      }
      on rx do if noisy then writeln(here.id); else assert(here.id==lid);
      on rx.x do if noisy then writeln(here.id); else assert(here.id==lid);
    }
    
    var cx = new unmanaged C();
    if noisy {
      writeln(cx.locale.id);
      writeln(cx.x.locale.id);
    } else {
      assert(cx.locale.id==lid);
      assert(cx.x.locale.id==lid);
    }
    on Locales[rid] {
      if noisy {
        writeln(cx.locale.id);
        writeln(cx.x.locale.id);
      } else {
        assert(cx.locale.id==lid);
        assert(cx.x.locale.id==lid);
      }
      on cx do if noisy then writeln(here.id); else assert(here.id==lid);
      on cx.x do if noisy then writeln(here.id); else assert(here.id==lid);
    }
    delete cx;
  }
}

f(0, numLocales-1);
f(numLocales-1, 0);
