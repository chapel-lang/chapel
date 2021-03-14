use Time;

proc main {
  var s: sync int;
  begin {
    sleep(1);
    writeln("I am here");
    s.writeEF(1);
  }
  s.readFE();
  writeln("And now I am here");
}

