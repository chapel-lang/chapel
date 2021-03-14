// Test that isRefIter() works for non-ref (val) these() iterators on records

use printrefness;

//
// record with val versions of serial, standalone, and l/f iters
//
record R {
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
var r: R;
writeln(isRefIter(r.these()));

writeln(isRefIter(r.these(tag=iterKind.standalone)));

writeln(isRefIter(r.these(tag=iterKind.leader)));

writeln(isRefIter(r.these(tag=iterKind.follower, 0)));


//
// ensure iterators are correctly identified as val iters when passed as an
// argument to a proc
//
printIterRefness(r.these());

printIterRefness(r.these(tag=iterKind.standalone));

printIterRefness(r.these(tag=iterKind.leader));

printIterRefness(r.these(tag=iterKind.follower, 0));


//
// ensure iterators are correctly identified as val iters when passed as an
// argument to a wrapping iter
//
for i in printIterRefnessWrapper(r.these()) do;

for i in printIterRefnessWrapperP(r.these(tag=iterKind.standalone)) do;

for i in printIterRefnessWrapperP(r.these(tag=iterKind.leader)) do;

for i in printIterRefnessWrapper(r.these(tag=iterKind.follower, 0)) do;


//
// ensure iterators are correctly identified as val iters when a record
// implementing a these() iterator is passed as an argument to a wrapping iter
//
for i in printIterRefnessWrapper2(r) do;
