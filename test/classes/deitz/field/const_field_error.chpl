class C {
  var x: int = 1;
  const y: int = 2;
}

var cc = new C(); var c = cc.borrow();
c.x = 3;
c.y = 4;
writeln(c);
