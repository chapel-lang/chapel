
import MemMove;

record R {
  var x : int;
  proc deinit() {
    writeln("R.deinit: ", x);
  }
}

proc main() {
  {
    var a = 1, b = 9;

    MemMove.moveSwap(lhs=a, b);

    MemMove.moveSwap(x=a, b);
    writeln(a, " :: ", b);
  }
  {
    var a = 1;

    var c = MemMove.moveToValue(a);
    writeln(c);
  }
  {
    var a = 1;


    var c : int;
    MemMove.moveInitialize(lhs=c, a);
    writeln(c);
  }
  {
    pragma "no auto destroy"
    var a = new R(5);
    pragma "no auto destroy"
    var b = new R(10);
    if MemMove.needsDeinit(a.type) {
      MemMove.explicitDeinit(a);
    }
    if MemMove.needsDestroy(b.type) {
      MemMove.destroy(b);
    }
  }
  {
    var A, B : [1..10] int;
    MemMove.moveInitializeArrayElements(A, 1, 6, 5);
    MemMove.moveInitializeArrayElements(A, 1, B, 1, 10);
  }
}
