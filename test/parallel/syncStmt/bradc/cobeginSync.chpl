use Time;
var num: sync int = 0;

proc foo() {
  sleep(1);
  num.writeEF(num.readFE() + 1);
}

sync cobegin {
  begin foo();
  begin foo();
  begin foo();
  begin foo();
  begin foo();
  begin foo();
  begin foo();
  begin foo();
  begin foo();
  begin foo();
}

sync {
  cobegin {
    begin foo();
    begin foo();
    begin foo();
    begin foo();
    begin foo();
    begin foo();
    begin foo();
    begin foo();
    begin foo();
    begin foo();
  }
}

writeln("num is: ", num.readXX());
