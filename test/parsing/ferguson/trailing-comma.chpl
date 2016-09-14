
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
}

proc testarrays() {
  var a = [1];
  assert(a.size==1);
  var at = [1,];
  assert(a.size==1);
  var b = [1,2];
  assert(b.size==2);
  var bt = [1,2,];
  assert(bt.size==2);
  var c = [1,2,3];
  assert(c.size==3);
  var ct = [1,2,3,];
  assert(ct.size==3);
}

testtuples();
testarrays();
writeln("OK");
