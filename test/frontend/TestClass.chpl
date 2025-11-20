use Print;

class C {
  var x : int;
  var y : real;

  proc init(_x: int = 0, _y: real = 0.0) {
    x = _x;
    y = _y;
  }
}

class Parent {
  var x : int;
}

class Child : Parent {
  var y : real;

  proc init(_x: int = 0, _y: real = 0.0) {
    super.init(_x);
    y = _y;
  }
}

proc returnOwned(cond: bool) : Parent {
  if cond then
    return new owned Child(1, 2.0);
  else
    return new owned Parent(3);
}

proc returnShared(cond: bool) : Parent {
  if cond then
    return new shared Child(1, 2.0);
  else
    return new shared Parent(3);
}

proc main() {
  var c = new unmanaged C(10, 42.0);

  println(c.x);
  println(c.y);

  var z = new unmanaged Child(3, 9.0);
  println(z.x);
  println(z.y);

  {
    var x = new C(5, 42.0);
    println(x.x);

    var o = new owned C(7, 84.0);
    println(o.x);

    var t = returnOwned(true);
    println(t.x);

    var f = returnOwned(false);
    println(f.x);
  }

  {
    var o = new shared C(7, 84.0);
    println(o.x);

    var t = returnShared(true);
    println(t.x);

    var f = returnShared(false);
    println(f.x);
  }
}

