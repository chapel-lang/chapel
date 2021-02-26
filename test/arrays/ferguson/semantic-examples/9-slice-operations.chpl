var A:[1..10] int;

proc foo() {
  return A[1..5];
}

proc test0() {
  writeln("test0");
  ref B = A[1..5];
  B = 1;
  writeln(A);
}
A=0;
test0();


proc test1a() {
  writeln("test1a");
  var B = A[1..5];
  B = 1;
  writeln(A);
}
A=0;
test1a();

proc test1b() {
  writeln("test1b");
  var B:[1..5] int = A[1..5];
  B = 1;
  writeln(A);
}
A=0;
test1b();

proc test2a() {
  writeln("test2a");
  var B = foo();
  B = 1;
  writeln(A);
}
A=0;
test2a();

proc test2b() {
  writeln("test2b");
  var B:[1..5] int = foo();
  B = 1;
  writeln(A);
}
A=0;
test2b();

proc test2c() {
  writeln("test2c");
  ref B = foo();
  B = 1;
  writeln(A);
}
A=0;
test2c();
