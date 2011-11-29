proc main(){
  var Dom : domain(opaque);
  var Idx = Dom.create();
  var A: [Dom] int;
  A[Idx] = 2;
  writeln(A);
  Dom.create();
  Dom.remove(Idx);
  writeln(A);
}
