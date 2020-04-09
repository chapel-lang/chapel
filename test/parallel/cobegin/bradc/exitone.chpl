use Time;

var s: sync int;

proc sleepAndExit() {
  sleep(5);
  writeln("Done sleeping...about to exit");
  exit(0);
}

proc spin() {
  writeln("Spinning...");
  while (1) {
    s.readXX();
  }
}


cobegin {
  spin();
  sleepAndExit();
}
