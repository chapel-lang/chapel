var a = 1;
var go = false;



def work() {
  var b = 2;

  begin {
    while (!go);
    b = 2 * a;
    writeln( "b is ", b);
    a = 2 * b;
  }
}


def jam_records() {
  cobegin {
    work();
    begin {
      while (a == 1);
      work();
    }
  }
}


jam_records();
go = true;

