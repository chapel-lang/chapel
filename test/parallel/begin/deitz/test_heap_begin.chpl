use Time;

foo();

class MyNum {
  var x:int;
}

proc foo() {
  var c = new MyNum(17);
  begin {
    sleep(1);
    writeln(c.x);
  }
  c.x = 18;
}
