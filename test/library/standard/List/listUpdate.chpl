use List;

record r { var x: int = 0; }

record myWorker1 {
  proc this(idx, ref val) {
    val = new r(64);
    return none;
  }
}

record myWorker2 {
  proc this(idx, ref val) {
    val = new r(idx);
    return none;
  }
}

// Basic test using a record worker.
proc test1() {
  var lst = new list(r);
  lst.pushBack(new r());

  writeln(lst);
  var worker = new myWorker1();
  lst.update(0, worker);
  writeln(lst);
}
test1();

// Similar test but with a parSafe=true list.
proc test2() {
  var lst = new list(r, parSafe=true);

  for i in 0..7 do lst.pushBack(new r(0));
  writeln(lst);
  var worker = new myWorker2();
  coforall i in 0..7 do lst.update(i, worker);
  writeln(lst);
}
test2();

// Now a non-parsafe test using a FCF instead.
proc test3() {
  var lst = new list(int);
  for i in 0..7 do lst.pushBack(0);

  writeln(lst);

  // TODO: FCFs cannot take variables by ref right now.
  for i in 0..7 do
    lst.update(i, lambda(i: int, val: int) {
      writeln(val);
      return none;
    });

  writeln(lst);
}
test3();

record myWorker3 {
  proc this(idx, ref val) throws {
    val = idx;
    if val >= 7 then
      throw new IllegalArgumentError();
    return none;
  }
}

proc test4() {
  var lst = new list(int);
  for i in 0..7 do lst.pushBack(0);

  writeln(lst);

  for i in 0..7 do
    try {
      lst.update(i, new myWorker3());
    } catch e: IllegalArgumentError {
      writeln(lst);
    } catch {
      halt("Should not reach here!");
    }

  return;
}
test4();
