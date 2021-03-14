
record Foo {
  var x : int;
}

var A : [1..4] [1..4] Foo;

iter allFoo() ref {
  for a in A do
    for f in a do
      yield f;
}

iter allFoo(param tag:iterKind) ref where tag == iterKind.leader {
  forall a in A do
    for f in a do
      yield f;
}

iter allFoo(param tag:iterKind, followThis) ref where tag == iterKind.follower {
  yield followThis;
}

forall f in allFoo() do f.x = 1;
writeln(A[1]); // Should print 1s

for f in allFoo() do f.x = 2;
writeln(A[1]); // Prints 2s as expected

forall a in A do
  for f in a do
    f.x = 3;

writeln(A[1]); // Prints 3s as expected
