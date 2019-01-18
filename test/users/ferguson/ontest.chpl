class C {
  var x:int;
}

proc main() {
  var c = new unmanaged C(5);
  var d = new unmanaged C(4);
  var x:int;

  on d {
    var y:int;
    for i in 1..100000 {
      y = c.x; // if this reads x = c.x there is no problem
    }
    x = y;
  }

  writeln(x);

  delete d;
  delete c;
}
