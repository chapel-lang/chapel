use List;

class C { var x: int = 0; }

// Test mutability (it should be a separate copy).
proc test1() {
  var lst = new list(shared C);
  for i in 0..3 do lst.pushBack(new shared C());

  writeln(lst);

  for i in 0..3 {
    var val = lst.getBorrowed(i);
    assert(isBorrowedClass(val));
    val.x = i;
  }

  writeln(lst);
}
test1();

// Test use when parSafe=true.
proc test2() {
  var lst = new list(shared C, parSafe=true);
  for i in 0..3 do lst.pushBack(new shared C());

  writeln(lst);

  coforall i in 0..3 {
    var val = lst.getBorrowed(i);
    assert(isBorrowedClass(val));
    val.x = i;
  }

  writeln(lst);
}
test2();
