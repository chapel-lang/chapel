class A { }
class c0 {
  iter getChildren(): unmanaged A? { yield nil; }
}

class c1 {
  iter getChildren(): unmanaged A? { yield nil; }
}

var c = new unmanaged c1();

for o in c.getChildren() {
  writeln(o);
}

delete c;
