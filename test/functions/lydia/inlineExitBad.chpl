inline proc exiter(x: int): int {
  if (x > 10) {
    exit(0);
  } else {
    return x;
  }
}

proc callExiter(x: int) {
  writeln(exiter(x));
}

writeln("starting program!");
callExiter(5);
callExiter(11);
writeln("Should never reach here!");
