//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//

/* START_EXAMPLE */
// These will all print 'true'.
writeln((0..9).exterior(1)  == 10..10);
writeln((0..9).exterior(2)  == 10..11);
writeln((0..9).exterior(-1) == -1..-1);
writeln((0..9).exterior(-2) == -2..-1);
/* STOP_EXAMPLE */
