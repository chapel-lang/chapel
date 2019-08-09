// chplvis: Begin Tasks

// Example 4, begin tasks as shown in chplvis
// This is a contrived example to have tasks live
// across a tagVdebug() call.

use VisualDebug;
use BlockDist;

const space =  { 0 .. #numLocales };
const Dspace = space dmapped Block (boundingBox=space);

startVdebug("E4");

var go$: [Dspace] single bool;
var done$: [Dspace] single bool;

// Start a begin task on all locales.  The task will start and then block.
coforall loc in Locales do
  on loc do begin { // start a async task

              go$[here.id]; // Block until ready!
              writeln ("Finishing running the 'begin' statement on locale "
                       , here.id, ".");
              done$[here.id] = true;
           }

tagVdebug("loc");

coforall loc in Locales do
    on loc do writeln("Hello from ", here.id);

tagVdebug("finish");

// Let all tasks go
go$ = true;

// Wait until all tasks are finished
done$;

stopVdebug();


