use Time;

var sw = new stopwatch();

sw.start();
sw.restart();
sw.stop(); // would issue a warning if not running

sw.restart();
sw.restart();
sw.stop(); // would issue a warning if not running
