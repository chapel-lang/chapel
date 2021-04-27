use Set;

proc test1() {
  var a: set(int);
  var b: set(int, parSafe=true);
  var c: set(int, (a.parSafe || b.parSafe));

  c = a;
}
test1();

