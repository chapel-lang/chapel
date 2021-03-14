
iter myIter()           { yield 5; }
iter myIter(param tag)  { yield 6; }
iter notherIter()       { yield 7; }

for idx in zip(myIter(iterKind.leader), notherIter()) do
  writeln(idx);

for idx in zip(notherIter(), myIter(iterKind.standalone)) do
  writeln(idx);
