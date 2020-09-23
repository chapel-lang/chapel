var go: sync bool;
proc ensureDefaultInit(arg) { }
ensureDefaultInit(go); // make sure go is default-initialized

var a: sync int;

var c = 1;

proc work() {
  var b = 2;

  begin with (in b, ref c) {
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
