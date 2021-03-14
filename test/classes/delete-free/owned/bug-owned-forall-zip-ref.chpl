
proc test1() {
  var a = new owned C();
  var b = new owned C();
  a.i = 2;
  b.i = 20;

  writeln("test1");
  for a1 in a {
    writeln(a1);
  }

  forall (a1,b1) in zip(a,b) {
    writeln(a1, " ",b1);
  }
}
test1();

proc test2() {
  var a = new owned C();
  var b = new owned C();
  a.i = 2;
  b.i = 20;

  writeln("test2");
  for a1 in a {
    writeln(a1);
  }

  for (a1, b1) in zip(a, b) {
    b1 = a1;
  }

  forall (a1,b1) in zip(a,b) {
    writeln(a1, " ",b1);
  }
}
test2();

proc test3() {
  var a = new owned C();
  var b = new owned C();
  a.i = 2;
  b.i = 20;

  writeln("test3");
  for a1 in a {
    writeln(a1);
  }
  for a1 in a {
    writeln(a1);
  }
  for a1 in a {
    writeln(a1);
  }
 
  for (a1,b1) in zip(a,b) {
    writeln(a1, " ",b1);
  }
 
  for (a1,b1) in zip(a,b) {
    writeln(a1, " ",b1);
  }
 
  for (a1,b1) in zip(a,b) {
    writeln(a1, " ",b1);
  }
}
test3();

proc test4() {
  var a = new owned C();
  var b = new owned C();
  a.i = 2;
  b.i = 20;

  writeln("test4");
  forall a1 in a {
    writeln(a1);
  }
  forall a1 in a {
    writeln(a1);
  }
  forall a1 in a {
    writeln(a1);
  }
}
test4();

proc test5() {
  var a = new owned C();
  var b = new owned C();
  a.i = 2;
  b.i = 20;

  writeln("test5");
  forall (a1,b1) in zip(a,b) {
    writeln(a1, " ",b1);
  }
 
  forall (a1,b1) in zip(a,b) {
    writeln(a1, " ",b1);
  }
 
  forall (a1,b1) in zip(a,b) {
    writeln(a1, " ",b1);
  }
}
test5();


proc test6() {
  var a = new owned C();
  var b = new owned C();
  a.i = 2;
  b.i = 20;

  writeln("test6");
  for a1 in a {
    writeln(a1);
  }

  forall (a1, b1) in zip(a, b) {
    b1 = a1;
  }

  forall (a1,b1) in zip(a,b) {
    writeln(a1, " ",b1);
  }
}
test6();



class C {
  var i : int;
  iter these() ref
  {
    yield i;
  }

  iter these(param tag : iterKind) where tag==iterKind.leader {
    yield i;
  }

  iter these(param tag : iterKind, followThis) ref
  where tag==iterKind.follower {
    yield i;
  }
}
