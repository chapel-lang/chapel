
proc testtuples() {
  var at = (1,);
  assert(at.size==1);
  var b = (1,2);
  assert(b.size==2);
  var bt = (1,2,);
  assert(bt.size==2);
  var c = (1,2,3);
  assert(c.size==3);
  var ct = (1,2,3,);
  assert(ct.size==3);
  writeln("Tuples OK");
}

proc testarrays() {
  var a = [1];
  assert(a.size==1);
  var at = [1,];
  assert(at.size==1);
  var b = [1,2];
  assert(b.size==2);
  var bt = [1,2,];
  assert(bt.size==2);
  var c = [1,2,3];
  assert(c.size==3);
  var ct = [1,2,3,];
  assert(ct.size==3);
  writeln("Array literals OK");
}

proc testassocdoms() {
  var a = {1};
  assert(a.size==1);
  var at = {1,};
  assert(at.size==1);
  var b = {1,2};
  assert(b.size==2);
  var bt = {1,2,};
  assert(bt.size==2);
  var c = {1,2,3};
  assert(c.size==3);
  var ct = {1,2,3,};
  assert(ct.size==3);
  writeln("Assoc domains OK");
}

proc testassocarrays() {
  var a = [1 => "one"];
  assert(a.size==1);
  var at = [1 => "one",];
  assert(at.size==1);
  var b = [1 => "one", 2 => "two"];
  assert(b.size==2);
  var bt = [1 => "one", 2 => "two",];
  assert(bt.size==2);
  var c = [1 => "one", 2 => "two", 3 => "three"];
  assert(c.size==3);
  var ct = [1 => "one", 2 => "two", 3 => "three",];
  assert(ct.size==3);
  writeln("Assoc arrays OK");
}

testtuples();
testarrays();
testassocdoms();
testassocarrays();
