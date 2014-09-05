use Time;

var s: sync int;
var x: int = 0;

begin with (ref x) {
  while true {
    s.readXX();
    x += 1;
  }
}

sleep(1); halt("a thread is out of control");
