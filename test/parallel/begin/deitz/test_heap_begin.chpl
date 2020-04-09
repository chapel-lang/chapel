use Time;

foo();

class MyNum {
  var x:int;
}

proc foo() {
  var c = new unmanaged MyNum(17);

  begin {
    sleep(1);
    writeln(c.x);

    delete c;
  }

  c.x = 18;
}
