
use Print;

proc value() {
  return 7;
}

proc test(arg: int) {
  var x = 6;
  select arg {
    when 1 do println(1);
    when 2 do println(4);
    when 3, 4, 5 do println(9);
    when x do println(36);
    when value() do println(49);
    otherwise println(1234);
  }
}

proc paramValue() param do return 7;

proc testParam(param p: int) {
  var x = 5;
  var y = 3;
  select p {
    when x do println(1234);           // make sure values still work
    when y, 1, 2 do println(1);        // mixture of param and value
    when 6 do println(42);             // param only
    when paramValue() do println(777); // param returned via procedure
    otherwise do println(999);         // otherwise
  }
}

proc helper() {
  println(42);
  return 5;
}

proc valueRet(arg: int) {
  select arg {
    when 1 do return 1;       // single-expr case
    when 2 do return 2;       // intentionally do not pass '2' to test AST
    when 3, 4, 5 do return 5; // multi-expr case
    otherwise do return 42;   // otherwise
  }
}

proc valHelper(arg: int) {
  return arg;
}

proc selectReturnProc(arg: int) {
  select arg {
    when valHelper(1) do return 1;
    when valHelper(2) do return 2;
    when valHelper(3), valHelper(4), valHelper(5) do return 5;
  }

  return 0;
}

proc main() {
  test(1);
  test(2);
  test(3);
  test(4);
  test(5);
  test(6);
  test(7);
  test(8);
  test(9);
  test(10);

  testParam(1);
  testParam(2);
  testParam(3);
  testParam(4);
  testParam(5);
  testParam(6);
  testParam(7);
  testParam(8);
  testParam(9);
  testParam(10);

  // Should see only one '42' printed
  select helper() {
    when 1 do println(0);
    when 2 do println(0);
    when 5 do println(5);
    otherwise do println(999);
  }

  println(valueRet(1));
  println(valueRet(5));
  println(valueRet(99));

  println(selectReturnProc(1));
}
