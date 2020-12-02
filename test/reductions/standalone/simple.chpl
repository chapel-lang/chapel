// This program used to produce the incorrect result (count == 0 instead of 5)
iter myIter() {
  halt("Dummy serial iterator");
}

iter myIter(param tag) ref where tag==iterKind.standalone {
  forall i in 1..5 do yield i;
}

var count: int;
forall i in myIter() with (+ reduce count) {
  //writeln(i);
  count += 1;
}
writeln(count);
