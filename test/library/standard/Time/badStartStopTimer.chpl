use Time;

var timer: Timer;
timer.stop();  // warn -- stop on never started timer
timer.start(); // ok
timer.start(); // warn -- start on running timer
timer.stop();  // ok
timer.stop();  // warn -- stop on stopped timer
