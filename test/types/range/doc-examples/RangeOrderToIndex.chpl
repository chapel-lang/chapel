//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//

/* START_EXAMPLE */
// These will all print 'true'.
writeln((0..10).orderToIndex(4) == 4);
writeln((1..10).orderToIndex(3) == 4);
writeln((3..5).orderToIndex(1)  == 4);
writeln((0..10 by 2).orderToIndex(2) == 4);
/* STOP_EXAMPLE */
