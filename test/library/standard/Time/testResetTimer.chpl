use Time;

var sw = new stopwatch();

sw.start();
sw.reset();
writeln(sw.elapsed()); // should be 0
sw.start(); // would issue a warning if already running

sw.stop();
sw.reset();
writeln(sw.elapsed()); // should be 0
sw.start(); // would issue a warning if already running
sw.stop();
