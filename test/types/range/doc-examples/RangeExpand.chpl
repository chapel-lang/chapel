//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//

/* START_EXAMPLE */
// These will all print 'true'.
writeln((0..9).expand(1)  == -1..10);
writeln((0..9).expand(2)  == -2..11);
writeln((0..9).expand(-1) == 1..8);
writeln((0..9).expand(-2) == 2..7);
/* STOP_EXAMPLE */
