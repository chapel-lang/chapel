//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//

/* START_EXAMPLE */
// These will all print 'true'.
writeln((0..10).indexOrder(4) == 4);
writeln((1..10).indexOrder(4) == 3);
writeln((3..5).indexOrder(4) == 1);
writeln((0..10 by 2).indexOrder(4) == 2);
writeln((3..5 by 2).indexOrder(4) == -1);
/* STOP_EXAMPLE */
