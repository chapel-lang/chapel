// This program used to produce the incorrect result (count == 0 instead of 5)
// when the standalone iterator (incorrectly) had the 'ref' return intent
iter myIter() {
  halt("Dummy serial iterator");
}

iter myIter(param tag) where tag==iterKind.standalone {
  forall i in 1..5 do yield i;
}

var count: int;
forall i in myIter() with (+ reduce count) {
  //writeln(i);
  count += 1;
}
writeln(count);
