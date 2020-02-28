proc test1() {
  writeln("test1");
  var a:range;
  writeln(a.type:string, " ", a);

  var b;
  b = 1..10;
  writeln(b.type:string, " ", b);
  
  var c:range(?);
  c = 2..20 by 2;
  writeln(c.type:string, " ", c);
  
  var d:range;
  d = 3..30;
  writeln(d.type:string, " ", d);
}
test1();

// If these become errors, split them into a separate test.
// Here a variable declared with type `range` is initialized
// to a non-default range.
proc test2() {
  writeln("test2");
  var a:range = 4..40 by 2;
  writeln(a.type:string, " ", a);
  
  var b:range;
  b = 5..50 by 2;
  writeln(b.type:string, " ", b);
}
test2();
