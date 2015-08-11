// Test that isRefIter() works for non-ref (val) these() iterators on classes


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
var c = new C();
writeln(isRefIter(c.these()));

writeln(isRefIter(c.these(tag=iterKind.standalone)));

writeln(isRefIter(c.these(tag=iterKind.leader)));

writeln(isRefIter(c.these(tag=iterKind.follower, nil)));


//
// ensure iterators are correctly identified as val iters when passed as an
// argument to a proc
//
proc printIterRefness(myIter) where  isRefIter(myIter) { writeln("ref iter"); }
proc printIterRefness(myIter) where !isRefIter(myIter) { writeln("val iter"); }

printIterRefness(c.these());

printIterRefness(c.these(tag=iterKind.standalone));

printIterRefness(c.these(tag=iterKind.leader));

printIterRefness(c.these(tag=iterKind.follower, nil));


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

for i in printIterRefnessWrapper(c.these()) do;

for i in printIterRefnessWrapper(c.these(tag=iterKind.standalone)) do;

for i in printIterRefnessWrapper(c.these(tag=iterKind.leader)) do;

for i in printIterRefnessWrapper(c.these(tag=iterKind.follower, nil)) do;


//
// ensure iterators are correctly identified as val iters when a class
// implementing a these() iterator is passed as an argument to a wrapping iter
//
iter printIterRefnessWrapper2(myCls) ref where isRefIter(_getIterator(myCls)) {
  writeln("ref iter");
  for i in myCls do yield i;
}
iter printIterRefnessWrapper2(myCls) where !isRefIter(_getIterator(myCls)) {
  writeln("val iter");
  for i in myCls do yield i;
}

for i in printIterRefnessWrapper2(c) do;
