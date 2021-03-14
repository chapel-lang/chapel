// Cf. test/expressions/vass/eval-of-arg-of-.type.chpl

iter myIter() {
  writeln("start myIter");
  yield 777;
  writeln("finish myIter");
}

type t = ( [idx in myIter()] idx ).type;
writeln(t:string);

type q = ( [(i1,i2) in zip(myIter(),myIter())] i1+i2 ).type;
writeln(q:string);
