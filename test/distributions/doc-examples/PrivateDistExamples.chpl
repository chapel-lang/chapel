//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//

/* START_EXAMPLE */
use PrivateDist;

var A: [PrivateSpace] int;
forall a in A do
  a = a.locale.id;
writeln(A);
/* STOP_EXAMPLE */
