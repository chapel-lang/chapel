use Map;

record r { var x: int = 0; }

record myWorker {
  proc this(const ref key: int, ref val: int) {
    writeln(val);
  }
}

proc test() {
  var m = new map(int, r);
  m.add(0, new r());
  var idx = 0;
  m.update(idx, new myWorker());
}
test();

