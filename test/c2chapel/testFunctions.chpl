
use functionGen;

proc main() {
  test_ints(1, -42);

  test_unsigned(1:uint, 42:uint);

  test_strings("hello world");

  var x = 42;
  writeln("x before = ", x);
  test_pointer(x);
  writeln("x after = ", x);

  var A : [1..10] int = 1..10;
  test_array(A[1], A.size);

  writeln("test_return(4)  == ", test_return(4));
  writeln("test_return(-1) == ", test_return(-1));
}
