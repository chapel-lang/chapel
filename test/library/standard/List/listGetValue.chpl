use List;

record r { var x: int = 0; }

// Test mutability (it should be a separate copy).
proc test1() {
  var lst = new list(r);
  for i in 0..3 do lst.pushBack(new r());

  writeln(lst);

  for i in 0..3 {
    var val = lst.getValue(i);
    val.x = i;
  }

  writeln(lst);
}
test1();

// Test use when parSafe=true.
proc test2() {
  var lst = new list(r, parSafe=true);
  for i in 0..3 do lst.pushBack(new r());

  writeln(lst);

  coforall i in 0..3 {
    var val = lst.getValue(i);
    val.x = i;
  }

  writeln(lst);
}
test2();
