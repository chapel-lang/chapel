// Test that isRefIter() works for ref these() iterators on records

use printrefness;

//
// record with ref versions of serial, standalone, and l/f iters
//
record R {
  var i: int;

  iter ref these() ref                                            { yield i; }

  iter ref these(param tag: iterKind) ref
    where tag==iterKind.standalone                                { yield i; }

  iter ref these(param tag: iterKind) ref
    where tag==iterKind.leader                                    { yield i; }

  iter ref these(param tag: iterKind, followThis) ref
    where tag==iterKind.follower                                  { yield i; }
}


//
// ensure iterators are correctly idenified as ref iters
//
var r: R;
writeln(isRefIter(r.these()));

writeln(isRefIter(r.these(tag=iterKind.standalone)));

writeln(isRefIter(r.these(tag=iterKind.leader)));

writeln(isRefIter(r.these(tag=iterKind.follower, 0)));


//
// ensure iterators are correctly identified as ref iters when passed as an
// argument to a proc
//
printIterRefness(r.these());

printIterRefness(r.these(tag=iterKind.standalone));

printIterRefness(r.these(tag=iterKind.leader));

printIterRefness(r.these(tag=iterKind.follower, 0));


//
// ensure iterators are correctly identified as ref iters when passed as an
// argument to a wrapping iter
//
for i in printIterRefnessWrapper(r.these()) do;

for i in printIterRefnessWrapperP(r.these(tag=iterKind.standalone)) do;

for i in printIterRefnessWrapperP(r.these(tag=iterKind.leader)) do;

for i in printIterRefnessWrapper(r.these(tag=iterKind.follower, 0)) do;


//
// ensure iterators are correctly identified as ref iters when a record
// implementing a these() iterator is passed as an argument to a wrapping iter
//
for i in printIterRefnessWrapper2(r) do;
