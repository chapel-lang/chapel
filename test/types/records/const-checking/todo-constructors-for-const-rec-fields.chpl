// Constructing a record with field(s) of a record type that itself includes
// const fields.

record RECTYPE {
  var VARFIELD: int;
  const CONSTFIELD: int;
}

var GLOBREC = new RECTYPE(10,20);

record OUTERREC {
  var outervar: RECTYPE;
  const outerconst: RECTYPE;
  // constructor #1
  proc OUTERREC(b:bool) {
    outervar = GLOBREC;      // legal
    outerconst = GLOBREC;    // legal
    setupRec(outervar, 3);   // legal
    setupRec(outerconst, 4); // legal
  }
  // constructor #2
  proc OUTERREC(i:int) {
    initOR();
    outervar.VARFIELD *= i;  // legal
  }
  proc initOR() {
    outervar = GLOBREC;      // legal
    outerconst = GLOBREC;    // legal
  }
}

proc setupRec(out result: RECTYPE, j: int) {
  result.VARFIELD += j;
  // Presently the following is illegal.
  // TODO: maybe there should be a way to do this legally, in this context?
  //result.CONSTFIELD = 10;
}

var o1 = new OUTERREC(true);
writeln(o1);
var o2 = new OUTERREC(123);
writeln(o2);
