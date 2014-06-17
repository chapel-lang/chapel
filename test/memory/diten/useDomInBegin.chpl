proc f() {
  use Time;
  var D = {1..10};
  begin {
    sleep(1);
    var A: [D] real;
  }
}

config const n = 10;

proc main {
  for i in 1..n do
    f();
}
