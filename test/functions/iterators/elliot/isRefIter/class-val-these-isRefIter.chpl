// Test that isRefIter() works for non-ref (val) these() iterators on classes

use printrefness;

//
// class with val versions of serial, standalone, and l/f iters
//
class C {
  var i: int;

  iter these()                                                { yield i; }

  iter these(param tag: iterKind)
    where tag==iterKind.standalone                                { yield i; }

  iter these(param tag: iterKind)
    where tag==iterKind.leader                                    { yield i; }

  iter these(param tag: iterKind, followThis)
    where tag==iterKind.follower                                  { yield i; }
}


//
// ensure iterators are correctly idenified as val iters
//
var c = new borrowed C();
writeln(isRefIter(c.these()));

writeln(isRefIter(c.these(tag=iterKind.standalone)));

writeln(isRefIter(c.these(tag=iterKind.leader)));

writeln(isRefIter(c.these(tag=iterKind.follower, 0)));


//
// ensure iterators are correctly identified as val iters when passed as an
// argument to a proc
//
printIterRefness(c.these());

printIterRefness(c.these(tag=iterKind.standalone));

printIterRefness(c.these(tag=iterKind.leader));

printIterRefness(c.these(tag=iterKind.follower, 0));


//
// ensure iterators are correctly identified as val iters when passed as an
// argument to a wrapping iter
//
for i in printIterRefnessWrapper(c.these()) do;

for i in printIterRefnessWrapperP(c.these(tag=iterKind.standalone)) do;

for i in printIterRefnessWrapperP(c.these(tag=iterKind.leader)) do;

for i in printIterRefnessWrapper(c.these(tag=iterKind.follower, 0)) do;


//
// ensure iterators are correctly identified as val iters when a class
// implementing a these() iterator is passed as an argument to a wrapping iter
//
for i in printIterRefnessWrapper2(c) do;
