//  Example 1 using visual debug

use VisualDebug;

startVdebug("E1");

coforall loc in Locales do
   on loc do writeln("Hello from locale " + here.id + ".");

stopVdebug();
