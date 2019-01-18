/*
This tests the situation where a reduce intent and/or in intent
occurs in a forall loop, whose the parallel iterator contains a yield
that is in 2 parallel loops.

Specifically, in a parallel for loop that's in another parallel for loop.

When this is no longer an error, test for absence of data races, like
  parallel/forall/vass/reduce-with-forall-in-par-iter
  parallel/forall/vass/reduce-fatter-with-forall-in-par-iter
*/

iter myiter() {
  yield 555;
}

iter myiter(param tag) where tag == iterKind.standalone {
  coforall ooo in 1..3 {
    yield 666;
  }
}

iter pariter() {
  yield 333;
}

iter pariter(param tag) where tag == iterKind.standalone {
  for loc in Locales.these(iterKind.standalone) do
    for kkk in myiter(iterKind.standalone) do
      yield kkk + loc.id;
}

proc main {
  var result1 = 20000, result2 = 9;
  forall iii in pariter() with (
                                in result1,
                                + reduce result2
                                ) {
    writeln(result1, result2);
  }
}
