//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//

/* START_EXAMPLE */
class C {
  var x: int;
}

var D = {0..0};
var A: [D] shared C = [new shared C(0)];
manage D.unsafeAssign({0..1}, checks=true) as mgr {
  // 'D' has a new index '1', so 'A' has a new element at '1',
  // which we need to initialize:
  mgr.initialize(A, 1, new shared C(1));
}
writeln(A);
/* STOP_EXAMPLE */
