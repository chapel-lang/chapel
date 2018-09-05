class C {
 proc init() {}
 proc p(x) { }
 proc t(x) { }
}

class D : C {
 proc init() {
   super.init();
 }
 override proc p(x) {
   writeln("In D.p(", x, ")");
 }
 override proc t(x) {
   writeln("In D.t(", x, ")");
 }

}

{
  var d = new owned D();
  writeln(d.type : string);
  d.p(1);
}

{
  var e = new shared D();
  writeln(e.type : string);
  e.p(2);
}

{
  var f = new owned D();
  writeln(f.type : string);
  f.t(3);
}

{
  var g = new shared D();
  writeln(g.type : string);
  g.t(4);
}
