import Set.set;

var id = 0;

record r { var id: int = 0; }
proc r.init() {
  this.id = id;
  this.complete();
  id += 1;
  writeln('init r: ', id);
}

proc r.deinit() {
  writeln('deinit r: ', id);
}

proc test() {
  var s: set(r);
  var foo = new r();
  s.add(foo);
  s.remove(foo);
}
test();

