use Memory.Initialization as Old;
import MemMove;

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
}
