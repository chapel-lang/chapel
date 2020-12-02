class C { var x: int; }

proc main () {
  var tmp:[1..2] owned C? = for i in 1..2 do new owned C(i);
  var a:[1..2] owned C = tmp:owned C;
  var b = a;
  writeln(a);
  writeln(b);
}
