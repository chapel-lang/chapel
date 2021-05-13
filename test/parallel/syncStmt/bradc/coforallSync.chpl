use Time;
var num: sync int = 0;

proc foo() {
  sleep(1);
  num.writeEF(num.readFE() + 1);
}

sync coforall 1..10 {
  begin foo();
}

sync {
  coforall 1..10 {
    begin foo();
  }
}

writeln("num is: ", num.readXX());
