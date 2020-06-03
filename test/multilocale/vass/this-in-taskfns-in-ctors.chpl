// Verifies updates to 'this' within tasks/on blocks within
// a constructor or a method.

const loc = Locales[numLocales-1];

record RR {
  var xx, yy: int;
  // the default initializer
  proc init() {
    this.complete();
    var done$: sync bool;
    on loc {
      this.xx = 555;
    }
    begin {
      doModify(this, 666);
      done$ = true;
    }
    done$;
  }
  // method with args
  proc modify(ee: int, ff: int) {
    var done$: sync bool;
    on loc {
      this.xx = ee;
    }
    begin {
      doModify(this, ff);
      done$ = true;
    }
    done$;
  }
} // record RR

proc doModify(ref recv: RR, newval: int) { recv.yy = newval; }

record QQ {
  var aa, bb: int;
  // non-default initializer
  proc init(cc: int, dd: int) {
    this.complete();
    var done$: sync bool;
    on loc {
      this.aa = cc;
    }
    begin {
      doModify(this, dd);
      done$ = true;
    }
    done$;
  }
  // method with no args
  proc modify() {
    var done$: sync bool;
    on loc {
      this.aa = 171717;
    }
    begin {
      doModify(this, 181818);
      done$ = true;
    }
    done$;
  }
} // record QQ

proc doModify(ref recv: QQ, newval: int) { recv.bb = newval; }

writeln("start");

var rr = new RR();
var qq = new QQ(777, 888);
writeln(rr, qq);

rr.modify(151515, 161616);
qq.modify();
writeln(rr, qq);
