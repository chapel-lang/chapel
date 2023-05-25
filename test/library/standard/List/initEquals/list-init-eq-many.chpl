use List;

// test copy from list
proc newCopyFromList() {
  writeln("newCopyFromList");
  var x:list(int);
  x.pushBack(1); x.pushBack(2);
  writeln("x:", x.type:string, " = ", x);


  var a = new list(x);
  writeln("a:", a.type:string, " = ", a);
  var b = new list(parSafe=false, x);
  writeln("b:", b.type:string, " = ", b);
  var c = new list(parSafe=true, x);
  writeln("c:", c.type:string, " = ", c);
  
  var y:list(int, parSafe=true);
  y.pushBack(1); y.pushBack(2);
  writeln("y:", y.type:string, " = ", y);

  var d = new list(y);
  writeln("d:", d.type:string, " = ", d);
  var e = new list(parSafe=false, y);
  writeln("e:", e.type:string, " = ", e);
  var f = new list(parSafe=true, y);
  writeln("f:", f.type:string, " = ", f);
}
newCopyFromList();

proc initCopyFromList() {
  writeln("initCopyFromList");
  var x:list(int);
  x.pushBack(1); x.pushBack(2);
  writeln("x:", x.type:string, " = ", x);

  var a: list = x;
  writeln("a:", a.type:string, " = ", a);
  var b: list(parSafe=false, ?) = x;
  writeln("b:", b.type:string, " = ", b);
  var c: list(parSafe=true, ?) = x;
  writeln("c:", c.type:string, " = ", c);

  var y:list(parSafe=true, int);
  y.pushBack(1); y.pushBack(2);
  writeln("y:", y.type:string, " = ", y);

  var d: list = y;
  writeln("d:", d.type:string, " = ", d);
  var e: list(parSafe=false, ?) = y;
  writeln("e:", e.type:string, " = ", e);
  var f: list(parSafe=true, ?) = y;
  writeln("f:", f.type:string, " = ", f);

  // other forms
  var g = y;
  writeln("g:", d.type:string, " = ", g);
  var h:list(int, false) = y;
  writeln("h:", h.type:string, " = ", h);
  var i:list(int) = y;
  writeln("i:", i.type:string, " = ", i);

  var z:list(int(8));
  z.pushBack(1); z.pushBack(2);
  writeln("z:", z.type:string, " = ", z);

  var j: list(int) = z;
  writeln("j:", j.type:string, " = ", j);
  var k: list(int, parSafe=false) = z;
  writeln("k:", k.type:string, " = ", k);
  var l: list(int, parSafe=true) = z;
  writeln("l:", l.type:string, " = ", l);
}
initCopyFromList();

// test copy from array
proc newCopyFromArray() {
  writeln("newCopyFromArray");
  var x:[1..2] int = 1..2;
  writeln("x:", x.type:string, " = ", x);

  var a = new list(x);
  writeln("a:", a.type:string, " = ", a);
  var b = new list(parSafe=false, x);
  writeln("b:", b.type:string, " = ", b);
  var c = new list(parSafe=true, x);
  writeln("c:", c.type:string, " = ", c);
}
newCopyFromArray();

proc initCopyFromArray() {
  writeln("initCopyFromArray");
  var x:[1..2] int = 1..2;
  writeln("x:", x.type:string, " = ", x);

  var a: list = x;
  writeln("a:", a.type:string, " = ", a);
  var b: list(parSafe=false, ?) = x;
  writeln("b:", b.type:string, " = ", b);
  var c: list(parSafe=true, ?) = x;
  writeln("c:", c.type:string, " = ", c);

  var z:[1..2] int(8) = (1:int(8))..(2:int(8));
  writeln("z:", z.type:string, " = ", z);

  var j: list(int) = z;
  writeln("j:", j.type:string, " = ", j);
  var k: list(int, parSafe=false) = z;
  writeln("k:", k.type:string, " = ", k);
  var l: list(int, parSafe=true) = z;
  writeln("l:", l.type:string, " = ", l);
}
initCopyFromArray();


// test copy from range
proc newCopyFromRange() {
  writeln("newCopyFromRange");
  var x = 1..2;
  writeln("x:", x.type:string, " = ", x);

  var a = new list(x);
  writeln("a:", a.type:string, " = ", a);
  var b = new list(parSafe=false, x);
  writeln("b:", b.type:string, " = ", b);
  var c = new list(parSafe=true, x);
  writeln("c:", c.type:string, " = ", c);
}
newCopyFromRange();

proc initCopyFromRange() {
  writeln("initCopyFromRange");
  var x = 1..2;
  writeln("x:", x.type:string, " = ", x);

  var a: list = x;
  writeln("a:", a.type:string, " = ", a);
  var b: list(parSafe=false, ?) = x;
  writeln("b:", b.type:string, " = ", b);
  var c: list(parSafe=true, ?) = x;
  writeln("c:", c.type:string, " = ", c);

  var z = (1:int(8))..(2:int(8));
  writeln("z:", z.type:string, " = ", z);

  var j: list(int) = z;
  writeln("j:", j.type:string, " = ", j);
  var k: list(int, parSafe=false) = z;
  writeln("k:", k.type:string, " = ", k);
  var l: list(int, parSafe=true) = z;
  writeln("l:", l.type:string, " = ", l);
}
initCopyFromRange();


// test copy from iterator expr
iter myIter() {
  for i in 1..2 {
    yield i;
  }
}

proc newCopyFromIter() {
  writeln("newCopyFromIter");
  var a = new list(for i in 1..2 do i);
  writeln("a:", a.type:string, " = ", a);
  var b = new list(myIter());
  writeln("b:", b.type:string, " = ", b);
  var c = new list((1..2).these());
  writeln("c:", c.type:string, " = ", c);
  var d = new list(parSafe=false, [i in 1..2] i);
  writeln("d:", d.type:string, " = ", d);
  var e = new list(parSafe=true, myIter());
  writeln("e:", e.type:string, " = ", e);
}
newCopyFromIter();

proc initCopyFromIter() {
  writeln("initCopyFromIter");
  var a:list = for i in 1..2 do i;
  writeln("a:", a.type:string, " = ", a);
  var b:list = myIter();
  writeln("b:", b.type:string, " = ", b);
  var c:list = (1..2).these();
  writeln("c:", c.type:string, " = ", c);
  var d:list(parSafe=false, ?) = [i in 1..2] i;
  writeln("d:", d.type:string, " = ", d);
  var e:list(parSafe=true, ?) = myIter();
  writeln("e:", e.type:string, " = ", e);

  var j: list(int) = for i in 1..2 do i:int(8);
  writeln("j:", j.type:string, " = ", j);
  var k: list(int, parSafe=false) = for i in 1..2 do i:int(8);
  writeln("k:", k.type:string, " = ", k);
  var l: list(int, parSafe=true) = for i in 1..2 do i:int(8);
  writeln("l:", l.type:string, " = ", l);
}
initCopyFromIter();
