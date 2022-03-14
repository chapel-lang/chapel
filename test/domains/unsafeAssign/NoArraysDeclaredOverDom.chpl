class C { var x = 0; }

// Check for behavior when no arrays are declared over 'D'.
proc test1() {
  var D = {0..0};

  manage D.unsafeAssign({0..1}, checks=true) as mgr do;
}
test1();

