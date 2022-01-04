use BlockDist;

config const n = 10;

var space = {1..n};
var dom = space dmapped Block(space, targetLocales=[here.getChild(1),]);
var arr: [dom] int;

ref data = arr._value.myLocArr!.myElems;
forall i in dom do
  data[i] = 1;

writeln(arr);
