use VisualDebug;

startVdebug("OH1");
for ix in 1 .. 40 {
  tagVdebug("tag 1");
  pauseVdebug();
  resumeVdebug("resume 1");
}
stopVdebug();

writeln("done.");
