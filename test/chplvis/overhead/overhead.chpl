config var dir = "OH";

use VisualDebug;

startVdebug(dir);
tagVdebug("tag 1");
pauseVdebug();
tagVdebug("resume 1");
stopVdebug();

writeln("done.");
