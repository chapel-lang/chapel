use Map;

record r { var x: int = 0; }

record myWorker1 {
  proc this(const key, ref val) {
    val = new r(64);
    return none;
  }
}

record myWorker2 {
  proc this(const key, ref val) {
    val = new r(key);
    return none;
  }
}

// Basic test using a record worker.
proc test1() {
  var m = new map(int, r);
  m.add(0, new r());

  writeln(m);
  var worker = new myWorker1();
  var idx = 0;
  m.update(idx, worker);
  writeln(m);
}
test1();

// Similar test but with a parSafe=true map.
proc test2() {
  var m = new map(int, r, parSafe=true);
  for i in 0..7 do m.add(i, new r(0));

  coforall i in 0..7 do m.update(i, new myWorker2());
  for i in 0..7 do assert(m.getValue(i).x == i);
}
test2();

// Now a non-parsafe test using a FCF instead.
proc test3() {
  var m = new map(int, r);
  for i in 0..7 do m.add(i, new r(0));

  // TODO: FCFs cannot take variables by ref right now.
  for i in 0..7 do
    m.update(i, lambda(key: int, val: r) {
      writeln(val);
      return none;
    });
}
test3();

record myWorker3 {
  proc this(const ref key, ref val) throws {
    val = new r(key);
    if val.x >= 7 then
      throw new IllegalArgumentError();
    return none;
  }
}

proc test4() {
  var m = new map(int, r);
  for i in 0..7 do m.add(i, new r(0));

  for i in 0..7 do
    try {
      m.update(i, new myWorker3());
    } catch e: IllegalArgumentError {
      for i in 0..7 do assert(m.getValue(i).x == i);
    } catch {
      halt("Should not reach here!");
    }

  return;
}
test4();

