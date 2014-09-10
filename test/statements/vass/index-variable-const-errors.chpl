
var GLOBAL: int;
var TUPLE: (int,int);

// 'ref' iterator
iter singleVar() ref {
  yield GLOBAL;
}

// non-'ref' iterator
iter singleVal() {
  yield GLOBAL;
}

iter tupleVar() ref {
  yield TUPLE;
}

iter tupleVal() {
  yield TUPLE;
}

proc main() {

  /////////// non-tuple is yielded ///////////


  // _indexOfInterest: _ref(int)
  for isVar in singleVar() {
    isVar = 11001;
  }

  // _indexOfInterest: int
  for isVal in singleVal() {
    isVal = 12001;            // error
  }


  /////////// tuple is yielded, not deconstructed ///////////


  // _indexOfInterest: _ref(2*int)
  for itVar in tupleVar() {
    itVar = (13001, 13002);
    itVar(1) = 13003;
    itVar(2) = 13004;
  }

  // _indexOfInterest: 2*int
  for itVal in tupleVal() {
    itVal = (14001, 14002);   // error
    itVal(1) = 14003;         // TODO: must be an error
    itVal(2) = 14004;         // TODO: must be an error
  }


  /////////// tuple is yielded, deconstructed ///////////


  // _indexOfInterest: _ref(2*int)
  // iVar1, iVar2: _ref(int)
  for (iVar1, iVar2) in tupleVar() {
    iVar1 = 15001;
    iVar2 = 15002;
  }

  // _indexOfInterest: 2*int
  // iVal1, iVal2: int
  for (iVal1, iVal2) in tupleVal() {
    iVal1 = 16001;            // error
    iVal2 = 16002;            // error
  }


  /////////// zippered ///////////


  // _indexOfInterest: (_ref(int),int)
  for isZip in zip (singleVar(), singleVal()) {
    isZip = (17001, 17002);   // error
    isZip(1) = 17003;
    isZip(2) = 17004;         // TODO: must be an error
  }

  // _indexOfInterest: (_ref(int),int)
  // iVar1: _ref(int)
  // iVal2: int
  for (iVar1, iVal2) in zip (singleVar(), singleVal()) {
    iVar1 = 18001;
    iVal2 = 18002;            // error
  }


  /////////// done ///////////

  compilerError("done");  // no need to execute
}
