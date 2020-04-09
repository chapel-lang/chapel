record R {
  // It works with 
  //    var s = "hello";
  // but not with
  var s:string = "hello";
}

proc test(in x)
{
  writeln("test x:", x);
  x = "later";
  writeln("test x:", x);
}
proc test2(in x = "yo")
{
  writeln("test2 x:", x);
  x = "bye";
  writeln("test2 x:", x);
}

var sup = "sup";
proc test3(ref x = sup, zero=0)
{
  writeln("test3 x:", x);
  x = "cya";
  writeln("test3 x:", x);
}



proc run() {
  var a = new R();
  var b = new R("goodbye");
  writeln(a);
  writeln(b);

  var hi = "hello";
  test(hi);
  test2();
  test2(hi);
  writeln("now hi is ", hi);
  test3();
  test3(hi);
  writeln("now hi is ", hi);
};

run();
