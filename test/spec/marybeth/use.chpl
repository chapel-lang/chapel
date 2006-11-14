use Time;

var programTimer:Timer;

programTimer.start();
writeln("Write one line.");
programTimer.stop();
var totalTime = programTimer.elapsed();

if (totalTime > 0) then writeln("Non-zero time for writing one line.");
