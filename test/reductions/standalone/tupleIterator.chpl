// This program used used to not compile due to assertion error in [implementForallIntents.cpp:1125]
iter myIter() {
  halt("Dummy serial iterator");
}

iter myIter(param tag) ref where tag==iterKind.standalone {
  forall i in zip(1..5, 1..5) do yield i;
}

var count: int;
forall i in myIter() with (+ reduce count) {
  //writeln(i);
  count += 1;
}
writeln(count);
