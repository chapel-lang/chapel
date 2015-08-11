// Test that isRefIter() works for non-ref (val) these() iterators on records


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

writeln(isRefIter(r.these(tag=iterKind.follower, nil)));


//
// ensure iterators are correctly identified as val iters when passed as an
// argument to a proc
//
proc printIterRefness(myIter) where  isRefIter(myIter) { writeln("ref iter"); }
proc printIterRefness(myIter) where !isRefIter(myIter) { writeln("val iter"); }

printIterRefness(r.these());

printIterRefness(r.these(tag=iterKind.standalone));

printIterRefness(r.these(tag=iterKind.leader));

printIterRefness(r.these(tag=iterKind.follower, nil));


//
// ensure iterators are correctly identified as val iters when passed as an
// argument to a wrapping iter
//
iter printIterRefnessWrapper(myIter) ref where isRefIter(myIter) {
  writeln("ref iter");
  for i in myIter do yield i;
}
iter printIterRefnessWrapper(myIter) where !isRefIter(myIter) {
  writeln("val iter");
  for i in myIter do yield i;
}

for i in printIterRefnessWrapper(r.these()) do;

for i in printIterRefnessWrapper(r.these(tag=iterKind.standalone)) do;

for i in printIterRefnessWrapper(r.these(tag=iterKind.leader)) do;

for i in printIterRefnessWrapper(r.these(tag=iterKind.follower, nil)) do;


//
// ensure iterators are correctly identified as val iters when a record
// implementing a these() iterator is passed as an argument to a wrapping iter
//
iter printIterRefnessWrapper2(myRec) ref where isRefIter(_getIterator(myRec)) {
  writeln("ref iter");
  for i in myRec do yield i;
}
iter printIterRefnessWrapper2(myRec) where !isRefIter(_getIterator(myRec)) {
  writeln("val iter");
  for i in myRec do yield i;
}

for i in printIterRefnessWrapper2(r) do;
