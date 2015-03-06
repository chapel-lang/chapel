/*
 * Verify user defined counts reduction works with defaults.
 */

use counts;

var particles = [6, 7, 6, 3, 8, 2, 8, 4, 8, 3],
  octantCounts = counts reduce particles;
writeln("octant counts: ", octantCounts);
