
iter myIter() {
  writeln("start myIter");
  yield 777;
  writeln("finish myIter");
}

type t = (+ reduce [idx in myIter()] idx).type;
writeln(t:string);

type q = (minloc reduce [idx in myIter()] (idx*0.1,idx)).type;
writeln(q:string);
