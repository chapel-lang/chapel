use Types;

use Copyable;

class C { var x: int; }
record ContainingNonNilableOwned { var z: owned C; }

proc main() {

  checkNo(ContainingNonNilableOwned,
          new ContainingNonNilableOwned(new owned C()));
}
