use List;

record r { var x: int = 0; }

record myWorker {
  proc this(idx, ref val: int) {
    val = idx;
  }
}

proc test() {
  var lst = new list(r);
  lst.pushBack(new r());
  writeln(lst);
  var worker = new myWorker();
  lst.update(0, worker);
  writeln(lst);
}
test();
