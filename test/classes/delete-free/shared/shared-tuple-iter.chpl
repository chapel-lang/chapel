class C { var x: int; }

proc test1() {
  writeln("test1");
  var tup = (new shared C(1), new shared C(2));
  for x in tup {
    writeln(x, ":", x.type:string);
  }
  for x in tup {
    writeln(x, ":", x.type:string);
  }
}
test1();

proc test2() {
  writeln("test2");
  var a = new shared C(1);
  var b = new shared C(2);
  var tup = (a, b);
  for x in tup {
    writeln(x, ":", x.type:string);
  }
  for x in tup {
    writeln(x, ":", x.type:string);
  }
}
test2();

proc test3() {
  writeln("test3");
  var a = new shared C(1);
  var b = new shared C(2);
  for x in (a, b) {
    writeln(x, ":", x.type:string);
  }
  for x in (a, b) {
    writeln(x, ":", x.type:string);
  }
}
test3();
