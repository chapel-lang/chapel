class C1 { var x = 0; }
class C2 : C1 { var x = 0; }

proc print(val: shared C1) {
  writeln(val);
}

proc test() {
  var c = new shared C2();
  print(c);
}
test();

