config var notaparam = 42;
class C {
  param p = notaparam;
  proc init() { }
}

var c = new owned C();
writeln(c);
