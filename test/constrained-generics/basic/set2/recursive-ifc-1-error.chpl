// This code contains a "recursive" interface, i.e.
// its associated type is constrained by the same interface.
//
// It is similar to recursive-ifc-1-success.chpl
// except it tests whether we report an error successfully when due.

interface IFC {
  type AT;
  AT implements IFC;
  proc reqFun(formal: Self);
}

int implements IFC;  // error: reqFun is not implemented
proc int.AT type return int;
