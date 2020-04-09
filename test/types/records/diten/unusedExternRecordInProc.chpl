proc foo() {
  extern "struct timeval" record timeval {
    var tv_sec: int;
    var tv_usec: int;
  }

  var tv: timeval;
}

writeln("Not calling foo.");
// foo();
