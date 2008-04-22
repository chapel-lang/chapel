def main(){
  var Dom : domain(opaque);
  var Idx = Dom.create();
  var A: [Dom] int;
  A[Idx] = 2;
  writeln(A);
  Dom.create();
  writeln(A);
  Dom.remove(Idx);
  writeln(A);
}
