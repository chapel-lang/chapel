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
    on loc {
      this.xx = 555;
    }
    begin {
      this.yy = 666;
    }
    chpl_task_sleep(1);
  }
  // method with args
  proc modify(ee: int, ff: int) {
    on loc {
      this.xx = ee;
    }
    begin {
      this.yy = ff;
    }
    chpl_task_sleep(1);
  }
} // record RR

record QQ {
  var aa, bb: int;
  // non-default constructor
  proc QQ(cc: int, dd: int) {
    on loc {
      this.aa = cc;
    }
    begin {
      this.bb = dd;
    }
    chpl_task_sleep(1);
  }
  // method with no args
  proc modify() {
    on loc {
      this.aa = 171717;
    }
    begin {
      this.bb = 181818;
    }
    chpl_task_sleep(1);
  }
} // record QQ

writeln("start");

var rr = new RR();
var qq = new QQ(777, 888);
writeln(rr, qq);

rr.modify(151515, 161616);
qq.modify();
writeln(rr, qq);
