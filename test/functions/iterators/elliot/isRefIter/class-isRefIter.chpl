// Test that isRefIter() works for various non-these() iterators on classes

use printrefness;

//
// class with ref and non-ref versions of serial, standalone, and l/f iters
//
class C {
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
}


//
// ensure ref iterators are correctly identified
//
var ownC = new owned C();
var c = ownC.borrow();
writeln(isRefIter(c.myValIter()));
writeln(isRefIter(c.myRefIter()));

writeln(isRefIter(c.myValIter(tag=iterKind.standalone)));
writeln(isRefIter(c.myRefIter(tag=iterKind.standalone)));

writeln(isRefIter(c.myValIter(tag=iterKind.leader)));
writeln(isRefIter(c.myRefIter(tag=iterKind.leader)));

writeln(isRefIter(c.myValIter(tag=iterKind.follower, 0)));
writeln(isRefIter(c.myRefIter(tag=iterKind.follower, 0)));


//
// check ref-ness of iters when passed as an argument to a proc
//
printIterRefness(c.myValIter());
printIterRefness(c.myRefIter());

printIterRefness(c.myValIter(tag=iterKind.standalone));
printIterRefness(c.myRefIter(tag=iterKind.standalone));

printIterRefness(c.myValIter(tag=iterKind.leader));
printIterRefness(c.myRefIter(tag=iterKind.leader));

printIterRefness(c.myValIter(tag=iterKind.follower, 0));
printIterRefness(c.myRefIter(tag=iterKind.follower, 0));


//
// check ref-ness of iters when passed as an argument to a wrapping iter
//
for i in printIterRefnessWrapper(c.myValIter()) do;
for i in printIterRefnessWrapper(c.myRefIter()) do;

for i in printIterRefnessWrapperP(c.myValIter(tag=iterKind.standalone)) do;
for i in printIterRefnessWrapperP(c.myRefIter(tag=iterKind.standalone)) do;

for i in printIterRefnessWrapperP(c.myValIter(tag=iterKind.leader)) do;
for i in printIterRefnessWrapperP(c.myRefIter(tag=iterKind.leader)) do

for i in printIterRefnessWrapper(c.myValIter(tag=iterKind.follower, 0)) do;
for i in printIterRefnessWrapper(c.myRefIter(tag=iterKind.follower, 0)) do;
