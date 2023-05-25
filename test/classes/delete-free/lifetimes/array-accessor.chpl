pragma "safe"
module l15 {

class MyClass {
  var x:int;
}

proc ref MyClass.get(i: 1*int) ref {
  return this.x;
}

proc ref MyClass.get(i ...1) ref {
  return this.get(i);
}

proc test() {
  var c = (new owned MyClass(1)).borrow();

  ref rx = c.get(1);
  rx += 1;
  ref rx2 = c.get((1,));
  rx2 += 1;

  writeln(c);
}

test();
}
