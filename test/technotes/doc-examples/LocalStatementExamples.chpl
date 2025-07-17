//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//

proc test0() {
  var x: int;
  var A: [0..7] int = 0..7;
  /* START_EXAMPLE_0 */
  local do
    x = A(5);
  /* STOP_EXAMPLE_0 */
  assert(x == 5);
}
test0();

inline proc initializeMyData(data) {}
inline proc compute(data) {}

proc test1() {
  var data: int;
  /* START_EXAMPLE_1 */
  local {
    initializeMyData(data);
    compute(data);
  }
  /* STOP_EXAMPLE_1 */
}
test1();

proc test2() {
  var data: int;
  /* START_EXAMPLE_2 */
  local data.locale == here {
    initializeMyData(data);
    compute(data);
  }
  /* STOP_EXAMPLE_2 */
}
test2();

proc test3() {
  var data: int;
  /* START_EXAMPLE_3 */
  if data.locale == here {
    local {
      initializeMyData(data);
      compute(data);
    }
  } else {
    initializeMyData(data);
    compute(data);
  }
  /* STOP_EXAMPLE_3 */
}
test3();
