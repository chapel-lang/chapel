// This test uses debug output from DefaultRectangular
// to show allocations, post-allocation, and deallocation.

// If the output changes, just make sure:
//   * the number of allocations matches the number of deallocations
//   * the number of allocations is reasonable/expected
//   * postalloc is being called 

writeln("alloc int test begins");

proc test1() {
  writeln("alloc int test1");
  var A:[1..10] int;
}
test1();

proc test2() {
  // note this case doesn't use same code path & has different debug output
  writeln("alloc int test2");
  var A = for i in 1..10 do i;
}
test2();

proc test2t() {
  writeln("alloc int test2t");
  var A:[1..10] int = for i in 1..10 do i;
}
test2t();

proc test3() {
  writeln("alloc int test3");
  var A = forall i in 1..10 do i;
}
test3();

proc test3t() {
  writeln("alloc int test3t");
  var A:[1..10] int = forall i in 1..10 do i;
}
test3t();

proc test4() {
  writeln("alloc int test4");
  var A:[1..10] int = 1..10;
}
test4();

proc test5() {
  writeln("alloc int test5");
  var A:[1..10] int = 1;
}
test5();

proc test6() {
  writeln("alloc int test6");
  var A:[1..10] int = 1;
  var B = A; // copy elision
}
test6();

proc test6t() {
  writeln("alloc int test6t");
  var A:[1..10] int = 1;
  var B:[1..10] int = A; // copy elision
}
test6t();

proc test7() {
  writeln("alloc int test7");
  var A:[1..10] int = 1;
  var B = A;
  writeln(A); // no copy elision
}
test7();

proc test7t() {
  writeln("alloc int test7t");
  var A:[1..10] int = 1;
  var B:[1..10] int = A;
  writeln(A); // no copy elision
}
test7t();


proc test8() {
  writeln("alloc int test7");
  var A:[1..2] int = (1,2);
}
test8();

writeln("alloc int test ends");
