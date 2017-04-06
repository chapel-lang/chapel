
var GLOBAL: int;

// 'ref' iterator
iter singleVar() ref {
  yield GLOBAL;
}

// non-'ref' iterator
iter singleVal() {
  yield GLOBAL;
}

proc main() {

  // _indexOfInterest: (_ref(int),int)
  for isZip in zip (singleVar(), singleVal()) {
    isZip = (17001, 17002);   // error
    isZip(1) = 17003;
    isZip(2) = 17004;         // error
  }


  /////////// done ///////////

}
