use VisualDebug;

startVdebug("OH2");
for ix in 1 .. 40 {
  tagVdebug("tag 1");
  pauseVdebug();
  tagVdebug("resume 1");
}
stopVdebug();

writeln("done.");
