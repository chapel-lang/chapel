// Test assignments to records with consts.

record RECTYPE {
  var VARFIELD: int;
  const CONSTFIELD: int;
}

var RECGLOBvar: RECTYPE;
const RECGLOBconst: RECTYPE;

proc TEST1() {
  var RECVAR: RECTYPE = new RECTYPE();
  RECVAR = RECGLOBvar; // error - assignment to a record with a 'const' field
}

proc TEST2() {
  const RECCONST: RECTYPE = new RECTYPE();
  RECCONST = RECGLOBvar; // error -- assignment to a 'const'
}

proc TEST3() {
  var RECVAR1 = new RECTYPE(111, 222); // legal
  var RECVAR2: RECTYPE = new RECTYPE(); // legal
}

proc TEST4() {
  const RECCONST1 = new RECTYPE(111, 222);  // legal
  const RECCONST2: RECTYPE = new RECTYPE(); // legal
}

proc TEST5() {
  const RECCONST: RECTYPE;
  RECCONST.VARFIELD = 333;   // error
  RECCONST.CONSTFIELD = 333; // error
}

proc TEST6() {
  var RECVAR: RECTYPE;
  RECVAR.VARFIELD = 444;   // legal
  RECVAR.CONSTFIELD = 444; // error
}

proc TEST7(in arg: RECTYPE) { }
proc TEST8(ref arg: RECTYPE) { }
proc TEST9(const ref arg: RECTYPE) { }

TEST1();
TEST2();
TEST3();
TEST4();
TEST5();
TEST6();
TEST7(RECGLOBvar);    // legal
TEST7(RECGLOBconst);  // legal
TEST8(RECGLOBvar);    // legal
TEST8(RECGLOBconst);  // error
TEST9(RECGLOBvar);    // legal
TEST9(RECGLOBconst);  // legal

/////////////////////////////////

record OUTERREC {
  var outervar: RECTYPE;
  const outerconst: RECTYPE;
}

var OUTERGLOBvar: OUTERREC;
const OUTERGLOBconst: OUTERREC;

proc quest1() {
  var RECVAR: OUTERREC = new OUTERREC();
  RECVAR = OUTERGLOBvar; // error - assignment to a record with a 'const' field
}

proc quest2() {
  const RECCONST: OUTERREC = new OUTERREC();
  RECCONST = OUTERGLOBvar; // error -- assignment to a 'const'
}

proc quest3() {
  var RECVAR = new OUTERREC(); // legal
}

proc quest4() {
  const RECVAR = new OUTERREC(); // legal
}


proc quest5() {
  var recvar: OUTERREC;
  recvar.outervar.VARFIELD   = 44444411;   // legal
  recvar.outervar.CONSTFIELD = 44444422;   // error
  recvar.outerconst.VARFIELD   = 44444433;  // error
  recvar.outerconst.CONSTFIELD = 44444455;  // error
}

proc quest6() {
  const recconst: OUTERREC;
  recconst.outervar.VARFIELD   = 55555511;   // error
  recconst.outervar.CONSTFIELD = 55555522;   // error
  recconst.outerconst.VARFIELD   = 55555533;  // error
  recconst.outerconst.CONSTFIELD = 55555544;  // error
}

proc quest7(in arg: OUTERREC) { }
proc quest8(ref arg: OUTERREC) { }
proc quest9(const ref arg: OUTERREC) { }

quest1();
quest2();
quest3();
quest4();
quest5();
quest6();
quest7(OUTERGLOBvar);    // legal
quest7(OUTERGLOBconst);  // legal
quest8(OUTERGLOBvar);    // legal
quest8(OUTERGLOBconst);  // error
quest9(OUTERGLOBvar);    // legal
quest9(OUTERGLOBconst);  // legal
