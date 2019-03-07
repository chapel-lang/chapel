// Functions+iterators that report iterator-refness of the argument.

//
// the argument is an iterator call
//
proc printIterRefness(myIter) where  isRefIter(myIter) { writeln("ref iter"); }
proc printIterRefness(myIter) where !isRefIter(myIter) { writeln("val iter"); }

//
// the argument is an iterator call
//
iter printIterRefnessWrapper(myIter) ref where isRefIter(myIter) {
  writeln("ref iter");
  for i in myIter do yield i;
}
iter printIterRefnessWrapper(myIter) where !isRefIter(myIter) {
  writeln("val iter");
  for i in myIter do yield i;
}

//
// the argument is a parallel iterator call
// avoid iterating over it
//
var globalVar: int;
iter printIterRefnessWrapperP(myIter) ref where isRefIter(myIter) {
  writeln("ref iter");
  yield globalVar;
}
iter printIterRefnessWrapperP(myIter) where !isRefIter(myIter) {
  writeln("val iter");
  yield globalVar;
}

//
// the argument is a record or class
//
iter printIterRefnessWrapper2(myRec) ref where isRefIter(_getIterator(myRec)) {
  writeln("ref iter");
  for i in myRec do yield i;
}
iter printIterRefnessWrapper2(myRec) where !isRefIter(_getIterator(myRec)) {
  writeln("val iter");
  for i in myRec do yield i;
}
