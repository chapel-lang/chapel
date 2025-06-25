//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//

/* START_EXAMPLE */
use ReplicatedDist;

const Dbase = {0..5};  // A default-distributed domain
const Drepl = Dbase dmapped new replicatedDist();
var Abase: [Dbase] int = 0..5;
var Arepl: [Drepl] int;

forall loc in Locales do on loc {
  // Only the current locale's replicand is accessed
  Arepl[here.id] = Abase[here.id];
}

// Only the current locale's replicand is accessed
Arepl[0] = 9;

// Access other locales' replicand with the replicand(loc) method
Arepl.replicand(Locales[1]) = Arepl + 1;

writeln(Abase);
for loc in Locales do on loc do writeln(here, ": ", Arepl);
/* STOP_EXAMPLE */


