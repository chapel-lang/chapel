use Memory.Initialization as Old;
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
    Old.moveSwap(lhs=a, b);
    MemMove.moveSwap(lhs=a, b);
    Old.moveSwap(x=a, b);
    MemMove.moveSwap(x=a, b);
    writeln(a, " :: ", b);
  }
  {
    var a = 1;
    var b = Old.moveToValue(a);
    var c = MemMove.moveToValue(b);
    writeln(c);
  }
  {
    var a = 1;
    var b : int;
    Old.moveInitialize(lhs=b, a);
    var c : int;
    MemMove.moveInitialize(lhs=c, b);
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
}
