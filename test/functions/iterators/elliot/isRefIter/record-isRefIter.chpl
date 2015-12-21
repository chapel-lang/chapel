// Test that isRefIter() works for various non-these() iterators on records


//
// record with ref and non-ref versions of serial, standalone, and l/f iters
//
record R {
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
var r: R;
writeln(isRefIter(r.myValIter()));
writeln(isRefIter(r.myRefIter()));

writeln(isRefIter(r.myValIter(tag=iterKind.standalone)));
writeln(isRefIter(r.myRefIter(tag=iterKind.standalone)));

writeln(isRefIter(r.myValIter(tag=iterKind.leader)));
writeln(isRefIter(r.myRefIter(tag=iterKind.leader)));

writeln(isRefIter(r.myValIter(tag=iterKind.follower, 0)));
writeln(isRefIter(r.myRefIter(tag=iterKind.follower, 0)));


//
// check ref-ness of iters when passed as an argument to a proc
//
proc printIterRefness(myIter) where  isRefIter(myIter) { writeln("ref iter"); }
proc printIterRefness(myIter) where !isRefIter(myIter) { writeln("val iter"); }

printIterRefness(r.myValIter());
printIterRefness(r.myRefIter());

printIterRefness(r.myValIter(tag=iterKind.standalone));
printIterRefness(r.myRefIter(tag=iterKind.standalone));

printIterRefness(r.myValIter(tag=iterKind.leader));
printIterRefness(r.myRefIter(tag=iterKind.leader));

printIterRefness(r.myValIter(tag=iterKind.follower, 0));
printIterRefness(r.myRefIter(tag=iterKind.follower, 0));


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

for i in printIterRefnessWrapper(r.myValIter()) do;
for i in printIterRefnessWrapper(r.myRefIter()) do;

for i in printIterRefnessWrapper(r.myValIter(tag=iterKind.standalone)) do;
for i in printIterRefnessWrapper(r.myRefIter(tag=iterKind.standalone)) do;

for i in printIterRefnessWrapper(r.myValIter(tag=iterKind.leader)) do;
for i in printIterRefnessWrapper(r.myRefIter(tag=iterKind.leader)) do

for i in printIterRefnessWrapper(r.myValIter(tag=iterKind.follower, 0)) do;
for i in printIterRefnessWrapper(r.myRefIter(tag=iterKind.follower, 0)) do;
