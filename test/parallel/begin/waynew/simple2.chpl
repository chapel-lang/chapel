var a = 1;
var go = false;



def work() {
  var b = 2;

  begin {
    while (!go);
    b = 2 * a;
    a = 2 * b;
    writeln( "a is ", a);
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
a = 2;
go = true;

