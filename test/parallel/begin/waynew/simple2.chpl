var go: sync bool;

var a: sync int;

var c = 1;

proc work() {
  var b = 2;

  begin with (ref b, ref c) {
    go;
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
      a;
      work();
    }
  }
}


jam_records();
go = true;

