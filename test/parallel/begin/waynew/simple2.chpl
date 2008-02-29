var go: sync bool;

var a: sync int;

var c = 1;

def work() {
  var b = 2;

  begin {
    go;
    b = 2 * c;
    writeln("b is ", b);
    c = 2 * b;
    a = 2 * b;
    go = true;
  }
}


def jam_records() {
  cobegin {
    work();
    begin {
      a;
      work();
    }
  }
}


jam_records();
go = true;

