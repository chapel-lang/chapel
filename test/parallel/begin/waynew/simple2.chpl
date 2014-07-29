var go: sync bool;

var a: sync int;

var c = 1;

proc work() {
  var b = 2;

  begin ref(b, c) {
    go.readFE();
    b = 2 * c;
    writeln("b is ", b);
    c = 2 * b;
    a = 2 * b;
    go = true;
  }
}


proc jam_records() {
  cobegin {
    work();
    begin {
      a.readFE();
      work();
    }
  }
}


jam_records();
go = true;

