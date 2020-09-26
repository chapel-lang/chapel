use List;

record r { var x: int = 0; }

// Serial test.
proc test1() {
  var lst = new list(r);
  for i in 0..3 do lst.append(new r());

  writeln(lst);
  for i in 0..3 do lst.set(i, new r(i));
  writeln(lst);
}
test1();

// Parallel test.
proc test2() {
  var lst = new list(r, parSafe=true);
  for i in 0..3 do lst.append(new r());

  writeln(lst);

  coforall i in 0..3 with (ref lst) do lst.set(i, new r(i));

  writeln(lst);
}
test2();

