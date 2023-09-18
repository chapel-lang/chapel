use List;

class C {
  var x: int;
}

proc test() {
  var lst:list(owned C);

  lst.pushBack(new C(1));

  var lst2 = lst; // this should be allowed and be from copy elision
  writeln(lst2);
}
test();
