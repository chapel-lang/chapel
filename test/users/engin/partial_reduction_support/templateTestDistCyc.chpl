use CyclicDist;
use CommDiagnostics;

config const N = 16;

/*const space = {0..#N, 0..#N, 0..#N};*/
const space = {0..#N, 0..#N};
const ParentDom = space dmapped Cyclic(space.low);
var arr: [ParentDom] int;

for (i,j) in ParentDom {
  arr[i,j] = i*N**1 + j*N**0;
}

/*writeln(arr);*/
/*writeln("Reduction over array iterator");*/
//this doesn't sync appropriately
/*writeln(+ reduce arr.partialThese(1,(1,)));*/
/*writeln("Reduction over domain iterator");*/

//full test
/*startCommDiagnostics();*/
/*foo(arr, 2);*/
writeln("Naive template result");
/*writeln(dsiPartialReduce_template(arr, 1));*/
/*startCommDiagnostics();*/
/*writeln(getCommDiagnostics());*/

writeln("Optimized template result");
/*startCommDiagnostics();*/
writeln(arr._value.dsiPartialReduce_templateopt(1));
/*startCommDiagnostics();*/
/*writeln(getCommDiagnostics());*/

/*coforall l in Locales do on l do */
  /*writeln(arr._value.myLocArr);*/

