class c0 {
  iter getChildren(): locale { yield nil; }
}

class c1 {
  iter getChildren(): locale { yield nil; }
}

var c = new c1();
for o in c.getChildren() {
  writeln(o);
}
