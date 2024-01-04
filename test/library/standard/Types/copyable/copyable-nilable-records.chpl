use Types;

use Copyable;

class C { var x: int; }
record ContainingNilableOwned { var zq: owned C?; }

proc main() {

  checkMutable(ContainingNilableOwned, new ContainingNilableOwned());
}
