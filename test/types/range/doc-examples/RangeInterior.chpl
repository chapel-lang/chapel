//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//

/* START_EXAMPLE */
// These will all print 'true'.
writeln((0..9).interior(1)  == 9..9);
writeln((0..9).interior(2)  == 8..9);
writeln((0..9).interior(-1) == 0..0);
writeln((0..9).interior(-2) == 0..1);
/* STOP_EXAMPLE */
