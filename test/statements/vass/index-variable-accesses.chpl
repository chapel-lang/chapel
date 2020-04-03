
var GLOBAL: [11..16] int;
var TUPLE:  [21..24] (int,int);

// 'ref' iterator
iter singleVar(ix: int) ref {
  yield GLOBAL[ix];
}

// non-'ref' iterator
iter singleVal(ix: int) {
  yield GLOBAL[ix];
}

iter tupleVar(ix: int) ref {
  yield TUPLE[ix];
}

iter tupleVal(ix: int) {
  yield TUPLE[ix];
}

proc main() {

  /////////// non-tuple is yielded ///////////


  // _indexOfInterest: _ref(int)
  for isVar in singleVar(11) {
    isVar = 11001;
  }

  // _indexOfInterest: int
  for isVal in singleVal(12) {
    //illegal: isVal = 12001;            // error
  }


  /////////// tuple is yielded, not deconstructed ///////////


  // _indexOfInterest: _ref(2*int)
  for itVar in tupleVar(21) {
    itVar = (13001, 13002);
    itVar(0) = 13003;
    itVar(1) = 13004;
  }

  // _indexOfInterest: 2*int
  for itVal in tupleVal(22) {
    //illegal: itVal = (14001, 14002);   // error
    //illegal: itVal(1) = 14003;         // TODO: must be an error
    //illegal: itVal(2) = 14004;         // TODO: must be an error
  }


  /////////// tuple is yielded, deconstructed ///////////


  // _indexOfInterest: _ref(2*int)
  // iVar1, iVar2: _ref(int)
  for (iVar1, iVar2) in tupleVar(23) {
    iVar1 = 15001;
    iVar2 = 15002;
  }

  // _indexOfInterest: 2*int
  // iVal1, iVal2: int
  for (iVal1, iVal2) in tupleVal(24) {
    //illegal: iVal1 = 16001;            // error
    //illegal: iVal2 = 16002;            // error
  }


  /////////// zippered ///////////


  // _indexOfInterest: (_ref(int),int)
  for isZip in zip (singleVar(13), singleVal(14)) {
    //illegal: isZip = (17001, 17002);   // error
    isZip(0) = 17003;                    // would be nice to allow this
    //illegal: isZip(2) = 17004;         // TODO: must be an error
  }

  // _indexOfInterest: (_ref(int),int)
  // iVar1: _ref(int)
  // iVal2: int
  for (iVar1, iVal2) in zip (singleVar(15), singleVal(16)) {
    iVar1 = 18001;
    //illegal: iVal2 = 18002;            // error
  }


  /////////// done ///////////

  writeln(GLOBAL);
  writeln(TUPLE);
}
