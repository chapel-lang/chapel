// Test that isRefIter() works for various standalone iterators. Standalone in
// the sense that they're not iterators on a class or record


//
// declare ref and non-ref versions of serial, standalone, and l/f iters
//
var i: int;

iter myValIter()                                                { yield i; }
iter myRefIter() ref                                            { yield i; }

iter myValIter(param tag: iterKind)
  where tag==iterKind.standalone                                { yield i; }
iter myRefIter(param tag: iterKind) ref
  where tag==iterKind.standalone                                { yield i; }

iter myValIter(param tag: iterKind)
  where tag==iterKind.leader                                    { yield i; }
iter myRefIter(param tag: iterKind) ref
  where tag==iterKind.leader                                    { yield i; }

iter myValIter(param tag: iterKind, followThis)
  where tag==iterKind.follower                                  { yield i; }
iter myRefIter(param tag: iterKind, followThis) ref
  where tag==iterKind.follower                                  { yield i; }


//
// ensure ref iterators are correctly identified
//
writeln(isRefIter(myValIter()));
writeln(isRefIter(myRefIter()));

writeln(isRefIter(myValIter(tag=iterKind.standalone)));
writeln(isRefIter(myRefIter(tag=iterKind.standalone)));

writeln(isRefIter(myValIter(tag=iterKind.leader)));
writeln(isRefIter(myRefIter(tag=iterKind.leader)));

writeln(isRefIter(myValIter(tag=iterKind.follower, nil)));
writeln(isRefIter(myRefIter(tag=iterKind.follower, nil)));


//
// check ref-ness of iters when passed as an argument to a proc
//
proc printIterRefness(myIter) where  isRefIter(myIter) { writeln("ref iter"); }
proc printIterRefness(myIter) where !isRefIter(myIter) { writeln("val iter"); }

printIterRefness(myValIter());
printIterRefness(myRefIter());

printIterRefness(myValIter(tag=iterKind.standalone));
printIterRefness(myRefIter(tag=iterKind.standalone));

printIterRefness(myValIter(tag=iterKind.leader));
printIterRefness(myRefIter(tag=iterKind.leader));

printIterRefness(myValIter(tag=iterKind.follower, nil));
printIterRefness(myRefIter(tag=iterKind.follower, nil));


//
// check ref-ness of iters when passed as an argument to a wrapping iter
//
iter printIterRefnessWrapper(myIter) ref where isRefIter(myIter) {
  writeln("ref iter");
  for i in myIter do yield i;
}
iter printIterRefnessWrapper(myIter) where !isRefIter(myIter) {
  writeln("val iter");
  for i in myIter do yield i;
}

for i in printIterRefnessWrapper(myValIter()) do;
for i in printIterRefnessWrapper(myRefIter()) do;

for i in printIterRefnessWrapper(myValIter(tag=iterKind.standalone)) do;
for i in printIterRefnessWrapper(myRefIter(tag=iterKind.standalone)) do;

for i in printIterRefnessWrapper(myValIter(tag=iterKind.leader)) do;
for i in printIterRefnessWrapper(myRefIter(tag=iterKind.leader)) do

for i in printIterRefnessWrapper(myValIter(tag=iterKind.follower, nil)) do;
for i in printIterRefnessWrapper(myRefIter(tag=iterKind.follower, nil)) do;
