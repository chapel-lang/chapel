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
}
