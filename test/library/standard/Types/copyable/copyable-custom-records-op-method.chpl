use Types;

use Copyable;

class C { var x: int; }
record CustomContainingNilableOwned {
  var zq: owned C?;
  proc init() {
    this.zq = nil;
  }
  proc init=(other) {
    this.zq = new owned C();
  }
}
operator CustomContainingNilableOwned.=(ref lhs: CustomContainingNilableOwned,
       const ref rhs: CustomContainingNilableOwned) {
  lhs.zq = new owned C();
}

record CustomContainingNonNilableOwned {
  var z: owned C;
  proc init() {
    this.z = new owned C();
  }
  proc init=(other) {
    this.z = new owned C();
  }
}
operator
CustomContainingNonNilableOwned.=(ref lhs: CustomContainingNonNilableOwned,
       const ref rhs: CustomContainingNonNilableOwned) {
  lhs.z = new owned C();
}

proc main() {

  checkNormal(CustomContainingNilableOwned, new CustomContainingNilableOwned());
  checkNormal(CustomContainingNonNilableOwned,
              new CustomContainingNonNilableOwned());
}
