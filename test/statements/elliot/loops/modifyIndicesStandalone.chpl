// verify that const checking of loop indices is working correctly for
// standalone iters

forall i in 1..10 {
  i = 1;
  writeln(i);
}

forall i in (1..10).these() {
  i = 1;
  writeln(i);
}

for i in (1..10).these(tag=iterKind.standalone) {
  i = 1;
  writeln(i);
}
