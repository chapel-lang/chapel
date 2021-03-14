// dummy serial version
//
// It doesn't really matter what the 'inner' iterators do
iter inner() {
  for i in 1..10 do yield i;
}
iter inner(param tag: iterKind) where tag == iterKind.standalone {
  for i in 1..10 do yield i;
}

// dummy serial version
//
// The issue with 'outer' seems to be iteration over the 'inner' iter with
// a specified tag.
iter outer() {
  for i in inner() {
    yield i;
    yield i;
  }
}
iter outer(param tag: iterKind) where tag == iterKind.standalone {
  // The following does not encounter a compiler failure: forall i in inner()

  for i in inner(tag) {
    // Only fails with multiple yields
    yield i;
    yield i;
  }
}

proc main() {
  // Only seems to appear with reductions (perhaps reduce-intent related?)
  const total = + reduce outer();
  writeln(total);
}
