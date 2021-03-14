class C { var x: int; }

proc id(arg) {
  return arg;
}

proc main () {
  var tmp:[1..2] owned C? = for i in 1..2 do new owned C(i);
  var a:[1..2] owned C = tmp:owned C;
  id(a);
  writeln(a);
}
