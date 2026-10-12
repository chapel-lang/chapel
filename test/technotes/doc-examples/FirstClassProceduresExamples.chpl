//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//

proc test0() {
  /* START_EXAMPLE_0 */
  type T = proc(x: int, y: int): int;
  writeln(T:string); // 'proc(x: int, y: int): int'
  /* STOP_EXAMPLE_0 */
}
test0();

proc test1() {
  /* START_EXAMPLE_1 */
  // The procedure named 'foo' is not an anonymous procedure.
  proc foo(x: int, y: int): int { return x + y; }

  // Define an anonymous procedure bound to the constant variable 'bar'.
  const bar = proc(a: int, b: int): int { return a + b; };
  /* STOP_EXAMPLE_1 */
}
test1();

proc test2() {
  /* START_EXAMPLE_2 */
  proc myfunc(x:int) { return x + 1; }
  const p = myfunc;
  writeln(p(3));  // outputs: 4
  /* STOP_EXAMPLE_2 */
}
test2();

proc test3() {
  /* START_EXAMPLE_3 */
  const p = proc(x: int) { return x + 1; };
  writeln(p(3));  // outputs: 4
  /* STOP_EXAMPLE_3 */
}
test3();
