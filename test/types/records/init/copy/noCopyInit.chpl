record R {
  proc init=(const ref other: R) where false {
    // the false where clause says no copy init allowed
  }
}

var r = new R();
var r2 = r;
