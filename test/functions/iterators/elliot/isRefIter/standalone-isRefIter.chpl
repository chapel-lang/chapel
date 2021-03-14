// Test that isRefIter() works for various standalone iterators. Standalone in
// the sense that they're not iterators on a class or record

use printrefness;

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

writeln(isRefIter(myValIter(tag=iterKind.follower, 0)));
writeln(isRefIter(myRefIter(tag=iterKind.follower, 0)));


//
// check ref-ness of iters when passed as an argument to a proc
//
printIterRefness(myValIter());
printIterRefness(myRefIter());

printIterRefness(myValIter(tag=iterKind.standalone));
printIterRefness(myRefIter(tag=iterKind.standalone));

printIterRefness(myValIter(tag=iterKind.leader));
printIterRefness(myRefIter(tag=iterKind.leader));

printIterRefness(myValIter(tag=iterKind.follower, 0));
printIterRefness(myRefIter(tag=iterKind.follower, 0));


//
// check ref-ness of iters when passed as an argument to a wrapping iter
//
for i in printIterRefnessWrapper(myValIter()) do;
for i in printIterRefnessWrapper(myRefIter()) do;

for i in printIterRefnessWrapperP(myValIter(tag=iterKind.standalone)) do;
for i in printIterRefnessWrapperP(myRefIter(tag=iterKind.standalone)) do;

for i in printIterRefnessWrapperP(myValIter(tag=iterKind.leader)) do;
for i in printIterRefnessWrapperP(myRefIter(tag=iterKind.leader)) do

for i in printIterRefnessWrapper(myValIter(tag=iterKind.follower, 0)) do;
for i in printIterRefnessWrapper(myRefIter(tag=iterKind.follower, 0)) do;
