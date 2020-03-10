record R {
  proc deinit() { }
}

config const option = true;

proc refIdentity(ref arg) ref {
  return arg;
}

proc test1() {
  var a:R;
  ref r = a;
  var b = a; // copy elision
  writeln(r); // expect an error
}
test1();

proc test2() {
  var a:R;
  ref r = refIdentity(a); 
  var b = a; // copy elision
  writeln(r); // expect an error
}
test2();

proc test3() {
  var a:R;
  ref r = refIdentity(a); 
  if option {
    var b = a; // copy elision
    return;
  }
  writeln(r); // this one is OK
}
test3();

proc test4() {
  var a:R;
  {
    ref r = refIdentity(a); 
    var b = a; // copy elision
    writeln(r);
    return;
  }
}
test4();

proc test5() {
  var a:R;
  ref r = refIdentity(a); 
  {
    var b = a; // copy elision
    writeln(r);
  }
}
test5();

class C { var x: int; }

proc test6() {
  var a:owned C = new C(1);
  var b = a.borrow();
  {
    var b = a; // copy elision
    return;
  }
}
test6();
