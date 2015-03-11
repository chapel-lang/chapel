// Const checking for array fields of a record.

var ARR: [1..2] int;

record REC {
  var   arvar: [1..2] int;
  const arcst: [1..2] int;
  proc REC() {
    arvar = ARR;
    arcst = ARR;
    arvar[1] = 1111;
    arcst[2] = 1111;
  }
}

class CLSS {
  var   cvar: [1..2] int;
  const ccst: [1..2] int;
  proc CLSS() {
    cvar = ARR;
    ccst = ARR;
    cvar[1] = 1111;
    ccst[2] = 1111;
  }
}

var cvar = new CLSS();
const cconst = new CLSS();
var qvar: REC;
const qconst: REC;

proc main {
  cvar.cvar = ARR;
  cvar.ccst = ARR;
  cconst.cvar = ARR;
  cconst.ccst = ARR;
  cvar.cvar[1] = 1111;
  cvar.ccst[1] = 6666;
  cconst.cvar[1] = 1111;
  cconst.ccst[1] = 6666;

  qvar.arvar = ARR;
  qvar.arcst = ARR;
  qconst.arvar = ARR;
  qconst.arcst = ARR;
  qvar.arvar[1] = 1111;
  qvar.arcst[1] = 6666;
  qconst.arvar[1] = 6666;
  qconst.arcst[1] = 6666;
}
