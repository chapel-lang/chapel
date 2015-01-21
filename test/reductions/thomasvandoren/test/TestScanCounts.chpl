/*
 * Verify user defined counts scan works with defaults.
 */

use scanCounts;

var particles = [6, 7, 6, 3, 8, 2, 8, 4, 8, 3],
  octantRankings = scanCounts scan particles;
writeln("octant rankings: ", octantRankings);
