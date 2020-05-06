class C { var x: int = 0; }

proc test() {
  var own = new owned C(128);
  var dom: domain(owned C);
  dom.add(own);
  writeln(dom);
  return;
}
test();

