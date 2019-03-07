class C {
  var notaparam = 42;
  param p = notaparam;
  proc init() { }
}

var c = new owned C();
writeln(c);
