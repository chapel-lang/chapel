// Constructors, initialization.

record RECTYPE {
  var VARFIELD: int;
  const CONSTFIELD: int;

  proc RECTYPE() {
    writeln("RECTYPE constructor");
    VARFIELD   = 200;
    CONSTFIELD = 300;
    initRT();
  }

  proc initialize() {
    writeln("RECTYPE.initialize()");
    initRT();
  }

  proc initRT() {
    writeln("RECTYPE.initRT()");
    VARFIELD   *= 2;
    CONSTFIELD *= 3;
    setupInt(VARFIELD);
    setupInt(CONSTFIELD);
  }
}

proc setupInt(ref ARG: int) {
  ARG += 1;
}

writeln("Note: this test is about record field accesses, not about");
writeln("how constructors and initialize() are currently invoked.");
writeln("If/when the latter changes, the .good should be adjusted.");
writeln();

var r1: RECTYPE;
writeln(r1);  //(401,901)
writeln();

var r2 = new RECTYPE();
writeln(r2);  //(401,901)
writeln();
