inline proc exiter() {
  exit(0);
}

proc callExiter() {
  exiter();
}

writeln("starting program!");
callExiter();
writeln("Should never reach here!");
