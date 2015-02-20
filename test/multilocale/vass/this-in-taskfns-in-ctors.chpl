// Verifies updates to 'this' within tasks/on blocks within
// a constructor or a method.

// Style note: I use chpl_task_sleep to avoid 'sync' statements.
// I avoid 'sync' statements and do not import the Time module
// - in order to reduce the size of the AST and the generated code.
// That is useful for ease of debugging.

extern proc chpl_task_sleep(t:uint): void;
const loc = Locales[numLocales-1];

record RR {
  var xx, yy: int;
  // the default constructor
  proc RR() {
    var done$: sync bool;
    on loc {
      this.xx = 555;
    }
    begin {
      this.yy = 666;
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
      this.yy = ff;
      done$ = true;
    }
    done$;
  }
} // record RR

record QQ {
  var aa, bb: int;
  // non-default constructor
  proc QQ(cc: int, dd: int) {
    var done$: sync bool;
    on loc {
      this.aa = cc;
    }
    begin {
      this.bb = dd;
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
      this.bb = 181818;
      done$ = true;
    }
    done$;
  }
} // record QQ

writeln("start");

var rr = new RR();
var qq = new QQ(777, 888);
writeln(rr, qq);

rr.modify(151515, 161616);
qq.modify();
writeln(rr, qq);
